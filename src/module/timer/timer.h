#pragma once

#include "core/basic_type/map.h"
#include "core/basic_type/vector.h"
#include "core/fundamental/function.h"
#include "core/basic_type/basic_type.h"
#include "module/singleton.h"

class Timer : public Singleton<Timer> {
public:
    Timer();

    void update(Core::U64 deltaMs);

    typedef Core::Function<void> Callback;

    Core::U32 setTimeout(Core::S64 duration, const Timer::Callback& action);

    Core::U32 setInterval(Core::S64 duration, const Timer::Callback& action);

    void clear(Core::U32 ticket);

private:
    Core::U32 addContext(Core::S64 duration, const Timer::Callback& action, Core::S32 repeat);

    struct Context {
        Core::U64 start;
        Core::U64 duration;
        Callback action;
        Core::S32 repeat;
    };

    Core::Map<Core::U32, Context> contexts;
    Core::Vector<Core::Tuple<Core::U32, Context>> pendingAdd;
    Core::Vector<Core::U32> pendingRemove;
    Core::U32 nextTicket;
};
