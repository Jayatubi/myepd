#include "application.h"
#include "module/gfx/gfx.h"
#include "module/console/console.h"
#include "module/network/network.h"
#include "module/clock/clock.h"
#include "module/event/event.h"
#include "module/timer/timer.h"
#include "module/battery/battery.h"
#include "module/weather/weather.h"

namespace Details
{
    template <typename ValueType>
    struct HasUpdate
    {
        template <typename TestType>
        static Core::Meta::Yes Tester(decltype(std::declval<TestType>().update(std::declval<Core::U64>())) *);

        template <typename TestType>
        static Core::Meta::No Tester(...);

        enum
        {
            Result = Core::Meta::IsSame<decltype(Tester<ValueType>(nullptr)), Core::Meta::Yes>::Result
        };
    };

}

template <typename... ClassTypes>
class Modules
{
public:
    Modules() = default;
    ~Modules() = default;
    static void Tick(Core::U64 deltaMs) {};
};

template <typename ThisType, typename... RestTypes>
class Modules<ThisType, RestTypes...> : public Modules<RestTypes...>
{
public:
    typedef Modules<RestTypes...> Base;

public:
    Modules()
    {
        ThisType::release();
    }

    ~Modules()
    {
        ThisType::renew();
    }

    static void Tick(Core::U64 deltaMs)
    {
        Base::Tick(deltaMs);
        if constexpr (Details::HasUpdate<ThisType>::Result)
        {
            ThisType::instance().update(deltaMs);
        }
    };
};

Application::Application()
{
}

typedef Modules<
    Console,
    GFX,
    Network,
    Clock,
    Timer,
    Event,
    Battery,
    Weather>
    RegisterdModules;

void Application::Bootstrap()
{
    RegisterdModules();
    invalidate("*");
}

void Application::Teardown()
{
}

void Application::Tick()
{
    auto now = millis();
    static Core::U64 lastMs = 0;
    RegisterdModules::Tick(lastMs != 0 ? now - lastMs : 0);
    lastMs = now;

    if (flags.size() > 0)
    {
        repaint();
        flags.clear();
    }
}

bool Application::with_flag(const String &flag)
{
    return flags.contains(flag) || flags.contains("*");
}

void Application::invalidate(const String &flag)
{
    flags.insert(flag);
}

void Application::repaint()
{
}
