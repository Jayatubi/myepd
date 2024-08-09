#include "weather.h"
#include "module/private.h"
#include "module/console/console.h"
#include "module/network/network.h"
#include <HttpClient.h>
#include <ArduinoJson.h>

Weather::Weather()
    : _state(idle) {
    server = "https://api.seniverse.com/v3/weather";
    apiKey = WEATHER_API_KEY;
    cityCode = "chengdu";
    lang = "zh-Hans";
}

const Weather::Now& Weather::now() const {
    return _now;
}

const Core::Vector<Weather::Daily>& Weather::dailies() const {
    return _dailies;
}

void Weather::update(Core::U64 deltaMs) {
    switch (_state) {
        case idle: {
            changeState(fetching);
            break;
        }
        case fetching: {
            auto& network = Network::instance();
            network.wakeup();
            if (network.online()) {
                fetchNow();
                fetchDailies();
                changeState(fetched);
                _refetchTimeout = 60_m;
            }
            break;
        }
        case fetched: {
            _refetchTimeout -= deltaMs;
            if (_refetchTimeout <= 0) {
                changeState(idle);
            }
            break;
        }
    }
}

void Weather::fetchNow() {
    String url = server
        + "/now.json"
        + "?key=" + apiKey
        + "&location=" + cityCode
        + "&language=" + lang
        + "&unit=" + "c";

    HTTPClient http;
    http.begin(url);
    http.setTimeout(5000);
    auto code = http.GET();
    if (code == HTTP_CODE_OK) {
        JsonDocument doc;
        deserializeJson(doc, http.getString());
        auto now = doc["results"][0]["now"];
        _now.temperature = now["temperature"];
        _now.code = now["code"];
        _now.text = now["text"].as<String>();
        _city = doc["results"][0]["location"]["name"].as<String>();
    } else {
        Console::instance().serial().println(http.errorToString(code));
    }
}

void Weather::fetchDailies() {
    String url = server
        + "/daily.json"
        + "?key=" + apiKey
        + "&location=" + cityCode
        + "&language=" + lang
        + "&unit=" + "c";

    HTTPClient http;
    http.begin(url);
    http.setTimeout(5000);
    auto code = http.GET();
    if (code == HTTP_CODE_OK) {
        JsonDocument doc;
        deserializeJson(doc, http.getString());
        _dailies.clear();
        for (const auto& daily : doc["results"][0]["daily"].as<JsonArray>()) {
            Daily _daily;
            _daily.dayCode = daily["code_day"];
            _daily.dayText = daily["text_day"].as<String>();
            _daily.nightCode = daily["code_night"];
            _daily.nightText = daily["text_night"].as<String>();
            _daily.high = daily["high"];
            _daily.low = daily["low"];
            _daily.wind = daily["wind_direction_degree"];
            _daily.rainfall = daily["rainfall"];
            _daily.precip = daily["precip"];
            _daily.humidity = daily["humidity"];
            strptime(daily["date"], "%Y-%m-%d", &_daily.timeinfo);
            _dailies.emplace_back(_daily);
        }
    } else {
        Console::instance().serial().println(http.errorToString(code));
    }
}

Weather::State Weather::state() const {
    return _state;
}

void Weather::changeState(Weather::State newState) {
    if (_state != newState) {
        _state = newState;

        Event_WeatherChange event;
        event.state = _state;
        Event::instance().On(event);
    }
}

const String& Weather::city() const {
    return _city;
}
