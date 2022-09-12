#pragma once

#include "VMXPi.h"

#define PERIODIC_RATE_HZ 50

class TimedRobot {
  protected:
    VMXPi *vmx;
  public:
    TimedRobot();

    virtual ~TimedRobot();

    void start();

    virtual void robotPeriodic() = 0;
};

