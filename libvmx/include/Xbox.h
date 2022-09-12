#pragma once

#include "Joy.h"

enum Hand {RIGHT, LEFT};

class Xbox : public Joy {
  public:
    Xbox(int joynum);

    double getX(Hand hand);
    double getY(Hand hand);
    double getTrigger(Hand hand);

    int getPOV();

    bool getBumper(Hand hand);
    bool getStickPressed(Hand hand);
    bool getA();
    bool getB();
    bool getX();
    bool getY();
    bool getBack();
    bool getStart();
    bool getCenterButton();
};

