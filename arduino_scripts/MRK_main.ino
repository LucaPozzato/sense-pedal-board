#include <Arduino.h>
#include <Wire.h>
#include <WiFiNINA.h>
#include <WiFiSSLClient.h>
#include "Arduino_BHY2Host.h"
#include "arduino_secrets.h" // Include your WiFi credentials

// Wi-Fi credentials loaded from arduino_secrets.h
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// Wi-Fi client for secure communication
WiFiSSLClient client;

// Define pins for button and built-in LED
#define BUTTON_PIN A0
#define LOUDNESS_PIN A1
#define BRIGHTNESS_PIN A2

// Threshold for yaw (you can adjust this value)
float yawThreshold = 45.0; // Yaw value that triggers the HTTP request
// Variables to store sensor data received from the slave
float yawValue;

// Timer variables
unsigned long lastDataSentTime = 0;
unsigned long dataSendInterval = 60000; // Interval to send data (1 minute)

void setup() {
  Serial.begin(115200);      // Initialize serial communication at a baud rate of 115200 (same as slave)
  while (!Serial);           // Wait for the Serial Monitor to open (useful for debugging)
  
  // Attempt to connect to Wi-Fi until successful
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print("Attempting to connect to network:");
    Serial.println(ssid);
    delay(5000); // Wait before retrying
  }
  Serial.println("Connected to WiFi");
  
  // Initialize the I2C bus as the master
  Wire.begin();              // Sets up the Arduino as an I2C master
  Serial.println("Setup complete");
}

void loop() {
  unsigned long currentMillis = millis();
  static unsigned long lastRequestTime = 0;
  
  if (currentMillis - lastRequestTime >= 1000) {  // Every second
    lastRequestTime = currentMillis;
    
    // Request data from the slave device (Nicla Sense ME) with I2C address 0x29
    if (Wire.requestFrom(0x29, sizeof(yawValue)) == sizeof(yawValue)) {
      Wire.readBytes((char*)&yawValue, sizeof(yawValue));
      Serial.print("Yaw received: ");
      Serial.println(yawValue);

      if (yawValue >= yawThreshold) {
        Serial.println("Yaw exceeds threshold. Sending data to server...");
        sendReqToServer(); // Send request to server
      }
    } else {
      Serial.println("Error: Failed to read yaw data");
    }
  }
}

void sendReqToServer() {
  String data = "yaw=" + String(yawValue);  // Only send yaw data

  // Send HTTP request to server (replace with your server URL)
  if (client.connect("your-server.com", 80)) { // Replace with your server
    client.print("POST /yaw-data HTTP/1.1\r\n");
    client.print("Host: your-server.com\r\n"); // Replace with your server
    client.print("Content-Type: application/x-www-form-urlencoded\r\n");
    client.print("Content-Length: " + String(data.length()) + "\r\n");
    client.print("\r\n");
    client.print(data); // Send the data in the body of the POST request
    client.print("\r\n");
  }
}