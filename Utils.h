#ifndef _UTILS_H
#define _UTILS_H

double clip(double in, double min, double max);
double curve(double in, double curve);
double map(double in, double inmin, double inmax, double outmin, double outmax);
double deadzone(double in, double deadzone);

#endif