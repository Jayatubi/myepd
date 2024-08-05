#pragma once

#include "core/fundamental/function.h"
#include "module/gfx/gfx.h"
#include "module/application.h"

class App : public Application {
public:
    void Bootstrap() override;

protected:
    void repaint() override;

private:
    void setupNetwork();

    void setupClock();

    void setupBattery();

    void setupWeather();

    void repaintInRegion(
        uint16_t left, uint16_t top, uint16_t right, uint16_t bottom,
        const Core::Function<void, GFX&>& callback);

    void repaintStatusbar();

    void repaintTime();

    void repaintWeather();
};
