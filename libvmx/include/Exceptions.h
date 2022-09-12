#pragma once

#include <stdexcept>
#include <VMXErrors.h>
#include <cstring>

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

class VMXError : public std::exception {
public:
  explicit VMXError(VMXErrorCode code)
  {
    const char* prefix = "VMX Error: ";
    int prefix_len = strlen(prefix);

    strcpy(message, prefix);
    strncpy(message + prefix_len, GetVMXErrorString(code), sizeof(message) - prefix_len-1);
    message[sizeof(message) - 1] = '\0';
  }

  virtual const char* what() const noexcept override {
    return message;
  }
private:
  char message[256];
};
