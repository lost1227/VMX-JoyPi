#ifndef _ROBOT_H
#define _ROBOT_H

#include "TimedRobot.h"
#include "Xbox.h"
#include "DifferentialDrive.h"

class Robot : public TimedRobot {
  private:
    Xbox *xbox;
    DifferentialDrive *drive;
  public:
    Robot();
    ~Robot();

    void robotPeriodic();
};

#endif