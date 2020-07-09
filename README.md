# MS5611

Library for the MS5611 Barometric Pressure Sensor based on the Arduino framework. This library is intended to use the SPI interface only. This library has been tested on Teensy 3.6 and Arduino Nano.

**/!\ NOTICE /!\\**

**Pre-release, not fully tested yet**

## TODO

- [ ] Add CRC
- [ ] Write keywords
- [ ] Use non-blocking delays

## Installation

### As a library

This repository can be downloaded as a zip file and extracted in your Arduino library folder

### Development

1. Install PlatformIO
2. Clone this repository

Run the project in a PlatformIO terminal using

```shell
platformio run -t upload # Defaults, upload to Teensy 3.6
# OR
platformio run -t upload -e nano
```

## Unit testing

This library uses PlatformIO unit tests to verify the code validity

```shell
platformio test
```

## Folder structure

```cpp
├── doc/
├── examples/                   // Example sketches
│   └── BasicPressure/
│       └── BasicPressure.ino
├── src/                        // Source files for the library, compatible with PlatformIO
│   ├── MS5611/
│   │   ├── MS5611.cpp
│   │   └── MS5611.h
│   └── MS5611.h                // Dummy header for compliance with the Arduino library format
├── test/                       // Unit tests
│   ├── test_main.cpp
│   └── test.py
├── library.properties
├── LICENSE
├── platformio.ini
└── README.md                   // This file
```