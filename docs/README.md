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
