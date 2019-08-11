#include "Utils.h"

#include <math.h>

double clip(double in, double min, double max) {
  if(in < min) return min;
  if(in > max) return max;
  return in;
}

double curve(double in, double curve) {
  if (curve == 0)
    return in;
  double powed = pow(abs(in), curve);
  if(in > 0)
    return powed;
  else
    return -powed;
}
double map(double in, double inmin, double inmax, double outmin, double outmax) {
  return ( ( (in - inmin) / (inmax - inmin) ) * (outmax - outmin) ) + outmin;
}
double deadzone(double in, double deadzone) {
  deadzone = abs(deadzone);
  if(in < -deadzone)
    return map(in, -1, -deadzone, -1, 0);
  else if(in > deadzone)
    return map(in, deadzone, 1, 0, 1);
  else
    return 0;
}