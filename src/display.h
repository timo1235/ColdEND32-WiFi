#pragma once

/*

  ColdEND32 v1.2 Minimum Quantity Lubrication
  https://www.end-cnc-shop.de

  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License

  Written by Tilman, 2021-12-11
  Last edited by Tilman, 2021-12-29

*/

#if defined SSD1306 || defined SH1106
    #define OLED
    #include <Arduino.h>
    #include <U8g2lib.h>   // Required library: https://github.com/olikraus/U8g2_Arduino
    #include <Wire.h>
extern U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2;
#endif

#ifdef HT16K33
    #define LED
    #define LED1_ADD   0x74             // I2C address of first (mist) LED display (needs to be jumpered!)
    #define LED2_ADD   0x70             // I2C address of second (spit) LED display (needs to be jumpered!)
    #define BRIGHTNESS 6                // Brightness of LED displays
    #include <Adafruit_LEDBackpack.h>   // Required library: https://github.com/adafruit/Adafruit_LED_Backpack
    #include <Wire.h>
extern Adafruit_7segment led1;
extern Adafruit_7segment led2;
#endif

#if defined LCD16X2 || defined LCD16X4
    #define LCD
    #define LCD_ADD 0x27             // I2C address of LCD controller.
    #include <LiquidCrystal_I2C.h>   // Required library: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
    #include <Wire.h>
extern LiquidCrystal_I2C lcd;
#endif

void dispContent();

void refDisplay();