#include "VoltageMonitor.h"

#include "Exceptions.h"

#include <stdio.h>

VoltageMonitor::VoltageMonitor(VMXPi *vmx) {
  this->vmx = vmx;
  lasttime = vmx->time.GetCurrentMicroseconds();
}

bool VoltageMonitor::check_voltage() {
  VMXErrorCode vmxerr;
  float voltage = 0;

  if(!vmx->power.GetSystemVoltage(voltage, &vmxerr)) {
    fprintf(stderr, "Could not read voltage\n");
    throw E_VOLT_READ;
  }

  if(voltage < CUTOFF_VOLTAGE) {
    return vmx->time.GetCurrentMicroseconds() - lasttime > CUTOFF_TIME_MS;
  } else {
    lasttime = vmx->time.GetCurrentMicroseconds();
    return false;
  }
}