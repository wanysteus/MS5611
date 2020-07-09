#include "MS5611.h"

/*
  MS5611 constructor
*/
MS5611::MS5611(uint8_t cs_pin, SPIClass &spi_interface, uint32_t spi_freq = MS5611_MAX_SPI_FREQ) {
  _cs_pin = cs_pin;
  _spi = &spi_interface;
  _spi_freq = spi_freq;

  _spi_settings = SPISettings(_spi_freq, MSBFIRST, SPI_MODE0);

  _spi->begin();
  pinMode(_cs_pin, OUTPUT);
  deselect();
}

/*
  Initialize the chip

  Must be called before using the chip
  Will reset the chip and read the calibration data
*/
void MS5611::begin() {
  reset();
  readProm();
  setOSR(MS5611_OSR_256);
}

/*
  Read and convert pressure and temperature values from the MS5611 chip

  This method updates the attribute 'temperature' when called
  The temperature is given in 0.01°C
  This method updates the attribute 'pressure' when called
  Pressure is given in 0.01 mbar
*/
void MS5611::update() {
  // Read temperature and pressure
  uint32_t D1, D2;
  D1 = readAdc(MS5611_PRES);
  D2 = readAdc(MS5611_TEMP);

  convertMeasurement(D1, D2);
}

/*
  Set the Oversampling Ratio

  @param osr Oversampling Ratio
          - 0x00 for 256
          - 0x02 for 512
          - 0x04 for 1024
          - 0x06 for 2048
          - 0x08 for 4096
  
  @return 0 if the given osr is valid
          -1 otherwise
*/
uint8_t MS5611::setOSR(uint8_t osr) {
  switch (osr) {
    case MS5611_OSR_256:
      _osr = MS5611_OSR_256;
      _del = 1;  //ms
      break;

    case MS5611_OSR_512:
      _osr = MS5611_OSR_512;
      _del = 2;  //ms
      break;

    case MS5611_OSR_1024:
      _osr = MS5611_OSR_1024;
      _del = 3;  //ms
      break;

    case MS5611_OSR_2048:
      _osr = MS5611_OSR_2048;
      _del = 5;  //ms
      break;

    case MS5611_OSR_4096:
      _osr = MS5611_OSR_4096;
      _del = 10;  //ms
      break;

    default:
      _osr = MS5611_OSR_256;
      _del = 1;  //ms
      return -1;
      break;
  }
  return 0;
}

/*
  Send the reset command to the chip
*/
void MS5611::reset() {
  _spi->beginTransaction(_spi_settings);
  select();
  _spi->transfer(MS5611_RESET);
  delay(4);
  deselect();
  _spi->endTransaction();
}

/*
  Read calibration data from the MS5611 chip
*/
void MS5611::readProm() {
  for (uint8_t i = 0; i < 8; i++) {
    uint8_t data[2];
    readBytes(MS5611_PROM + 2 * i, 2, data);
    _c[i] = (data[0] << 8) + data[1];
  }
}

void MS5611::convertMeasurement(uint32_t D1, uint32_t D2) {
  // Convert temperature measurement to °C
  uint32_t dT, dTC6, Tref;
  int32_t Temp;
  Tref = (uint32_t)_c[5] << 8;
  if (D2 < Tref) {
    dT = Tref - D2;
    dTC6 = ((uint64_t)dT * (uint64_t)_c[6]) >> 23;
    Temp = 2000 - dTC6;
  } else {
    dT = D2 - Tref;
    dTC6 = ((uint64_t)dT * (uint64_t)_c[6]) >> 23;
    Temp = 2000 + dTC6;
  }

  // Convert pressure measurement to mbar
  uint64_t offT1, TCOdT;
  uint64_t sensT1, TCSdT;
  int64_t off, sens;
  int64_t P;
  offT1 = (uint64_t)_c[2] << 16;
  TCOdT = ((uint64_t)_c[4] * (uint64_t)dT) >> 7;
  sensT1 = (uint64_t)_c[1] << 15;
  TCSdT = ((uint64_t)_c[3] * (uint64_t)dT) >> 8;
  if (D2 < Tref) {
    off = offT1 - TCOdT;
    sens = sensT1 - TCSdT;
  } else {
    off = offT1 + TCOdT;
    sens = sensT1 + TCSdT;
  }

// Second order temperature compensation
#ifndef MS5611_SKIP_COMP
  uint32_t T2, off2, sens2;
  if (Temp < 2000) {
    T2 = (uint64_t)dT * (uint64_t)dT >> 31;
    off2 = 5 * (Temp - 2000) * (Temp - 2000) >> 1;
    sens2 = off2 >> 1;

    if (Temp < -1500) {
      uint32_t dtdt2;
      dtdt2 = (Temp + 1500) * (Temp + 1500);
      off2 += 7 * dtdt2;
      sens2 += 11 * dtdt2 >> 1;
    }
    Temp -= T2;
    off -= off2;
    sens -= sens2;
  }
#endif

  P = (D1 * sens) >> 21;
  P = (P - off) >> 15;

  temperature = Temp;
  pressure = (int32_t)P;
}

/*
  Start a conversion, wait, and read the result

  @param addr Address of the register to read
              - 0x40 for the pressure
              - 0x50 for the temperature

  @return val Value read from the register
*/
uint32_t MS5611::readAdc(uint8_t addr) {
  uint32_t val = 0;
  uint8_t data[3];

  readByte(addr + _osr);     // Trigger the conversion
  delay(_del);               // Wait for conversion to complete
  readBytes(0x00, 3, data);  // Read measurement
  val += data[0] << 16;
  val += data[1] << 8;
  val += data[2];

  return val;
}

/*
  Read a single byte

  @param address Address of the register to read

  @return val Byte read from the register
*/
uint8_t MS5611::readByte(uint8_t address) {
  uint8_t val;
  _spi->beginTransaction(_spi_settings);
  select();
  val = _spi->transfer(address);
  deselect();
  return val;
}

/*
  Read multiple bytes

  @param addr  Address of the register to read
  @param count Number of bytes to read
  @param data  Pointer to an array when to save the data read
*/
void MS5611::readBytes(uint8_t addr, uint8_t count, uint8_t *data) {
  _spi->beginTransaction(_spi_settings);
  select();

  _spi->transfer(addr);
  for (uint8_t i = 0; i < count; i++) {
    data[i] = _spi->transfer(0x00);
  }

  _spi->endTransaction();
  deselect();
}

/*
  Pull the CS pin LOW to enable the SPI interface on the chip
*/
void MS5611::select() {
  digitalWrite(_cs_pin, LOW);
}

/*
  Pull the CS pin HIGH to disable the SPI interface on the chip
*/
void MS5611::deselect() {
  digitalWrite(_cs_pin, HIGH);
}