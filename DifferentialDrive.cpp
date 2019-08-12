#include "DifferentialDrive.h"
#include "SpeedController.h"
#include "Utils.h"
#include <stdio.h>

DifferentialDrive::DifferentialDrive(VMXPi *vmx, int frontLeft, int frontRight, int backLeft, int backRight) {
  this->frontLeft = new SpeedController(vmx, frontLeft);
  this->frontRight = new SpeedController(vmx, frontRight);
  this->backLeft = new SpeedController(vmx, backLeft);
  this->backRight = new SpeedController(vmx, backRight);
}

DifferentialDrive::~DifferentialDrive() {
  this->stop();
  delete frontLeft;
  delete frontRight;
  delete backLeft;
  delete backRight;
}

void DifferentialDrive::arcadeDrive(double moveRequest, double turnRequest, double speedLimiter) {
  moveRequest = Utils::clip(moveRequest, -1, 1);
  turnRequest = Utils::clip(turnRequest, -1, 1);
  speedLimiter = Utils::clip(speedLimiter, 0, 1);

  double left = (moveRequest + turnRequest) * speedLimiter;
  double right = (moveRequest - turnRequest) * speedLimiter;

  frontLeft->setSpeed(left);
  backLeft->setSpeed(left);
  frontRight->setSpeed(right);
  backRight->setSpeed(right);
}

void DifferentialDrive::stop() {
  printf("Stopping motors!\n");
  frontLeft->setSpeed(0);
  backLeft->setSpeed(0);
  frontRight->setSpeed(0);
  backRight->setSpeed(0);
}
