#include <Arduino.h>
#include "clock.h"
#include "module/network/network.h"
#include "module/private.h"

Clock::Clock()
    : _state(idle) {
    time_zone = TIME_ZONE;
    ntp_server = NTP_SERVER;
}

Clock::~Clock() {
}

void Clock::update(Core::U64 deltaMs) {
    switch (_state) {
        case idle: {
            changeState(prepare);
            break;
        }
        case prepare: {
            auto& network = Network::instance();
            network.wakeup();
            if (network.online()) {
                configTzTime(time_zone, ntp_server);
                changeState(syncing);
                _syncTimeout = 5_m;
            }
            break;
        }
        case syncing: {
            if (getLocalTime(&_timeinfo, 0)) {
                changeState(synced);
            } else {
                _syncTimeout -= deltaMs;
                if (_syncTimeout <= 0) {
                    _resyncTimeout = 60_m;
                    changeState(prepare);
                }
            }
        }
            break;
        case synced: {
            _resyncTimeout -= deltaMs;
            if (_resyncTimeout <= 0) {
                changeState(idle);
            }
        }
            break;
    }
}

void Clock::resync() {
    changeState(idle);
}

time_t Clock::getTime() const {
    time_t now;
    time(&now);
    return now;
}

Core::S32 Clock::getYear() {
    return 1900 + updateTimeinfo().tm_year;
}

Core::S32 Clock::getMonth() {
    return updateTimeinfo().tm_mon;
}

Core::S32 Clock::getDate() {
    return updateTimeinfo().tm_mday;
}

Core::S32 Clock::getDay() {
    return updateTimeinfo().tm_wday;
}

Core::S32 Clock::getHours() {
    return updateTimeinfo().tm_hour;
}

Core::S32 Clock::getMinutes() {
    return updateTimeinfo().tm_min;
}

Core::S32 Clock::getSeconds() {
    return updateTimeinfo().tm_sec;
}

tm& Clock::updateTimeinfo() {
    // TODO: Cache if possible
    getLocalTime(&_timeinfo, 0);
    return _timeinfo;
}

Core::U64 Clock::getMilliseconds() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

Clock::State Clock::state() const {
    return _state;
}

void Clock::changeState(Clock::State newState) {
    if (_state != newState) {
        _state = newState;
        Event_ClockStateChange event;
        event.state = _state;
        Event::instance().On(event);
    }
}
