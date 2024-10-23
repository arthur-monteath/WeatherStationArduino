#include <SoftwareSerial.h>
#include <ArduinoJson.h>

// Define pins for SoftwareSerial
SoftwareSerial esp8266(10, 11); // RX, TX

void sendCommand(const char* command, unsigned int timeout) {
  esp8266.println(command);
  delay(timeout);
  while (esp8266.available()) {
    Serial.write(esp8266.read());
  }
}

void sendResponse(int connectionId, String data) {
  String command = "AT+CIPSEND=" + String(connectionId) + "," + String(data.length());
  sendCommand(command.c_str(), 1000);
  esp8266.print(data);
  delay(100);
  sendCommand(("AT+CIPCLOSE=" + String(connectionId)).c_str(), 1000);
}

void setupESP() {   // Initialize serial for debugging
  esp8266.begin(115200);  // Initialize ESP8266 serial
  
  delay(2000); // Give some time for ESP8266 to initialize

  sendCommand("AT+RST", 2000); // Reset module
  sendCommand("AT+CWMODE=1", 1000); // Set Wi-Fi mode to STA
  sendCommand("AT+CWDHCP=1,1", 1000); // Enable DHCP
  sendCommand("AT+CWJAP=\"EAR_WiFi\",\"Ear@2020\"", 5000); // Connect to Wi-Fi

  // Configuring static IP
  sendCommand("AT+CIPSTA=\"172.16.23.202\",\"172.16.20.1\",\"255.255.248.0\"", 2000);

  // Set DNS
  sendCommand("AT+CIPDNS=1,\"8.8.8.8\"", 1000);

  // Start a server on port 80
  sendCommand("AT+CIPMUX=1", 1000); // Enable multiple connections
  sendCommand("AT+CIPSERVER=1,80", 1000); // Start server on port 80
}

void connectionLoop(String jsonData)
{
  if (Serial.available() > 0) {
    jsonData = Serial.readStringUntil('\n');
    Serial.println("Data received from Arduino: " + jsonData);
  }

  if (esp8266.available()) {
    String response = esp8266.readStringUntil('\n');
    if (response.indexOf("+IPD") != -1) {
      int connectionId = response.charAt(5) - '0';
      sendResponse(connectionId, jsonData);
    }
  }
}