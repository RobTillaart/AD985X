//
//    FILE: AD9851_demo_2.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: demo

#include "AD985X.h"

AD9851 freqGen;

uint32_t freq = 0;
uint32_t prev = 0;
uint32_t maxFreq;

void setup()
{
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("AD985X_LIB_VERSION: \t");
  Serial.println(AD985X_LIB_VERSION);

  help();

  freqGen.begin(10, 9, 8, 7, 6);
  freqGen.powerUp();
  maxFreq = freqGen.getMaxFrequency();
  Serial.print(" MAX_FREQ: ");
  Serial.println(maxFreq);
  Serial.print("REF_CLOCK: ");
  Serial.println(freqGen.getRefClock());
}

void loop()
{
  if (Serial.available() > 0)
  {
    int c = Serial.read();
    switch (c)
    {
      case '0'...'9' :
        freq *= 10;
        freq += c - '0';
        break;
      case '?' :
        help();
        break;
      case 'R' :
        freqGen.reset();
        freq = freqGen.getFrequency();
        break;
      case 'P' :
        freqGen.powerDown();
        break;
      case 'U' :
        freqGen.powerUp();
        break;
      case 'H' :
        freqGen.setRefClockHigh();
        Serial.print("REF_CLOCK: ");
        Serial.println(freqGen.getRefClock());
        break;
      case 'L' :
        freqGen.setRefClockLow();
        Serial.print("REF_CLOCK: ");
        Serial.println(freqGen.getRefClock());
        break;
      case '+' :
        freq += 1;
        break;
      case '-' :
        freq -= 1;
        break;
      case '*' :
        freq *= 10;
        break;
      case '/' :
        freq /= 10;
        break;
    }
    if (freq > maxFreq) freq = maxFreq;
  }

  // UPDATE AD985X IF NEW VALUE
  if (prev != freq)
  {
    prev = freq;

    freqGen.setFrequency(freq);
    freq = freqGen.getFrequency();
    Serial.println(freq);
  }
}

void help()
{
  Serial.println();
  Serial.println("     HELP AD985X");
  Serial.println("+ :  freq = freq + 1");
  Serial.println("- :  freq = freq - 1");
  Serial.println("* :  freq = freq * 10");
  Serial.println("/ :  freq = freq / 10");
  Serial.println("? :  help");
  Serial.println("R :  AD9851 reset");
  Serial.println("P :  AD9851 power down");
  Serial.println("U :  AD9851 power up");
  Serial.println("H :  AD9851 set reference clock HIGH");
  Serial.println("L :  AD9851 set reference clock LOW");
  Serial.println();
}

// END OF FILE
