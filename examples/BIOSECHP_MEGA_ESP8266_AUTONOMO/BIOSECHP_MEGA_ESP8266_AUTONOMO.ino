
/*
  ===================================================
  BIOSECHP - PRUEBA DE COMUNICACION AUTONOMA
  Arduino Mega 2560 + WiFi ESP8266EX
  Firmware ESP-AT 2.3.0.0

  Serial  : Computador USB - COM16
  Serial3 : ATmega2560 <-> ESP8266

  OBJETIVO:
  El Mega envia comandos AT automaticamente
  y muestra las respuestas del ESP8266.

  No modifica el firmware ESP8266.
  No conecta motores ni redes WiFi.
  ===================================================
*/

void enviarComando(const char* comando,
                   unsigned long esperaMs) {

  // Descartar datos anteriores pendientes
  while (Serial3.available() > 0) {
    Serial3.read();
  }

  Serial.println();
  Serial.print("MEGA ENVIA: ");
  Serial.println(comando);

  // Enviar comando desde el Mega al ESP8266
  Serial3.print(comando);
  Serial3.print("\r\n");

  unsigned long inicio = millis();

  // Recibir y mostrar la respuesta
  while (millis() - inicio < esperaMs) {
    while (Serial3.available() > 0) {
      Serial.write(Serial3.read());
    }
  }

  Serial.println();
  Serial.println("--- FIN RESPUESTA ---");
}

void setup() {

  Serial.begin(115200);
  Serial3.begin(115200);

  // Tiempo para abrir el Monitor Serie
  delay(3000);

  Serial.println();
  Serial.println("==============================");
  Serial.println("BIOSECHP - PRUEBA AUTONOMA");
  Serial.println("MEGA 2560 + ESP8266");
  Serial.println("==============================");

  enviarComando("AT", 1500);

  enviarComando("AT+GMR", 2500);

  enviarComando("AT+CWMODE?", 1500);

  Serial.println();
  Serial.println("PRUEBA FINALIZADA");
}

void loop() {
  // Los comandos se envian solo al iniciar.
}
