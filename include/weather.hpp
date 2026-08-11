#ifndef WEATHER_HPP
#define WEATHER_HPP

#include <string>

namespace weather {

struct WeatherData {
    double current_temp;
    double temp_max;
    double temp_min;
    int weather_code;
    std::string description;
};

std::string fetchWeatherJSON();
WeatherData parseWeather(const std::string& json);
std::string weatherCodeToString(int code);

} // namespace weather

#endif
