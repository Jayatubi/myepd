#include "gfx.h"

#define EPD_RST_PIN         26
#define EPD_DC_PIN          27
#define EPD_CS_PIN          15
#define EPD_BUSY_PIN        25
#define EPD_SCK_PIN         13
#define EPD_MOSI_PIN        14

GxEPD2_213_BN screen(EPD_CS_PIN, EPD_DC_PIN, EPD_RST_PIN, EPD_BUSY_PIN);

GFX::GFX() {
  SPI.begin(EPD_SCK_PIN, -1, EPD_MOSI_PIN, -1);
  SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));

  display = new DisplayType(screen);
  u8g2 = new U8G2_FOR_ADAFRUIT_GFX();
  u8g2->begin(*display);

  display->init(115200, true, 20, false);
  display->setRotation(1);
  display->setFullWindow();

  u8g2->setFontDirection(0);
  u8g2->setForegroundColor(GxEPD_BLACK);
  u8g2->setBackgroundColor(GxEPD_WHITE);
  u8g2->setFont(u8g2_font_b12_b_t_japanese3);
}

GFX::~GFX() {
  if (u8g2 != nullptr) {
    delete u8g2;
    u8g2 = nullptr;
  }
  if (display != nullptr) {
    delete display;
    display = nullptr;
  }
}

U8G2_FOR_ADAFRUIT_GFX &GFX::GetU8G2() {
  assert(u8g2);
  return *u8g2;
}

GFX::DisplayType &GFX::GetDisplay() {
  assert(display);
  return *display;
}

void GFX::BeginFrame() {
  display->clearScreen(0xFF);
  display->firstPage();
}

bool GFX::EndFrame() {
  return display->nextPage();
}
