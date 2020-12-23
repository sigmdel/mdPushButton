/*
 * push_button_basic.ino
 * 
 * Example program for the mdPushButton library
 * <https://github.com/sigmdel/mdPushButton>
 *
 */

// SPDX-License-Identifier: 0BSD 
 
#include <Arduino.h>           // for PlatformIO
#include "mdPushButton.h"

// Set the following 3 macros to correspond to the
// connection with the board. 
//
// In PlatformIO, the macros can be defined in the 
// platformio.ini configuration file 

#ifndef SERIAL_BAUD
  #define SERIAL_BAUD 9600
#endif

#ifndef BUTTON_PIN
  #define BUTTONN_PIN 7
#endif

#ifndef ACTIVE
  #define ACTIVE == LOW
#endif


// ---------------------------------------------------


#if (ACTIVE == HIGH) 
  // Connection of an active HIGH normally open button would be
  //                         _________________
  //           __||__       |
  // Vcc ------o    o--+---=|BUTTON_PIN (GPIO)
  //                   |    |________________
  //                   |   ____
  //                   +--[____]---GND   (external pull-down resistor)
  //              
  // If there's no external pull-down resistor, set useInternalPullResistor to true
  // Many micro-controllers do not have internal pull-down resistors 
  //
  mdPushButton button = mdPushButton(BUTTON_PIN, HIGH, true);
#else  
  // Connection of an active LOW normally open button would be
  //                         ________________
  //           __||__       |
  // GND ------o    o--+---=|BUTTON_PIN (GPIO)
  //                   |    |________________
  //                   |   ____
  //                   +--[____]---Vcc   (external pull-up resistor)
  //              
  // If there's no external pull-up resistor, set useInternalPullResistor to true
  // Most micro-controlers have internal pull-up resistors
  // 
  // Acive LOW normally open push button without an external pull-up resistor
  // is the default connection
  //
  mdPushButton button = mdPushButton(BUTTON_PIN);
#endif

void DumpButtonTimings(void) {
  Serial.print("  Debounce press time: ");
  Serial.println(button.setDebouncePressTime(0xFFFF));

  Serial.print("  Debounce release time: ");
  Serial.println(button.setDebounceReleaseTime(0xFFFF));

  Serial.print("  Multiple click wait time: ");
  Serial.println(button.setMultiClickTime(0xFFFF));

  Serial.print("  Hold time: ");
  Serial.println(button.setHoldTime(0xFFFF));

  Serial.print("  Status check interval: ");
  Serial.println(button.setCheckInterval(0xFFFF));

  Serial.println("  All times in milliseconds");
}

void ButtonPressed(int clicks) {
  switch (clicks) {
    case -1: Serial.println("Long button press"); DumpButtonTimings(); break;
    case  1: Serial.println("Button pressed once"); break;
  default  : Serial.print("Button pressed "); Serial.print(clicks); Serial.println(" times"); break;
  }
}


void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial) delay(10);

  Serial.println("\nsetup() starting...");
  
  #if defined(ESP8266)
    Serial.println("\n\n"); 
  #endif   
}

void loop() {
  switch (int clicks = button.status()) {
    case -1: Serial.println("Long button press"); break;
    case  0: /* ignore this case */; break;
    case  1: Serial.println("Button pressed once"); break;
  default  : Serial.print("Button pressed "); Serial.print(clicks); Serial.println(" times"); break;
  }
}
