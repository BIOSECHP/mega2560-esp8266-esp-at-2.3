# Mega 2560 + WiFi ESP8266EX — ESP-AT 2.3.0.0 Adaptation

**BIOSECHP | Verified Technical Procedure — Single-Board Validation**

[Español](#español) · [English](#english)

> **Status:** Technical documentation draft. Tested on one Arduino Mega 2560 + WiFi ESP8266EX combined board. This project is not an official Espressif release. The exact hardware revision has not yet been identified. Compatibility with other board revisions is not guaranteed.

---

## Español

### 1. Resumen

Este repositorio documenta la adaptación de una **imagen oficial Espressif ESP-AT 2.3.0.0 para ESP8266**, utilizada en una tarjeta combinada **Arduino Mega 2560 + WiFi ESP8266EX**, así como las pruebas realizadas para comprobar su funcionamiento y la comunicación UART con el microcontrolador ATmega2560.

**No se desarrolló un firmware nuevo.** Se utilizaron archivos y herramientas oficiales de Espressif, adaptando los parámetros UART y la configuración de la cabecera de memoria flash.

La contribución técnica de **BIOSECHP** consiste en documentar la adaptación específica, el procedimiento empleado, las verificaciones de integridad y los resultados experimentales obtenidos sobre una tarjeta física.

### 2. Hardware verificado

- Microcontrolador ATmega2560.
- Microcontrolador WiFi ESP8266EX.
- Tarjeta combinada Arduino Mega 2560 + WiFi ESP8266EX.
- Convertidor USB-serie CH340.
- Puerto de comunicación identificado durante las pruebas como COM16 (puede variar entre computadores).
- Memoria flash SPI de 4 MiB, identificada mediante el chip marcado `25VQ32`.
- Selector UART `RXD0/TXD0` / `RXD3/TXD3`.
- Banco de ocho interruptores DIP.
- Firmware original identificado mediante `AT+GMR`: `AT version:1.1.0.0`.

**Alcance:** las pruebas descritas corresponden a una unidad física. La identificación comercial genérica de la tarjeta no garantiza que otras revisiones utilicen las mismas conexiones internas.

### 3. Procedimiento de adaptación

El procedimiento realizado comprendió las siguientes etapas:

1. Identificar el microcontrolador ESP8266EX y su memoria flash SPI de 4 MiB.

2. Obtener una copia completa del firmware original de la tarjeta y verificarla frente al dispositivo físico mediante `esptool verify-flash`.

3. Obtener la distribución oficial **ESP-AT 2.3.0.0 para ESP8266**, incluida la imagen precompilada para ESP-WROOM-02.

4. Obtener las herramientas oficiales de Espressif y utilizar la rama `release/v2.3.0.0_esp8266` del repositorio ESP-AT.

5. Generar los parámetros correspondientes al perfil `WROOM-02-N`, configurado con:

   - UART TX: GPIO1.
   - UART RX: GPIO3.
   - Control de flujo CTS/RTS desactivado.

6. Sustituir el bloque de parámetros de **4096 bytes** en el desplazamiento `0xF1000` de una copia de la imagen factory oficial de 2 MiB.

7. Preparar la imagen adaptada utilizando el modo de memoria flash **DOUT a 40 MHz**.

8. Grabar la imagen en el desplazamiento `0x0` de la memoria flash del ESP8266.

9. Verificar la escritura mediante la herramienta de programación y comprobar el arranque con comandos AT.

10. Realizar pruebas de comunicación con el ATmega2560 mediante `Serial3`.

**Precaución:** este procedimiento describe una adaptación comprobada en una unidad. No debe aplicarse indiscriminadamente a otras tarjetas sin verificar previamente la revisión del hardware, el tamaño de la memoria, las conexiones UART y los parámetros de arranque.

### 4. Resultados experimentales

| Prueba | Resultado |
|---|---|
| Programación del ATmega2560 mediante Blink | PASS |
| Comunicación USB mediante CH340 | PASS |
| Respuesta del ESP8266 al comando `AT` | PASS |
| Identificación mediante `AT+GMR` | PASS |
| Firmware ESP-AT 2.3.0.0, perfil WROOM-02-N | PASS |
| Búsqueda de redes inalámbricas mediante `AT+CWLAP` | PASS |
| Comunicación USB → Mega → ESP8266 por `Serial3` | PASS |
| Comunicación bidireccional entre Mega y ESP8266 | PASS |
| Ejecución autónoma de comandos AT desde el Mega | PASS |
| Consulta del modo WiFi mediante `AT+CWMODE?` | PASS |
| Asociación autónoma a una red WiFi | NO ENSAYADA |
| Servidor web controlado desde el Mega | NO ENSAYADO |

### 5. Validación de comunicación autónoma

Se desarrolló un sketch de prueba ejecutado en el ATmega2560 que envía automáticamente comandos al ESP8266 a través de `Serial3`.

La comunicación se configuró a **115200 baudios**.

Los comandos ejecutados fueron:

```text
AT
AT+GMR
AT+CWMODE?
```

Los resultados observados incluyeron:

```text
MEGA ENVIA: AT
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

**Interpretación técnica:** el ATmega2560 generó comandos AT sin intervención manual, los transmitió al ESP8266 mediante UART y recibió respuestas válidas.

La respuesta `+CWMODE:1` confirma que el ESP8266 se encontraba configurado en modo estación WiFi. No demuestra, por sí sola, que existiera una conexión activa a una red inalámbrica.

Este resultado constituye una validación adicional de la comunicación entre ambos microcontroladores después de actualizar el firmware.

El registro detallado de las verificaciones se encuentra en [docs/VALIDACION.md](docs/VALIDACION.md).

### 6. Integridad y procedencia

Los archivos utilizados durante las pruebas fueron identificados mediante huellas criptográficas SHA-256.

#### Programa puente Serial3

- Archivo de respaldo: `PUENTE_SERIAL3_VALIDADO.ino`.
- Algoritmo: SHA-256.
- Huella verificada:

`716698717A8CC1935F62FA91EE1DE4135EF882BEC7136BD6DCA6D9F920CC4727`

La comparación entre el programa original y su copia produjo el resultado `COINCIDEN: True`.

#### Imagen intermedia DIO a 80 MHz

- Archivo: `ESP_AT_2.3_MEGA_WIFI_CANDIDATO.bin`.
- Configuración de partida: DIO a 80 MHz.
- SHA-256:

`6898EC86335719D1E51BE83E45F234327277DA9A8634AFF4D4685E2E5202217F`

Esta huella corresponde a una imagen intermedia de preparación y **no identifica el binario definitivo grabado**.

#### Imagen final adaptada — DOUT a 40 MHz

- Archivo: `ESP_AT_2.3_MEGA_WIFI_DOUT_40M.bin`.
- Tamaño: **2.097.152 bytes (2 MiB)**.
- Firmware base: Espressif ESP-AT 2.3.0.0.
- Perfil de parámetros UART: `WROOM-02-N`.
- Configuración de memoria flash: **DOUT, 40 MHz**.
- Algoritmo de integridad: SHA-256.

**Huella SHA-256 definitiva:**

`876C2FA5E777849E18DC2D8940DA639E7A205B88D7CFEFEBD2FDC1966BBCCF44`

**Nota de verificación:** esta huella identifica el archivo binario final utilizado durante la actualización y verificado mediante cálculo local. No constituye una nueva lectura de los 4 MiB completos de la memoria física del ESP8266.

Durante el procedimiento de programación, la herramienta reportó la verificación satisfactoria de la escritura.

El respaldo original completo de 4 MiB se conserva de manera independiente y **no se distribuye en este repositorio**.

Tampoco se incorporan contraseñas, credenciales inalámbricas ni datos privados del dispositivo.

### 7. Fuentes y atribución

Este proyecto utiliza tecnología y herramientas desarrolladas por **Espressif Systems**.

Repositorios oficiales:

- [Espressif ESP-AT](https://github.com/espressif/esp-at)
- [Espressif esptool](https://github.com/espressif/esptool)

El código fuente, los binarios oficiales, las herramientas y las funcionalidades originales de ESP-AT pertenecen a sus respectivos autores y se encuentran sujetos a sus condiciones de licencia.

**BIOSECHP no reclama autoría sobre el firmware ESP-AT original.**

La contribución documentada corresponde al procedimiento de adaptación de configuración, la integración con el hardware evaluado y la validación experimental.

### 8. Limitaciones

La validación presenta las siguientes limitaciones:

- Las pruebas se realizaron sobre una única tarjeta física.
- La revisión exacta de la PCB no ha sido identificada.
- No se ha demostrado compatibilidad con otras variantes comerciales.
- No se ha realizado una prueba documentada de asociación autónoma a una red WiFi desde el ATmega2560.
- No se ha validado todavía un servidor web controlado por el Mega.
- La documentación de reconstrucción reproducible está pendiente de completar.

Estas limitaciones se mantienen explícitas para evitar interpretaciones que excedan los resultados experimentales obtenidos.

### 9. Próximos pasos antes de la publicación pública

- Identificar la referencia y revisión exacta de la PCB.
- Incorporar fotografías del hardware y de los interruptores DIP.
- Documentar las configuraciones UART utilizadas.
- Incorporar los comandos exactos y scripts reproducibles de adaptación.
- Verificar la procedencia y las licencias de los archivos que eventualmente se distribuyan.
- Completar la revisión de seguridad y privacidad.
- Realizar una revisión final de la documentación en ambos idiomas.

---

## English

### Overview

This repository documents a **hardware-specific configuration adaptation** of Espressif's official **ESP-AT 2.3.0.0** firmware for an **Arduino Mega 2560 + WiFi ESP8266EX** combined board.

It also documents the experimental verification procedures performed on one physical board.

**This project does not implement a new ESP8266 firmware.**

The firmware foundation, original functionality and development tools originate from Espressif Systems.

BIOSECHP's contribution consists of documenting the hardware-specific configuration adaptation, the integration procedure, integrity verification and observed experimental results.

### Hardware

The tested system included:

- ATmega2560 microcontroller.
- ESP8266EX WiFi microcontroller.
- CH340 USB-to-serial converter.
- 4 MiB SPI flash memory, marked `25VQ32`.
- Eight-position DIP switch bank.
- UART selector with `RXD0/TXD0` and `RXD3/TXD3` positions.

The original ESP8266 firmware reported AT version 1.1.0.0.

The exact PCB revision remains unidentified.

### Firmware adaptation

A complete 4 MiB backup of the original ESP8266 flash was created and verified against the physical device before the upgrade.

The official ESP-AT 2.3.0.0 image was adapted using the `WROOM-02-N` UART configuration:

- TX: GPIO1.
- RX: GPIO3.
- Hardware flow control disabled.

The corresponding 4096-byte configuration parameter block was incorporated into a copy of the official factory image.

The final image was prepared with a **DOUT, 40 MHz** flash boot-header configuration.

The adapted firmware was programmed and subsequently tested on the target board.

### Experimental validation

| Test | Result |
|---|---|
| ATmega2560 programming | PASS |
| USB communication through CH340 | PASS |
| ESP8266 AT response | PASS |
| ESP-AT 2.3.0.0 identification | PASS |
| WiFi access-point scanning | PASS |
| Bidirectional Mega-to-ESP8266 Serial3 communication | PASS |
| Autonomous AT command transmission from ATmega2560 | PASS |
| WiFi station-mode query | PASS |
| Autonomous WiFi association | NOT TESTED |
| Browser-based web server | NOT TESTED |

### Autonomous UART validation

A dedicated sketch running on the ATmega2560 automatically transmitted the following commands to the ESP8266 over `Serial3` at 115200 baud:

```text
AT
AT+GMR
AT+CWMODE?
```

The ESP8266 returned valid responses, including:

```text
AT version:2.3.0.0
Bin version:2.3.0(WROOM-02-N)
+CWMODE:1
OK
```

This demonstrates autonomous AT command transmission and response reception between the ATmega2560 and the ESP8266.

The `+CWMODE:1` result confirms station-mode configuration, not an active WiFi connection.

### Final firmware image integrity

**File:** `ESP_AT_2.3_MEGA_WIFI_DOUT_40M.bin`

**Size:** 2,097,152 bytes (2 MiB)

**Flash configuration:** DOUT, 40 MHz

**SHA-256:**

`876C2FA5E777849E18DC2D8940DA639E7A205B88D7CFEFEBD2FDC1966BBCCF44`

This hash identifies the locally verified final firmware file used during the upgrade. It does not represent a separate post-flashing readback of the full 4 MiB device memory.

### Attribution

Original firmware and tools:

- [Espressif ESP-AT](https://github.com/espressif/esp-at)
- [Espressif esptool](https://github.com/espressif/esptool)

**BIOSECHP documents the tested configuration adaptation and integration procedure, not the original Espressif firmware implementation.**

All upstream components remain subject to their respective licenses.

### Safety and compatibility

The described adaptation has been tested on one physical board.

**Do not flash the image to an unverified hardware revision.**

Identify the target hardware, verify memory configuration, inspect UART connections and create a verified device backup before programming.

No original full-device backup, network credentials or private device data are distributed in this repository.

### Documentation status

This repository remains under technical preparation.

The documented bench tests were successful within their stated scope. Complete reproduction instructions, exact board identification and cross-board compatibility testing remain future work.

Detailed Spanish validation records: [docs/VALIDACION.md](docs/VALIDACION.md).

---

**BIOSECHP — Engineering, Embedded Systems, Hardware Integration and Technical Validation**

*Technical documentation — November 15, 2026.*
