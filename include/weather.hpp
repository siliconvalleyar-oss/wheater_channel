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
    std::string query;   // ciudad tal como la pidio el usuario (para cache)
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
// Si refresh=false, usa la cache local de geocodificacion cuando es fresca.
Location resolveCity(const std::string& city, bool refresh = false);

// Obtiene y parsea el clima actual para una ubicacion resuelta.
// Si refresh=false, usa la cache local de clima cuando es fresca (TTL 10 min).
WeatherData fetchWeather(const Location& loc, bool refresh = false);

// Parsea un JSON de respuesta de Open-Meteo. ok=false si falla el parseo.
WeatherData parseWeather(const std::string& jsonStr);

std::string weatherCodeToString(int code);
std::string weatherEmoji(int code);

// Indica si hay una entrada de cache fresca para la clave dada (TTL en segundos).
bool cacheFresh(const std::string& key, int ttlSeconds);

} // namespace weather

#endif
