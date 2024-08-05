#pragma once

#include "module/event/event_define.h"
#include "module/singleton.h"


class Weather : public Singleton<Weather> {
public:
    enum State {
        idle,
        querying,
        queried
    };

    Weather();

    void fetch();

private:
    void setState(State newState);

private:
    State _state;
    String server;
    String api_key;
    String lang;
    String city;

    String lastError;
};

struct Event_WeatherStateChange : public EventBase {
DECLARE_EVENT(Event_WeatherStateChange)
};
