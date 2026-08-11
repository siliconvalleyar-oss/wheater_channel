# API Reference

## Open-Meteo Forecast API

### Endpoint

```
https://api.open-meteo.com/v1/forecast
```

### Parámetros

| Parámetro | Valor | Descripción |
|-----------|-------|-------------|
| latitude | -34.6037 | Latitud de Buenos Aires |
| longitude | -58.3816 | Longitud de Buenos Aires |
| current_weather | true | Incluye clima actual |
| daily | temperature_2m_max,temperature_2m_min | Temperaturas máximas y mínimas |
| timezone | America/Argentina/Buenos_Aires | Zona horaria |

### Respuesta

```json
{
  "current_weather": {
    "temperature": 8.6,
    "weathercode": 1
  },
  "daily": {
    "temperature_2m_max": [11.3],
    "temperature_2m_min": [3.4]
  }
}
```

### Códigos de Clima (WMO)

| Código | Descripción |
|--------|-------------|
| 0 | Cielo despejado |
| 1 | Mayormente despejado |
| 2 | Parcialmente nublado |
| 3 | Nublado |
| 45, 48 | Niebla |
| 51, 53, 55 | Llovizna |
| 61, 63, 65 | Lluvia |
| 80, 81, 82 | Chubascos |
| 95, 96, 99 | Tormenta |

### Notas

- No requiere API key
- Datos en tiempo real
- Licencia: CC-BY-SA 4.0
