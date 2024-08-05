#include <Arduino.h>
#include "clock.h"
#include "module/network/network.h"
#include "module/private.h"

Clock::Clock()
    : _state(idle), _lastMilliseconds(0), _deltaTime(0) {
    time_zone = TIME_ZONE;
    ntp_server = NTP_SERVER;
}

Clock::~Clock() {
}

void Clock::update(Core::U64 frameCount) {
    auto nowMilliseconds = millis();
    if (_lastMilliseconds != 0) {
        _deltaTime = nowMilliseconds - _lastMilliseconds;
    }
    _lastMilliseconds = nowMilliseconds;

    switch (_state) {
        case idle:
            if (Network::instance().is_ready()) {
                configTzTime(time_zone, ntp_server);
                setState(syncing);
                _retryTimeout = 5000;
            }
            break;
        case syncing: {
            if (getLocalTime(&_timeinfo, 0)) {
                setState(synced);
            } else {
                _retryTimeout -= _deltaTime;
                if (_retryTimeout <= 0) {
                    setState(idle);
                }
            }
        }
            break;
        case synced: {
        }
            break;
    }
}

void Clock::resync() {
    setState(idle);
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
    static Core::U64 updateMilliseconds = 0;
    if (updateMilliseconds != _lastMilliseconds) {
        getLocalTime(&_timeinfo, 0);
        updateMilliseconds = _lastMilliseconds;
    }
    return _timeinfo;
}

Core::U64 Clock::getMilliseconds() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

Core::U32 Clock::deltaTime() {
    return _deltaTime;
}


Clock::State Clock::state() const {
    return _state;
}

void Clock::setState(Clock::State newState) {
    if (_state != newState) {
        _state = newState;
        if (auto eventInstance = Event::checkInstance()) {
            Event_ClockStateChange event{};
            event.state = _state;
            eventInstance->On(event);
        }
    }
}
