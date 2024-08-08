#include "gfx.h"

#define EPD_RST_PIN         26
#define EPD_DC_PIN          27
#define EPD_CS_PIN          15
#define EPD_BUSY_PIN        25
#define EPD_SCK_PIN         13
#define EPD_MOSI_PIN        14

GFX::GFX() {
    SPI.begin(EPD_SCK_PIN, -1, EPD_MOSI_PIN, -1);
    SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));

    _display = new DisplayType(GxEPD2_213_BN(EPD_CS_PIN, EPD_DC_PIN, EPD_RST_PIN, EPD_BUSY_PIN));
    _display->init(0, true, 2, false);
    _display->setRotation(1);
    _display->clearScreen();

    _u8g2 = new U8G2_FOR_ADAFRUIT_GFX();
    _u8g2->begin(*_display);
    _u8g2->setForegroundColor(GxEPD_BLACK);
    _u8g2->setBackgroundColor(GxEPD_WHITE);
    _u8g2->setFont(u8g2_font_wqy14_t_gb2312);
}

void GFX::poweroff() {
    _display->powerOff();
}

GFX::~GFX() {
    if (_u8g2 != nullptr) {
        delete _u8g2;
        _u8g2 = nullptr;
    }
    if (_display != nullptr) {
        delete _display;
        _display = nullptr;
    }
}

U8G2_FOR_ADAFRUIT_GFX& GFX::u8g2() {
    assert(_u8g2);
    return *_u8g2;
}

GFX::DisplayType& GFX::display() {
    assert(_display);
    return *_display;
}

void GFX::beginFrame() {
    _display->firstPage();
}

bool GFX::endFrame() {
    return _display->nextPage();
}

int16_t GFX::screenWidth() const {
    return _display->width();
}

int16_t GFX::screenHeight() const {
    return _display->height();
}

int8_t GFX::lineHeight() const {
    return _u8g2->u8g2.font_info.ascent_para - _u8g2->u8g2.font_info.descent_para;
}

void GFX::drawBitmap(const Core::U8* bitmap, Core::S16 x, Core::S16 y, Core::S16 w, Core::S16 h, Core::F32 scale) {
    const auto line = (w + 7) / 8;
    auto sw = (Core::S16)(w * scale);
    auto sh = (Core::S16)(h * scale);

    for (Core::S16 py = 0; py < sh; py++) {
        for (Core::S16 px = 0; px < sw; px++) {
            auto sx = (Core::S16)(px / scale);
            auto sy = (Core::S16)(py / scale);
            auto bit = bitmap[sy * line + sx / 8] & (0x80 >> (sx % 8));
            _display->drawPixel(x + px, y + py, bit ? GxEPD_BLACK : GxEPD_WHITE);
        }
    }
}
