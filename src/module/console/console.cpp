#include "console.h"

Console::Console() {
    Serial.begin(115200);
}

Console::~Console() {
    Serial.end();
}

Console::LowlevelType& Console::serial() {
    return Serial;
}
