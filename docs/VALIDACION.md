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
- Archivo del puente en este repositorio: [`examples/PUENTE_SERIAL3_VALIDADO/PUENTE_SERIAL3_VALIDADO.ino`](../examples/PUENTE_SERIAL3_VALIDADO/PUENTE_SERIAL3_VALIDADO.ino).
- Programa autónomo [`BIOSECHP_MEGA_ESP8266_AUTONOMO.ino`](../examples/BIOSECHP_MEGA_ESP8266_AUTONOMO/BIOSECHP_MEGA_ESP8266_AUTONOMO.ino): **1.656 bytes**; SHA-256 `28ED72F92D238ED3E9F60BC4677D1A5B977B4AC8C881908D07174C06C02668D2`, calculado sobre la copia incluida en el repositorio y contrastado con la huella registrada por el operador.
- Imagen de firmware **DOUT/40 MHz** utilizada en la actualización (2.097.152 bytes): SHA-256 `876C2FA5E777849E18DC2D8940DA639E7A205B88D7CFEFEBD2FDC1966BBCCF44`, calculado sobre el archivo local según el registro de la sesión. **No** se interpreta como lectura posterior de toda la memoria del dispositivo. Véase [`PROCEDIMIENTO_REPRODUCIBLE.md`](PROCEDIMIENTO_REPRODUCIBLE.md).

## Evidencia fotográfica incorporada

Las cuatro imágenes de la unidad ensayada están incorporadas en [`hardware/images/`](../hardware/images/) y se describen en [`hardware/README.md`](../hardware/README.md): cara superior, cara inferior, detalle ESP8266/memoria SPI y detalle ATmega2560. Sus huellas están registradas en [`hardware/IMAGES_SHA256.txt`](../hardware/IMAGES_SHA256.txt). La evidencia identifica componentes, pero **no confirma el fabricante ni la revisión exacta de la PCB**.

## Límites del resultado de la prueba autónoma

El mensaje `PRUEBA FINALIZADA` se imprime al terminar la secuencia del sketch; **no** significa que el programa haya evaluado automáticamente todas las respuestas. El dictamen PASS de esta sesión se basa en las respuestas `OK`, versión y modo estación efectivamente observados en el Monitor Serie. No demuestra asociación WiFi ni servidor web.

## Aspectos pendientes antes de una publicación pública o reproducción independiente

1. Identificar fabricante y revisión física de la PCB, y validar una matriz DIP/selector para la revisión concreta.
2. Revisar la titularidad y las licencias de código, documentación, fotografías y componentes de terceros antes de conceder permisos públicos o redistribuir binarios.
3. Revisar los materiales para descartar credenciales, identificadores privados y otros datos sensibles antes de hacer público el repositorio.
4. Completar y repetir independientemente los argumentos de generación de parámetros y construcción de la imagen; conservar las advertencias de compatibilidad documentadas en [`PROCEDIMIENTO_REPRODUCIBLE.md`](PROCEDIMIENTO_REPRODUCIBLE.md).
5. No generalizar la compatibilidad a todas las placas Mega + WiFi; las pruebas descritas corresponden a **una unidad física**.
