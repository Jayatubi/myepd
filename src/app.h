#pragma once

#include "core/fundamental/function.h"
#include "module/gfx/gfx.h"
#include "module/application.h"

class App : public Application {
public:
    App();
    void Bootstrap() override;
    void Tick() override;
protected:
    void repaint() override;

private:
    void setupNetwork();

    void setupClock();

    void setupBattery();

    void setupWeather();

private:
    void repaintInRegion(
        uint16_t left, uint16_t top, uint16_t right, uint16_t bottom,
        const Core::Function<void, GFX&>& callback);

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
    void alignText(Core::S32 x, Core::S32 y, Core::S32 w, Core::S32 h, App::HAlign hAlign, VAlign vAlign, const String& text);

    void repaintStatusbar();

    void repaintTime();

    void repaintWeather();

    void sleep();

    void resetPowerSaving();
private:
    Core::S32 powerSaving;
};
