#include "weather.hpp"
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace weather {

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string fetchWeatherJSON() {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        const char* url =
            "https://api.open-meteo.com/v1/forecast?latitude=-34.6037&longitude=-58.3816"
            "&current_weather=true&daily=temperature_2m_max,temperature_2m_min"
            "&timezone=America%2FArgentina%2FBuenos_Aires";

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
        }

        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

std::string weatherCodeToString(int code) {
    switch (code) {
        case 0:  return "Cielo despejado";
        case 1:  return "Mayormente despejado";
        case 2:  return "Parcialmente nublado";
        case 3:  return "Nublado";
        case 45: return "Niebla";
        case 48: return "Niebla con escarcha";
        case 51: return "Llovizna ligera";
        case 53: return "Llovizna moderada";
        case 55: return "Llovizna densa";
        case 61: return "Lluvia ligera";
        case 63: return "Lluvia moderada";
        case 65: return "Lluvia intensa";
        case 80: return "Chubascos ligeros";
        case 81: return "Chubascos moderados";
        case 82: return "Chubascos violentos";
        case 95: return "Tormenta";
        case 96: return "Tormenta con granizo ligero";
        case 99: return "Tormenta con granizo intenso";
        default: return "Desconocido";
    }
}

WeatherData parseWeather(const std::string& jsonStr) {
    WeatherData data{};
    try {
        auto j = json::parse(jsonStr);
        data.current_temp = j["current_weather"]["temperature"];
        data.weather_code = j["current_weather"]["weathercode"];
        data.temp_max = j["daily"]["temperature_2m_max"][0];
        data.temp_min = j["daily"]["temperature_2m_min"][0];
        data.description = weatherCodeToString(data.weather_code);
    } catch (const std::exception& e) {
        std::cerr << "Error parseando JSON: " << e.what() << "\n";
    }
    return data;
}

} // namespace weather
