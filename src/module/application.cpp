#include "application.h"
#include "module/gfx/gfx.h"
#include "module/console/console.h"
#include "module/network/network.h"
#include "module/clock/clock.h"
#include "module/event/event.h"
#include "module/timer/timer.h"
#include "module/battery/battery.h"
#include "module/weather/weather.h"

namespace Details {
    template<typename ValueType>
    struct HasUpdate {
        template<typename TestType>
        static Core::Meta::Yes Tester(decltype(std::declval<TestType>().update(std::declval<Core::U64>()))*);

        template<typename TestType>
        static Core::Meta::No Tester(...);

        enum {
            Result = Core::Meta::IsSame<decltype(Tester<ValueType>(nullptr)), Core::Meta::Yes>::Result
        };
    };

}

template<typename ... ClassTypes>
class Modules {
public:
    Modules() = default;

    ~Modules() = default;

    virtual void Tick(Core::U64 frameCount) {
    };
};

template<typename ThisType, typename ... RestTypes>
class Modules<ThisType, RestTypes ...> : public Modules<RestTypes ...> {
public:
    typedef Modules<RestTypes ...> Base;
public:
    Modules() {
        ThisType::renew();
    }

    ~Modules() {
        ThisType::release();
    }

    template<typename TestType>
    void TickImpl(typename Core::Meta::EnableIf<Details::HasUpdate<TestType>::Result, Core::U64>::Result frameCount) {
        ThisType::instance().update(frameCount);
        Base::Tick(frameCount);
    }

    template<typename TestType>
    void TickImpl(typename Core::Meta::EnableIf<!Details::HasUpdate<TestType>::Result, Core::U64>::Result frameCount) {
        Base::Tick(frameCount);
    }

    void Tick(Core::U64 frameCount) override {
        TickImpl<ThisType>(frameCount);
    };
};

typedef Modules<
    GFX,
    Console,
    Network,
    Clock,
    Timer,
    Event,
    Battery,
    Weather
> AllModules;
AllModules* allModules = nullptr;

Application::Application()
    : repaintTicket(0) {
}

void Application::Bootstrap() {
    assert(allModules == nullptr);
    if (allModules == nullptr) {
        allModules = new AllModules();
    }

    invalidate("*");
}

void Application::Teardown() {
    if (allModules != nullptr) {
        delete allModules;
        allModules = nullptr;
    }
}

void Application::Tick(Core::U64 frameCount) {
    if (allModules != nullptr) {
        allModules->Tick(frameCount);
    }
}

bool Application::with_flag(const String& flag) {
    return flags.contains(flag) || flags.contains("*");
}

void Application::invalidate(const String& flag) {
    flags.insert(flag);

    if (repaintTicket == 0) {
        repaintTicket = Timer::instance().setTimeout(0, [this]() {
            repaint();
            flags.clear();
            repaintTicket = 0;
        });
    }
}

void Application::repaint() {

}
