
[![Arduino CI](https://github.com/RobTillaart/AD985X/workflows/Arduino%20CI/badge.svg)](https://github.com/marketplace/actions/arduino_ci)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](https://github.com/RobTillaart/AD985X/blob/master/LICENSE)
[![GitHub release](https://img.shields.io/github/release/RobTillaart/AD985X.svg?maxAge=3600)](https://github.com/RobTillaart/AD985X/releases)

# AD985X

Arduino library for AD9850 and AD9851 function generators.


## Description

Library for the AD9850 and AD9851 function generators.
The library has a AD9850 base class that implements the commonalities.
The AD9851 is derived and has its own **setFrequency()** methods.
Furthermore the AD9851 also has function to select the reference clock,
a feature the AD9850 does not have.

**Warning**
The library is not suitable for AD9852 as that is a function generator with
way more functionality.

## Connection

TO elaborate

```
          TOP VIEW
        +-----------+
        |   X-TAL   |
        |         L |
    VCC | o       o | VCC
    CLK | o       o | D0
   PUFD | o       o | D1
   DATA | o       o | D2
  RESET | o       o | D3
    GND | o CCC   o | D4
  QOUT1 | o CCC   o | D5
  QOUT2 | o       o | D6
  ZOUT1 | o       o | D7 ----- SELECT SERIAL LOW
  ZOUT2 | o  PP   o | GND
        |    PP     |
        +-----------+

  XTAL = crystal
  L    = LED
  C    = chip
  P    = potmeter => for duty cycle square wave

```


## Interface

### Constructors

- **AD9850()** 40 MHz signal generator
- **AD9851()** 70 MHz signal generator, derived from AD9850 with some extra options.


### Common interface

- **begin(selectPin, resetPin, FQUDPin, dataOut = 0, clock = 0)**  
For hardware SPI only use the first three parameters, 
for SW SPI you need to define the other two too.
  - selectPin = chip select
  - resetPin = reset
  - FQUD = Frequency UpDate Pin
- **reset()** resets the function generator.
- **powerDown()** idem
- **powerUp()** idem
- **setFrequency(uint32_t freq)** SetFrequency sets the frequency and is limited by the 
MaxFrequency of the class used. For the AD9850 => 40 MHz, for the AD9851 => 70 MHz. 
  - Note that the quality of the signal gets less at higher frequencies. 
- **setFrequencyF(float freq)** SetFrequencyF sets the frequency with a float with a maximum of **two** decimals. 
Note that a float only has a mantisse of 6-7 digits so above ~1.000.000 decimals are lost.
- **getMaxFrequency()** returns the maximum frequency setable. For the AD9850 this is 20 MHz.
For the AD9851 this is 70 MHz.
- **getFrequency()** returns the frequency set. As it returns a float it might loose some accuracy at higher frequencies.
- **setPhase(uint8_t phase = 0)** set the phase in units of 11.25°  0..31 allowed. 
Default it sets the phase to 0.
- **getPhase()** returns the phase set, 0 by default.


### AD9850 specific

The AD9850 has no specific functions.


### AD9851 specific

- **setRefClockHigh()** set reference clock to 180 Mhz.
- **setRefClockLow()**  set reference clock to 30 Mhz.
- **getRefClock()** returns 30 or 180.
- **setAutoRefClock(bool arc)** sets a flag so the library switches automatically
to the reference clock of 180 MHz when the frequency is set above 10 MHz and 
to 30 MHz when the frequency is set to 10 MHz or lower.
The initial value is **false** == OFF for backwards compatibility. 
- **getAutoRefClock()** returns true is automode is set. 
- **void setARCCutOffFreq(uint32_t Hz = 10000000UL )** set cut off ferquency for the auto reference clock. max value is 30 MHz, typical 10MHz
- **uint32_t getARCCutOffFreq()** returns cut off frequency set.


- Note: the autoRefClock mode does **NOT** automatically adjust the calibration offset.
- Note: **reset()** does **NOT** reset the autoRefClock flag.


## Operation

See examples


### Operational notes

- The refClock needs to be set HIGH for frequencies above 1 MHz to have a smooth sinus. 
- The quality of the signal becomes less at higher frequencies.
- If the calibration offset is not 0, it needs to be set by the user after every startup, and after switching the reference clock. The user is also responsible to store it e.g. in EEPROM to make it persistent.
- Experimental parts may change or removed in the future.

