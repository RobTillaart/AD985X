
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

### Constructors

- **AD9850()** 20 MHz signal generator
- **AD9851()** 70 MHz signal generator, derived from AD9850 with some extra options.

### Common interface

- **begin(selectPin, resetPin, FQUDPin, dataOut = 0, clock = 0)**  
For hardware SPI only use the first three parameters, for SW SPI you need to define 
the other three too.
  - selectPin = chip select
  - resetPin = reset
  - FQUD = Frequency UpDate Pin
- **reset()** reset the chip.
- **powerDown()** idem
- **powerUp()** idem
- **setFrequency(uint32_t freq)** SetFrequency sets the frequency and is limited by the 
MaxFrequency of the class used. For the AD9850 => 20 MHz, for the AD9851 => 70 MHz. 
  - Note that the quality of the signal gets less at higher frequencies. 
  - Note that in the base class this function is virtual.
- **setFrequencyF(float freq)** SetFrequencyF sets the frequency with float.
- **getMaxFrequency()** returns the maximum frequency setable. For the AD9850 this is 20 MHz, for the AD9851 this is 70 MHz. Note that in the base class this function is virtual.
- **getFrequency()** returns the frequency set. As it returns a float it might loose some accuracy at higher digits.
- **setPhase(uint8_t phase = 0)** set the phase in units of 11.25°  0..31 allowed. 
Default it sets the phase to 0.
- **getPhase()** returns the set phase.

#### Experimental

- **setCalibrationOffset(int32_t offset = 0)** sets an offset to calibrate the frequency.
- **getCalibrationOffset()** reads back the offset set.
- Note: **reset()** resets the offset too.
- Note: setting the offset reduces the range of frequencies (at the ends of scale).

### AD9850 specific

The AD9850 has no specific functions.

### AD9851 specific

- **setRefClockHigh()** set reference clock to 180 Mhz.
- **setRefClockLow()**  set reference clock to 30 Mhz.
- **getRefClock()** returns 30 or 180.

#### Experimental

- **setAutoRefClock(bool arc)** sets a flag so the library switches automatically
to the reference clock of 180 MHz when the frequency is set above 1 MHz and 
to 30 MHz when the frequency is set to 1 MHz or lower.
- **getAutoRefClock()** returns true is automode is set. 
Default is **false** == OFF for backwards compatibility. 
- Note: the autoRefClock mode does **NOT** automatically adjust the calibration offset.
- Note: **reset()** does **NOT** reset the autoRefClock flag.

## Operation

See examples

### Operational notes

- The refClock needs to be set HIGH for frequencies above 1 MHz to have a smooth sinus. 
- The quality of the signal becomes less at 20 MHz. 
- If the calibration offset is not 0, it needs to be set by the user after every startup, and after switching the reference clock. The user is also responsible to store it e.g. in EEPROM to make it persistent.
- Experimental parts may change or removed in the future.

