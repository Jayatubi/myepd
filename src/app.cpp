
#include "app.h"
#include "module/gfx/gfx.h"
#include "module/network/network.h"
#include "module/clock/clock.h"
#include "module/timer/timer.h"
#include "module/battery/battery.h"
#include "module/location/location.h"
#include "module/weather/weather.h"
#include "module/console/console.h"
#include "icons/icon_wifi.h"
#include "icons/icon_battery.h"
#include "icons/icon_location.h"
#include "icons/icon_weather.h"
#include "core/basic_type/math.h"

static const int statusIconSize = 16;
static const int weatherIconSize = 48;

App::App()
    : Application(), powerSaving(0) {
}

void App::Bootstrap() {
    Application::Bootstrap();

    setupBattery();
    setupNetwork();
    setupClock();
    setupWeather();

    resetPowerSaving();
}

void App::Tick() {
    Application::Tick();

    if (powerSaving-- <= 0 && Network::instance().state() == Network::State::idle) {
        sleep();
    }
}


void App::resetPowerSaving() {
    powerSaving = 100;
}

void App::setupNetwork() {
    bool connected = false;
    Event::instance().Listen(Core::Bind([=, this](const Event_NetworkStateChange& event) mutable {
        bool newConnected = event.state == Network::State::connected;
        if (connected != newConnected) {
            invalidate("network");
            connected = true;
        }
    }));
}

void App::setupClock() {
    Core::U32 ticket = 0;

    bool synced = false;
    Event::instance().Listen(Core::Bind([=, this](const Event_ClockStateChange& event) mutable {
        bool newSynced = event.state == Clock::synced;
        if (synced != newSynced) {
            if (newSynced) {
                auto& timer = Timer::instance();
                timer.setTimeout((60 - Clock::instance().getSeconds()) * 1000, [this, ticket]() mutable {
                    ticket = Timer::instance().setInterval(1_m, [this]() {
                        invalidate("time");
                    });
                    invalidate("time");
                });
            } else {
                Timer::instance().clear(ticket);
            }
            invalidate("time");
            synced = newSynced;
        }
    }));
}

void App::setupLocation() {
    Event::instance().Listen(Core::Bind([this](const Event_LocationStateChanged& event) {
        invalidate("location");
    }));
}

void App::setupWeather() {
    bool fetched = false;
    Event::instance().Listen(Core::Bind([=, this](const Event_WeatherChange& event) mutable {
        bool newFetched = event.state == Weather::State::fetched;
        if (fetched != newFetched) {
            invalidate("weather");
            fetched = newFetched;
        }
    }));
}

void App::setupBattery() {
    Event::instance().Listen(Core::Bind([this](const Event_BatteryLevelChange& event) {
        invalidate("battery");
    }));
}

void App::repaint() {
    if (with_flag("network") || with_flag("battery") || with_flag("location")) {
        repaintStatusbar();
    }
    if (with_flag("weather")) {
        repaintWeather();
    }
    if (with_flag("time")) {
        repaintTime();
    }

    resetPowerSaving();
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

    repaintInRegion(
        left, top, right, bottom,
        [&](GFX& gfx) {
            int px = right - statusIconSize;

            auto nextIcon = [&](const Core::U8* bitmap) {
                gfx.drawBitmap(bitmap, px, top, statusIconSize, statusIconSize);
                px -= statusIconSize;
            };

            nextIcon(getBatteryIcon(Battery::instance().level()));
            if (Network::instance().state() == Network::State::connected) {
                nextIcon(Icon_WiFi);
            }
            if (Location::instance().state() == Location::State::locating) {
                nextIcon(Icon_Location);
            }
        });
}

void App::repaintTime() {
    auto& gfx = GFX::instance();
    auto left = 0;
    auto top = gfx.screenHeight() - 1 - gfx.lineHeight();
    auto right = gfx.screenWidth();
    auto bottom = gfx.screenHeight() - 1;

    repaintInRegion(
        left, top, right, bottom,
        [&](GFX& gfx) {
            auto& clock = Clock::instance();
            if (clock.state() == Clock::State::synced) {
                static const char* weekDays[] = {"周日", "周一", "周二", "周三", "周四", "周五", "周六"};
                static char buffer[0xFF] = {0};
                snprintf(buffer, sizeof(buffer), "%04d/%02d/%02d %s %02d:%02d",
                         clock.getYear(), clock.getMonth() + 1, clock.getDate(), weekDays[clock.getDay()],
                         clock.getHours(), clock.getMinutes());
                auto& u8g2 = gfx.u8g2();
                u8g2.setCursor(right - u8g2.getUTF8Width(buffer), bottom);
                u8g2.print(buffer);
            }
        });
}

void App::repaintWeather() {
    auto& weather = Weather::instance();
    if (weather.state() == Weather::State::fetched) {
        auto& gfx = GFX::instance();
        auto left = 0;
        auto top = 30;
        auto right = gfx.screenWidth();
        auto bottom = gfx.screenHeight() - 1 - gfx.lineHeight();

        repaintInRegion(
            left, top, right, bottom,
            [&](GFX& gfx) {
                const auto& dailies = Weather::instance().dailies();
                const Core::S32 gridSize = gfx.screenWidth() / dailies.size();
                const Core::F32 iconScale = 0.75f;
                const Core::S32 scaledIconSize = weatherIconSize * iconScale;
                const Core::S32 temperatureWidth = 20;

                int px = 0;

                auto nextIcon = [&](const Weather::Daily& daily) {
                    String weatherText = daily.dayText != daily.nightText ? daily.dayText + "转" + daily.nightText : daily.dayText;
                    const auto hour = Clock::instance().getHours();
                    bool daylight = Core::inRange(hour, 6, 18);
                    auto code = daylight ? daily.dayCode : daily.nightCode;

                    gfx.drawBitmap(getWeatherIcon(code),
                                   px + (gridSize - temperatureWidth - scaledIconSize) / 2,
                                   top,
                                   weatherIconSize, weatherIconSize, iconScale);

                    auto lineHeight = gfx.lineHeight();
                    auto tx = px + gridSize - temperatureWidth;
                    auto ty = top;
                    auto tw = temperatureWidth;
                    auto th = scaledIconSize;
                    gfx.alignText(tx, ty, tw, th, GFX::Center, GFX::Top, String(daily.high) + "°");
                    gfx.alignText(tx, ty, tw, th, GFX::Center, GFX::Bottom, String(daily.low) + "°");
                    gfx.display().drawLine(
                        tx + tw, ty + th / 2,
                        tx, ty + th / 2,
                        GxEPD_BLACK);

                    static char buffer[0xFF] = {0};
                    snprintf(buffer, sizeof(buffer), "%02d/%02d", daily.timeinfo.tm_mon + 1, daily.timeinfo.tm_mday);
                    gfx.alignText(px, top + scaledIconSize, gridSize, lineHeight * 2, GFX::Center, GFX::Bottom, buffer);

                    px += gridSize;
                };

                for (const auto& daily : dailies) {
                    nextIcon(daily);
                }
            });

        repaintInRegion(
            0, 0, gfx.screenWidth() / 2, statusIconSize,
            [&](GFX& gfx) {
                auto& u8g2 = gfx.u8g2();
                u8g2.setCursor(0, statusIconSize);
                gfx.u8g2().printf(
                    "%s %d° 湿度 %d%%",
                    weather.city().c_str(), weather.now().temperature,
                    weather.dailies()[0].humidity
                );
            });
    }
}

void App::sleep() {
    pinMode(GPIO_NUM_12, INPUT_PULLUP);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, LOW);
    esp_sleep_enable_timer_wakeup(30_s * 1000);
    esp_light_sleep_start();
//    esp_deep_sleep_start();
    resetPowerSaving();
}
