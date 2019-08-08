#ifndef _JOY_H
#define _JOY_H

#include <stdint.h>

#define _MAX_AXIS_VALUE 32767

class Joy {
  private:
    int fd;
    char num_buttons;
    char num_axes;
    bool *button_states;
    int16_t *axes_states;
  public:
    Joy(int joynum);
    ~Joy();
    bool getRawButton(int button_idx);
    double getRawAxis(int axis_idx);
    int getNumButtons();
    int getNumAxes();
    void pollEvents();
};

#endif
