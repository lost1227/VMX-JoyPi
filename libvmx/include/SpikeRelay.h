#pragma once

#include "VMXPi.h"

/**
 * See https://web.archive.org/web/20030419143006/http://www.innovationfirst.com/FirstRobotics/pdfs/SpikeBLUEUsersManual.pdf
 */
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

