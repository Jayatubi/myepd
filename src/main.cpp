#include <GxEPD2_BW.h>
#include <U8g2_for_Adafruit_GFX.h>
#include "application.h"
#include "module/console.h"
#include "module/gfx.h"


void repaint(U8G2_FOR_ADAFRUIT_GFX &u8g2, GFX::DisplayType &display) {
  u8g2.setCursor(10, 50);
  u8g2.println("こんにちは");
  display.drawRoundRect(0, 0, display.width() - 1, display.height() - 1, 5, GxEPD_BLACK);
}

void setup() {
  Application::Bootstrap();
  Console::GetInstance().GetDevice().println("setup");
}

void loop() {
  auto &gfx = GFX::GetInstance();
  gfx.BeginFrame();
  do {
    repaint(gfx.GetU8G2(), gfx.GetDisplay());
  } while (gfx.EndFrame());

  delay(60000);
}
