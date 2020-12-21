/*
 * mdButton.cpp
 * 
 * A push button Arduino library capable of returning the number of
 * consecutive button presses made in quick succession or if the 
 * button was held down for a long time. 
 * 
 * Version 0.1.0, 2020-12-16.
 *
 * Michel Deslierres <sigmdel.ca/michel>
 * 
 */
 
 // SPDX-License-Identifier: 0BSD

#include "Arduino.h"
#include "mdPushButton.h"


// If active = LOW (the default) then the input must be pulled high most of the time
//   This can be done with the internal pullup (set pullup = true which is also the default value) 
//   or an external pull up resistor say 4.7K + connected to the  GPIO pin and Vcc
//
// If active = HIGH then the input must be pulled low most of the time
//   This can be done with the internal pulldown (set pullup = true which is also the default value) 
//   or an external pull up resistor say 4.7K + connected to the  GPIO pin and Vcc

mdPushButton::mdPushButton(uint8_t pin, uint8_t active, bool useInternalPullResistor) {
  _pin = pin;
  _active = active;

  int mode = INPUT;
  if (useInternalPullResistor) {
    if (active == LOW)
      mode = INPUT_PULLUP;
    else {
      // mode = INPUT_PULLDOWN where defined
      #if defined(INPUT_PULLDOWN) 
        mode = INPUT_PULLDOWN;
      #elif defined(ESP8266)
        if (pin == 16) mode = INPUT_PULLDOWN_16;
      #endif  
    }
  }  

  pinMode(_pin, mode);
//dbg:  Serial.printf("\n\nlast_state: %d\n", last_state);
//dbg:  Serial.printf("_acive %s (%d)", (_active) ? "HIGH" : "LOW", _active);
//  digitalWrite(_pin, !active);
  buttonState_ = AWAIT_PRESS;
  version = VERSION;
}

//Set attributes
void mdPushButton::setDebouncePressTime(uint16_t value){debouncePressTime_ = value;}
void mdPushButton::setDebounceReleaseTime(uint16_t value){debounceReleaseTime_ = value;}
void mdPushButton::setMultiClickTime(uint16_t value){multiClickTime_ = value;}
void mdPushButton::setHoldTime(uint16_t value){holdTime_ = value;}

void mdPushButton::OnButtonClicked( callback_int cb ){_OnClick1 = cb;}
void mdPushButton::OnButtonClicked( callback_int_int cb ){_OnClick2 = cb;}

int mdPushButton::status(void) {
  int stat = _update();
  if (stat) {
    if (_OnClick2) _OnClick2(_pin, stat);
    if (_OnClick1) _OnClick1(stat);
  }  
  return stat;
}  


int mdPushButton::_update(void) {

/* dbg:
  if (digitalRead(_pin) != last_state) {
    last_state = digitalRead(_pin);
    Serial.printf("%d  == _active (%d) %s, !_active %d \n", last_state, _active, (last_state == _active) ? "true" : "false", !_active);
  }*/
  if (buttonState_ == AWAIT_PRESS) {
    clicks_ = 0;
    if (digitalRead(_pin) == _active) {
      buttonState_ = DEBOUNCE_PRESS;
      eventTime_ = millis();  
      //dbg: Serial.println("buttonState_ = DEBOUNCE_PRESS");
    }  
  } 
  
  else if (buttonState_ == DEBOUNCE_PRESS) {
    if ((millis() - eventTime_) > debouncePressTime_) { 
      buttonState_ = AWAIT_RELEASE;
      eventTime_ = millis(); 
      //dbg: Serial.println("buttonState_ = AWAIT_RELEASE");
    }  
  } 
  
  else if (buttonState_ == AWAIT_RELEASE) {
    //dbg: Serial.printf("%d ", digitalRead(_pin));
    if (!(digitalRead(_pin) == _active)) {
      if ((millis() - eventTime_) > holdTime_) {
        buttonState_ = AWAIT_PRESS;
        //dbg: Serial.println("buttonState_ = AWAIT_PRESS --> return -1");
        return -1;
      }
      buttonState_ = DEBOUNCE_RELEASE;
      //dbg: Serial.println("buttonState_ = DEBOUNCE_PRESS");
      eventTime_ = millis();
    }
  } 

  else if (buttonState_ == DEBOUNCE_RELEASE) {
    if ((millis() - eventTime_) > debounceReleaseTime_) { 
      clicks_ += 1;
      buttonState_ = AWAIT_MULTI_PRESS;
      //dbg: Serial.println("buttonState_ = AWAIT_MULTIP_PRESS");
      eventTime_ = millis(); 
    }  
  } 
  
  else {   // (buttonState == AWAIT_MULTI_PRESS) 
    if (digitalRead(_pin) == _active) {
      buttonState_ = DEBOUNCE_PRESS;
      //dbg: Serial.println("buttonState_ = DEBOUNCE_PRESS");
      eventTime_ = millis(); 
    } 
    else if ((millis() - eventTime_) > multiClickTime_) {
      buttonState_ = AWAIT_PRESS;
      //dbg: Serial.printf("buttonState_ = AWAIT_PRESS --> return %d\n", clicks_);
      return clicks_;
    } 
  }
  return 0; 
}

