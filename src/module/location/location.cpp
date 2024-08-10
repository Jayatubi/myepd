#include "location.h"
#include "module/network/network.h"
#include "module/private.h"
#include <MD5Builder.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <module/console/console.h>

Location::Location()
    : _state(idle) {
    server = "https://apis.map.qq.com";
}

void Location::update(Core::S64 deltaMs) {
    switch (_state) {
        case idle: {
            setState(locating);
            break;
        }
        case locating: {
            auto& network = Network::instance();
            network.wakeup();
            if (network.online()) {
                if (locate()) {
                    setState(located);
                    _relocateTimeout = 60_m;
                }
            }
            break;
        }
        case located: {
            _relocateTimeout -= deltaMs;
            if (_relocateTimeout <= 0) {
                setState(idle);
            }
            break;
        }
    }
}

Location::State Location::state() const {
    return _state;
}

const Location::Detail& Location::detail() const {
    return _detail;
}

void Location::setState(Location::State newState) {
    if (_state != newState) {
        _state = newState;

        Event_LocationStateChanged event;
        event.state = _state;
        Event::instance().On(event);
    }
}

bool Location::locate() {
    String path = "/ws/location/v1/ip";
    String params = path + "?key=" + LOCATE_KEY;
    MD5Builder md5;
    md5.begin();
    md5.add(params + LOCATE_SK);
    md5.calculate();
    String sig = md5.toString();

    String url = server + params + "&sig=" + sig;

    bool succeed = false;

    HTTPClient http;
    http.begin(url);
    http.setTimeout(5000);
    auto code = http.GET();
    Console::instance().printf("%s %d\n", url.c_str(), code);
    if (code == HTTP_CODE_OK) {
        JsonDocument doc;
        deserializeJson(doc, http.getString());
        if (doc["status"] == 0) {
            auto result = doc["result"];
            _detail.ip = result["ip"].as<String>();
            _detail.latitude = result["location"]["lat"];
            _detail.longitude = result["location"]["lng"];
            succeed = true;
        }
    }

    return succeed;
}
