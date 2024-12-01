// We want to use the NICLA to continously collect data and send them only when requested by the master MRK
// We want yaw -> measures rate of rotations around z axis

#include "Arduino.h"
#include "Arduino_BHY2.h"
#include <Wire.h>

// Initialize sensors
SensorXYZ gyro(SENSOR_ID_GYRO);
// SensorXYZ mag(SENSOR_ID_MAG);
// SensorQuaternion quat(SENSOR_ID_GAMERV);
SensorOrientation orient(SENSOR_ID_ORI);

float pitch, yaw, data;
float lower_pitch = 15, upper_pitch = 35, lower_yaw = 128, upper_yaw = 4500;

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
  // mag.begin();
  // quat.begin();
  orient.begin();
}

void loop()
{
  // Update all connected sensors and retrieve their latest data
  BHY2.update(); // Fetch new sensor data from the Nicla Sense ME

  // pitch = -atan2(quat.y(), quat.x()) * 180.0 / PI;
  // pitch = quat.z() / sqrt(1.0 - quat.w() * quat.w());

  pitch = orient.pitch();

  if (pitch > upper_pitch) {
    pitch = upper_pitch;
  } else if (pitch < lower_pitch) {
    pitch = lower_pitch;
  }
  pitch = (float)((int) (127 - ((pitch - lower_pitch)/(upper_pitch - lower_pitch) * 127)));
  data = pitch;

  delay(10);

  yaw = gyro.z();
  if (yaw > upper_yaw) {
    yaw = upper_yaw;
  } else if (yaw < lower_yaw) {
    yaw = lower_yaw;
  }

  data = yaw;
    
   // Get angular velocity on the Z-axis in radians per second (or degrees per second if scaled appropriately)
  delay(10);     // Delay for 1 second to control the update rate
}

// Function to send data to the master when requested via I2C
void sendData()
{
  // Send the yaw value as bytes over I2C
  Wire.write((uint8_t *)&data, sizeof(data));
}