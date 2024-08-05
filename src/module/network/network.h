#pragma once

#include <IPAddress.h>
#include "module/event/event_define.h"
#include "module/singleton.h"

class Network : public Singleton<Network> {
public:

    enum State {
        idle,
        connecting,
        ready,
        fail
    };

    Network();

    void update(int64_t frame_count);

    const char* get_ssid() const;

    IPAddress get_ipaddress() const;

    bool is_ready() const;

    State state() const;

private:
    void changeState(State newState);

private:
    State _state;
    const char* ssid;
    const char* password;
};

struct Event_NetworkStateChange : public EventBase {
DECLARE_EVENT(Event_NetworkStateChange)

    Network::State state;
};
