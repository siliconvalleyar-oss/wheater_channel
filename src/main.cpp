#include "weather.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string city;
    bool refresh = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h" || arg == "--help") {
            std::cout << "Uso: " << (argc > 0 ? argv[0] : "weather")
                      << " [ciudad] [-r|--refresh]\n";
            std::cout << "Muestra el clima actual de la ciudad indicada.\n";
            std::cout << "Si no se pasa ciudad, usa Buenos Aires por defecto.\n";
            std::cout << "-r, --refresh  Fuerza actualizacion ignorando la cache local.\n";
            return 0;
        }
        if (arg == "-r" || arg == "--refresh") {
            refresh = true;
        } else if (!city.empty()) {
            city += " " + arg;
        } else {
            city = arg;
        }
    }

    if (city.empty()) {
        city = "Buenos Aires";
    }

    std::cout << "Consultando clima para: " << city << "...\n";

    weather::Location loc = weather::resolveCity(city, refresh);
    if (!loc.ok) {
        std::cerr << "No se pudo encontrar la ciudad: " << city << "\n";
        return 1;
    }

    bool usedCache = !refresh && weather::cacheFresh("wx_" + loc.query, 600);
    weather::WeatherData data = weather::fetchWeather(loc, refresh);
    if (!data.ok) {
        std::cerr << "Error obteniendo/procesando datos del clima\n";
        return 1;
    }

    if (usedCache) {
        std::cout << "(desde cache local)\n";
    }

    std::cout << "==============================\n";
    std::cout << loc.name;
    if (!loc.country.empty()) std::cout << ", " << loc.country;
    std::cout << "\n";
    std::cout << weather::weatherEmoji(data.weather_code) << " "
              << data.description << "\n";
    std::cout << "Temperatura actual: " << data.current_temp << " C\n";
    std::cout << "Maxima: " << data.temp_max << " C\n";
    std::cout << "Minima: " << data.temp_min << " C\n";
    std::cout << "Viento: " << data.windspeed << " km/h ("
              << data.winddirection << "°)\n";
    if (!data.time.empty()) {
        std::cout << "Hora local: " << data.time << "\n";
    }
    std::cout << "==============================\n";

    return 0;
}
