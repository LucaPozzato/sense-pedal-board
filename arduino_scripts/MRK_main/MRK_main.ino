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

// Variables to store sensor data received from the slave
float data, pitch, yaw;
int result = 0;
int last_value = 0;

void setup()
{
  // Debugging: Check if the Wi-Fi module is detected
  if (WiFi.status() == WL_NO_MODULE)
  {
    while (true)
      ; // Halt execution
  }

  while (WiFi.begin(ssid, pass) != WL_CONNECTED)
  {
    delay(1000); // Wait before retrying
  }

  // Initialize the I2C bus as the master
  Wire.begin(); // Sets up the Arduino as an I2C master
}

void loop()
{
  static unsigned long lastRequestTime = 0;
  unsigned long currentMillis = millis();

  while (WiFi.status() != WL_CONNECTED)
  {
        WiFi.begin(ssid, pass);
        delay(1000);  // Wait before retrying
  }

  if (currentMillis - lastRequestTime >= 5)
  {
    lastRequestTime = currentMillis;

    // Request data from the slave device (Nicla Sense ME) with I2C address 0x29
    if (Wire.requestFrom(0x29, sizeof(data)) == sizeof(data))
    {
      Wire.readBytes((char *)&(data), sizeof(data));

      if (data < 128) {
        pitch = data;
        result = simple_pitch_classifier(pitch);
        if (result != last_value) {
          last_value = result;
          updatePedalValue(result);
        }
      }
      else {
        yaw = data;
        result = simple_yaw_classifier(yaw);
        if (result == 1)
        {
          nextPedal(); // Send request to server
        }
      }
    }
  }
}

void nextPedal()
{
  // Send HTTP request to server
  if (client.connect(server_ip, server_port))
  { // Replace with your server
    client.print("GET /next_pedal HTTP/1.1\r\n");
    client.print("Host: ");
    client.print(server_ip);
    client.print("\r\n"); // Replace with your server
  }
}

void updatePedalValue(int value)
{
  // Send HTTP request to server
  if (client.connect(server_ip, server_port))
  { // Replace with your server
    String jsonData = "{\"pedal value\":\"" + String(value) + "\"}";

    client.println("POST /pedal_value HTTP/1.1");
    client.print("Host: ");
    client.println(server_ip);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(jsonData.length());
    client.println(); // Empty line to separate headers from body
    client.println(jsonData);
  }
}