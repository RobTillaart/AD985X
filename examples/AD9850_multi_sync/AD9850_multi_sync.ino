//
//    FILE: AD9850_multi_sync.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo multi device
//     URL: https://github.com/RobTillaart/AD985X

// measure speed difference when updating multiple devices
// - sequentially
// - simultaneously


#include "AD985X.h"

#define SELECT1   2
#define SELECT2   3
#define SELECT3   4
#define SELECT4   5
#define SELECT5   6

#define RESET     9
#define FQ_UD     10

// we want to control four hardware devices
// so we declare three software objects
AD9850 freqGen0;
AD9850 freqGen1;
AD9850 freqGen2;
AD9850 freqGen3;
AD9850 freqGen4;

float    freq = 10000;


void setup()
{
  // OPEN SERIAL for messages and debugging etc
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("AD985X_LIB_VERSION: \t");
  Serial.println(AD985X_LIB_VERSION);

  // initialize three devices
  freqGen0.begin(SELECT1, RESET, FQ_UD, 11, 12);
  freqGen0.powerUp();
  freqGen1.begin(SELECT2, RESET, FQ_UD, 11, 12);
  freqGen1.powerUp();
  freqGen2.begin(SELECT3, RESET, FQ_UD, 11, 12);
  freqGen2.powerUp();
  freqGen3.begin(SELECT4, RESET, FQ_UD, 11, 12);
  freqGen3.powerUp();
  freqGen4.begin(SELECT5, RESET, FQ_UD, 11, 12);
  freqGen4.powerUp();

  test1();

  test2();

  Serial.println("\ndone...");
}


void loop()
{
}

void test1()
{
  uint32_t start = micros();
  freqGen0.setFrequencyF(freq);
  // freqGen1.setFrequencyF(freq);
  // freqGen2.setFrequencyF(freq);
  // freqGen3.setFrequencyF(freq);
  //freqGen4.setFrequencyF(freq);
  uint32_t duration = micros() - start;

  Serial.print("SEQUENTIAL:\t");
  Serial.println(duration);
  delay(100);
}

void test2()
{
  uint32_t start = micros();
  digitalWrite(SELECT1, HIGH);
  // digitalWrite(SELECT2, HIGH);
  // digitalWrite(SELECT3, HIGH);
  // digitalWrite(SELECT4, HIGH);
  //digitalWrite(SELECT5, HIGH);

  // writing to device 1 will automatically also write to device 2 and 3
  freqGen0.setFrequency(freq);
  // as setFrequency pulls the SELECT line low we need to pull it high again.
  digitalWrite(SELECT1, HIGH);

  // update the frequency of all three devices - shared FQ_UD line
  digitalWrite(FQ_UD, HIGH);
  digitalWrite(FQ_UD, LOW);

  // restore the unselect state for all devices
  digitalWrite(SELECT1, LOW);
  // digitalWrite(SELECT2, LOW);
  // digitalWrite(SELECT3, LOW);
  // digitalWrite(SELECT4, LOW);
  //digitalWrite(SELECT5, LOW);
  uint32_t duration = micros() - start;

  Serial.print("SIMULTANEOUS:\t");
  Serial.println(duration);
  delay(100);
}





// -- END OF FILE --
