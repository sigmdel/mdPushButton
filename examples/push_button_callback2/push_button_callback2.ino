/*
 * push_button_callback2.ino
 * 
 * Example program for the mdPushButton library
 * 
 */

// SPDX-License-Identifier: 0BSD 
 

#include "mdPushButton.h"

#define BAUD 115200

// Four push buttons arranged as cursor keys
#define LEFT_BUTTON_PIN D7
#define RIGHT_BUTTON_PIN D8
#define UP_BUTTON_PIN D2
#define DOWN_BUTTON_PIN D1

// Common button pressed event handler
void ButtonPressed(int pin, int clicks) {
  if (clicks == 1) switch (pin) {
    case LEFT_BUTTON_PIN: Serial.println("move LEFT"); break;
    case RIGHT_BUTTON_PIN: Serial.println("move RIGHT"); break;
    case UP_BUTTON_PIN: Serial.println("move UP"); break;
    case DOWN_BUTTON_PIN: Serial.println("move DOWN"); break;
  }
}

mdPushButton buttonLeft = mdPushButton(LEFT_BUTTON_PIN);
mdPushButton buttonRight = mdPushButton(RIGHT_BUTTON_PIN);
mdPushButton buttonUp = mdPushButton(UP_BUTTON_PIN);
mdPushButton buttonDown = mdPushButton(DOWN_BUTTON_PIN);

void setup() {
  Serial.begin(BAUD);
  while (!Serial) delay(10);
  #if defined(ESP8266)
    Serial.println("\n\n"); 
  #endif   
  // four buttons with common button handler
  buttonLeft.OnButtonClicked(ButtonPressed);
  buttonRight.OnButtonClicked(ButtonPressed);
  buttonUp.OnButtonClicked(ButtonPressed);
  buttonDown.OnButtonClicked(ButtonPressed);
}

void loop() {
  buttonLeft.status();
  buttonRight.status();
  buttonUp.status();
  buttonDown.status();
}
