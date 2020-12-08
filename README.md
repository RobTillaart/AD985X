
[![Arduino CI](https://github.com/RobTillaart/AD985X/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/AD985X/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/AD985X.svg?maxAge=3600)](https://github.com/RobTillaart/AD985X/releases)

# AD985X

Arduino library for AD9850 and AD9851 function generators.


## Description

Experimental library for the AD9850 and AD9851 function generators.
The library has a AD985X base class that implements the commonalities.
The AD9850 and AD9851 are derived and have their own setFrequency method.
Furthermore the AD9851 also has function to select the reference clock,
a feature the AD9850 does not have.

The libraries are not extensively tested so please file an issue if problems arise.

**Warning**
The library is not suitable for AD9852 as that is a function generator with
way more functionality.


## Interface

### COnstructors

- **AD985X()** base class, do not use directly as it will not compile.
- **AD9850()**
- **AD9851()**

### Basic interface
- **begin(selectPin, resetPin, FQUDPin, dataOut = 0, clock = 0)**  
For hardware SPI only use the first three parameters, for SW SPI you need to define 
the other three too.
  - selectPin = chip select
  - resetPin = reset
  - FQUD = Frequency UpDate Pin
- **reset()** reset the chip.
- **powerDown()** idem
- **powerUp()** idem
- **setFrequency(uint32_t freq)** Note setFrequency is different implemented 
per derived class. In the base class it is virtual.
- **getFrequency()** returns the fequency set
- **setPhase(uint8_t phase = 0)** set the phase in units of 11.25°  0..31 allowed. 
Default it sets the phase to 0.
- **getPhase()** returns the set phase.

### AD9851 specific

The AD9851 also implements:
- **setRefClock(uint8_t clock)** HIGH or LOW ??? 180 / 30 depends on chip used.  
  TODO REDO INTERFACE HERE
- **getRefClock()**


## Operation

See examples
