

/*
Author:........: Joe Pitz
Name:..........: Current_Logger
Date:..........: 10/21/2023
Copyright:.....: Attribution-NonCommercial 4.0 International
                 https://creativecommons.org/licenses/by-nc/4.0/

Description:...: Measure current using an AC/DC clamp meter.
We will be checking the voltage and printing it every 5 seconds (5000 milliseconds). Do not use the delay() function.
The delay() function stops the processor. The best way is to use the millis() function.
Check the following link for a good  explanation of the millis() function:
https://www.baldengineer.com/blink-without-delay-explained.html

*/

// Include Statements (Tells the compiler which libraries to reference)
#include <Adafruit_ADS1X15.h>
#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <MUIU8g2.h>

// Variables and constants
const float f40amp = 0.01900;
const float f400amp = 0.2400;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
unsigned long interval = 5000;  // 5000 milliseconds, 5 seconds

// Objects
Adafruit_ADS1X15 ads;

// setup OLED Display I2C pins are 14, 12
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,/*clock*/14,/*data*/12,U8X8_PIN_NONE);

void setup() {
  // Setup Serial Monitor
  Serial.begin(115200);
  // Set OLED display
  u8g2.begin();
  // set up wire for ads sdk, data pins
  Wire.begin(4,5);
  
 if (!ads.begin()) {
    Serial.println("Failed to initialize ADS.");
    while (1);
 }  
  // cannot use ads.setGain(), we need a custom multipler see f40amp, f400amp constants
}

void loop() {
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) 
  {
    previousMillis = currentMillis;

    // read the current from the ADC board
    int16_t  ad0 = ads.readADC_Differential_0_1();
    Serial.println(float(ad0) * f40amp,3);

    // print the current to the LED
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_9x18_mr);
    u8g2.drawStr(1,10,"Amperage:");
    u8g2.setFont(u8g2_font_lubR19_tn);
    u8g2.setCursor(10,55);
    u8g2.print(float(ad0) * f40amp,3);
    u8g2.sendBuffer();

  }
}