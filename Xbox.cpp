#include "Xbox.h"
#include "Exceptions.h"

Xbox::Xbox(int portnum) : Joy(portnum) {}

double Xbox::getX(Hand hand) {
  if(hand == RIGHT) {
    return this->getRawAxis(3);
  } else if(hand == LEFT) {
    return this->getRawAxis(0);
  } else {
    throw E_XBOX_INVALID_HAND;
  }
}
double Xbox::getY(Hand hand) {
  if(hand == RIGHT) {
    return -1 * this->getRawAxis(4);
  } else if (hand == LEFT) {
    return -1 * this->getRawAxis(1);
  } else {
    throw E_XBOX_INVALID_HAND;
  }
}
double Xbox::getTrigger(Hand hand) {
  if (hand == RIGHT) {
    return (this->getRawAxis(5) + 1) / 2;
  } else if(hand == LEFT) {
    return (this->getRawAxis(2) + 1) / 2;
  } else {
      throw E_XBOX_INVALID_HAND;
  }
}

int Xbox::getPOV() {
  int x = this->getRawAxis(6);
  int y = -1 * this->getRawAxis(7);

  if(y == 0 && x == 0)
    return 0;
  if(y == 1 && x == 1)
    return 45;
  if(y == 0 && x == 1)
    return 90;
  if(y == -1 && x == 1)
    return 135;
  if(y == -1 && x == 0)
    return 180;
  if(y == -1 && x == -1)
    return 225;
  if(y == 0 && x == -1)
    return 270;
  if(y == 1 && x == -1)
    return 315;
  if(y == 1 && x == 0)
    return 360;
  
  throw E_XBOX_INVALID_POV;

  return -1;
}

bool Xbox::getBumper(Hand hand) {
  if(hand == RIGHT) {
    return this->getRawButton(5);
  } else if(hand == LEFT) {
    return this->getRawButton(4);
  } else {
    throw E_XBOX_INVALID_HAND;
  }
}

bool Xbox::getStickPressed(Hand hand) {
  if(hand == RIGHT) {
    return this->getRawButton(10);
  } else if(hand == LEFT) {
    return this->getRawButton(9);
  } else {
    throw E_XBOX_INVALID_HAND;
  }
}

bool Xbox::getA() {
  return this->getRawButton(0);
}

bool Xbox::getB() {
  return this->getRawButton(1);
}

bool Xbox::getX() {
  return this->getRawButton(2);
}

bool Xbox::getY() {
  return this->getRawButton(3);
}

bool Xbox::getBack() {
  return this->getRawButton(6);
}
bool Xbox::getStart() {
  return this->getRawButton(7);
}
bool Xbox::getCenterButton() {
  return this->getRawButton(8);
}
