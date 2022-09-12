#include "DifferentialDrive.h"
#include "SpeedController.h"
#include "Utils.h"
#include <stdio.h>

DifferentialDrive::DifferentialDrive(VMXPi *vmx, int frontLeft, int frontRight, int backLeft, int backRight) {
  this->frontLeft = new SpeedController(vmx, frontLeft);
  this->frontRight = new SpeedController(vmx, frontRight);
  this->backLeft = new SpeedController(vmx, backLeft);
  this->backRight = new SpeedController(vmx, backRight);
  
  this->frontRight->setInverted(true);
  this->backRight->setInverted(true);

  reversed = false;
}

DifferentialDrive::DifferentialDrive(VMXPi *vmx, int left, int right) {
  this->frontLeft = new SpeedController(vmx, left);
  this->frontRight = new SpeedController(vmx, right);
  this->backLeft = NULL;
  this->backRight = NULL;

  this->frontRight->setInverted(true);

  reversed = false;
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

  if(reversed)
    moveRequest = -1 * moveRequest;

  double left = (moveRequest + turnRequest) * speedLimiter;
  double right = (moveRequest - turnRequest) * speedLimiter;

  if(frontLeft)
    frontLeft->setSpeed(left);
  if(backLeft)
    backLeft->setSpeed(left);
  if(frontRight)
    frontRight->setSpeed(right);
  if(backRight)
    backRight->setSpeed(right);
}

void DifferentialDrive::stop() {
  printf("Stopping motors!\n");
  if(frontLeft)
    frontLeft->setSpeed(0);
  if(backLeft)
    backLeft->setSpeed(0);
  if(frontRight)
    frontRight->setSpeed(0);
  if(backRight)
    backRight->setSpeed(0);
}

void DifferentialDrive::setReversed(bool reversed) {
  this->reversed = reversed;
}
