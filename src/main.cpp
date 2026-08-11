#include "weather.hpp"
#include <iostream>

int main() {
    std::string json = weather::fetchWeatherJSON();

    if (json.empty()) {
        std::cerr << "No se pudo obtener el clima\n";
        return 1;
    }

    weather::WeatherData data = weather::parseWeather(json);

    if (data.current_temp == 0.0 && data.temp_max == 0.0 && data.temp_min == 0.0) {
        std::cerr << "Error procesando datos del clima\n";
        return 1;
    }

    std::cout << "Buenos Aires\n";
    std::cout << "Temperatura actual: " << data.current_temp << " C\n";
    std::cout << "Maxima: " << data.temp_max << " C\n";
    std::cout << "Minima: " << data.temp_min << " C\n";
    std::cout << "Condicion: " << data.description << "\n";

    return 0;
}
