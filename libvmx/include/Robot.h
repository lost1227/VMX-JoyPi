#pragma once

#include "TimedRobot.h"
#include "Xbox.h"
#include "DifferentialDrive.h"
#include "SpikeRelay.h"
#include "VoltageMonitor.h"

class Robot : public TimedRobot {
  private:
    Xbox *xbox;
    DifferentialDrive *drive;
    SpikeRelay *horn;
    VoltageMonitor *volt;
  public:
    Robot();
    ~Robot();

    void robotPeriodic();
};

