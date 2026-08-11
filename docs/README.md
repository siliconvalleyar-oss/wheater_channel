# Weather Channel

Aplicación CLI para obtener el clima actual de Buenos Aires, incluyendo temperatura, máxima, mínima y condición del día.

## Características

- Temperatura actual
- Temperatura máxima y mínima del día
- Descripción de la condición climática
- Interfaz CLI en español

## Estructura del Proyecto

```
weather_channel/
├── src/                 # Código fuente C++
│   ├── main.cpp
│   └── weather.cpp
├── include/             # Headers (.h / .hpp)
│   ├── weather.hpp
│   └── json.hpp         # nlohmann/json (header-only)
├── obj/                 # Archivos objeto (.o) generados
├── bin/                 # Binario compilado
├── script_tools/        # Scripts bash
│   ├── weather.sh
│   └── install_deps.sh
├── docs/                # Documentación
├── Makefile
├── SKILLS.md
└── README.md
```

## Requisitos

- g++ 17 o superior
- libcurl4-openssl-dev
- nlohmann/json (incluido)

## Compatibilidad

| Plataforma | Estado |
|------------|--------|
| Linux x86_64 | Soportado |
| Raspberry Pi OS (armhf/arm64) | Soportado |
| macOS | Soportado |

## Raspberry Pi

Instalar dependencias y compilar directamente en la Raspberry Pi:

```bash
sudo apt-get update
sudo apt-get install -y g++ make libcurl4-openssl-dev
make run
```

El código es portable C++17 y no requiere librerías nativas adicionales.
`nlohmann/json` es header-only y funciona en ARM.
`libcurl` está disponible en los repositorios de Raspberry Pi OS.

Si se requiere cross-compilation desde x86_64:

```bash
# En Ubuntu/Debian host
sudo apt-get install g++-arm-linux-gnueabihf
make CXX=arm-linux-gnueabihf-g++
```

## Uso

### C++ (compilado)

```bash
make run
```

### Bash

```bash
./script_tools/weather.sh
```

## Instalación de Dependencias

```bash
sudo ./script_tools/install_deps.sh
```

## API

Ver [docs/API.md](docs/API.md) para detalles del endpoint utilizado.

## Compilación Manual

```bash
mkdir -p obj bin
g++ -std=c++17 -Wall -Wextra -pedantic -I./include \
    -c src/weather.cpp -o obj/weather.o
g++ -std=c++17 -Wall -Wextra -pedantic -I./include \
    -c src/main.cpp -o obj/main.o
g++ obj/weather.o obj/main.o -o bin/weather -lcurl
./bin/weather
```

## Licencia

MIT
