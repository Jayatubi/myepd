#include "battery.h"
#include "core/basic_type/math.h"
#include "module/timer/timer.h"
#include "module/console/console.h"

#define DEFAULT_VREF                1100
#define CUSTOM_CONVERSION_FACTOR    2.989f
#define FULL_BATTERY_VOLTAGE        4.2f
#define LOW_BATTERY_VOLTAGE         3.3f

Battery::Battery()
    : channel(ADC1_CHANNEL_0), atten(ADC_ATTEN_DB_11), sampleIndex(0) {

    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(channel, atten);
    esp_adc_cal_characterize(ADC_UNIT_1, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adc_chars);

    samples.resize(10);

    measureBatteryLevel();
}

Battery::~Battery() {
}


void Battery::measureBatteryLevel() {
    // 使用库进行校准
    auto raw_voltage = esp_adc_cal_raw_to_voltage(adc1_get_raw(channel), &adc_chars);
    auto voltage = (float) raw_voltage / 1000.0f * CUSTOM_CONVERSION_FACTOR; // 将mV转换为V，应用校正转换因子
    // 应用滤波
    samples[sampleIndex] = voltage;
    sampleIndex = (sampleIndex + 1) % samples.size();

    int count = 0;
    auto total = samples.reduce(0.f, [&count](Core::F32 sum, const Core::F32& value) {
        count += value > 0 ? 1 : 0;
        return sum + value;
    });
    if (count > 0) {
        setLevel(Core::clamp(Core::normalize(total / count, LOW_BATTERY_VOLTAGE, FULL_BATTERY_VOLTAGE), 0.f, 1.f) * 100);
    }
}

Core::U8 Battery::level() {
    return _level;
}

void Battery::setLevel(Core::U8 newLevel) {
    if (_level != newLevel) {
        _level = newLevel;
        if (auto eventInstance = Event::checkInstance()) {
            Event_BatteryLevelChange event{};
            event.level = _level;
            eventInstance->On(event);
        }
    }
}
