#include "TimedRobot.h"
#include "Exceptions.h"

#include <stdio.h>
#include <stdint.h>

void timer_notify_handler(void *param, uint64_t timestamp_us) {
  TimedRobot *robot = (TimedRobot *)param;
  robot->robotPeriodic();
}

TimedRobot::TimedRobot() {
  VMXErrorCode vmxerr;
  vmx = new VMXPi(false, PERIODIC_RATE_HZ);
  if(!vmx->IsOpen()) {
    throw std::runtime_error("VMX is not open");
  }
  if(!vmx->io.DeallocateAllResources(&vmxerr)) {
    printf("Error deallocating all resources: %s\n", GetVMXErrorString(vmxerr));
  }
}

TimedRobot::~TimedRobot() {
  delete vmx;
}

void TimedRobot::start() {
  this->vmx->time.RegisterTimerNotificationRelative(timer_notify_handler, 1000000/PERIODIC_RATE_HZ, (void *)this, true);
}
