/*This sketch runs an hours-minutes-seconds binary clock with one LED matrix
  based on MAX72xx chipset and DS3231 RTC (Real time clock). Written for an Arduino nano clone.
  This runs a 24-hour display (i.e. not am/pm).

  Wiring for the MAX72xx LED matrix: different from library examples.
    Pin D10 is connected to the DIN (DataIn)
    Pin D11 is connected to CS (LOAD)
    Pin D12 is connected to the CLK
     
  DS3231RTC (I2C bus)
    Nano A4 to sca, A5 to scl.

*/

#include <DS1307RTC.h>
#include <Time.h>
#include <Wire.h>
#include "LedControl.h"
#include "RTClib.h"

//Pin definition constants
const int mDataIn=50, mLoad=51, mClk=53;  //Max7219 LED matrix


int h1, h2, m1, m2, s1, s2; // Each digit
int tChange; //Scratch used during change time via pushbutton

// Matrix library instance
LedControl lc=LedControl(mDataIn, mClk, mLoad,1); //Pin assignments and number of matrices (1)

RTC_DS1307 rtc;

void setup() {
  
  //Initialize the MAX72XX in power-saving mode.
  lc.shutdown(0,false);
  lc.setIntensity(0,1); // Set brightness to a low value
  lc.clearDisplay(0); // Clear the display
  delay(100); // Wait after initializing display
  
  Serial.begin(57600);
  #ifndef ESP8266
  while (!Serial);
  #endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {

  DateTime now = rtc.now();

  //Isolate hours, minutes, seconds, one digit to each LED matrix column.
  h1=n1(now.hour());
  h2=n2(now.hour());
  m1=n1(now.minute());
  m2=n2(now.minute());
  s1=n1(now.second());
  s2=n2(now.second());
  
  // Cast as (change integer to) byte, write to LED row.
  // (Row runs from pin to pin on model used.)
  lc.setRow(0,0,byte(h1));
  lc.setRow(0,1,byte(h2));
  lc.setRow(0,3,byte(m1));
  lc.setRow(0,4,byte(m2));
  lc.setRow(0,6,byte(s1));
  lc.setRow(0,7,byte(s2));
  }
  

  int n1(int num)  // Function to isolate first digit of 2-digit integer
  {
    num = num / 10; //Integer division by 10 (discard remainder)
    return num;
  }
  
  int n2(int num)  // Function to isolate second digit of 2-digit integer
  {
    num = num % 10; //Modulo division by 10 (keep remainder only)
    return num;
  }
  
  
