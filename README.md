# MS5611

Library for the MS5611 Barometric Pressure Sensor based on the Arduino framework. This library is intended to use the SPI interface only. This library has been tested on Teensy 3.6 and Arduino Nano.

**/!\ NOTICE /!\\**

**Pre-release, not fully tested yet**

## TODO

- [ ] Add CRC
- [ ] Write keywords
- [ ] Use non-blocking delays

## Folder structure

```cpp
├── doc/
├── examples/                   // Example sketches
│   └── BasicPressure/
│       └── BasicPressure.ino
├── library.properties
├── LICENSE
├── platformio.ini
├── README.md                   // This file
├── src/                        // Source files for the library
│   ├── MS5611
│   │   ├── MS5611.cpp
│   │   └── MS5611.h
│   └── MS5611.h
├── test/                       // Unit tests
│   └── test_main.cpp
└── test.py
```