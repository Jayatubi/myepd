#include "network.h"
#include "module/private.h"
#include "module/event/event.h"

#include <WiFi.h>

Network::Network():
    _awake(false) {
    _state = idle;
    ssid = WIFI_SSID;
    password = WIFI_PASSWORD;
    lifetime = 0;
}

void Network::wakeup() {
    if (_state == idle) {
        _awake = true;
    }
    if (_state == connected) {
        resetLifetime();
    }
}

void Network::update(int64_t deltaMs) {
    const auto status = WiFiClass::status();
    switch (_state) {
        case idle: {
            if (_awake) {
                WiFiClass::mode(WIFI_STA);
                WiFi.begin(ssid, password);
                changeState(connecting);
            }
        }
            break;
        case connecting: {
            if (status == WL_CONNECTED) {
                resetLifetime();
                changeState(connected);
            }
            if (status == WL_CONNECT_FAILED) {
                changeState(idle);
            }
        }
            break;
        case connected: {
            lifetime -= deltaMs;
            if (status != WL_CONNECTED || lifetime <= 0) {
                WiFi.disconnect(true);
                _awake = false;
                changeState(idle);
            }
        }
            break;
        default:break;
    }
}

void Network::resetLifetime() {
    lifetime = 30_s;
}

const char* Network::get_ssid() const {
    return ssid;
}

IPAddress Network::get_ipaddress() const {
    IPAddress ip;

    if (_state == connected) {
        ip = WiFi.localIP();
    }

    return ip;
}

bool Network::online() const {
    return _state == connected;
}

Network::State Network::state() const {
    return _state;
}

void Network::changeState(Network::State newState) {
    if (_state != newState) {
        _state = newState;
        Event_NetworkStateChange event{};
        event.state = _state;
        Event::instance().On(event);
    }
}
