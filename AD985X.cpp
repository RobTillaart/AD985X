//
//    FILE: AD985X.cpp
//  AUTHOR: Rob Tillaart
// VERSION: 0.2.0
//    DATE: 2019-02-08
// PURPOSE: Class for AD9850 and AD9851 function generator
//
//  HISTORY:
//  0.1.0   2019-03-19  initial version
//  0.1.1   2020-12-09  add arduino-ci
//  0.1.2   2020-12-27  add setAutoMode() + offset
//  0.2.0   2020-12-28  major refactor class hierarchy + float frequency

#include "AD985X.h"


// UNO HARDWARE SPI           PINS
#define SPI_CLOCK             13
#define SPI_MISO              12
#define SPI_MOSI              11

#define AD985X_POWERDOWN      0x04


AD9850::AD9850()
{
}

void AD9850::begin(int select, int resetPin, int FQUDPin, int dataOut , int clock)
{
  _select = select;
  _reset  = resetPin;
  _fqud   = FQUDPin;
  pinMode(_select, OUTPUT);
  pinMode(_reset,  OUTPUT);
  pinMode(_fqud,   OUTPUT);
  digitalWrite(_select, LOW);
  digitalWrite(_reset,  LOW);
  digitalWrite(_fqud,   LOW);
  _useHW     = true;

  // SW SPI
  if ((dataOut != 0) && (_clock != 0))
  {
    _dataOut   = dataOut;
    _clock     = clock;
    pinMode(_dataOut,  OUTPUT);
    pinMode(_clock,    OUTPUT);
    digitalWrite(_dataOut,  LOW);
    digitalWrite(_clock,    LOW);
    _useHW     = false;
  }

  if (_useHW)
  {
    SPI.begin();  // set MOSI & MISO pin right.
  }

  reset();
}

void AD9850::reset()
{
  pulsePin(_reset);
  if (_useHW) pulsePin(SPI_CLOCK);
  else pulsePin(_clock);

  _config = 0;    // 0 phase   no powerdown   30 MHz
  _freq   = 0;
  _factor = 0;
  _offset = 0;
  writeData();
}

void AD9850::powerDown()
{
  _config |= AD985X_POWERDOWN;      // keep phase and REFCLK as is.
  writeData();
}

void AD9850::powerUp()
{
  _config &= ~AD985X_POWERDOWN;      // TODO MAGIC NR.
  writeData();
}

void AD9850::setPhase(uint8_t phase)
{
  if (phase > 31) return;
  _config &= 0x07;
  _config |= (phase << 3);
  writeData();
}

void AD9850::pulsePin(uint8_t pin)
{
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}

void AD9850::writeData()
{
  // Serial.println(_factor, HEX);
  // Serial.println(_config, HEX);
  uint32_t data = _factor;
  if (_useHW)
  {
    SPI.beginTransaction(SPISettings(2000000, LSBFIRST, SPI_MODE0));
    digitalWrite(_select, LOW);

    SPI.transfer(data & 0xFF);
    data >>= 8;
    SPI.transfer(data & 0xFF);
    data >>= 8;
    SPI.transfer(data & 0xFF);
    SPI.transfer(data >> 8);
    SPI.transfer(_config);

    digitalWrite(_select, HIGH);
    SPI.endTransaction();
  }
  else
  {
    digitalWrite(_select, LOW);

    swSPI_transfer(data & 0xFF);
    data >>= 8;
    swSPI_transfer(data & 0xFF);
    data >>= 8;
    swSPI_transfer(data & 0xFF);
    swSPI_transfer(data >> 8);
    swSPI_transfer(_config);

    digitalWrite(_select, HIGH);
  }

  // update frequency + phase + control bits.
  pulsePin(_fqud);
}

// simple one mode version
void AD9850::swSPI_transfer(uint8_t value)
{
  // for (uint8_t mask = 0x80; mask; mask >>= 1)   // MSBFIRST
  for (uint8_t mask = 0x01; mask; mask <<= 1)   // LSBFIRST
  {
    digitalWrite(_dataOut,(value & mask) != 0);
    digitalWrite(_clock, HIGH);
    digitalWrite(_clock, LOW);
  }
}

void AD9850::setFrequency(uint32_t freq)
{
  // freq OUT = (Δ Phase × CLKIN)/2^32
  // 64 bit math to keep precision to the max
  if (freq > AD9850_MAX_FREQ) freq = AD9850_MAX_FREQ;
  _factor = (147573952590ULL * freq) >> 32;  //  (1 << 64) / 125000000
  _freq = freq;
  _factor += _offset;
  writeData();
}

// from 16777216 and up the uint32_t is more precise ...
// TODO: test accuracy decimals
void AD9850::setFrequencyF(float freq)
{
  // freq OUT = (Δ Phase × CLKIN)/2^32
  // 64 bit math to keep precision to the max
  if (freq > AD9850_MAX_FREQ) freq = AD9850_MAX_FREQ;
  _factor = uint64_t(147573952590ULL * freq) >> 32;  //  (1 << 64) / 125000000
  _freq = freq;
  _factor += _offset;
  writeData();
}

////////////////////////////////////////////////////////
//
// AD9851
//

#define AD9851_REFCLK        0x01    // bit is a 6x multiplier bit P.14 datasheet

void AD9851::setFrequency(uint32_t freq)
{
  if (freq > AD9851_MAX_FREQ) freq = AD9851_MAX_FREQ;
  // AUTO SWITCH REFERENCE FREQUENCY
  if (_autoRefClock && (freq > 1000000))
  {
    _config |= AD9851_REFCLK;
  }
  else
  {
    _config &= ~AD9851_REFCLK;
  }

  if (_config & AD9851_REFCLK)  // 6x 30 = 180 MHz
  {
    _factor = (102481911520ULL * freq) >> 32;  //  (1 << 64) / 180000000
  }
  else                          // 1x 30 = 30 MHz
  {
    _factor = (614891469123ULL * freq) >> 32;  //  (1 << 64) / 30000000
  }
  _freq = freq;
  _factor += _offset;
  writeData();
}

// from 16777216 and up the uint32_t is more precise..
// TODO: test accuracy decimals
void AD9851::setFrequencyF(float freq)
{
  if (freq > AD9851_MAX_FREQ) freq = AD9851_MAX_FREQ;
  // AUTO SWITCH REFERENCE FREQUENCY
  if (_autoRefClock && (freq > 1000000))
  {
    _config |= AD9851_REFCLK;
  }
  else
  {
    _config &= ~AD9851_REFCLK;
  }

  if (_config & AD9851_REFCLK)  // 6x 30 = 180 MHz
  {
    _factor = uint64_t(102481911520ULL * freq) >> 32;  //  (1 << 64) / 180000000
  }
  else                          // 1x 30 = 30 MHz
  {
    _factor = uint64_t(614891469123ULL * freq) >> 32;  //  (1 << 64) / 30000000
  }
  _freq = freq;
  _factor += _offset;
  writeData();
}

void AD9851::setRefClockHigh()
{
  _config |= AD9851_REFCLK;
  writeData();
}

void AD9851::setRefClockLow()
{
  _config &= ~AD9851_REFCLK;
  writeData();
}

uint8_t AD9851::getRefClock()
{
  return (_config & AD9851_REFCLK) ? 180 : 30;
}

// -- END OF FILE --
