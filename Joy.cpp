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
  const char *basename = "/dev/input/js";
  char *fd_name = new char[strlen(basename) + 4];
  sprintf(fd_name, "%s%d", basename, joynum);

  fd = open(fd_name, O_RDONLY | O_NONBLOCK);
  if(fd < 0) {
    throw E_JOY_OPEN;
  }
  if(ioctl(fd, JSIOCGBUTTONS, &num_buttons) < 0) {
    throw E_JOY_IOCTL_BUTTONS;
  }
  if(ioctl(fd, JSIOCGAXES, &num_axes) < 0) {
    throw E_JOY_IOCTL_AXES;
  }
  
  button_states = new bool[num_buttons];
  axes_states = new int16_t[num_axes];

  delete[] fd_name;
}

Joy::~Joy() {
  delete[] button_states;
  delete[] axes_states;
  
  close(fd);
}

void Joy::pollEvents() {
  struct js_event event;

  while(read(fd, &event, sizeof(event)) > 0) {
    if( (event.type & ~JS_EVENT_INIT) == JS_EVENT_BUTTON) {
      if(event.number > num_buttons) {
        throw E_JOY_EVENT_NUMBER_RANGE;
      }
      button_states[event.number] = event.value == 1;
    } else if( (event.type & ~JS_EVENT_INIT) == JS_EVENT_AXIS) {
      if(event.number > num_axes) {
        throw E_JOY_EVENT_NUMBER_RANGE;
      }
      axes_states[event.number] = event.value;
    } else {
      throw E_JOY_EVENT_TYPE;
    }
  }
  if(errno != EAGAIN) {
    throw E_JOY_READ;
  }
}

bool Joy::getRawButton(int button_idx) {
  if(button_idx > num_buttons) {
    throw E_JOY_BUTTON_IDX_RANGE;
  }
  return button_states[button_idx];
}

double Joy::getRawAxis(int axis_idx) {
  if(axis_idx > num_axes) {
    throw E_JOY_AXIS_IDX_RANGE;
  }
  return axes_states[axis_idx] / ((double)_MAX_AXIS_VALUE);
}

int Joy::getNumButtons() {
  return num_buttons;
}

int Joy::getNumAxes() {
  return num_axes;
}
