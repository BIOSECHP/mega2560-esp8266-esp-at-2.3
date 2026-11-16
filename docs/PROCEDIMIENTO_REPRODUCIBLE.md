# BIOSECHP — Procedimiento documentado de adaptación ESP-AT 2.3.0.0

**Proyecto:** Mega 2560 + WiFi ESP8266EX · **Estado:** registro técnico de una ejecución satisfactoria; reproducción independiente pendiente.  
**Creador y responsable técnico:** Jorge Eliecer Chacón Pinzón.  
**Organización responsable del proyecto:** BIOSECHP USA LLC.  
**Tecnología de base:** Espressif ESP-AT y esptool, sujetos a sus licencias originales.

> **Aviso:** Este documento no es una receta universal para flashear placas Mega+WiFi. La revisión exacta de la PCB evaluada no se determinó. Los comandos de programación que figuran más abajo se conservan como **registro histórico de lo ejecutado**, no como instrucción para ejecutarlos de nuevo en otro dispositivo. Una imagen incompatible puede impedir el arranque. No ejecutar `write-flash` sin identificar el hardware, verificar y conservar un respaldo íntegro y establecer un plan de restauración.

## 1. Hardware y límites

Una placa combinada ATmega2560 + ESP8266EX, convertidor CH340, memoria SPI marcada `25VQ32` (4 MiB), ocho interruptores DIP y selector `RXD0/TXD0` / `RXD3/TXD3`. Puerto detectado en la sesión como `COM16`, no universal. Fotografías originales y detalles en [`../hardware/README.md`](../hardware/README.md).

La memoria *física* observada es de **4 MiB**, mientras que la imagen *factory* adaptada mide **2 MiB**; estos valores no deben confundirse. No se ha demostrado compatibilidad con otras revisiones, asociación WiFi autónoma ni servidor web.

## 2. Fuentes oficiales y herramientas utilizadas

- Espressif ESP-AT: <https://github.com/espressif/esp-at>, rama `release/v2.3.0.0_esp8266`.
- Versión oficial: <https://github.com/espressif/esp-at/releases> (`v2.3.0.0_esp8266`).
- Guía oficial de binarios: <https://docs.espressif.com/projects/esp-at/en/release-v2.3.0.0_esp8266/AT_Binary_Lists/index.html>.
- esptool: <https://github.com/espressif/esptool>; documentación sobre modos de flash: <https://docs.espressif.com/projects/esptool/en/latest/esp8266/esptool/flash-modes.html>.

**Sesión registrada:** Windows PowerShell, Arduino IDE 2.3.11 y esptool 5.6.0. Paquete Espressif `ESP-WROOM-02-AT-V2.3.0.0.zip`. El material original del fabricante no se redistribuye aquí.

## 3. Respaldo previo — ejecutado y comprobado

Se leyó la memoria completa original de **4.194.304 bytes** y se validó frente al dispositivo físico mediante `verify-flash`. La copia **no** se publica ni debe incluirse en GitHub.

| Artefacto privado | Tamaño | SHA-256 |
|---|---:|---|
| `ESP8266_AT_1.1_ORIGINAL_4MB.bin` | 4.194.304 | `1FD49CFEAFFEF05554A059FD05D2E1B2ED4EFC9804E5FB94CD7FED7B1DB1BB6B` |

El comando exacto de lectura usado en aquella sesión no quedó preservado en este documento. Para evitar una reproducción incorrecta, **no se sustituye con un comando supuesto**.

## 4. Parámetros UART — ejecutado

La imagen de origen para ESP-WROOM-02 empleaba configuración UART distinta de la que funcionó con el USB/puente serie de esta tarjeta. Con el generador oficial `tools/factory_param_generate.py` del repositorio ESP-AT se produjo el perfil **`WROOM-02-N`**: TX **GPIO1**, RX **GPIO3**, CTS/RTS **deshabilitados**. El bloque generado tiene **4.096 bytes**.

| Archivo | Tamaño | SHA-256 |
|---|---:|---|
| `factory_param_WROOM-02-N.bin` | 4.096 | `33F01C5DC7AC2516CE1E490E05B4F527BBA1C1273E88E691036A4C539475A0D1` |

**Pendiente de reproducibilidad independiente:** registrar y ensayar nuevamente los argumentos exactos del generador de parámetros con la misma versión oficial; no se ha conservado en estas notas una línea de comandos exacta validada para esa etapa. Referirse a la ayuda de la herramienta y a los parámetros de la rama indicada; no atribuir a BIOSECHP el código del generador.

## 5. Preparación de imagen — ejecutada

Se reemplazó el bloque de parámetros de 4.096 bytes en el desplazamiento **`0xF1000`** dentro de una copia de la imagen *factory* oficial de **2.097.152 bytes**. Se generó primero una imagen candidata **DIO/80 MHz**, y luego una variante con cabecera **DOUT/40 MHz** utilizando herramientas Espressif.

| Artefacto | Tamaño | SHA-256 |
|---|---:|---|
| `ESP_AT_2.3_MEGA_WIFI_CANDIDATO.bin` (intermedio DIO/80 MHz) | 2.097.152 | `6898EC86335719D1E51BE83E45F234327277DA9A8634AFF4D4685E2E5202217F` |
| `ESP_AT_2.3_MEGA_WIFI_DOUT_40M.bin` (**final grabado**) | 2.097.152 | `876C2FA5E777849E18DC2D8940DA639E7A205B88D7CFEFEBD2FDC1966BBCCF44` |

La huella final fue calculada de nuevo con PowerShell sobre el binario local el 15 de noviembre de 2026. No representa una lectura física independiente posterior de los 4 MiB.

### Comprobación de integridad de solo lectura

```powershell
$Ruta = 'D:\RESPALDOS_ARDUINO\MEGA_WIFI_ESP8266\ESP_AT_2.3_MEGA_WIFI_DOUT_40M.bin'
Get-Item -LiteralPath $Ruta | Select-Object FullName,Length,LastWriteTime
(Get-FileHash -LiteralPath $Ruta -Algorithm SHA256).Hash
```

El valor esperado es `876C2FA5E777849E18DC2D8940DA639E7A205B88D7CFEFEBD2FDC1966BBCCF44`.

## 6. Registro histórico de programación — **NO EJECUTAR como instrucción genérica**

En la tarjeta de prueba, con el ESP en modo bootloader, se utilizó el siguiente comando (variables del entorno local). El proceso reportó escritura de **2.097.152 bytes** y **`Hash of data verified`**:

```powershell
# REGISTRO HISTORICO: comando que se ejecutó durante la actualización; NO reejecutar.
$Nuevo = 'D:\RESPALDOS_ARDUINO\MEGA_WIFI_ESP8266\ESP_AT_2.3_MEGA_WIFI_DOUT_40M.bin'
# py -m esptool --chip esp8266 --port COM16 --baud 115200 `
#    --before no-reset --after no-reset `
#    write-flash --flash-mode keep --flash-freq keep --flash-size keep `
#    0x0 $Nuevo
```

`keep` preserva los valores de cabecera de la imagen que se programa; no vuelve universalmente segura una imagen adaptada. El acceso al bootloader dependió de DIP y del pulsador MODE/FLASH de esta unidad. **No se publica aquí una secuencia universal de DIP**: falta identificar formalmente la revisión y documentar una matriz comprobada para todo el procedimiento.

## 7. Validación funcional posterior — realizada

Con el puerto serie a **115200 baudios**:

| Prueba | Resultado observado |
|---|---|
| `AT` | `OK` |
| `AT+GMR` | `AT version:2.3.0.0`, `Bin version:2.3.0(WROOM-02-N)` |
| `AT+CWLAP` | búsqueda de puntos de acceso, finalizó `OK` |
| Puente USB → ATmega2560 → ESP por `Serial3` | comandos y respuestas correctos |
| Sketch autónomo Mega → ESP por `Serial3` | `AT`, `AT+GMR`, `AT+CWMODE?`, respuestas correctas |
| `AT+CWMODE?` | `+CWMODE:1` y `OK` |

`+CWMODE:1` significa **modo estación configurado**; no prueba conexión a un punto de acceso. No se ensayaron conexión autónoma WiFi ni servidor web. La leyenda `PRUEBA FINALIZADA` del sketch señala el final de su secuencia, no una evaluación automática de `OK`; los resultados se comprobaron visualmente en el Monitor Serie.

Consulte [`VALIDACION.md`](VALIDACION.md), [`../examples/README.md`](../examples/README.md) y [`../hardware/README.md`](../hardware/README.md).

## 8. Pendientes para una reproducción por terceros

1. Confirmar fabricante y revisión exacta de PCB, y documentar la matriz DIP/selector correspondiente a cada operación.
2. Recuperar o reconstituir **y verificar en una unidad de prueba** los comandos exactos de generación, sustitución del bloque y cambio de cabecera.
3. Hacer una reproducción independiente con un procedimiento seguro de respaldo/restauración, sin emplear imágenes ni respaldos de otra placa sin validación.
4. Verificar licencias antes de distribuir binarios derivados o scripts; no incluir respaldos privados, claves ni credenciales.

## 9. Reconocimientos y uso

**Creador y desarrollador principal:** Jorge Eliecer Chacón Pinzón.  
**Organización responsable:** BIOSECHP USA LLC.  
**Firmware y herramientas de base:** Espressif Systems.

Véanse [`../AUTHORS.md`](../AUTHORS.md), [`../LICENSING.md`](../LICENSING.md) y [`../THIRD_PARTY_NOTICES.md`](../THIRD_PARTY_NOTICES.md). La política de licencia para los materiales originales de BIOSECHP continúa pendiente de formalización. Este documento técnico **no concede permisos adicionales**.

---

**BIOSECHP — Technical provenance note (English summary):** One Mega 2560 + ESP8266EX combined board was upgraded using Espressif ESP-AT 2.3.0.0 official materials, a WROOM-02-N UART parameter configuration (GPIO1/GPIO3, no hardware flow control), and a DOUT/40 MHz final image. The final 2 MiB artifact SHA-256 is `876C2FA5E777849E18DC2D8940DA639E7A205B88D7CFEFEBD2FDC1966BBCCF44`. Boot, AT commands, WiFi AP scan, Serial3 bidirectional bridge and autonomous AT request/response were verified on this single board. This record is **not** a universal flashing guide; board revision and independent reproduction remain unverified.
