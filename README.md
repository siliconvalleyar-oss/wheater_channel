# Weather Channel

> Aplicación CLI multi-interfaz para consultar el clima de Buenos Aires en tiempo real.

Incluye un cliente **C++17** compilado y un script **Bash**, ambos alimentados por la API gratuita de **Open-Meteo**. Sin API key, sin registro, datos en tiempo real.

---

## Tabla de Contenidos

- [Características](#características)
- [Estructura del Proyecto](#estructura-del-proyecto)
- [Requisitos](#requisitos)
- [Instalación de Dependencias](#instalación-de-dependencias)
- [Uso Rápido](#uso-rápido)
  - [C++ (compilado)](#c-compilado)
  - [Bash](#bash)
- [Compilación Manual](#compilación-manual)
- [Documentación](#documentación)
- [Skills](#skills)
- [API Reference](#api-reference)
- [Extensibilidad](#extensibilidad)
- [Testing](#testing)
- [Licencia](#licencia)

---

## Características

- Temperatura actual
- Temperatura máxima y mínima del día
- Descripción de la condición climática (WMO codes)
- Interfaz CLI en español
- Multiplataforma (Linux, macOS)
- Sin dependencias externas complejas

---

## Estructura del Proyecto

```
weather_channel/
├── src/                      # Código fuente C++
│   ├── main.cpp              # Punto de entrada
│   └── weather.cpp           # Lógica HTTP y parsing
├── include/                  # Headers públicos
│   ├── weather.hpp           # API pública del módulo
│   └── json.hpp              # nlohmann/json (header-only)
├── obj/                      # Archivos objeto (.o) generados (gitignored)
├── bin/                      # Binario compilado (gitignored)
├── script_tools/             # Scripts bash auxiliares
│   ├── weather.sh            # Cliente bash del clima
│   └── install_deps.sh       # Instalador de dependencias
├── docs/                     # Documentación técnica
│   ├── README.md             # Documentación del usuario
│   ├── BUILD.md              # Guía de compilación
│   └── API.md                # Referencia de API y WMO codes
├── Makefile                  # Build system
├── SKILLS.md                 # Skills del proyecto
├── .gitignore                # Exclusiones de git
└── README.md                 # Este archivo
```

---

## Requisitos

| Herramienta | Versión mínima | Propósito |
|-------------|---------------|-----------|
| g++ | 17 | Compilador C++ |
| libcurl4-openssl-dev | - | Cliente HTTP |
| make | - | Build system |
| curl | - | Usado por script bash |

---

## Instalación de Dependencias

```bash
sudo ./script_tools/install_deps.sh
```

O manualmente:

```bash
sudo apt-get update
sudo apt-get install -y libcurl4-openssl-dev
```

---

## Uso Rápido

### C++ (compilado)

```bash
make run
```

Salida esperada:

```
Buenos Aires
Temperatura actual: 8.6 C
Maxima: 11.3 C
Minima: 3.4 C
Condicion: Mayormente despejado
```

### Bash

```bash
./script_tools/weather.sh
```

Salida esperada:

```
📍 Buenos Aires
🌡️  Temperatura actual: 8.6°C
🔺 Máxima: 11.3°C
🔻 Mínima: 3.4°C
☁️  Condición: Mayormente despejado
```

---

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

---

## Documentación

| Documento | Descripción |
|-----------|-------------|
| [docs/README.md](docs/README.md) | Documentación del usuario |
| [docs/BUILD.md](docs/BUILD.md) | Guía de compilación y troubleshooting |
| [docs/API.md](docs/API.md) | Referencia de API y códigos WMO |
| [SKILLS.md](SKILLS.md) | Skills, extensibilidad y arquitectura |

---

## Skills

El proyecto expone tres skills operativos documentados en `SKILLS.md`:

| Skill | Descripción | Comando |
|-------|-------------|---------|
| `weather-cpp` | Cliente C++ para Open-Meteo | `make run` |
| `weather-bash` | Cliente Bash para Open-Meteo | `./script_tools/weather.sh` |
| `install-deps` | Instala dependencias del sistema | `sudo ./script_tools/install_deps.sh` |

---

## API Reference

- **Proveedor**: Open-Meteo
- **Endpoint**: `https://api.open-meteo.com/v1/forecast`
- **Ubicación**: Buenos Aires, Argentina
  - Latitud: `-34.6037`
  - Longitud: `-58.3816`
- **Zona horaria**: `America/Argentina/Buenos_Aires`
- **API Key**: No requerida
- **Licencia de datos**: CC-BY-SA 4.0

Ver [docs/API.md](docs/API.md) para parámetros completos, respuesta JSON y tabla de códigos WMO.

---

## Extensibilidad

### Agregar una nueva ciudad

Modificar latitud/longitud en:
- `src/weather.cpp:20` (URL de Open-Meteo)
- `script_tools/weather.sh:4` (URL de Open-Meteo)

### Agregar soporte Fahrenheit

1. Extender `weatherCodeToString()` en `src/weather.cpp`
2. Agregar flag `--fahrenheit` en `script_tools/weather.sh`
3. Documentar en `docs/API.md`

### Agregar tests

1. Crear directorio `tests/`
2. Usar un framework como [Catch2](https://github.com/catchorg/Catch2)
3. Mockear `fetchWeatherJSON()` para testing unitario

---

## Testing

```bash
make run
```

Ambas interfaces (C++ y Bash) consultan el mismo endpoint y deberían devolver valores equivalentes.

---

## Troubleshooting

### `curl/curl.h: No such file or directory`

```bash
sudo apt-get install libcurl4-openssl-dev
```

### `nlohmann/json.hpp: No such file or directory`

```bash
./script_tools/install_deps.sh
```

### `Permission denied` en scripts

```bash
chmod +x script_tools/*.sh
```

---

## Licencia

MIT
