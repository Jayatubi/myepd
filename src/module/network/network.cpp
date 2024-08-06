#include "network.h"
#include "module/private.h"
#include "module/event/event.h"

#include <WiFi.h>

Network::Network():
    _requested(false) {
    _state = idle;
    ssid = WIFI_SSID;
    password = WIFI_PASSWORD;
    // Disconnect if idle for 60 seconds
    duration = 60000;
    lifetime = 0;
}

void Network::prepare() {
    if (_state == idle) {
        _requested = true;
    }
    if (_state == connected) {
        resetLifetime();
    }
}

void Network::update(int64_t deltaMs) {
    const auto status = WiFiClass::status();
    switch (_state) {
        case idle: {
            if (_requested) {
                WiFiClass::mode(WIFI_STA);
                WiFi.begin(ssid, password);
                changeState(connecting);
                _requested = false;
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
                changeState(idle);
            }
        }
            break;
        default:break;
    }
}

void Network::resetLifetime() {
    lifetime = duration;
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

bool Network::is_ready() const {
    return _state == connected;
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
