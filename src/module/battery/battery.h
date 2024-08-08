#pragma once

#include <esp_adc_cal.h>
#include "module/singleton.h"
#include "core/basic_type/vector.h"
#include "module/event/event_define.h"

class Battery : public Singleton<Battery> {
public:
    Battery();

    ~Battery();

    Core::U8 level();

    void sampleBatteryLevel();

    void update(Core::U64 deltaMs);
private:
    void setLevel(Core::U8 newLevel);

private:
    adc1_channel_t channel;
    adc_atten_t atten;
    esp_adc_cal_characteristics_t adc_chars;
    Core::Vector<Core::F32> samples;
    Core::U32 sampleIndex;
    Core::U8 _level;
    Core::S64 sampleInterval;
};

struct Event_BatteryLevelChange : public EventBase {
DECLARE_EVENT(Event_BatteryLevelChange)

    Core::U8 level;
};
