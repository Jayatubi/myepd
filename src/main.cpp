#include "app.h"
#include "module/console/console.h"

App app;

void setup() {
    app.Bootstrap();
}


void loop() {
    app.Tick();
}
