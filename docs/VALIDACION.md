# BIOSECHP — Registro de validación (15 de noviembre de 2026)

## Alcance

Placa combinada Arduino Mega 2560 + WiFi ESP8266EX, con interfaz USB CH340. Pruebas de banco realizadas sobre **una unidad**. Número de revisión de la PCB pendiente de confirmar. No se han probado motores, conexión a un punto de acceso ni servidor web.

## Firmware

Respuesta obtenida por `AT+GMR` a través del puente UART y posteriormente mediante el sketch autónomo:

```text
AT version:2.3.0.0(2522d50 - ESP8266 - Aug 13 2025 08:41:04)
SDK version:v3.4-112-gc965e03
compile time(795c42d):Aug 14 2025 03:16:30
Bin version:2.3.0(WROOM-02-N)

OK
```

## Validación UART: puente manual

- USB CH340 detectado en las pruebas como COM16.
- ATmega2560: `Serial.begin(115200)` hacia el PC.
- ATmega2560: `Serial3.begin(115200)` hacia el ESP8266.
- Selector pequeño en `RXD3/TXD3`.
- Durante la prueba de comunicación: DIP 1, 2, 3 y 4 en ON; DIP 5, 6, 7 y 8 en OFF (configuración reportada como funcional en la unidad).
- Comandos manuales enviados en Monitor Serie: `AT`, `AT+GMR`, `AT+CWMODE?`, `AT+CWLAP`.
- Resultado: respuestas correctas y búsqueda WiFi completada.

## Validación UART: emisión autónoma desde Mega

- Programa `BIOSECHP_MEGA_ESP8266_AUTONOMO.ino` compilado y cargado en el ATmega2560.
- Tras el arranque, el propio ATmega2560 transmitió secuencialmente `AT`, `AT+GMR` y `AT+CWMODE?` por `Serial3` a 115200 baudios.
- El Monitor Serie se usó **solo para observar** las respuestas, no para introducir comandos.
- Resultados proporcionados por el operador:

```text
AT

OK

--- FIN RESPUESTA ---

MEGA ENVIA: AT+GMR
AT+GMR
AT version:2.3.0.0(2522d50 - ESP8266 - Aug 13 2025 08:41:04)
SDK version:v3.4-112-gc965e03
compile time(795c42d):Aug 14 2025 03:16:30
Bin version:2.3.0(WROOM-02-N)

OK

--- FIN RESPUESTA ---

MEGA ENVIA: AT+CWMODE?
AT+CWMODE?
+CWMODE:1

OK

--- FIN RESPUESTA ---

PRUEBA FINALIZADA
```

### Dictamen

**PASS:** emisión autónoma de tres comandos AT desde el ATmega2560, recepción de respuestas válidas y lectura correcta de la versión ESP-AT 2.3.0.0 y del modo WiFi estación.

**Límite:** `+CWMODE:1` confirma la configuración de modo estación, no acredita asociación a una red ni conexión a Internet. La existencia de una respuesta `OK` no implica que exista un servidor web funcional.

## Respaldo y trazabilidad

- Respaldo integral original ESP8266 (4 MiB): conservado fuera del repositorio público. Fue verificado mediante lectura/comparación con `esptool`.
- Programa puente `PUENTE_SERIAL3_VALIDADO.ino`: SHA-256 `716698717A8CC1935F62FA91EE1DE4135EF882BEC7136BD6DCA6D9F920CC4727`; original y copia coinciden.
- Ubicación local del programa puente: `D:\RESPALDOS_ARDUINO\MEGA_WIFI_ESP8266\PRUEBAS\PUENTE_SERIAL3_VALIDADO\PUENTE_SERIAL3_VALIDADO.ino`.
- Programa autónomo reportado en disco D: (1656 bytes) y compilado exitosamente. Falta calcular su hash SHA-256 si se incluirá en el manifiesto de publicación.

## Pendientes de publicación

1. Registrar fotografías y revisión física de la PCB.
2. Validar permisos/licencias y scripts antes de publicar binarios o cambios de parámetros.
3. Comprobar que los registros y fotografías no contengan contraseñas ni identificadores privados.
4. Conservar el historial de pruebas y cualquier condición de hardware específica; no generalizar compatibilidad a todas las placas Mega + WiFi.
