#!/bin/bash
set -euo pipefail

echo "=== Instalando dependencias del proyecto ==="

if ! command -v g++ &> /dev/null; then
    echo "Instalando g++..."
    sudo apt-get update
    sudo apt-get install -y build-essential
else
    echo "g++ ya instalado: $(g++ --version | head -n1)"
fi

if ! dpkg -s libcurl4-openssl-dev &> /dev/null; then
    echo "Instalando libcurl4-openssl-dev..."
    sudo apt-get update
    sudo apt-get install -y libcurl4-openssl-dev
else
    echo "libcurl4-openssl-dev ya instalado"
fi

if [ ! -f "../include/json.hpp" ] && [ ! -f "include/json.hpp" ]; then
    echo "Descargando nlohmann/json..."
    curl -sL "https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp" \
        -o include/json.hpp
    echo "json.hpp descargado en include/"
else
    echo "json.hpp ya presente"
fi

echo "=== Dependencias instaladas correctamente ==="
