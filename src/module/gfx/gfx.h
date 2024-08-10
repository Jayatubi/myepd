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

    void poweroff();

    void beginFrame();

    bool endFrame();

    U8G2_FOR_ADAFRUIT_GFX& u8g2();

    DisplayType& display();

    Core::S32 screenWidth() const;

    Core::S32 screenHeight() const;

    Core::S32 lineHeight() const;

    void drawBitmap(const Core::U8* bitmap, Core::S16 x, Core::S16 y, Core::S16 w, Core::S16 h, Core::F32 scale = 1);


    enum HAlign {
        Left,
        Center,
        Right
    };
    enum VAlign {
        Top,
        Middle,
        Bottom
    };
    void alignText(Core::S32 x, Core::S32 y, Core::S32 w, Core::S32 h, HAlign hAlign, VAlign vAlign, const String& text);

private:
    U8G2_FOR_ADAFRUIT_GFX* _u8g2;
    DisplayType* _display;
};
