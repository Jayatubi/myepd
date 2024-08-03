#pragma once

#include "fundamental/singleton.h"
#include <Arduino.h>

class Console : public Singleton<Console> {
  typedef decltype(Serial) LowlevelType;
 public:
  Console();
  ~Console();
  LowlevelType &GetDevice();
};
