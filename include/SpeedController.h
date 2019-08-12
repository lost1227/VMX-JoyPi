#ifndef _SPEED_CONTROLLER_H
#define _SPEED_CONTROLLER_H

#include "VMXPi.h"

#define PWM_FREQ 200 //hz
#define STOPPED_DUTY_CYCLE 128 // out of 255

class SpeedController {
  private:
    VMXPi *vmx;
    VMXResourceHandle pwm_generator;
    VMXChannelIndex output_port;
  public:
    SpeedController(VMXPi *vmx, int port);
    ~SpeedController();
    void setSpeed(double speed);
};

#endif
