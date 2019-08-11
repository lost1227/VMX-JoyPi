#include "TimedRobot.h"
#include "Exceptions.h"

#include <stdio.h>
#include <stdint.h>

void timer_notify_handler(void *param, uint64_t timestamp_us) {
  TimedRobot *robot = (TimedRobot *)param;
  robot->robotPeriodic();
}

TimedRobot::TimedRobot() {
  vmx = new VMXPi(PERIODIC_RATE_HZ, false);
  if(!vmx->IsOpen()) {
    throw E_VMX_NOT_OPEN;
  }
}

TimedRobot::~TimedRobot() {
  delete vmx;
}

void TimedRobot::start() {
  this->vmx->time.RegisterTimerNotificationRelative(timer_notify_handler, 1000000/PERIODIC_RATE_HZ, (void *)this, true);
}
