#ifndef _SPIKE_RELAY_H
#define _SPIKE_RELAY_H

#include "VMXPi.h"

class SpikeRelay {
  private:
    VMXPi *vmx;
    VMXChannelIndex fport, rport;
    VMXResourceHandle fport_dio_res, rport_dio_res;
  public:
    enum Value {off, forward, reverse, on};
    SpikeRelay(VMXPi *vmx, int fwd_port, int rev_port);
    void set(Value value);
};

#endif