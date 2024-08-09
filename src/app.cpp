
#include "app.h"
#include "module/gfx/gfx.h"
#include "module/network/network.h"
#include "module/clock/clock.h"
#include "module/timer/timer.h"
#include "module/battery/battery.h"
#include "module/weather/weather.h"
#include "module/console/console.h"
#include "icons/icon_wifi.h"
#include "icons/icon_battery.h"
#include "icons/icon_weather.h"

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
        Console::instance().printf("sleep\n");
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
//    if (with_flag("borders")) {
//        auto& gfx = GFX::instance();
//        repaintInRegion(0, 0, gfx.screenWidth(), gfx.screenHeight(),
//                        [](GFX& gfx) {
//                            for (int y = gfx.screenHeight() - 1; y >= 0; y -= 8) {
//                                gfx.display().drawLine(0, y, gfx.screenWidth(), y, GxEPD_BLACK);
//                            }
//                        });
//    }
    if (with_flag("network") || with_flag("battery")) {
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

    repaintInRegion(
        left, top, right, bottom,
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
                const Core::S32 temperatureWidth = 30;

                int px = 0;

                auto nextIcon = [&](const Weather::Daily& daily) {
                    String weatherText = daily.dayText != daily.nightText ? daily.dayText + "转" + daily.nightText : daily.dayText;
                    // TODO: based on real time
                    bool daylight = true;
                    auto code = daylight ? daily.dayCode : daily.nightCode;

                    gfx.drawBitmap(getWeatherIcon(code),
                                   px + (gridSize - temperatureWidth - scaledIconSize) / 2,
                                   top,
                                   weatherIconSize, weatherIconSize, iconScale);

                    auto lineHeight = gfx.lineHeight();
                    auto tx = px + gridSize - temperatureWidth;
                    auto ty = top + (scaledIconSize - temperatureWidth) / 2;
                    auto tw = temperatureWidth;
                    auto th = temperatureWidth;
                    alignText(tx, ty, tw, th, Right, Top, String(daily.high) + "°");
                    alignText(tx, ty, tw, th, Left, Bottom, String(daily.low) + "°");
                    gfx.display().drawLine(
                        tx + tw, ty + th / 2,
                        tx, ty + th / 2,
                        GxEPD_BLACK);

                    alignText(px, top + scaledIconSize, gridSize, lineHeight * 2, Center, Bottom, String(daily.timeinfo.tm_mon + 1) + "月" + String(daily.timeinfo.tm_mday) + "日");

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
//    pinMode(GPIO_NUM_12, INPUT_PULLUP);
//    esp_sleep_enable_ext0_wakeup(GPIO_NUM_12, LOW);
//    esp_deep_sleep_start();
    esp_sleep_enable_timer_wakeup(30 * 1000 * 1000);
    esp_light_sleep_start();
    resetPowerSaving();
}

void App::alignText(Core::S32 x, Core::S32 y, Core::S32 w, Core::S32 h, App::HAlign hAlign, VAlign vAlign, const String& text) {
    auto& u8g2 = GFX::instance().u8g2();
    auto px = 0;
    auto py = 0;
    switch (hAlign) {
        case Left:px = x;
            break;
        case Center:px = x + (w - u8g2.getUTF8Width(text.c_str())) / 2;
            break;
        case Right:px = x + w - u8g2.getUTF8Width(text.c_str());
            break;
    }

    auto lineHeight = GFX::instance().lineHeight();

    switch (vAlign) {
        case Top:py = y + lineHeight;
            break;
        case Middle:py = y + (h - lineHeight) / 2;
            break;
        case Bottom:py = y + h;
            break;
    }
    u8g2.setCursor(px, py);
    u8g2.print(text);
}
