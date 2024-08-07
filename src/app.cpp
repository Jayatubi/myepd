#include "app.h"
#include "module/gfx/gfx.h"
#include "module/network/network.h"
#include "module/clock/clock.h"
#include "module/timer/timer.h"
#include "module/battery/battery.h"
#include "module/weather/weather.h"
#include "icons/icon_wifi.h"
#include "icons/icon_battery.h"

static const int statusIconSize = 16;

void App::Bootstrap() {
    Application::Bootstrap();

    setupBattery();
    setupNetwork();
    setupClock();
    setupWeather();
}

void App::setupNetwork() {
    Event::instance().Listen(Core::Bind([this](const Event_NetworkStateChange& event) {
        invalidate("network");
    }));
}

void App::setupClock() {
    Core::U32 ticket = 0;
    Event::instance().Listen(Core::Bind([this, ticket](const Event_ClockStateChange& event) mutable {
        if (event.state == Clock::synced) {
            auto& timer = Timer::instance();
            timer.setTimeout((60 - Clock::instance().getSeconds()) * 1000, [this, ticket]() mutable {
                ticket = Timer::instance().setInterval(60_m, [this]() {
                    invalidate("time");
                });
                invalidate("time");
            });
        } else {
            Timer::instance().clear(ticket);
        }
        invalidate("time");
    }));
}

void App::setupWeather() {
    Event::instance().Listen(Core::Bind([this](const Event_WeatherChange& event) mutable {
        invalidate("weather");
    }));
}

void App::setupBattery() {
    Event::instance().Listen(Core::Bind([this](const Event_BatteryLevelChange& event) {
        invalidate("battery");
    }));
}

void App::repaint() {
    if (with_flag("borders")) {
        auto& gfx = GFX::instance();
        repaintInRegion(0, 0, gfx.screenWidth(), gfx.screenHeight(),
                        [](GFX& gfx) {
                            for (int y = gfx.screenHeight() - 1; y >= 0; y -= 8) {
                                gfx.display().drawLine(0, y, gfx.screenWidth(), y, GxEPD_BLACK);
                            }
                        });
    }
    if (with_flag("network") || with_flag("battery")) {
        repaintStatusbar();
    }
    if (with_flag("weather")) {
        repaintWeather();
    }
    if (with_flag("time")) {
        repaintTime();
    }
}

void App::repaintInRegion(
    uint16_t left, uint16_t top, uint16_t right, uint16_t bottom,
    const Core::Function<void, GFX&>& callback) {

    auto& gfx = GFX::instance();

    auto& display = gfx.display();
    auto& u8g2 = gfx.u8g2();

    display.setPartialWindow(left, top, right - left, bottom - top);

    gfx.beginFrame();
    {
        u8g2.setCursor(left, top + gfx.lineHeight());
        callback(gfx);
    }
    gfx.endFrame();
}

void App::repaintStatusbar() {
    auto& gfx = GFX::instance();
    auto left = gfx.screenWidth() / 2;
    auto top = 0;
    auto right = gfx.screenWidth();
    auto bottom = statusIconSize;

    repaintInRegion(left, top, right, bottom,
                    [&](GFX& gfx) {
                        int px = right - statusIconSize;

                        auto nextIcon = [&](const Core::U8* bitmap) {
                            gfx.drawBitmap(bitmap, px, top, statusIconSize, statusIconSize);
                            px -= statusIconSize;
                        };

                        nextIcon(getBatteryIcon(Battery::instance().level()));

                        auto& network = Network::instance();
                        if (network.state() == Network::State::connected) {
                            nextIcon(Icon_WiFi);
                        }
                    });
}

void App::repaintTime() {
    auto& gfx = GFX::instance();
    auto left = 0;
    auto top = gfx.screenHeight() - 1 - gfx.lineHeight();
    auto right = gfx.screenWidth();
    auto bottom = gfx.screenHeight() - 1;

    repaintInRegion(left, top, right, bottom,
                    [&](GFX& gfx) {
                        auto& clock = Clock::instance();
                        if (clock.state() == Clock::State::synced) {
                            static const char* weekDays[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
                            static char buffer[0xFF] = {0};
                            snprintf(buffer, sizeof(buffer), "%04d/%02d/%02d %s %02d:%02d\n",
                                     clock.getYear(), clock.getMonth() + 1, clock.getDate(), weekDays[clock.getDay()],
                                     clock.getHours(), clock.getMinutes());
                            auto& u8g2 = gfx.u8g2();
                            u8g2.setCursor(right - u8g2.getUTF8Width(buffer), bottom);
                            u8g2.print(buffer);
                        }
                    });
}

void App::repaintWeather() {
    auto& gfx = GFX::instance();
    auto left = 0;
    auto top = 30;
    auto right = gfx.screenWidth();
    auto bottom = gfx.screenHeight() - 1 - gfx.lineHeight();

    repaintInRegion(left, top, right, bottom,
                    [&](GFX& gfx) {
                        auto& u8g2 = gfx.u8g2();
                        u8g2.print("Weather is coming soon...");
                    });

    auto& weather = Weather::instance();
    if (weather.state() == Weather::State::fetched) {
        repaintInRegion(0, 0, gfx.screenWidth() / 2, statusIconSize,
                        [&](GFX& gfx) {
                            auto& u8g2 = gfx.u8g2();
                            u8g2.setCursor(0, statusIconSize);
                            gfx.u8g2().printf("%s %d℃", weather.city().c_str(), weather.now().temperature);
                        });
    }
}
