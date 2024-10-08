#include "timer.h"

Timer::Timer()
    : nextTicket(0) {
}

Core::U32 Timer::setTimeout(Core::S64 duration, const Timer::Callback& action) {
    return addContext(duration, action, 1);
}

Core::U32 Timer::setInterval(Core::S64 duration, const Timer::Callback& action) {
    return addContext(duration, action, -1);
}

Core::U32 Timer::addContext(Core::S64 duration, const Timer::Callback& action, Core::S32 repeat) {
    auto ticket = ++nextTicket;
    Context context;
    context.start = millis();
    context.duration = duration;
    context.action = action;
    context.repeat = repeat;
    pendingAdd.push_back(Core::MakeTuple(ticket, context));
    pendingRemove.removeif([=](const auto& pendingTicket) {
        return pendingTicket == ticket;
    });
    return ticket;
}

void Timer::clear(Core::U32 ticket) {
    pendingRemove.push_back(ticket);
    pendingAdd.removeif([=](const auto& tuple) {
        return Core::getTuple<0>(tuple) == ticket;
    });
}

void Timer::update(Core::U64 deltaMs) {
    for (const auto& ticket : pendingRemove) {
        contexts.erase(ticket);
    }
    pendingRemove.clear();

    for (auto& entry : pendingAdd) {
        contexts.emplace(Core::getTuple<0>(entry), Core::getTuple<1>(entry));
    }
    pendingAdd.clear();

    const auto now = millis();

    for (auto& entry : contexts) {
        auto& context = entry.second;
        if (now - context.start >= context.duration) {
            if (context.repeat > 0) {
                if (--context.repeat == 0) {
                    pendingRemove.push_back(entry.first);
                }
            }
            context.action();
            // For next trigger, if possible
            context.start = now;
        }
    }
}
