#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <SD.h>

#define BTN_START     25
#define BTN_UP        6       
#define BTN_DOWN      8
#define BTN_OK        7
#define VALVE_PORT    22
#define PRESSURE_SENS A0
#define CURRENT_SENS  A1

#define COMP_PORTS     {23, 24, 26, 27, 28, 29}

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#endif