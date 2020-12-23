#pragma once
//
//    FILE: AD985X.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.1
//    DATE: 2019-02-08
// PURPOSE: Class for AD9851 function generator
//
//     URL: https://github.com/RobTillaart/AD985X
//

#include "Arduino.h"
#include "SPI.h"

#define AD985X_LIB_VERSION "0.1.1"

#define AD9850_MAX_FREQ      (20UL * 1000UL * 1000UL)
#define AD9851_MAX_FREQ      (70UL * 1000UL * 1000UL)

class AD985X
{
public:
  AD985X();

  // for HW SPI only use lower 3 parameters.
  void     begin(int select, int resetPin, int FQUDPin, int dataOut = 0, int clock = 0);
  void     reset();
  void     powerDown();
  void     powerUp();

  // 0 .. 10.000.000(?)
  virtual void setFrequency(uint32_t freq) = 0;      // = 0  produces right error message
  uint32_t     getFrequency() { return _freq; };
  virtual uint32_t getMaxFrequency() = 0;

  // 0 .. 31  steps of 11.25 degrees
  void     setPhase(uint8_t phase = 0);
  uint8_t  getPhase() { return (_config >> 3); };

protected:
  void     pulsePin(uint8_t pin);
  void     writeData();
  void     swSPI_transfer(uint8_t value);

  bool     _useHW   = true;
  uint8_t  _dataOut = 0;
  uint8_t  _clock   = 0;
  uint8_t  _select  = 0;

  uint32_t _freq    = 1;
  uint32_t _factor  = 0;
  uint8_t  _config  = 0;
  uint8_t  _reset   = 0;
  uint8_t  _fqud    = 0;
};


class AD9851 : public AD985X
{
public:
  void     setFrequency(uint32_t freq);  // 0..AD9851_MAX_FREQ
  uint32_t getMaxFrequency() { return AD9851_MAX_FREQ; };
  void     setRefClockHigh();   // 180 MHz
  void     setRefClockLow();    //  30 MHz
  uint8_t  getRefClock();
};

class AD9850 : public AD985X
{
public:
  void     setFrequency(uint32_t freq);  // 0..AD9850_MAX_FREQ
  uint32_t getMaxFrequency() { return AD9850_MAX_FREQ; };
};


// -- END OF FILE --
