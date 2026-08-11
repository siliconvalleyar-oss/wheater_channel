# Build Guide

## Requisitos

- g++ 17+
- libcurl4-openssl-dev
- make

## Compilación

```bash
make
```

Salida: `bin/weather`

## Ejecución

```bash
make run
```

## Limpieza

```bash
make clean
```

Elimina `obj/` y `bin/`.

## Instalación de Dependencias

```bash
sudo ./script_tools/install_deps.sh
```

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

## Estructura de Build

- `bin/weather` - Ejecutable compilado
- `obj/*.o` - Archivos objeto intermedios
- `bin/` y `obj/` son gitignored

## SemVer Tags

Cada push debe tener un tag semántico a partir de `v1.0.0`.

```bash
# Crear tag patch (bugfix)
./script_tools/version.sh patch

# Crear tag minor (nueva funcionalidad)
./script_tools/version.sh minor

# Crear tag major (cambio incompatible)
./script_tools/version.sh major
```

O usando Make:

```bash
make tag  # equivalente a version.sh patch
```

El script detecta automáticamente el último tag, incrementa la versión, crea el tag y lo pushea.

## Troubleshooting

### Error: curl/curl.h no encontrado

```bash
sudo apt-get install libcurl4-openssl-dev
```

### Error: nlohmann/json no encontrado

```bash
./script_tools/install_deps.sh
```

### Permisos denegados en install_deps.sh

```bash
chmod +x script_tools/install_deps.sh
sudo ./script_tools/install_deps.sh
```
