/*
Hello, World! example
June 11, 2015
Copyright (C) 2015 David Martinez
All rights reserved.
This code is the most basic barebones code for writing a program for Arduboy.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
*/

#include <Arduboy2.h>
#include <Adafruit_ESP8266.h>
#include <SoftwareSerial.h>
#define ESP_SSID "Han Shot First" // Your network name here
#define ESP_PASS "AceMarshallBatman" // Your network password here

#define HOST     "www.adafruit.com"     // Host to contact
#define PAGE     "/testwifi/index.html" // Web page to request
#define PORT     80                     // 80 = HTTP default port

#define LED_PIN  13

#define ARD_RX_ESP_TX   12
#define ARD_TX_ESP_RX   8
#define ESP_RST         4
SoftwareSerial softser(ARD_RX_ESP_TX, ARD_TX_ESP_RX); // Arduino RX = ESP TX, Arduino TX = ESP RX


// make an instance of arduboy used for many functions
Arduboy2 arduboy;
// Must declare output stream before Adafruit_ESP8266 constructor; can be
// a SoftwareSerial stream, or Serial/Serial1/etc. for UART.
Adafruit_ESP8266 wifi(&softser, &Serial, ESP_RST);
// Must call begin() on the stream(s) before using Adafruit_ESP8266 object.

// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
  // initiate arduboy instance
  arduboy.begin();

  // here we set the framerate to 15, we do not need to run at
  // default 60 and it saves us battery life
  arduboy.setFrameRate(15);

    char buffer[50];

  // Flash LED on power-up
  pinMode(LED_PIN, OUTPUT);
  for(uint8_t i=0; i<3; i++) {
    digitalWrite(LED_PIN, HIGH); delay(50);
    digitalWrite(LED_PIN, LOW);  delay(100);
  }

  // This might work with other firmware versions (no guarantees)
  // by providing a string to ID the tail end of the boot message:
  
  // comment/replace this if you are using something other than v 0.9.2.4!
  wifi.setBootMarker(F("Version:0.9.2.4]\r\n\r\nready"));

  softser.begin(9600); // Soft serial connection to ESP8266
  Serial.begin(57600); while(!Serial); // UART serial debug

  Serial.println(F("Adafruit ESP8266 Demo"));

  // Test if module is ready
  Serial.print(F("Hard reset..."));
  if(!wifi.hardReset()) {
    Serial.println(F("no response from module."));
    for(;;);
  }
  Serial.println(F("OK."));

  Serial.print(F("Soft reset..."));
  if(!wifi.softReset()) {
    Serial.println(F("no response from module."));
    for(;;);
  }
  Serial.println(F("OK."));

  Serial.print(F("Checking firmware version..."));
  wifi.println(F("AT+GMR"));
  if(wifi.readLine(buffer, sizeof(buffer))) {
    Serial.println(buffer);
    wifi.find(); // Discard the 'OK' that follows
  } else {
    Serial.println(F("error"));
  }

  Serial.print(F("Connecting to WiFi..."));
  if(wifi.connectToAP(F(ESP_SSID), F(ESP_PASS))) {

    // IP addr check isn't part of library yet, but
    // we can manually request and place in a string.
    arduboy.print(F("OK\nChecking IP addr..."));
    wifi.println(F("AT+CIFSR"));
    if(wifi.readLine(buffer, sizeof(buffer))) {
      arduboy.print(buffer);
      wifi.find(); // Discard the 'OK' that follows

      arduboy.print(F("Connecting to host..."));
      if(wifi.connectTCP(F(HOST), PORT)) {
        arduboy.print(F("OK\nRequesting page..."));
        if(wifi.requestURL(F(PAGE))) {
          arduboy.print("OK\nSearching for string...");
          // Search for a phrase in the open stream.
          // Must be a flash-resident string (F()).
          if(wifi.find(F("working"), true)) {
            arduboy.print(F("found!"));
          } else {
            arduboy.print(F("not found."));
          }
        } else { // URL request failed
          arduboy.print(F("error"));
        }
        wifi.closeTCP();
      } else { // TCP connect failed
        arduboy.print(F("D'oh!"));
      }
    } else { // IP addr check failed
      arduboy.print(F("error"));
    }
    wifi.closeAP();
  } else { // WiFi connection failed
    arduboy.print(F("FAIL"));
  }
}


// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  // first we clear our screen to black
  arduboy.clear();

  // we set our cursor 5 pixels to the right and 10 down from the top
  // (positions start at 0, 0)
  arduboy.setCursor(4, 9);

  // then we print to screen what is in the Quotation marks ""
  arduboy.print(F("Hello, world!"));

  // then we finaly we tell the arduboy to display what we just wrote to the display
  arduboy.display();
}
