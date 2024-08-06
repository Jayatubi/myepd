#include "weather.h"
#include "module/private.h"
#include "module/console/console.h"
#include "module/network/network.h"
#include <HttpClient.h>

Weather::Weather() {
    server = "https://api.seniverse.com/v3/weather/daily.json";
    api_key = WEATHER_API_KEY;
    city = "chengdu";
    lang = "zh-Hans";
}


void Weather::fetch() {
    String url = server
        + "?key=" + api_key
        + "&location=" + city
        + "&language=" + lang
        + "&unit=" + "c";

    Network::instance().prepare();

    HTTPClient http;
    http.begin(url);
    http.setTimeout(5000);
    auto code = http.GET();
    if (code == HTTP_CODE_OK) {
        Console::instance().serial().println(http.getString());
    } else {
        lastError = http.errorToString(code);
    }
}
