#include "weather.hpp"
#include <curl/curl.h>
#include <iostream>
#include <nlohmann/json.hpp>

#include <cctype>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace fs = std::filesystem;

using json = nlohmann::json;

namespace weather {

static constexpr int GEO_TTL = 86400; // coordenadas: 24 h
static constexpr int WX_TTL  = 600;   // clima: 10 min

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Realiza un GET con curl y devuelve el cuerpo. Reporta errores por stderr.
static std::string httpGet(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << "\n";
        }

        curl_easy_cleanup(curl);
    } else {
        std::cerr << "No se pudo inicializar curl\n";
    }

    return readBuffer;
}

// ---- Cache local basada en archivos en $HOME/.cache/wheater_channel ----

static std::string cacheDir() {
    const char* home = std::getenv("HOME");
    std::string base = home ? std::string(home) : "/tmp";
    return base + "/.cache/wheater_channel";
}

static std::string sanitize(const std::string& s) {
    std::string out;
    for (char c : s) {
        out += std::isalnum(static_cast<unsigned char>(c)) ? c : '_';
    }
    return out.empty() ? "x" : out;
}

static std::string cachePath(const std::string& key) {
    return cacheDir() + "/" + sanitize(key) + ".json";
}

bool cacheFresh(const std::string& key, int ttlSeconds) {
    std::error_code ec;
    std::string p = cachePath(key);
    if (!fs::exists(p, ec)) return false;
    auto ftime = fs::last_write_time(p, ec);
    auto now = fs::file_time_type::clock::now();
    auto age = std::chrono::duration_cast<std::chrono::seconds>(now - ftime).count();
    return age >= 0 && age <= ttlSeconds;
}

static std::string cacheRead(const std::string& key) {
    std::ifstream f(cachePath(key));
    if (!f) return "";
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

static void cacheWrite(const std::string& key, const std::string& content) {
    if (content.empty()) return;
    std::error_code ec;
    fs::create_directories(cacheDir(), ec);
    std::ofstream f(cachePath(key));
    if (f) f << content;
}

Location resolveCity(const std::string& city, bool refresh) {
    Location loc{};
    loc.query = city;

    std::string key = "geo_" + city;
    std::string body;

    if (!refresh && cacheFresh(key, GEO_TTL)) {
        body = cacheRead(key);
    } else {
        char* escaped = curl_easy_escape(nullptr, city.c_str(), 0);
        std::string url =
            "https://geocoding-api.open-meteo.com/v1/search?name=" + std::string(escaped)
            + "&count=1&language=es&format=json";
        curl_free(escaped);

        body = httpGet(url);
        if (!body.empty()) cacheWrite(key, body);
    }

    if (body.empty()) {
        return loc;
    }

    try {
        auto j = json::parse(body);
        if (!j.contains("results") || j["results"].empty()) {
            return loc;
        }
        auto r = j["results"][0];
        loc.latitude = r["latitude"];
        loc.longitude = r["longitude"];
        loc.name = r.value("name", "");
        loc.country = r.value("country", "");
        loc.timezone = r.value("timezone", "");
        loc.ok = true;
    } catch (const std::exception& e) {
        std::cerr << "Error geocodificando ciudad: " << e.what() << "\n";
    }

    return loc;
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

std::string weatherEmoji(int code) {
    if (code == 0) return "☀️";
    if (code == 1) return "🌤️";
    if (code == 2) return "⛅";
    if (code == 3) return "☁️";
    if (code == 45 || code == 48) return "🌫️";
    if (code >= 51 && code <= 55) return "🌦️";
    if (code == 61 || code == 63 || code == 65) return "🌧️";
    if (code >= 80 && code <= 82) return "🌧️";
    if (code >= 95 && code <= 99) return "⛈️";
    return "🌡️";
}

WeatherData fetchWeather(const Location& loc, bool refresh) {
    std::string key = "wx_" + loc.query;
    std::string body;

    if (!refresh && cacheFresh(key, WX_TTL)) {
        body = cacheRead(key);
    } else {
        std::string url =
            "https://api.open-meteo.com/v1/forecast?latitude=" + std::to_string(loc.latitude)
            + "&longitude=" + std::to_string(loc.longitude)
            + "&current_weather=true"
            + "&daily=temperature_2m_max,temperature_2m_min"
            + "&timezone=" + loc.timezone;

        body = httpGet(url);
        if (!body.empty()) cacheWrite(key, body);
    }

    WeatherData data = parseWeather(body);
    data.ok = data.ok && (body.find("current_weather") != std::string::npos);
    return data;
}

WeatherData parseWeather(const std::string& jsonStr) {
    WeatherData data{};
    try {
        auto j = json::parse(jsonStr);
        data.current_temp = j["current_weather"]["temperature"];
        data.weather_code = j["current_weather"]["weathercode"];
        data.windspeed = j["current_weather"]["windspeed"];
        data.winddirection = j["current_weather"]["winddirection"];
        data.time = j["current_weather"].value("time", "");
        data.temp_max = j["daily"]["temperature_2m_max"][0];
        data.temp_min = j["daily"]["temperature_2m_min"][0];
        data.description = weatherCodeToString(data.weather_code);
        data.ok = true;
    } catch (const std::exception& e) {
        std::cerr << "Error parseando JSON: " << e.what() << "\n";
    }
    return data;
}

} // namespace weather
