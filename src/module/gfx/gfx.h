#pragma once

#include "module/singleton.h"
#include <U8g2_for_Adafruit_GFX.h>
#include <epd/GxEPD2_213_BN.h>
#include <GxEPD2_BW.h>

class GFX : public Singleton<GFX> {
public:
    typedef GxEPD2_BW<GxEPD2_213_BN, GxEPD2_213_BN::HEIGHT> DisplayType;

    GFX();

    ~GFX();

    void beginFrame();

    bool endFrame();

    U8G2_FOR_ADAFRUIT_GFX& u8g2();

    DisplayType& display();

    int16_t screenWidth() const;

    int16_t screenHeight() const;

    int8_t lineHeight() const;

    void drawBitmap(const Core::U8* bitmap, Core::S16 x, Core::S16 y, Core::S16 w, Core::S16 h, Core::F32 scale = 1);

    void poweroff();

private:
    U8G2_FOR_ADAFRUIT_GFX* _u8g2;
    DisplayType* _display;
};
