#include "network.h"
#include "module/private.h"
#include "module/event/event.h"

#include <WiFi.h>

Network::Network() {
    _state = idle;
    ssid = WIFI_SSID;
    password = WIFI_PASSWORD;
}

void Network::update(int64_t frame_count) {
    const auto status = WiFiClass::status();
    switch (_state) {
        case idle: {
            WiFiClass::mode(WIFI_STA);
            WiFi.begin(ssid, password);
            changeState(connecting);
        }
            break;
        case connecting: {
            if (status == WL_CONNECTED) {
                changeState(ready);
            }
            if (status == WL_CONNECT_FAILED) {
                changeState(fail);
            }
        }
            break;
        case ready: {
            if (status == WL_CONNECTION_LOST) {
                changeState(idle);
            }
            break;
        }
        case fail: {
            changeState(idle);
        }
            break;
        default:break;
    }
}

const char* Network::get_ssid() const {
    return ssid;
}

IPAddress Network::get_ipaddress() const {
    IPAddress ip;

    if (_state == ready) {
        ip = WiFi.localIP();
    }

    return ip;
}

bool Network::is_ready() const {
    return _state == ready;
}

Network::State Network::state() const {
    return _state;
}

void Network::changeState(Network::State newState) {
    if (_state != newState) {
        _state = newState;
        if (auto eventInstance = Event::checkInstance()) {
            Event_NetworkStateChange event{};
            event.state = _state;
            eventInstance->On(event);
        }
    }
}
