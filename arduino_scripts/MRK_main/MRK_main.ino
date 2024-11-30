#include <Arduino.h>
#include <Wire.h>
#include <WiFiNINA.h>
#include <WiFiSSLClient.h>
#include "Arduino_BHY2Host.h"
#include "arduino_secrets.h" // Include your WiFi credentials
#include "model.h"           // Include simple classifier

// Wi-Fi credentials loaded from arduino_secrets.h
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
char server_ip[] = SERVER_IP;
uint16_t server_port = SERVER_PORT;

// Wi-Fi client for secure communication
WiFiClient client;

// Define pins for button and built-in LED
#define BUTTON_PIN A0
#define LOUDNESS_PIN A1
#define BRIGHTNESS_PIN A2

// Variables to store sensor data received from the slave
float yawValue;

// Timer variables
unsigned long lastDataSentTime = 0;
unsigned long dataSendInterval = 60000; // Interval to send data (1 minute)

void setup()
{
  Serial.begin(115200); // Initialize serial communication at a baud rate of 115200
  while (!Serial)
    ; // Wait for the Serial Monitor to open (useful for debugging)

  Serial.println("Starting Wi-Fi setup...");

  // Debugging: Check if the Wi-Fi module is detected
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println("Error: Wi-Fi module not detected!");
    while (true)
      ; // Halt execution
  }

  // Scan for Wi-Fi networks and print the results
  Serial.println("Scanning for available Wi-Fi networks...");
  int numNetworks = WiFi.scanNetworks();
  if (numNetworks == -1)
  {
    Serial.println("Error: Wi-Fi scan failed.");
  }
  else
  {
    Serial.print("Found ");
    Serial.print(numNetworks);
    Serial.println(" network(s):");
    for (int i = 0; i < numNetworks; i++)
    {
      Serial.print("SSID: ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" | Signal Strength: ");
      Serial.print(WiFi.RSSI(i));
      Serial.print(" dBm | Encryption: ");
      Serial.println(WiFi.encryptionType(i));
    }
  }

  // Attempt to connect to the specified Wi-Fi network
  Serial.print("Attempting to connect to network: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED)
  {
    Serial.println("Connection failed. Retrying...");
    delay(5000); // Wait before retrying
  }

  Serial.println("Connected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print the assigned IP address

  // Initialize the I2C bus as the master
  Wire.begin(); // Sets up the Arduino as an I2C master
  Serial.println("Setup complete");
}

void loop()
{
  unsigned long currentMillis = millis();
  static unsigned long lastRequestTime = 0;

  if (currentMillis - lastRequestTime >= 100)
  { // Every second
    lastRequestTime = currentMillis;

    // Request data from the slave device (Nicla Sense ME) with I2C address 0x29
    if (Wire.requestFrom(0x29, sizeof(yawValue)) == sizeof(yawValue))
    {
      Wire.readBytes((char *)&yawValue, sizeof(yawValue));

      // Send data to classifier
      int result = simple_classifier(yawValue);

      // Print the result
      if (result == 1)
      {
        Serial.println("Yaw exceeds threshold. Sending data to server...");
        nextPedal(); // Send request to server
      }
    }
    else
    {
      Serial.println("Error: Failed to read yaw data");
    }
  }
}

void nextPedal()
{
  // Send HTTP request to server
  if (client.connect(server_ip, server_port))
  { // Replace with your server
    Serial.println("Connected to server. Sending data...");
    client.print("GET /next_pedal HTTP/1.1\r\n");
    client.print("Host: "); 
    client.print(server_ip);
    client.print("\r\n");// Replace with your server
    Serial.println("Data sent successfully.");
  }
  else
  {
    Serial.println("Error: Failed to connect to server.");
  }
}

void updatePedalValue(int value) { 
    // Send HTTP request to server
  if (client.connect(server_ip, server_port))
  { // Replace with your server
    Serial.println("Connected to server. Sending data...");
    client.print("POST /pedal_value HTTP/1.1\r\n");
    client.print("Host: "); 
    client.print(server_ip);
    client.print("\r\n");// Replace with your server
    Serial.println("Data sent successfully.");
  }
  else
  {
    Serial.println("Error: Failed to connect to server.");
  }
}