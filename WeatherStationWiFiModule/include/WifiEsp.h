#include <WiFi.h>
#include <WiFiClientSecure.h>

const char* ssid = "EAR_WiFi";
const char* password = "Ear@2020";

IPAddress local_IP(172, 16, 23, 202); // 172.16.23.202:80
IPAddress gateway(172, 16, 20, 1);
IPAddress subnet(255, 255, 248, 0);
IPAddress dns(8, 8, 8, 8);

const char* host = "script.google.com";
const char* scriptPath = "/macros/s/AKfycbylVPo12rN6VPzG9_MgxiENgrHUm7qOyrFstVktJVSmAaiRrm4e8ZuGv8yG0kyn0JsC/exec";
const int port = 443; // 80 = HTTP; 443 = HTTPS

void setupWifi()
{
    if (!WiFi.config(local_IP, gateway, subnet, dns)) {
        Serial.println("STA Failed to configure");
    }

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

void uploadData(JsonDocument data)
{
    if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;
    
    client.setInsecure();

    // Attempt connection to google sheets
    if (client.connect(host, port)) {
      Serial.println("Connected to server");
      
      String jsonData;
      serializeJson(data, jsonData);

      String httpRequest = String("POST ") + scriptPath + " HTTP/1.1\r\n" +
                           "Host: " + host + "\r\n" +
                           "Content-Type: application/json\r\n" +
                           "Content-Length: " + jsonData.length() + "\r\n" +
                           "Connection: close\r\n\r\n" + 
                           jsonData + "\r\n";

      client.print(httpRequest);

      // Wait for server's response
      while (client.connected() || client.available()) {
        if (client.available()) {
          String response = client.readString();
          //Serial.println("Response from server: " + response);
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
}