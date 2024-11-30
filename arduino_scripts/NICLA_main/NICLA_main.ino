// We want to use the NICLA to continously collect data and send them only when requested by the master MRK
// We want yaw -> measures rate of rotations around z axis

#include "Arduino.h"
#include "Arduino_BHY2.h"
#include <Wire.h>

// Initialize sensors
SensorXYZ gyro(SENSOR_ID_GYRO);
float yaw;

void setup()
{
  Serial.begin(115200); // Start serial communication at a baud rate of 115200 -> 15200 baud means 115,200 bits per second.
  while (!Serial)
    ; // Wait for the Serial Monitor to open (optional for debugging)

  // Begin the Eslov communication interface (I2C communication for Nicla Sense ME)
  BHY2.begin(NICLA_I2C, NICLA_VIA_ESLOV);

  // Initialize I2C as a slave with the Eslov default address (0x29 for Nicla Sense ME)
  Wire.begin(0x29);
  Wire.onRequest(sendData); // Set the callback function to execute when the master requests data

  // Initialize gyroscope
  gyro.begin(); // Start the gyroscope sensor
}

void loop()
{
  // Update all connected sensors and retrieve their latest data
  BHY2.update(); // Fetch new sensor data from the Nicla Sense ME

  // Store the latest sensor values for yaw
  yaw = gyro.z(); // Get angular velocity on the Z-axis in radians per second (or degrees per second if scaled appropriately)
  delay(100);     // Delay for 1 second to control the update rate

  // For debug purposes we print data collected:
  Serial.println(yaw); // Yaw rate in rad/s
}

// Function to send data to the master when requested via I2C
void sendData()
{
  // Send the yaw value as bytes over I2C
  Wire.write((uint8_t *)&yaw, sizeof(yaw));
}
