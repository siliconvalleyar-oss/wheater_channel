#!/bin/bash
# Weather script for Buenos Aires using Open-Meteo API (no key required)

API_URL="https://api.open-meteo.com/v1/forecast?latitude=-34.6037&longitude=-58.3816&current_weather=true&daily=temperature_2m_max,temperature_2m_min&timezone=America%2FArgentina%2FBuenos_Aires"

response=$(curl -s "$API_URL")

temp=$(echo "$response" | grep -oP '"temperature":\K[0-9.]+')
temp_max=$(echo "$response" | grep -oP '"temperature_2m_max":\[\K[0-9.]+')
temp_min=$(echo "$response" | grep -oP '"temperature_2m_min":\[\K[0-9.]+')
weathercode=$(echo "$response" | grep -oP '"weathercode":\K[0-9]+')

declare -A codes
codes[0]="Cielo despejado"
codes[1]="Mayormente despejado"
codes[2]="Parcialmente nublado"
codes[3]="Nublado"
codes[45]="Niebla"
codes[48]="Niebla con escarcha"
codes[51]="Llovizna ligera"
codes[53]="Llovizna moderada"
codes[55]="Llovizna densa"
codes[61]="Lluvia ligera"
codes[63]="Lluvia moderada"
codes[65]="Lluvia intensa"
codes[80]="Chubascos ligeros"
codes[81]="Chubascos moderados"
codes[82]="Chubascos violentos"
codes[95]="Tormenta"
codes[96]="Tormenta con granizo ligero"
codes[99]="Tormenta con granizo intenso"

description="${codes[$weathercode]:-Desconocido}"

echo "📍 Buenos Aires"
echo "🌡️  Temperatura actual: ${temp}°C"
echo "🔺 Máxima: ${temp_max}°C"
echo "🔻 Mínima: ${temp_min}°C"
echo "☁️  Condición: ${description}"
