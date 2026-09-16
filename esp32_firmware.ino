#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// --- MQTT AYARLARI ---
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "cagla/marine/vessel1/telemetry";

WiFiClient espClient;
PubSubClient client(espClient);

// Başlangıç simülasyon koordinatları (Bodrum - Kos Arası Açık Deniz)
float currentLat = 36.9800;
float currentLon = 27.3500;
float currentSpeed = 12.4;

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("WiFi Baglaniyor: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Baglandi!");
  Serial.print("IP Adresi: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("MQTT Broker Baglantisi kuruluyor...");
    String clientId = "ESP32MarineClient-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println(" Baglandi!");
    } else {
      Serial.print(" Hata, rc=");
      Serial.print(client.state());
      Serial.println(" (5 sn sonra tekrar denenecek)");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Açık denizde güneybatıya doğru süzülme (Karaya çıkmaz)
  currentLat -= 0.0002;
  currentLon -= 0.0004;
  currentSpeed = 11.5 + (random(0, 30) / 10.0);
  float windSpeed = 16.0 + (random(0, 40) / 10.0);

  // JSON paketi oluştur
  StaticJsonDocument<200> doc;
  doc["lat"] = currentLat;
  doc["lon"] = currentLon;
  doc["speed"] = currentSpeed;
  doc["wind"] = windSpeed;

  char jsonBuffer[256];
  serializeJson(doc, jsonBuffer);

  // MQTT üzerinden fırlat
  client.publish(mqtt_topic, jsonBuffer);
  Serial.print("Yollandi -> ");
  Serial.println(jsonBuffer);

  // 2 saniyede bir telemetri bas
  delay(2000);
} 
