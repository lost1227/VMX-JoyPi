#include "SpeedController.h"
#include "Exceptions.h"

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
    throw E_SPEED_DIO_RANGE;
  }

  output_port = first_hicurrdio_channel + port;
  if(!vmx->io.ChannelSupportsCapability(output_port, VMXChannelCapability::DigitalOutput)) {
    fprintf(stderr, "HighCurrentDIO port %d is set to output, check jumper setting\n", output_port);
    throw E_SPEED_DIO_JUMPER;
  }

  PWMGeneratorConfig pwmgen_cfg(PWM_FREQ);

  if(!vmx->io.ActivateSinglechannelResource( VMXChannelInfo(output_port, VMXChannelCapability::PWMGeneratorOutput), &pwmgen_cfg, pwm_generator, &vmxerr)) {
    fprintf(stderr, "Error Activating Singlechannel Resource PWM Generator for Channel index %d.\n", output_port);
    throw E_SPEED_PWM_GENERATOR_ACTIVATION;
  }

  if(!vmx->io.PWMGenerator_SetDutyCycle(pwm_generator, 0, STOPPED_DUTY_CYCLE, &vmxerr)) {
    fprintf(stderr, "Failed to set DutyCycle for PWMGenerator Resource");
    throw E_SPEED_PWM_SET_DUTY_CYCLE;
  }

}

void SpeedController::setSpeed(double speed) {
  VMXErrorCode vmxerr;
  if(speed > 1)
    speed = 1;
  if (speed < -1)
    speed = -1;
  uint8_t duty_cycle = ((speed + 1) / 2) * 255;
  if(!vmx->io.PWMGenerator_SetDutyCycle(pwm_generator, 0, duty_cycle, &vmxerr)) {
    printf("Failed to set DutyCyclDutyCycle for PWMGenerator Resource");
    throw E_SPEED_PWM_SET_DUTY_CYCLE;
  }
}