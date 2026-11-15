# Mega 2560 + WiFi ESP8266EX — ESP-AT 2.3.0.0 Hardware Adaptation

**BIOSECHP | Technical integration and experimental validation**

[Español](#español) · [English](#english)

> **Project scope:** A configuration adaptation of Espressif's official ESP-AT 2.3.0.0 firmware, experimentally validated on one Arduino Mega 2560 + WiFi ESP8266EX combined board. This is an independent integration report, **not an official Espressif release**. Results must not be generalized to unverified board revisions.

---

## Español

### 1. Presentación

BIOSECHP documenta la adaptación de configuración de una **imagen oficial ESP-AT 2.3.0.0 de Espressif** para una tarjeta combinada **Arduino Mega 2560 + WiFi ESP8266EX**, así como los resultados de su verificación experimental.

El objetivo fue actualizar el firmware AT del ESP8266 y comprobar que el módulo continuara operando tanto por su interfaz serie como a través de la comunicación interna con el **ATmega2560**.

**No se desarrolló un firmware nuevo ni se modificó el código funcional de ESP-AT.** El trabajo consistió en adaptar parámetros de comunicación UART y la cabecera de configuración de la memoria flash, utilizando herramientas y archivos oficiales de Espressif, y verificar el resultado en hardware real.

### 2. Plataforma evaluada

- Tarjeta integrada con microcontroladores **ATmega2560** y **ESP8266EX**.
- Convertidor USB-serie **CH340**; durante las pruebas Windows lo identificó como **COM16** (número dependiente del equipo).
- Memoria flash SPI identificada por la marca **`25VQ32`**, con capacidad de **4 MiB**.
- Banco de **ocho interruptores DIP** y selector UART rotulado **`RXD0/TXD0` / `RXD3/TXD3`**.
- Firmware AT original reportado por `AT+GMR`: **`AT version:1.1.0.0`**.

La revisión exacta de la placa de circuito impreso (PCB) **no se ha identificado**. Por ello, las configuraciones descritas se refieren a la unidad ensayada y no constituyen una matriz universal de compatibilidad.

### 3. Adaptación realizada

Se efectuaron las siguientes operaciones, conservando un respaldo verificable antes de programar:

1. Identificación del ESP8266EX y de la memoria flash SPI de **4 MiB**.
2. Lectura de los **4 MiB** del dispositivo original y verificación del respaldo frente a la memoria física mediante `esptool verify-flash`.
3. Obtención de la distribución oficial **ESP-AT 2.3.0.0 para ESP8266**, incluida la imagen factory para **ESP-WROOM-02**, y de las herramientas de la rama **`release/v2.3.0.0_esp8266`** de ESP-AT.
4. Generación de parámetros con la herramienta oficial para el perfil **`WROOM-02-N`**: **TX GPIO1**, **RX GPIO3**, y control de flujo **CTS/RTS deshabilitado**.
5. Sustitución del bloque de parámetros de **4.096 bytes** en el desplazamiento **`0xF1000`** de una copia de la imagen factory oficial de **2 MiB**.
6. Preparación de la variante con cabecera de arranque para memoria flash **DOUT a 40 MHz**.
7. Escritura de la imagen adaptada a partir de **`0x0`**, con verificación reportada por la herramienta de programación.
8. Validación del arranque y de la comunicación con comandos AT, búsqueda de redes WiFi y pruebas UART a través del ATmega2560.

**Advertencia de compatibilidad:** este resumen documenta un procedimiento ejecutado satisfactoriamente en una unidad específica. Antes de reproducirlo en otra tarjeta deben comprobarse la revisión de PCB, el cableado UART, la capacidad de flash, la configuración de arranque y la posibilidad de restaurar un respaldo. Un binario inadecuado puede impedir que el ESP8266 arranque.

### 4. Resultados de validación

| Ensayo | Resultado |
|---|---|
| Programación del ATmega2560 con `Blink` | **PASS** |
| Comunicación USB mediante CH340 | **PASS** |
| Respuesta del ESP8266 a `AT` | **PASS** |
| Identificación mediante `AT+GMR` | **PASS** — ESP-AT 2.3.0.0 (`WROOM-02-N`) |
| Exploración de puntos de acceso con `AT+CWLAP` | **PASS** |
| Puente USB → Mega → ESP8266 por `Serial3` | **PASS** — comandos y respuestas |
| Envío autónomo desde el Mega por `Serial3` | **PASS** — `AT`, `AT+GMR`, `AT+CWMODE?` |
| Asociación autónoma a una red WiFi | **NO ENSAYADO** |
| Servidor web gestionado por el Mega | **NO ENSAYADO** |

Los resultados corresponden a las pruebas de laboratorio efectuadas en la unidad evaluada. El registro de alcance y evidencias se encuentra en [docs/VALIDACION.md](docs/VALIDACION.md).

### 5. Comunicación autónoma entre microcontroladores

Para complementar la prueba de puente serie, se ejecutó un sketch en el **ATmega2560** capaz de **generar por sí mismo** los comandos de diagnóstico y transmitirlos al ESP8266 mediante **`Serial3` a 115200 baudios**, sin introducir los comandos manualmente desde el computador.

Comandos enviados por el Mega:

```text
AT
AT+GMR
AT+CWMODE?
```

Fragmento representativo de las respuestas registradas:

```text
MEGA ENVIA: AT
AT
OK

MEGA ENVIA: AT+GMR
AT+GMR
AT version:2.3.0.0(2522d50 - ESP8266 - Aug 13 2025 08:41:04)
SDK version:v3.4-112-gc965e03
compile time(795c42d):Aug 14 2025 03:16:30
Bin version:2.3.0(WROOM-02-N)
OK

MEGA ENVIA: AT+CWMODE?
AT+CWMODE?
+CWMODE:1
OK

PRUEBA FINALIZADA
```

Estos resultados confirman **transmisión y recepción de comandos AT entre ambos procesadores** en la configuración ensayada. La respuesta **`+CWMODE:1`** identifica el **modo estación**, pero **no acredita una asociación activa a una red WiFi**.

### 6. Integridad y trazabilidad

Los siguientes identificadores criptográficos corresponden a archivos conservados durante el procedimiento:

| Artefacto | SHA-256 |
|---|---|
| Respaldo verificado del sketch puente `PUENTE_SERIAL3_VALIDADO.ino` | `716698717A8CC1935F62FA91EE1DE4135EF882BEC7136BD6DCA6D9F920CC4727` |
| Imagen intermedia DIO/80 MHz `ESP_AT_2.3_MEGA_WIFI_CANDIDATO.bin` | `6898EC86335719D1E51BE83E45F234327277DA9A8634AFF4D4685E2E5202217F` |
| **Imagen final DOUT/40 MHz** `ESP_AT_2.3_MEGA_WIFI_DOUT_40M.bin` | **`876C2FA5E777849E18DC2D8940DA639E7A205B88D7CFEFEBD2FDC1966BBCCF44`** |

**Imagen final:** 2.097.152 bytes (**2 MiB**); ESP-AT 2.3.0.0 oficial como base, perfil de parámetros UART **WROOM-02-N**, cabecera flash **DOUT a 40 MHz**.

La huella de la imagen final se obtuvo mediante cálculo SHA-256 sobre el archivo local utilizado para programar. **No equivale a una nueva lectura independiente de los 4 MiB completos de la memoria física posterior a la actualización.** La herramienta de escritura sí informó verificación satisfactoria de la grabación.

La huella de la imagen **intermedia DIO/80 MHz** no debe confundirse con la de la imagen final **DOUT/40 MHz**.

**Protección de información:** el respaldo original completo de **4 MiB** no forma parte de este repositorio; tampoco se publican claves, credenciales WiFi ni datos privados del dispositivo.

### 7. Procedencia y reconocimiento

El firmware original, los comandos AT y las herramientas de base son desarrollos de **Espressif Systems**:

- [ESP-AT — repositorio oficial](https://github.com/espressif/esp-at)
- [esptool — repositorio oficial](https://github.com/espressif/esptool)

**BIOSECHP no se atribuye la autoría del firmware ESP-AT ni de las herramientas originales.** La contribución documentada es la adaptación de configuración a un hardware concreto y su verificación experimental. Cualquier redistribución de componentes de terceros deberá respetar sus licencias correspondientes.

### 8. Alcance y limitaciones de los resultados

La actualización y las pruebas descritas fueron satisfactorias **en una sola unidad física**. La revisión exacta de la PCB no se ha identificado y no se ha demostrado que otras versiones comerciales utilicen el mismo cableado, parámetros de memoria o disposición de interruptores.

Las pruebas comprenden arranque, identificación del firmware, detección de redes, puente UART bidireccional y ejecución autónoma de comandos AT desde el ATmega2560. **No comprenden** una prueba de conexión autónoma a una red WiFi ni la puesta en funcionamiento de un servidor web administrado por el Mega.

Estas delimitaciones permiten interpretar los resultados sin atribuirles un alcance mayor al demostrado.

### 9. Alcance de la contribución y evolución técnica

Esta contribución de **BIOSECHP** presenta una **adaptación funcional y experimentalmente validada** de la configuración de ESP-AT 2.3.0.0 para la plataforma física evaluada. Su utilidad principal es aportar una referencia técnica trazable para comprender la compatibilidad entre el firmware oficial de Espressif y determinados diseños combinados Mega 2560 + ESP8266EX.

El proyecto puede ampliarse mediante documentación específica de revisiones de PCB, instrucciones automatizadas de reproducción y nuevas pruebas en otras variantes de hardware. Esas ampliaciones serían resultados independientes y **no alteran ni amplían retroactivamente** las verificaciones expuestas en este documento.

---

## English

### 1. Overview

This repository documents a **hardware-specific configuration adaptation of Espressif's official ESP-AT 2.3.0.0 firmware** for an **Arduino Mega 2560 + WiFi ESP8266EX** combined board, together with its observed bench-test results.

**No new ESP8266 firmware was developed, and the original ESP-AT functional code was not modified.** Official Espressif firmware and tools were used to adapt the UART configuration parameters and flash boot header. BIOSECHP's contribution is the documented hardware integration and experimental validation.

### 2. Evaluated hardware

- Combined **ATmega2560 + ESP8266EX** board.
- **CH340** USB-to-serial interface, enumerated as **COM16** on the test computer.
- **4 MiB** SPI flash, marked **`25VQ32`**.
- Eight DIP switches and a **`RXD0/TXD0` / `RXD3/TXD3`** UART selector.
- Original AT firmware reported by `AT+GMR`: **version 1.1.0.0**.

The exact PCB revision has **not** been identified. Accordingly, compatibility with other hardware variants is **not established**.

### 3. Configuration adaptation

Before programming, the complete **4 MiB** original flash contents were backed up and verified against the device using `esptool verify-flash`.

The official ESP-AT **2.3.0.0** factory image was adapted using Espressif tools, including those in the **`release/v2.3.0.0_esp8266`** branch. The **`WROOM-02-N`** factory parameters specify **UART TX GPIO1**, **UART RX GPIO3**, with **CTS/RTS hardware flow control disabled**. The **4,096-byte** parameter block was inserted at **`0xF1000`** in a copy of the official **2 MiB** factory image. Its boot header was prepared for **DOUT at 40 MHz**.

The resulting image was written from flash offset **`0x0`**, and the programming tool reported successful write verification. Subsequent tests verified boot and AT command operation.

**Compatibility warning:** These actions were validated on one tested board only. Do not flash an image onto another revision without confirming its PCB, UART wiring, flash capacity, boot settings, and the availability of a verified recovery backup.

### 4. Experimental results

| Test | Result |
|---|---|
| ATmega2560 `Blink` programming | **PASS** |
| CH340 USB serial communication | **PASS** |
| `AT` response | **PASS** |
| `AT+GMR` firmware identification | **PASS** — ESP-AT 2.3.0.0 (`WROOM-02-N`) |
| `AT+CWLAP` access-point scan | **PASS** |
| USB-to-Mega-to-ESP bridge using `Serial3` | **PASS** |
| Autonomous AT commands sent by ATmega2560 using `Serial3` | **PASS** |
| Autonomous WiFi association | **NOT TESTED** |
| Mega-controlled web server | **NOT TESTED** |

For the autonomous UART test, the Mega sent **`AT`**, **`AT+GMR`**, and **`AT+CWMODE?`** over **`Serial3` at 115200 baud**, and the ESP8266 returned valid responses. **`+CWMODE:1`** confirms station-mode configuration; it does **not** confirm an active WiFi association.

The Spanish test record is available in [docs/VALIDACION.md](docs/VALIDACION.md).

### 5. Firmware integrity

**Final adapted image:** `ESP_AT_2.3_MEGA_WIFI_DOUT_40M.bin`  
**File size:** 2,097,152 bytes (2 MiB)  
**Configuration:** ESP-AT 2.3.0.0, UART `WROOM-02-N`, flash **DOUT/40 MHz**  
**SHA-256:**

`876C2FA5E777849E18DC2D8940DA639E7A205B88D7CFEFEBD2FDC1966BBCCF44`

This hash identifies the **locally verified final binary file** used in the upgrade. It is **not** a separate full-device **4 MiB post-flash readback**. The flashing tool reported successful write verification.

The original complete flash backup, WiFi credentials and private device data are **not distributed**.

### 6. Attribution and technical scope

Upstream firmware and tools are provided by **Espressif Systems**:

- [ESP-AT](https://github.com/espressif/esp-at)
- [esptool](https://github.com/espressif/esptool)

**BIOSECHP does not claim authorship of the original ESP-AT firmware.** This contribution records a tested configuration adaptation, hardware integration, and experimental results. Applicable upstream licenses must be respected in any future redistribution.

The present report establishes successful operation **on one physical board only**; it is not a universal compatibility certification. Additional PCB identification, reproducible scripts, and validation of other hardware revisions may extend the project in the future, without changing the scope of these completed tests.

---

**BIOSECHP — Embedded Systems, Hardware Integration and Technical Validation**  
*Technical validation record — November 15, 2026.*
