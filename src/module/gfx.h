#pragma once

#include "fundamental/singleton.h"
#include "U8g2_for_Adafruit_GFX.h"
#include "epd/GxEPD2_213_BN.h"
#include "GxEPD2_BW.h"

class GFX : public Singleton<GFX> {
 public:
  typedef GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> DisplayType;
  GFX();
  ~GFX();

  void BeginFrame();
  bool EndFrame();

  U8G2_FOR_ADAFRUIT_GFX &GetU8G2();
  DisplayType& GetDisplay();
 private:
  U8G2_FOR_ADAFRUIT_GFX *u8g2;
  DisplayType* display;
};
