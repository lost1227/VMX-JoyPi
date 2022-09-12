#include "Watchdog.h"

#include "Exceptions.h"

#include <VMXPi.h>
#include <VMXIO.h>

Watchdog Watchdog::m_instance;

void Watchdog::feed_watchdog() {
  m_instance.do_feed_watchdog();
}

void Watchdog::do_feed_watchdog() {
  VMXPi *vmx = VMXPi::getInstance();
  if(vmx == nullptr) {
    return;
  }

  if(!vmx->IsOpen()) {
    return;
  }

  VMXIO &io = vmx->getIO();

  VMXErrorCode error_code;

  if(!did_setup) {
    if(!io.SetWatchdogManagedOutputs(true, true, false, &error_code)) {
      throw VMXError(error_code);
    }

    if(!io.SetWatchdogTimeoutPeriodMS(WATCHDOG_TIMEOUT_MS, &error_code)) {
      throw VMXError(error_code);
    }

    if(!io.SetWatchdogEnabled(true, &error_code)) {
      throw VMXError(error_code);
    }

    did_setup = true;
  }

  if(!io.FeedWatchdog(&error_code)) {
    throw VMXError(error_code);
  }
}
