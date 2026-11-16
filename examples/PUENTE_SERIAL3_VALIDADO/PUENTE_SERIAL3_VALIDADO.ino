
/*
  BIOSECHP - PRUEBA MEGA 2560 + ESP8266
  Puente serie USB <-> Serial3

  No controla motores.
  No modifica el firmware del ESP8266.
*/

void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("BIOSECHP - PRUEBA UART");
  Serial.println("Mega 2560 iniciado");
  Serial.println("Envia AT desde el Monitor Serie");
}

void loop() {
  // Computador -> Mega -> ESP8266
  while (Serial.available() > 0) {
    Serial3.write(Serial.read());
  }

  // ESP8266 -> Mega -> Computador
  while (Serial3.available() > 0) {
    Serial.write(Serial3.read());
  }
}
