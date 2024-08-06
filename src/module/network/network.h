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

    bool is_ready() const;

    State state() const;

    void prepare();

private:
    void changeState(State newState);

private:
    bool _requested;
    State _state;
    Core::S64 duration;
    Core::S64 lifetime;
    const char* ssid;
    const char* password;

    void resetLifetime();
};

struct Event_NetworkStateChange : public EventBase {
DECLARE_EVENT(Event_NetworkStateChange)

    Network::State state;
};
