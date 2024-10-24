#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "EAR_WiFi";
const char* password = "Ear@2020";

// Static IP configuration
IPAddress local_IP(172, 16, 23, 202); // 172.16.23.202:80
IPAddress gateway(172, 16, 20, 1);
IPAddress subnet(255, 255, 248, 0);
IPAddress dns(8, 8, 8, 8);

// API server details
const char* host = "script.google.com";
const char* scriptPath = "/macros/s/AKfycbzNawwnkY1OKcx0vpk-M8RpdLx12i7QK3IoniAU_eydkLgs4nq5QvnNOalZyU0pRocg/exec";  // Replace with your API host
const int port = 443; // 80 = HTTP; 443 = HTTPS

// Dummy temperature and humidity values
float temperature = 25.5;
float humidity = 60.2;

void setup() {

  Serial.begin(115200);
  Serial.println();

  // SDA: 21 SCL: 22
  // Configuring static IP
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("STA Failed to configure");
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if WiFi is connected
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    
    client.setInsecure();

    // Connect to the API server
    if (client.connect(host, port)) {
      Serial.println("Connected to server");
      
      // Create JSON object with temperature and humidity data
      JsonDocument jsonDoc;
      jsonDoc["temperature"] = temperature;
      jsonDoc["humidity"] = humidity;

      // Serialize JSON object to a string
      String jsonData;
      serializeJson(jsonDoc, jsonData);

      // Prepare HTTP POST request
      String httpRequest = String("POST ") + scriptPath + " HTTP/1.1\r\n" +  // Replace "/api" with your API path
                           "Host: " + host + "\r\n" +
                           "Content-Type: application/json\r\n" +
                           "Content-Length: " + jsonData.length() + "\r\n" +
                           "Connection: close\r\n\r\n" + 
                           jsonData + "\r\n";

      // Send the HTTP request to the server
      client.print(httpRequest);

      // Wait for server's response
      while (client.connected() || client.available()) {
        if (client.available()) {
          String response = client.readString();
          Serial.println("Response from server: " + response);
        }
      }

      // Close the connection
      client.stop();
      Serial.println("Disconnected from server");
    } else {
      Serial.println("Connection to server failed");
    }
  } else {
    Serial.println("WiFi not connected");
  }

  // Delay for 10 seconds before the next request
  delay(10000);
}
