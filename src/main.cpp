#include "weather.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string city;

    if (argc >= 2) {
        std::string arg = argv[1];
        if (arg == "-h" || arg == "--help") {
            std::cout << "Uso: " << (argc > 0 ? argv[0] : "weather")
                      << " [ciudad]\n";
            std::cout << "Muestra el clima actual de la ciudad indicada.\n";
            std::cout << "Si no se pasa ciudad, usa Buenos Aires por defecto.\n";
            return 0;
        }
        city = arg;
    } else {
        city = "Buenos Aires";
    }

    std::cout << "Consultando clima para: " << city << "...\n";

    weather::Location loc = weather::resolveCity(city);
    if (!loc.ok) {
        std::cerr << "No se pudo encontrar la ciudad: " << city << "\n";
        return 1;
    }

    weather::WeatherData data = weather::fetchWeather(loc);
    if (!data.ok) {
        std::cerr << "Error obteniendo/procesando datos del clima\n";
        return 1;
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
