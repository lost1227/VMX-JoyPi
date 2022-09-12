#pragma once

#include <stdint.h>

#define _MAX_AXIS_VALUE 32767

class Joy {
  private:
    bool connected;
    int joynum;
    int fd;
    uint8_t num_buttons;
    uint8_t num_axes;
    bool *button_states;
    int16_t *axes_states;
    void connect();
    void disconnect();
  public:
    Joy(int joynum);
    ~Joy();
    bool isConnected();
    bool getRawButton(int button_idx);
    double getRawAxis(int axis_idx);
    int getNumButtons();
    int getNumAxes();
    void pollEvents();
};
