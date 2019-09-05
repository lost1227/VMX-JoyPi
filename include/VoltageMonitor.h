#ifndef _VOLTAGE_MONITOR_H
#define _VOLTAGE_MONITOR_H

#include <stdint.h>

#include "VMXPi.h"

#define CUTOFF_VOLTAGE 8
#define CUTOFF_TIME_MS 0

class VoltageMonitor {
  private:
    VMXPi *vmx;
    uint32_t lasttime;
  public:
    VoltageMonitor(VMXPi *vmx);
    bool check_voltage();
};

#endif