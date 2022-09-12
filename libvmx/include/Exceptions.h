#pragma once

#include <stdexcept>

class JoystickNotPresentError : public std::runtime_error {
public:
  JoystickNotPresentError()
    : std::runtime_error("Joystick not present")
    {}
};

class ValueError : public std::runtime_error {
public:
  explicit ValueError(const char * message)
    : std::runtime_error(message)
    {}
};

class MisconfiguredHardwareError : public std::runtime_error {
public:
  explicit MisconfiguredHardwareError(const char* message)
    : std::runtime_error(message)
  {}
};

