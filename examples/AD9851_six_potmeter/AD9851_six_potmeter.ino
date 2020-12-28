//
//    FILE: AD9851_six_potmeter.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: demo


#include "AD985X.h"

AD9851 freqGen; 

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("AD985X_LIB_VERSION: \t");
  Serial.println(AD985X_LIB_VERSION);

  freqGen.begin(10, 9, 8);
}

void loop()
{
  float freq = readFreq();

  freqGen.setFrequencyF(freq);
  Serial.println(freq);
  delay(100);
}

float readFreq()
{
  float freq = 0;
  freq += (analogRead(A0) / 103) * 1000;
  freq += (analogRead(A1) / 103) * 100;
  freq += (analogRead(A2) / 103) * 10;
  freq += (analogRead(A3) / 103) * 1;
  freq += (analogRead(A4) / 103) * 0.1;
  freq += (analogRead(A5) / 103) * 0.01;
  return freq;
}

// END OF FILE
