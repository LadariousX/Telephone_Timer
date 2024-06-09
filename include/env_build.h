#ifndef ENV_BUILD_H
#define ENV_BUILD_H

#include <Arduino.h>
#include <LinkedList.h>
#include <network_config.h>

#define PIN_switch 15
#define PIN_bell 16
#define PIN_rotary 14

#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <RTClib.h>

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1        
#define SCREEN_ADDRESS 0x3C  

extern Adafruit_SSD1306 display;
extern RTC_DS3231 rtc;
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;




void buildDisplay();

void buildWifi();

void buildClocks();

#endif 