#include "SpikeRelay.h"

#include "Exceptions.h"

#include <stdint.h>
#include <stdio.h>

SpikeRelay::SpikeRelay(VMXPi *vmx, int fwd_port, int rev_port) {
  this->vmx = vmx;

  VMXErrorCode vmxerr;
  VMXChannelIndex first_hicurrdio_channel;
  uint8_t num_hicurrdio_channels;
  num_hicurrdio_channels = vmx->io.GetNumChannelsByType(VMXChannelType::HiCurrDIO, first_hicurrdio_channel);

  if(fwd_port > num_hicurrdio_channels) {
    fprintf(stderr, "HighCurrentDIO port %d is out of range\n", fwd_port);
    throw ValueError("Requested HighCurrentDIO port is out-of-range");
  }

  if(rev_port > num_hicurrdio_channels) {
    fprintf(stderr, "HighCurrentDIO port %d is out of range\n", rev_port);
    throw ValueError("Requested HighCurrentDIO port is out-of-range");
  }

  fport = first_hicurrdio_channel + fwd_port;
  if(!vmx->io.ChannelSupportsCapability(fport, VMXChannelCapability::DigitalOutput)) {
    fprintf(stderr, "HighCurrentDIO port %d is set to input, check jumper setting\n", fport);
    throw MisconfiguredHardwareError("Requested HighCurrentDIO port is set to input");
  }

  rport = first_hicurrdio_channel + rev_port;
  if(!vmx->io.ChannelSupportsCapability(rport, VMXChannelCapability::DigitalOutput)) {
    fprintf(stderr, "HighCurrentDIO port %d is set to input, check jumper setting\n", fport);
    throw MisconfiguredHardwareError("Requested HighCurrentDIO port is set to input");
  }

  DIOConfig dio_config_f(DIOConfig::OutputMode::PUSHPULL);
  DIOConfig dio_config_r(DIOConfig::OutputMode::PUSHPULL);

  if(!vmx->io.ActivateSinglechannelResource(VMXChannelInfo(fport, VMXChannelCapability::DigitalOutput), &dio_config_f, fport_dio_res, &vmxerr)) {
    fprintf(stderr, "Error Activating Singlechannel Resource DigitalOutput for Channel index %d.\n", fport);
    throw std::runtime_error("Unknown VMX error when activating Singlechannel DigitialOutput resource");
  }

  if(!vmx->io.ActivateSinglechannelResource(VMXChannelInfo(rport, VMXChannelCapability::DigitalOutput), &dio_config_r, rport_dio_res, &vmxerr)) {
    fprintf(stderr, "Error Activating Singlechannel Resource DigitalOutput for Channel index %d.\n", rport);
    throw std::runtime_error("Unknown VMX error when activating Singlechannel DigitialOutput resource");
  }

  this->set(off);
}

void SpikeRelay::set(Value value) {
  VMXErrorCode vmxerr;

  bool forward = (value == SpikeRelay::forward || value == SpikeRelay::on);
  bool reverse = (value == SpikeRelay::reverse || value == SpikeRelay::on);

  if(!vmx->io.DIO_Set(fport_dio_res, forward, &vmxerr)) {
    fprintf(stderr, "Failed to set state for DIO Resource\n");
    throw std::runtime_error("Unknown VMX error when setting DIO State");
  }

  if(!vmx->io.DIO_Set(rport_dio_res, reverse, &vmxerr)) {
    fprintf(stderr, "Failed to set state for DIO Resource\n");
    throw std::runtime_error("Unknown VMX error when setting DIO State");
  }

}
