#ifndef _UTILS_H
#define _UTILS_H
class Utils {
  public:
    static double clip(double in, double min, double max);
    static double curve(double in, double curve);
    static double map(double in, double inmin, double inmax, double outmin, double outmax);
    static double deadzone(double in, double deadzone);
};

#endif
