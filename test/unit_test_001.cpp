//
//    FILE: unit_test_001.cpp
//  AUTHOR: Rob Tillaart
//    DATE: 2020-12-03
// PURPOSE: unit tests for the AD9850 and AD9851 function generator
//          https://github.com/RobTillaart/AD985X
//          https://github.com/Arduino-CI/arduino_ci/blob/master/REFERENCE.md
//

// supported assertions
// ----------------------------
// assertEqual(expected, actual)
// assertNotEqual(expected, actual)
// assertLess(expected, actual)
// assertMore(expected, actual)
// assertLessOrEqual(expected, actual)
// assertMoreOrEqual(expected, actual)
// assertTrue(actual)
// assertFalse(actual)
// assertNull(actual)

#include <ArduinoUnitTests.h>

#include "Arduino.h"
#include "AD985X.h"


unittest_setup()
{
}

unittest_teardown()
{
}


unittest(test_constructor)
{
  fprintf(stderr, "VERSION: %s\n", AD985X_LIB_VERSION);
  AD9850 funcgen0;
  AD9851 funcgen1;

  assertEqual(AD9850_MAX_FREQ, funcgen0.getMaxFrequency());
  assertEqual(AD9851_MAX_FREQ, funcgen1.getMaxFrequency());
}


unittest(test_ad9850)
{
  AD9850 funcgen;
  funcgen.begin(4, 5, 6);

  funcgen.setFrequency(1000);
  long freq = funcgen.getFrequency();
  assertEqual(1000, freq);

  for (int ph = 0; ph < 32; ph += 4)
  {
    funcgen.setPhase(ph);
    int phase = funcgen.getPhase();
    assertEqual(ph, phase);
  }
}


unittest(test_ad9851)
{
  AD9851 funcgen;
  funcgen.begin(4, 5, 6);
  
  funcgen.setFrequency(1000);
  long freq = funcgen.getFrequency();
  assertEqual(1000, freq);

  for (int ph = 0; ph < 32; ph += 4)
  {
    funcgen.setPhase(ph);
    int phase = funcgen.getPhase();
    assertEqual(ph, phase);
  }

  funcgen.setRefClockHigh();
  assertEqual(180, funcgen.getRefClock());
  funcgen.setRefClockLow();
  assertEqual(30, funcgen.getRefClock());
}


unittest(test_ad9851_reset)
{
  AD9851 funcgen;
  funcgen.begin(4, 5, 6);
  
  funcgen.setFrequency(1000);
  assertEqual(1000, funcgen.getFrequency());
  funcgen.setPhase(14);
  assertEqual(14, funcgen.getPhase());
  funcgen.setRefClockHigh();
  assertEqual(180, funcgen.getRefClock());
  
  funcgen.reset();

  assertEqual(0,  funcgen.getFrequency());
  assertEqual(0,  funcgen.getPhase());
  assertEqual(30, funcgen.getRefClock());
}


unittest(test_ad9851_autoRefClock)
{
  AD9851 funcgen;
  funcgen.begin(4, 5, 6);

  assertFalse(funcgen.getAutoRefClock());
  for (uint32_t freq = 70; freq <= 70000000; freq *= 10)
  {
    funcgen.setFrequency(freq);
    fprintf(stderr, "freq %ld\t", freq);
    assertFalse(funcgen.getAutoRefClock());
  }

  funcgen.setAutoRefClock(true);
  assertFalse(funcgen.getAutoRefClock());
  for (uint32_t freq = 70; freq <= 1000000; freq *= 10)
  {
    funcgen.setFrequency(freq);
    fprintf(stderr, "freq %ld\t", freq);
    assertFalse(funcgen.getAutoRefClock());
  }
  funcgen.setFrequency(7000000);
  fprintf(stderr, "freq %ld\t", freq);
  assertTrue(funcgen.getAutoRefClock());
  funcgen.setFrequency(70000000);
  fprintf(stderr, "freq %ld\t", freq);
  assertTrue(funcgen.getAutoRefClock());
}


unittest(test_ad9851_offset)
{
  AD9851 funcgen;
  funcgen.begin(4, 5, 6);

  assertEqual(0, funcgen.getCalibrationOffset());
  funcgen.setFrequency(1000000);

  for (int32_t offset = -1000; offset <= 1000; offset += 10)
  {
    funcgen.setCalibrationOffset(offset);
    assertEqual(1000000, funcgen.getFrequency());
    assertEqual(offset, funcgen.getCalibrationOffset());
  }

  funcgen.setCalibrationOffset(offset);
  assertEqual(0, funcgen.getCalibrationOffset());
}


unittest_main()

// --------
