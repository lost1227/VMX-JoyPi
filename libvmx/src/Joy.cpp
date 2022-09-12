#include "Joy.h"
#include "Exceptions.h"

#include <linux/joystick.h>

#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <errno.h>

Joy::Joy(int joynum) {
  connected = false;

  this->joynum = joynum;

  fd = -1;

  num_buttons = 0;
  num_axes = 0;

  button_states = NULL;
  axes_states = NULL;

  button_states = new bool[num_buttons];
  axes_states = new int16_t[num_axes];

  try {
    this->connect();
  }
  catch(JoystickNotPresentError&) {}

}

Joy::~Joy() {
  delete[] button_states;
  delete[] axes_states;

  close(fd);
}

void Joy::connect() {
  const char *basename = "/dev/input/js";
  char *fd_name = new char[strlen(basename) + 4];
  sprintf(fd_name, "%s%d", basename, joynum);
  fd = open(fd_name, O_RDONLY | O_NONBLOCK);
  delete[] fd_name;
  if(fd < 0) {
    throw JoystickNotPresentError();
  }

  int new_num_buttons, new_num_axes;
  if(ioctl(fd, JSIOCGBUTTONS, &new_num_buttons) < 0) {
    throw std::runtime_error("ioctl: failed to retrieve joystick buttons");
  }
  if(ioctl(fd, JSIOCGAXES, &new_num_axes) < 0) {
    throw std::runtime_error("ioctl: failed to retrieve joystick axes");
  }

  if(new_num_buttons != num_buttons) {
    num_buttons = new_num_buttons;
    if(button_states) {
      delete[] button_states;
    }
    button_states = new bool[num_buttons];
  }
  if(new_num_axes != num_axes) {
    num_axes = new_num_axes;
    if(axes_states) {
      delete[] axes_states;
    }
    axes_states = new int16_t[num_axes];
  }

  memset(button_states, 0, num_buttons * sizeof(bool));
  memset(axes_states, 0, num_axes * sizeof(int16_t));

  connected = true;
}

void Joy::disconnect() {
  connected = false;
  memset(button_states, 0, num_buttons * sizeof(bool));
  memset(axes_states, 0, num_axes * sizeof(int16_t));

  close(fd);
}

void Joy::pollEvents() {
  struct js_event event;

  if(!connected) {
    try {
      this->connect();
    } catch (JoystickNotPresentError&) {
      return;
    }
  }

  while(read(fd, &event, sizeof(event)) > 0) {
    if( (event.type & ~JS_EVENT_INIT) == JS_EVENT_BUTTON) {
      if(event.number > num_buttons) {
        throw std::runtime_error("Received button event for an out-of-range button");
      }
      button_states[event.number] = event.value == 1;
    } else if( (event.type & ~JS_EVENT_INIT) == JS_EVENT_AXIS) {
      if(event.number > num_axes) {
        throw std::runtime_error("Received axis event for an out-of-range axis");
      }
      axes_states[event.number] = event.value;
    } else {
      throw std::runtime_error("Received joystick event of unknown type");
    }
  }
  if(errno == EAGAIN) {
    return;
  } else if(errno == ENODEV) {
    this->disconnect();
  } else {
    throw std::runtime_error("Unknown error when reading the joystick");
  }
}

bool Joy::isConnected() {
  return connected;
}

bool Joy::getRawButton(int button_idx) {
  if(button_idx > num_buttons) {
    throw ValueError("Requested button state for an out-of-range button");
  }
  return button_states[button_idx];
}

double Joy::getRawAxis(int axis_idx) {
  if(axis_idx > num_axes) {
    throw ValueError("Requested axis state for an out-of-range axis");
  }
  return axes_states[axis_idx] / ((double)_MAX_AXIS_VALUE);
}

int Joy::getNumButtons() {
  return num_buttons;
}

int Joy::getNumAxes() {
  return num_axes;
}
