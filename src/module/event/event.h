#pragma once

#include "core/fundamental/signal.h"
#include "core/basic_type/map.h"
#include "core/basic_type/list.h"
#include "core/fundamental/function.h"
#include "module/singleton.h"

struct EventBase {

};

class Event : public Singleton<Event> {

public:
    template<typename EventType>
    void Listen(const Core::Function<void, const EventType&>& action);

    template<typename EventType>
    void Stop(const Core::Function<void, const EventType&>& action);

    template<typename EventType>
    void On(const EventType& event);

private:
    typedef Core::Signal<void, const EventBase&> SignalType;
    typedef typename SignalType::FuncType FuncType;
    typedef Core::Map<Core::U32, SignalType> RouterType;
private:
    RouterType m_router;
};


template<typename EventType>
void Event::Listen(const Core::Function<void, const EventType&>& action) {
    typedef Core::UnaryProxyFunc<Core::Function<void, const EventType&>, const EventBase&> ProxyFuncType;
    m_router[ProxyFuncType::RawConcreteType::EventId].connect(ProxyFuncType(action));
}

template<typename EventType>
void Event::Stop(const Core::Function<void, const EventType&>& action) {
    typedef Core::UnaryProxyFunc<Core::Function<void, const EventType&>, const EventBase&> ProxyFuncType;
    m_router[ProxyFuncType::RawConcreteType::EventId].disconnect(ProxyFuncType(action));
}

template<typename EventType>
void Event::On(const EventType& event) {
    m_router[EventType::EventId](event);
}

