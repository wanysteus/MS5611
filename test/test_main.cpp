#include <Arduino.h>
#include <unity.h>

#define UNIT_TESTING
#include "MS5611.h"

MS5611 my_sensor(17, SPI, 10000000);

void overrideCalibration() {
  // Calibration values examples from the datasheet
  my_sensor._c[1] = 40127;
  my_sensor._c[2] = 36924;
  my_sensor._c[3] = 23317;
  my_sensor._c[4] = 23282;
  my_sensor._c[5] = 33464;
  my_sensor._c[6] = 28312;
}

void testPressureConversion0() {
  my_sensor.convertMeasurement(9085466, 8569150);
  TEST_ASSERT_EQUAL(100009, my_sensor.pressure);
}

void testTemperatureConversion0() {
  my_sensor.convertMeasurement(9085466, 8569150);
  TEST_ASSERT_EQUAL(2007, my_sensor.temperature);
}

void testPressureConversion1() {
  my_sensor.convertMeasurement(9085466, 6869150);
  TEST_ASSERT_EQUAL(83499, my_sensor.pressure);
}

void testTemperatureConversion1() {
  my_sensor.convertMeasurement(9085466, 6869150);
  TEST_ASSERT_EQUAL(-5072, my_sensor.temperature);
}

void testPressureConversion2() {
  my_sensor.convertMeasurement(4265350, 6269150);
  TEST_ASSERT_EQUAL(5139, my_sensor.pressure);
}

void testTemperatureConversion2() {
  my_sensor.convertMeasurement(4265350, 6269150);
  TEST_ASSERT_EQUAL(-8213, my_sensor.temperature);
}

void testPressureConversion3() {
  my_sensor.convertMeasurement(4265350, 7598340);
  TEST_ASSERT_EQUAL(7652, my_sensor.pressure);
}

void testTemperatureConversion3() {
  my_sensor.convertMeasurement(4265350, 7598340);
  TEST_ASSERT_EQUAL(-1705, my_sensor.temperature);
}

void testPressureConversion4() {
  my_sensor.convertMeasurement(6087840, 7990420);
  TEST_ASSERT_EQUAL(41055, my_sensor.pressure);
}

void testTemperatureConversion4() {
  my_sensor.convertMeasurement(6087840, 7990420);
  TEST_ASSERT_EQUAL(-100, my_sensor.temperature);
}

void testPressureConversion5() {
  my_sensor.convertMeasurement(8894340, 8190420);
  TEST_ASSERT_EQUAL(93850, my_sensor.pressure);
}

void testTemperatureConversion5() {
  my_sensor.convertMeasurement(8894340, 8190420);
  TEST_ASSERT_EQUAL(664, my_sensor.temperature);
}

void testPressureConversion6() {
  my_sensor.convertMeasurement(9764720, 9390420);
  TEST_ASSERT_EQUAL(119078, my_sensor.pressure);
}

void testTemperatureConversion6() {
  my_sensor.convertMeasurement(9764720, 9390420);
  TEST_ASSERT_EQUAL(4779, my_sensor.temperature);
}

void testDefaultOversampling() {
  TEST_ASSERT_EQUAL(MS5611_OSR_256, my_sensor._osr);
  TEST_ASSERT_GREATER_OR_EQUAL(0.6, my_sensor._del);
}

void testOversamplingSettings() {
  my_sensor.setOSR(MS5611_OSR_256);
  TEST_ASSERT_EQUAL(MS5611_OSR_256, my_sensor._osr);
  TEST_ASSERT_GREATER_OR_EQUAL(0.6, my_sensor._del);
  my_sensor.setOSR(MS5611_OSR_512);
  TEST_ASSERT_EQUAL(MS5611_OSR_512, my_sensor._osr);
  TEST_ASSERT_GREATER_OR_EQUAL(1.17, my_sensor._del);
  my_sensor.setOSR(MS5611_OSR_1024);
  TEST_ASSERT_EQUAL(MS5611_OSR_1024, my_sensor._osr);
  TEST_ASSERT_GREATER_OR_EQUAL(2.28, my_sensor._del);
  my_sensor.setOSR(MS5611_OSR_2048);
  TEST_ASSERT_EQUAL(MS5611_OSR_2048, my_sensor._osr);
  TEST_ASSERT_GREATER_OR_EQUAL(4.54, my_sensor._del);
  my_sensor.setOSR(MS5611_OSR_4096);
  TEST_ASSERT_EQUAL(MS5611_OSR_4096, my_sensor._osr);
  TEST_ASSERT_GREATER_OR_EQUAL(9.04, my_sensor._del);
  my_sensor.setOSR(1);  // Forbidden value
  TEST_ASSERT_EQUAL(MS5611_OSR_256, my_sensor._osr);
  TEST_ASSERT_GREATER_OR_EQUAL(0.6, my_sensor._del);
}

void setup() {
  delay(2000);

  my_sensor.begin();

  overrideCalibration();

  UNITY_BEGIN();
  RUN_TEST(testDefaultOversampling);
  RUN_TEST(testOversamplingSettings);
  RUN_TEST(testPressureConversion0);
  RUN_TEST(testTemperatureConversion0);
  RUN_TEST(testPressureConversion1);
  RUN_TEST(testTemperatureConversion1);
  RUN_TEST(testPressureConversion2);
  RUN_TEST(testTemperatureConversion2);
  RUN_TEST(testPressureConversion3);
  RUN_TEST(testTemperatureConversion3);
  RUN_TEST(testPressureConversion4);
  RUN_TEST(testTemperatureConversion4);
  RUN_TEST(testPressureConversion5);
  RUN_TEST(testTemperatureConversion5);
  RUN_TEST(testPressureConversion6);
  RUN_TEST(testTemperatureConversion6);
  UNITY_END();
}

void loop() {
}