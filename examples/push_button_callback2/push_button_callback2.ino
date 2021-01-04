/*
 * push_button_callback2.ino
 * 
 * Example program for the mdPushButton library
 * <https://github.com/sigmdel/mdPushButton>
 *
 */

// SPDX-License-Identifier: 0BSD 

#include <Arduino.h>           // for PlatformIO
#include "mdPushButton.h"

// Set the following macros if needed. In PlatformIO, the
// macros can be defined in the platformio.ini configuration file 

#ifndef SERIAL_BAUD
  #define SERIAL_BAUD 9600
#endif

// Four push buttons arranged as cursor keys
#ifndef LEFT_BUTTON_PIN
  #define LEFT_BUTTON_PIN 7
#endif
#ifndef RIGHT_BUTTON_PIN
  #define RIGHT_BUTTON_PIN 8
#endif
#ifndef UP_BUTTON_PIN
  #define UP_BUTTON_PIN 2
#endif
#ifndef DOWN_BUTTON_PIN
  #define DOWN_BUTTON_PIN 1
#endif

// Assuming the buttons are all active HIGH or all active LOW
#ifndef ACTIVE
  #define ACTIVE LOW
#endif

#ifndef DEBUG_PUSH_BUTTON
  #define DEBUG_PUSH_BUTTON 0 
  // 0 - no debug, 1 - printSetup() available, 2 - adds state machine debug output 
  // Arduino IDE: it will be necessary to add this define in mdPushButton.h if not set to 0
#endif 

// ---------------------------------------------------


#if (ACTIVE == HIGH) 
  // Connection of an active HIGH normally open button would be
  //                         _________________
  //           __||__       |
  // Vcc ------o    o--+---=|BUTTON_PIN (GPIO)
  //                   |    |________________
  //         ____      |   
  // GND ---[____]-----+  External pull-down resistor (optional in some cases)
  //              
  // If there's no external pull-down resistor, set useInternalPullResistor to true
  // Many micro-controllers do not have internal pull-down resistors 
  //
	mdPushButton buttonLeft = mdPushButton(LEFT_BUTTON_PIN, HIGH);
	mdPushButton buttonRight = mdPushButton(RIGHT_BUTTON_PIN, HIGH);
	mdPushButton buttonUp = mdPushButton(UP_BUTTON_PIN, HIGH);
	mdPushButton buttonDown = mdPushButton(DOWN_BUTTON_PIN, HIGH);
#else  
  // Connection of an active LOW normally open button would be
  //                         ________________
  //           __||__       |
  // GND ------o    o--+---=|BUTTON_PIN (GPIO)
  //                   |    |________________
  //         ____      |   
  // Vcc ---[____]-----+  External pull-up resistor (optional in most cases)
  //              
  // If there's no external pull-up resistor, set useInternalPullResistor to true
  // Most micro-controlers have internal pull-up resistors
  // 
  // Acive LOW normally open push button without an external pull-up resistor
  // is the default connection
  //
	mdPushButton buttonLeft = mdPushButton(LEFT_BUTTON_PIN);
	mdPushButton buttonRight = mdPushButton(RIGHT_BUTTON_PIN);
	mdPushButton buttonUp = mdPushButton(UP_BUTTON_PIN);
	mdPushButton buttonDown = mdPushButton(DOWN_BUTTON_PIN);
#endif


// Common button pressed event handler
void ButtonPressed(uint8_t pin, int clicks) {
  if (clicks == 1) switch (pin) {
    case LEFT_BUTTON_PIN: Serial.println("move LEFT"); break;
    case RIGHT_BUTTON_PIN: Serial.println("move RIGHT"); break;
    case UP_BUTTON_PIN: Serial.println("move UP"); break;
    case DOWN_BUTTON_PIN: Serial.println("move DOWN"); break;
  }
}


void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial) delay(10);
  #if defined(ESP8266)
    Serial.println("\n"); // skip boot garbage
  #endif   

  Serial.println("\nsetup() starting...");

  // four buttons with common button handler
  buttonLeft.OnButtonClicked(ButtonPressed);
  buttonRight.OnButtonClicked(ButtonPressed);
  buttonUp.OnButtonClicked(ButtonPressed);
  buttonDown.OnButtonClicked(ButtonPressed);

  Serial.println("setup() completed.");
}

void loop() {
  buttonLeft.status();
  buttonRight.status();
  buttonUp.status();
  buttonDown.status();
  delay(40 + random(21)); // rest of loop takes 40 to 60 ms to execute 
}
