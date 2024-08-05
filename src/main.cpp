#include "app.h"
#include "module/console/console.h"

App app;

void setup() {
    app.Bootstrap();
}


void loop() {
    static int64_t frameCount = 0;
    app.Tick(frameCount);
    frameCount++;
}
