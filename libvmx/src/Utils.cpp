#include "Utils.h"

#include <math.h>

double Utils::clip(double in, double min, double max) {
  if(in < min) return min;
  if(in > max) return max;
  return in;
}

double Utils::curve(double in, double curve) {
  if (curve == 0)
    return in;
  double powed = pow(abs(in), curve);
  if(in > 0)
    return powed;
  else
    return -powed;
}
double Utils::map(double in, double inmin, double inmax, double outmin, double outmax) {
  return ( ( (in - inmin) / (inmax - inmin) ) * (outmax - outmin) ) + outmin;
}
double Utils::deadzone(double in, double deadzone) {
  deadzone = abs(deadzone);
  if(in < -deadzone)
    return Utils::map(in, -1, -deadzone, -1, 0);
  else if(in > deadzone)
    return Utils::map(in, deadzone, 1, 0, 1);
  else
    return 0;
}
