# SKILLS.md - Weather Channel

## Descripción

Skill para obtener clima actual de Buenos Aires con temperatura, máxima, mínima y condición.

## Skills Disponibles

### 1. weather-cpp

**Descripción**: Cliente C++ para Open-Meteo API. Compilado con libcurl y nlohmann/json.

**Uso**:
```bash
make run
```

**Archivos**:
- `src/main.cpp`
- `src/weather.cpp`
- `include/weather.hpp`

### 2. weather-bash

**Descripción**: Script bash para obtener clima desde CLI sin compilación.

**Uso**:
```bash
./script_tools/weather.sh
```

### 3. install-deps

**Descripción**: Instala dependencias del sistema (g++, libcurl, json.hpp).

**Uso**:
```bash
sudo ./script_tools/install_deps.sh
```

## API

- **Proveedor**: Open-Meteo
- **Ubicación**: Buenos Aires, Argentina (-34.6037, -58.3816)
- **Datos**: Temperatura actual, máx, mín, código WMO
- **Licencia**: Sin API key, datos abiertos

## Extensiones

Para agregar nueva ciudad:
1. Modificar latitud/longitud en `src/weather.cpp` o `script_tools/weather.sh`
2. Actualizar documentación en `docs/`

Para agregar nueva unidad (Fahrenheit):
1. Extender `weatherCodeToString()` en `src/weather.cpp`
2. Agregar opción en bash script
3. Documentar en `docs/API.md`

## Testing

```bash
make run
```

## Dependencias Externas

- [Open-Meteo](https://open-meteo.com/) - API de clima gratuita
- [nlohmann/json](https://github.com/nlohmann/json) - Parser JSON C++
- [libcurl](https://curl.se/) - Cliente HTTP
