#include "DHT.h"

#define DHTPIN 25     // Pin digital seguro para el DHT11
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Usamos pines analógicos seguros (GPIO 32 y 33) y evitamos los 34 y 35
const int pinCO2 = 32;          
const int pinNivelAgua = 33;    

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // Comunicación con el Maestro
  dht.begin();
  Serial.println("--- NODO ESCLAVO GALLINERO INICIADO ---");
}

void loop() {
  delay(2000);

  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  int valCO2 = analogRead(pinCO2);
  int nivelAgua = analogRead(pinNivelAgua);
  float pesoAlimento = 1.50; // Aquí integrarás tu sensor HX711

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Error al leer el sensor DHT!");
    return;
  }

  // Empaquetar y enviar al Maestro por UART
  Serial2.print(temp);
  Serial2.print(",");
  Serial2.print(hum);
  Serial2.print(",");
  Serial2.print(valCO2);
  Serial2.print(",");
  Serial2.print(nivelAgua);
  Serial2.print(",");
  Serial2.println(pesoAlimento);
}