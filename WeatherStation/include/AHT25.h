#include<Wire.h>

#define AHT25_ADDRESS 0x38

bool initAHT25() {
  Wire.beginTransmission(AHT25_ADDRESS);
  Wire.write(0xE1);  // Initialization command for AHT25
  Wire.write(0x08);
  Wire.write(0x00);
  if (Wire.endTransmission() != 0) {
    return false;  // Transmission failed
  }
  delay(20);  // Wait for the sensor to initialize
  return true;
}

bool readAHT25(float* temperature, float* humidity) {
  Wire.beginTransmission(AHT25_ADDRESS);
  Wire.write(0xAC);  // Trigger measurement command
  Wire.write(0x33);
  Wire.write(0x00);
  if (Wire.endTransmission() != 0) {
    return false;  // Transmission failed
  }

  delay(80); // Wait for measurement to complete

  Wire.requestFrom(AHT25_ADDRESS, 6);
  if (Wire.available() != 6) {
    return false;  // Reading failed
  }

  uint8_t data[6];
  for (int i = 0; i < 6; i++) {
    data[i] = Wire.read();
  }

  // Convert the data
  uint32_t humidity_raw = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
  uint32_t temperature_raw = ((uint32_t)data[3] & 0x0F) << 16 | ((uint32_t)data[4] << 8) | data[5];

  *humidity = (float)humidity_raw * 100.0 / 1048576.0;
  *temperature = (float)temperature_raw * 200.0 / 1048576.0 - 50.0;

  return true;
}