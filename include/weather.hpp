#ifndef WEATHER_HPP
#define WEATHER_HPP

#include <string>

namespace weather {

struct Location {
    double latitude;
    double longitude;
    std::string name;
    std::string country;
    std::string timezone;
    bool ok;
};

struct WeatherData {
    double current_temp;
    double temp_max;
    double temp_min;
    int weather_code;
    double windspeed;
    int winddirection;
    std::string time;
    std::string description;
    bool ok;
};

// Resuelve el nombre de una ciudad a coordenadas via la API de geocodificacion
// de Open-Meteo. Devuelve ok=false si no se encontro la ciudad.
Location resolveCity(const std::string& city);

// Obtiene y parsea el clima actual para una ubicacion resuelta.
WeatherData fetchWeather(const Location& loc);

// Parsea un JSON de respuesta de Open-Meteo. ok=false si falla el parseo.
WeatherData parseWeather(const std::string& jsonStr);

std::string weatherCodeToString(int code);
std::string weatherEmoji(int code);

} // namespace weather

#endif
