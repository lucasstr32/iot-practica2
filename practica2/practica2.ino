// Librerías necesarias
#include <WiFi.h>
#include <PubSubClient.h>

// Conexión WiFi y Broker MQTT
const char* ssid = "Fibertel WiFi604 2.4GHz";
const char* password = "00444415607";
const char* mqtt_server = "192.168.0.146";

// Cliente MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Función para conectar/reconectar a MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando conexión MQTT... ");
    
    // Intentamos conectar con un ID único
    if (client.connect("ESP32Client_Ambiente")) {
      Serial.println("[CONECTADO] Exitosamente al Broker MQTT");
    } else {
      Serial.print("[FALLÓ] Código de error: ");
      Serial.print(client.state());
      Serial.println(" -> Reintentando en 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); // Pequeña pausa para que se estabilice el monitor serie
  
  Serial.println("\n--- Iniciando ESP32 ---");
  
  // Conexión WiFi
  Serial.print("Conectando a la red WiFi: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("."); // Efecto visual de carga
  }
  
  Serial.println("\n[CONECTADO] WiFi activo");
  Serial.print("Dirección IP asignada: ");
  Serial.println(WiFi.localIP());

  // Configuración del servidor MQTT
  client.setServer(mqtt_server, 1883);
}

void loop() {
  // Si el ESP32 se desconecta del MQTT, intenta reconectar
  if (!client.connected()) {
    reconnect();
  }
  client.loop(); // Mantiene viva la conexión y procesa mensajes entrantes

  // Simulación de datos
  float temp = 24.5; 
  float hum = 30.0;
  int cal = 10;

  char msg[100];
  sprintf(msg, "{\"temp\": %.2f, \"hum\": %.2f, \"cal\": %d}", temp, hum, cal);
  // Intentar publicar el mensaje
  Serial.print("Enviando datos al tópico 'sensor/ambiente': ");
  Serial.println(msg);

  if (client.publish("sensor/ambiente", msg)) {
    Serial.println("[ÉXITO] Mensaje enviado correctamente");
  } else {
    Serial.println("[ERROR] No se pudo enviar el mensaje");
  }

  Serial.println("-------------------------------------------");
  delay(5000); // Espera 5 segundos antes del próximo envío
}
