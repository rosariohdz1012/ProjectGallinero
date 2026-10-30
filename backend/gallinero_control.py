import time
import requests

ESP32_IP = "192.168.137.244"  # Cambia por la IP que te dé tu Maestro
URL = f"http://{ESP32_IP}/estado"

# Umbrales críticos del gallinero
TEMP_MAXIMA = 32.0
AGUA_MINIMA = 200  # Valor de referencia analógica del sensor de nivel
ALIMENTO_MINIMO = 0.5  # Kilos mínimos antes de alerta

print("--- SISTEMA CIBERFÍSICO DE GALLINERO AUTOMÁTICO INICIADO ---")
print(f"Monitoreando estación central en: {URL}\n")

try:
  while True:
    try:
      response = requests.get(URL, timeout=3)

      if response.status_code == 200:
        data = response.json()

        t = data.get("temperatura")
        h = data.get("humedad")
        c = data.get("co2")
        agua = data.get("nivel_agua")
        ali = data.get("alimento")

        print(
            f"📊 [ESTADO] Temp: {t}°C | Hum: {h}% | CO2: {c}ppm | Agua:"
            f" {agua} | Alimento: {ali}kg"
        )

        # Lógica de control crítico y alertas
        if t >= TEMP_MAXIMA:
          print("🚨 ¡ALERTA CALOR! Temperatura alta. Activando ventilador...")

        if agua <= AGUA_MINIMA:
          print(
              "🚨 ¡ALERTA AGUA! Nivel de agua bajo en el bebedero. Rellenar"
              " urgente."
          )

        if ali <= ALIMENTO_MINIMO:
          print(
              "⚠️ ¡AVISO ALIMENTO! La tolva de comida está por debajo del"
              " mínimo."
          )

        if t < TEMP_MAXIMA and agua > AGUA_MINIMA and ali > ALIMENTO_MINIMO:
          print("Estado: Condiciones óptimas en el gallinero.\n")

      else:
        print(f"Error de servidor HTTP. Código: {response.status_code}")

    except requests.exceptions.RequestException as e:
      print(f"Error de red al conectar con el ESP32: {e}")

    time.sleep(3)

except KeyboardInterrupt:
  print("\nMonitoreo del gallinero detenido por el usuario.")