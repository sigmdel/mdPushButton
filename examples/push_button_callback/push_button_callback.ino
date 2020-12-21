/*
 * push_button_callback.ino
 * 
 * Example program for the mdPushButton library
 * 
 */

// SPDX-License-Identifier: 0BSD 
 
#include "mdPushButton.h"

#define BAUD 115200
#define BUTTON_PIN D7

void ButtonPressed(int clicks) {
  switch (clicks) {
    case -1: Serial.println("Long button press"); break;
    case  1: Serial.println("Button pressed once"); break;
  default  : Serial.print("Button pressed "); Serial.print(clicks); Serial.println(" times"); break;
  }
}

mdPushButton button = mdPushButton(BUTTON_PIN);


void setup() {
  Serial.begin(BAUD);
  while (!Serial) delay(10);
  #if defined(ESP8266)
    Serial.println("\n\n"); 
  #endif   
  button.OnButtonClicked(ButtonPressed);
}

void loop() {
  button.status();
}
