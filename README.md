# Mega 2560 + WiFi ESP8266EX — ESP-AT 2.3.0.0 adaptation

**BIOSECHP | Verified technical procedure (single-board validation)**

[Español](#español) · [English](#english)

> **Status:** Documentation draft. Tested on one Mega 2560 + WiFi ESP8266EX combined board. This project is **not** an official Espressif release, and the exact hardware revision has not yet been identified. Do not assume compatibility with other boards.

## Español

### Resumen

Este repositorio documenta la adaptación de una **imagen oficial Espressif ESP-AT 2.3.0.0 para ESP8266**, utilizada en una tarjeta combinada **Arduino Mega 2560 + WiFi ESP8266EX**, y las pruebas realizadas para comprobar el funcionamiento del módulo y su comunicación UART con el ATmega2560.

**No se desarrolló un firmware nuevo:** se utilizaron archivos y herramientas oficiales de Espressif, modificando los parámetros UART de la imagen y su cabecera de configuración de memoria flash.

### Hardware verificado

- ATmega2560 y ESP8266EX integrados en una tarjeta Mega + WiFi.
- Convertidor USB-serie CH340, detectado durante las pruebas como COM16 (el número puede variar).
- Memoria flash SPI de 4 MiB identificada en la tarjeta (chip marcado `25VQ32`).
- Selector UART `RXD0/TXD0` / `RXD3/TXD3` y banco de ocho interruptores DIP.
- Firmware original informado por `AT+GMR`: `AT version:1.1.0.0`.

### Procedimiento de adaptación: resumen técnico

1. Respaldar **los 4 MiB de la memoria flash original** y comprobar el respaldo frente al dispositivo físico mediante `esptool verify-flash`.
2. Obtener la distribución oficial **ESP-AT 2.3.0.0 para ESP8266**, incluyendo la imagen precompilada para `ESP-WROOM-02` y el repositorio de herramientas de Espressif, rama `release/v2.3.0.0_esp8266`.
3. Generar los parámetros oficiales para el perfil **`WROOM-02-N`**, que utiliza UART TX **GPIO1** y RX **GPIO3**, sin control de flujo CTS/RTS.
4. Sustituir el bloque de parámetros de **4096 bytes** en el desplazamiento **`0xF1000`** de una copia de la imagen factory oficial de 2 MiB.
5. Preparar la variante con **modo flash DOUT y frecuencia de 40 MHz** mediante herramientas Espressif.
6. Grabar la imagen adaptada en offset `0x0`, verificar la escritura y comprobar el arranque con comandos AT.

**Precaución:** este resumen describe lo realizado en una unidad probada; no es todavía una receta universal de flasheo. Un firmware incorrecto o una configuración errónea de arranque pueden impedir el inicio del ESP8266. Conservar una copia verificada antes de programar.

### Resultados obtenidos

| Prueba | Resultado |
|---|---|
| ATmega2560, programa Blink | PASS |
| ESP8266, `AT` | PASS (`OK`) |
| ESP8266, `AT+GMR` | PASS — `ESP-AT 2.3.0.0`, perfil `WROOM-02-N` |
| ESP8266, `AT+CWLAP` | PASS — detección de redes WiFi |
| USB → Mega → ESP8266 mediante `Serial3` | PASS — comandos y respuestas |
| Mega → ESP8266 mediante `Serial3` **sin introducir comandos manualmente** | PASS — `AT`, `AT+GMR`, `AT+CWMODE?` |
| Conexión a una red WiFi y servidor web | **No ensayados en esta validación** |

En la prueba autónoma, el sketch del ATmega2560 generó los comandos por `Serial3` a **115200 baudios**. Se recibió `+CWMODE:1` y `OK` para `AT+CWMODE?`. Esto confirma el modo estación configurado, **no** una conexión WiFi activa.

Consulta [docs/VALIDACION.md](docs/VALIDACION.md) para el alcance y los resultados completos.

### Integridad y procedencia

- Copia verificada del sketch **puente Serial3** (SHA-256): `716698717A8CC1935F62FA91EE1DE4135EF882BEC7136BD6DCA6D9F920CC4727`.
- Imagen adaptada DIO 80 MHz previa a la variante DOUT (SHA-256): `6898EC86335719D1E51BE83E45F234327277DA9A8634AFF4D4685E2E5202217F`.
- **No se distribuye** el respaldo original de 4 MiB ni datos de redes o credenciales.

> Nota: el valor SHA-256 de la imagen intermedia anterior **no identifica** la imagen DOUT/40 MHz efectivamente grabada. La huella completa del binario final debe verificarse antes de publicarla como artefacto reproducible.

### Fuentes y atribución

- Espressif ESP-AT: https://github.com/espressif/esp-at
- Espressif esptool: https://github.com/espressif/esptool

ESP-AT, su código, binarios originales y herramientas son de **Espressif Systems**. BIOSECHP documenta la adaptación de configuración y su validación experimental. No se atribuye autoría sobre el firmware original.

### Próximos pasos antes de hacer público el repositorio

- Fotografiar e identificar la **revisión exacta de la PCB**, y comprobar el esquema del selector UART/DIP.
- Incorporar comandos reproducibles revisados y artefactos generados a partir de fuentes oficiales, sin respaldos privados.
- Confirmar la licencia y la forma de redistribución de cada componente.
- Revisar los documentos para evitar divulgar identificadores y datos privados.

---

## English

### Overview

This repository documents a **hardware-specific configuration adaptation** of Espressif's official **ESP-AT 2.3.0.0** firmware for an **Arduino Mega 2560 + WiFi ESP8266EX** combined board, together with observed bench-test results. **This is not an independently developed firmware.**

A verified 4 MiB backup of the board's original ESP8266 flash was made before upgrading. The official ESP-AT firmware image was configured for the **WROOM-02-N UART pin assignment (TX GPIO1, RX GPIO3, hardware flow control disabled)**; the image's flash boot header was configured for **DOUT at 40 MHz**.

**Validated on one physical unit:** AT commands, firmware identification, WiFi access-point scan, bidirectional USB-to-Mega-to-ESP UART bridge, and autonomous AT command transmission from the ATmega2560 over `Serial3` at 115200 baud. **WiFi association and a browser-based web server have not yet been tested.**

Detailed Spanish test notes: [docs/VALIDACION.md](docs/VALIDACION.md).

**Upstream authors:** Espressif Systems — https://github.com/espressif/esp-at. BIOSECHP documents the tested integration procedure and configuration adaptation, not the original firmware implementation.

**Safety:** Do not flash an unverified image to another hardware revision. Create and verify a full device backup first. No original 4 MiB board backup or credentials are included.
