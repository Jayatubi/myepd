#pragma once

#include "module/event/event_define.h"
#include "module/singleton.h"


class Weather : public Singleton<Weather> {
public:
    Weather();

    void fetch();

private:
    String server;
    String api_key;
    String lang;
    String city;

    String lastError;
};

struct Event_WeatherChange : public EventBase {
DECLARE_EVENT(Event_WeatherChange)
};
