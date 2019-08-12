#ifndef _DIFFERENTIAL_DRIVE_H
#define _DIFFERENTIAL_DRIVE_H

#include "VMXPi.h"
#include "SpeedController.h"

class DifferentialDrive {
  private:
    bool reversed;
    SpeedController *frontLeft;
    SpeedController *frontRight;
    SpeedController *backLeft;
    SpeedController *backRight;
  public:
    DifferentialDrive(VMXPi *vmx, int frontLeft, int frontRight, int backLeft, int backRight);
    ~DifferentialDrive();

    void arcadeDrive(double moveRequest, double turnRequest, double speedLimiter);
    void stop();
    void setReversed(bool reversed);
};

#endif
