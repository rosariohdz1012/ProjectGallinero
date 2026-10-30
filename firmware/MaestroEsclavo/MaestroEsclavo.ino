#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "LAPG5";
const char* password = "asd123QWE";

WebServer server(80);

// Variables globales del gallinero
String temperatura = "0.0";
String humedad = "0.0";
String co2 = "0";
String nivelAgua = "0";
String alimento = "0.0";

void handleGetEstado() {
  // Permite que el Dashboard web pueda consultar los datos sin bloqueos de seguridad
  server.sendHeader("Access-Control-Allow-Origin", "*");
  
  String json = "{";
  json += "\"temperatura\":" + temperatura + ",";
  json += "\"humedad\":" + humedad + ",";
  json += "\"co2\":" + co2 + ",";
  json += "\"nivel_agua\":" + nivelAgua + ",";
  json += "\"alimento\":" + alimento;
  json += "}";
  
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  delay(1000);

  Serial.println("\nConectando a Wi-Fi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n¡Conectado!");
  Serial.print("IP del Maestro: ");
  Serial.println(WiFi.localIP());

  server.on("/estado", HTTP_GET, handleGetEstado);
  server.begin();
}

void loop() {
  server.handleClient();

  // Leer datos provenientes del Esclavo por UART
  if (Serial2.available()) {
    String linea = Serial2.readStringUntil('\n');
    linea.trim();
    
    // Parsear la cadena separada por comas (T, H, CO2, Agua, Alimento)
    int p1 = linea.indexOf(',');
    int p2 = linea.indexOf(',', p1 + 1);
    int p3 = linea.indexOf(',', p2 + 1);
    int p4 = linea.indexOf(',', p3 + 1);

    if (p1 > 0 && p2 > 0 && p3 > 0 && p4 > 0) {
      temperatura = linea.substring(0, p1);
      humedad = linea.substring(p1 + 1, p2);
      co2 = linea.substring(p2 + 1, p3);
      nivelAgua = linea.substring(p3 + 1, p4);
      alimento = linea.substring(p4 + 1);

      Serial.println("Datos del gallinero actualizados en servidor.");
    }
  }
}