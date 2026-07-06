#include <WiFi.h>
#include "camera_stream.h"
#include <Arduino.h>
#include "camera_pins.h"

// Настройки Wi-Fi
const char* ssid = "d";
const char* password = "12345678";

// Порт сервера
WiFiServer server(80);

void setup() {
  pinMode(4,OUTPUT);
  Serial.begin(115200);
  cameraInit();  
  // Подключение к Wi-Fi
   WiFi.softAP(ssid, password);
 
  
  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
  
  server.begin();
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.accept();
  if (client) {
    Serial.println("New Client");
    webStream(client);
    Serial.println(WiFi.localIP());
  
  }
 
}
