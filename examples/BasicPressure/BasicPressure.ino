#include <Arduino.h>
#include <SPI.h>

#include "MS5611.h"

// Create a MS5611 instance for a chip on SPI0, at 10MHz, and with the Chip Select pin connected to pin 17
MS5611 my_sensor(17, SPI, 10000000);

void setup() {
  Serial.begin(115200);

  // Initialize the instance before use
  // This will reset the chip and read the calibration data from it
  my_sensor.begin();

  // Optional : select the Oversampling Ratio
  //my_sensor.setOSR(MS5611_OSR_256);  // Default
  //my_sensor.setOSR(MS5611_OSR_512);
  //my_sensor.setOSR(MS5611_OSR_1024);
  //my_sensor.setOSR(MS5611_OSR_2048);
  //my_sensor.setOSR(MS5611_OSR_4096);
}

void loop() {
  // Get a measurement from the chip
  my_sensor.update();

  // The update() method updates the 'pressure' and 'temperature' attributes
  int32_t temperature = my_sensor.temperature;  // In 0.01°C
  int32_t pressure = my_sensor.pressure;        // In 0.01 mbar

  float_t temperature_f = temperature / 100.0f;
  float_t pressure_f = pressure / 100.0f;

  Serial.print("Temperature: ");
  Serial.print(temperature_f);
  Serial.print("°C");
  Serial.print("\t\tPressure: ");
  Serial.print(pressure_f);
  Serial.println(" mbar");

  delay(1000);
}