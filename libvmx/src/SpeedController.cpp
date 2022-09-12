#include "Exceptions.h"
#include "SpeedController.h"
#include "Utils.h"

#include <stdint.h>
#include <stdio.h>

SpeedController::SpeedController(VMXPi *vmx, int port) {
  VMXErrorCode vmxerr;
  this->vmx = vmx;
  VMXChannelIndex first_hicurrdio_channel;
  uint8_t num_hicurrdio_channels;
  num_hicurrdio_channels = vmx->io.GetNumChannelsByType(VMXChannelType::HiCurrDIO, first_hicurrdio_channel);

  if(port > num_hicurrdio_channels) {
    fprintf(stderr, "HighCurrentDIO port %d is out of range\n", port);
    throw ValueError("Requested HighCurrentDIO port is out-of-range");
  }

  output_port = first_hicurrdio_channel + port;
  if(!vmx->io.ChannelSupportsCapability(output_port, VMXChannelCapability::DigitalOutput)) {
    fprintf(stderr, "HighCurrentDIO port %d is set to input, check jumper setting\n", output_port);
    throw MisconfiguredHardwareError("Requested HighCurrentDIO port is set to input");
  }

  PWMGeneratorConfig pwmgen_cfg(PWM_FREQ);

  if(!vmx->io.ActivateSinglechannelResource( VMXChannelInfo(output_port, VMXChannelCapability::PWMGeneratorOutput), &pwmgen_cfg, pwm_generator, &vmxerr)) {
    fprintf(stderr, "Error Activating Singlechannel Resource PWM Generator for Channel index %d.\n", output_port);
    throw std::runtime_error("Unknown VMX error when activating Singlechannel PWMGeneratorOutput resource");
  }

  if(!vmx->io.PWMGenerator_SetDutyCycle(pwm_generator, 0, STOPPED_DUTY_CYCLE, &vmxerr)) {
    fprintf(stderr, "Failed to set DutyCycle for PWMGenerator Resource");
    throw std::runtime_error("Unknown VMX error when setting PWMGenerator DutyCycle");
  }
  inverted = false;
}

SpeedController::~SpeedController() {
  VMXErrorCode vmxerr;
  if(!vmx->io.DeactivateResource(pwm_generator, &vmxerr)) {
    printf("Error deactivating pwm resource: %s", GetVMXErrorString(vmxerr));
  }
  if(!vmx->io.DeallocateResource(pwm_generator, &vmxerr)) {
    printf("Error deallocating pwm resource: %s", GetVMXErrorString(vmxerr));
  }
}

void SpeedController::setSpeed(double speed) {
  VMXErrorCode vmxerr;
  if(inverted)
    speed = -1 * speed;
  speed = Utils::clip(speed, -1, 1);

  double min_duty_cycle = (1/(1000.0/PWM_FREQ)) * 255;
  double max_duty_cycle = (2/(1000.0/PWM_FREQ)) * 255;

  uint8_t duty_cycle = Utils::map(speed, -1, 1, min_duty_cycle, max_duty_cycle);
  if(!vmx->io.PWMGenerator_SetDutyCycle(pwm_generator, 0, duty_cycle, &vmxerr)) {
    fprintf(stderr, "Failed to set DutyCyclDutyCycle for PWMGenerator Resource\n");
    throw std::runtime_error("Unknown VMX error when setting PWMGenerator DutyCycle");
  }
}

void SpeedController::setInverted(bool inverted) {
  this->inverted = inverted;
}
