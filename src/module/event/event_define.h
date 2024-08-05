#pragma once

#include "core/fundamental/string_hash.h"
#include "event.h"

#define DECLARE_EVENT(EventEnum) \
    public: \
        enum { EventId = Core::StringHash::StaticHash(#EventEnum) }; \
        static const char* EventName() { return #EventEnum; }
