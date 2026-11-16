# BIOSECHP — Arduino ESP8266 Serial3 Examples

## Español

### Descripción

Este directorio contiene dos programas Arduino utilizados durante la validación experimental de la comunicación entre el ATmega2560 y el ESP8266EX.

**Creador y desarrollador principal:** Jorge Eliecer Chacón Pinzón.

**Empresa responsable:** BIOSECHP USA LLC.

### Programas incluidos

**PUENTE_SERIAL3_VALIDADO**

Programa que permite transmitir comandos entre el Monitor Serie del computador y el ESP8266 utilizando el ATmega2560 como puente UART.

**BIOSECHP_MEGA_ESP8266_AUTONOMO**

Programa que genera automáticamente los comandos `AT`, `AT+GMR` y `AT+CWMODE?` desde el ATmega2560 y presenta las respuestas recibidas del ESP8266.

Ambos programas fueron comprobados experimentalmente en una tarjeta Mega 2560 + WiFi ESP8266EX.

### Autoría y licenciamiento

Se propone adoptar la licencia Apache License 2.0 exclusivamente para estos dos programas Arduino, una vez confirmadas las facultades para concederla.

**Estado actual:** licencia propuesta, todavía no otorgada mediante este documento.

La propuesta no se extiende automáticamente al firmware ESP-AT de Espressif, al resto de la documentación, a las fotografías, a las marcas de BIOSECHP ni a otros proyectos de la empresa.

Los programas se proporcionan como ejemplos técnicos de validación y no como sistemas certificados para aplicaciones críticas.

---

## English

### Description

This directory contains two Arduino sketches used to validate UART communication between an ATmega2560 and an ESP8266EX.

**Creator and Lead Developer:** Jorge Eliecer Chacón Pinzón.

**Responsible Organization:** BIOSECHP USA LLC.

The serial bridge sketch forwards commands between a computer and the ESP8266 using `Serial3`.

The autonomous test sketch sends `AT`, `AT+GMR` and `AT+CWMODE?` commands directly from the ATmega2560.

Both sketches were successfully tested on one physical Mega 2560 + WiFi ESP8266EX board.

### Licensing

Apache License 2.0 is the proposed license for these two original Arduino sketches, subject to confirmation of the authority to license them.

**Current status:** proposed license only; this document does not grant the proposed license.

Espressif firmware, BIOSECHP trademarks, photographs, other documentation and unrelated proprietary projects are excluded from this proposed licensing scope.

---

**BIOSECHP — Embedded Systems and Technical Validation**

*Project attribution: Jorge Eliecer Chacón Pinzón / BIOSECHP USA LLC.*
