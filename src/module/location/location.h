
#pragma once

#include "module/event/event_define.h"
#include "module/singleton.h"

class Location : public Singleton<Location>
{
public:
    enum State {
        idle,
        locating,
        located
    };

    struct Detail {
        String ip;
        Core::F32 latitude;
        Core::F32 longitude;
    };

    Location();

    void update(Core::S64 deltaMs);

    State state() const;
    const Detail& detail() const;
private:
    bool locate();
    void setState(Location::State newState);
private:
    String server;

    State _state;
    Core::S64 _relocateTimeout;
    Detail _detail;
};

class Event_LocationStateChanged : public EventBase
{
DECLARE_EVENT(Event_LocationStateChanged)
public:
    Location::State state;
};
