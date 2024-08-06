#pragma once


#include "core/basic_type/basic_type.h"
#include "core/basic_type/string.h"
#include "core/basic_type/set.h"

class Application {
public:
    Application();

    virtual void Bootstrap();

    virtual void Teardown();

    virtual void Tick();

    virtual void invalidate(const String& flag = "*");

    virtual void repaint();

protected:
    bool with_flag(const String& flag);

    Core::Set<String> flags;
};
