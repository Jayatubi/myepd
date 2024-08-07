#pragma once

#include <IPAddress.h>
#include "module/event/event_define.h"
#include "module/singleton.h"

class Network : public Singleton<Network> {
public:

    enum State {
        idle,
        connecting,
        connected,
    };

    Network();

    void update(int64_t deltaMs);

    const char* get_ssid() const;

    IPAddress get_ipaddress() const;

    bool online() const;

    State state() const;

    void wakeup();

private:
    void changeState(State newState);

private:
    bool _awake;
    State _state;
    Core::S64 lifetime;
    const char* ssid;
    const char* password;

    void resetLifetime();
};

struct Event_NetworkStateChange : public EventBase {
DECLARE_EVENT(Event_NetworkStateChange)

    Network::State state;
};
