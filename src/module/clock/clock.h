#pragma once

#include "module/event/event_define.h"
#include "module/singleton.h"

class Clock : public Singleton<Clock> {
public:
    enum State {
        idle,
        syncing,
        synced,
    };

    Clock();

    ~Clock();

    void update(Core::U64 frameCount);

    void resync();

    time_t getTime() const;

    Core::S32 getYear();

    Core::S32 getMonth();

    Core::S32 getDate();

    Core::S32 getDay();

    Core::S32 getHours();

    Core::S32 getMinutes();

    Core::S32 getSeconds();

    Core::U64 getMilliseconds();

    Core::U32 deltaTime();

    State state() const;

private:

    tm& updateTimeinfo();

    void setState(State newState);

private:
    State _state;
    tm _timeinfo{};

    const char* time_zone;
    const char* ntp_server;

    Core::S32 _retryTimeout;
    Core::U64 _lastMilliseconds;
    Core::U32 _deltaTime;
};

struct Event_ClockStateChange : public EventBase {
DECLARE_EVENT(Event_ClockStateChange)

    Clock::State state;
};
