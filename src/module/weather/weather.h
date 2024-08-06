#pragma once

#include "module/event/event_define.h"
#include "module/singleton.h"

class Weather : public Singleton<Weather> {
public:
    enum State {
        idle,
        fetching,
        fetched
    };
    struct Now {
        Core::U32 code;
        Core::U32 temperature;
    };

    struct Daily {
        Core::U32 daylight;
        Core::U32 night;
        Core::U32 wind;
        Core::U32 high;
        Core::U32 low;
        Core::U32 precip;
        Core::F32 rainfall;
    };
public:
    Weather();

    void update(Core::U64 deltaMs);

    State state() const;

    const Now& now() const;
    const Core::Vector<Daily>& dailies() const;

    const String& city() const;

private:
    void fetchNow();
    void fetchDailies();
    void changeState(Weather::State newState);

private:
    String server;
    String apiKey;
    String lang;
    String cityCode;
    String lastError;

    State _state;
    Now _now;
    Core::Vector<Daily> _dailies;
    String _city;
    Core::S64 _refetchTimeout;
};

struct Event_WeatherChange : public EventBase {
DECLARE_EVENT(Event_WeatherChange)
    Weather::State state;
};
