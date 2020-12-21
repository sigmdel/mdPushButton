/*
 * mdPushButton.h
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

#ifndef MDPUSHBUTTON_H
#define MDPUSHBUTTON_H

#include "Arduino.h"

#define VERSION 0x000001

#define DEFAULT_DEBOUNCE_PRESS_TIME      15  // delay to debounce the make part of the signal
#define DEFAULT_DEBOUNCE_RELEASE_TIME    30  // delay to debounce the break part of the signal
#define DEFAULT_MULTI_CLICK_TIME        400  // if 0, does not check for multiple button clicks
#define DEFAULT_HOLD_TIME              2000  // minimum time of button press for mdButton.status() to return a -1 (long button press)
#define DEFAULT_CHECK_INTERVAL           50  // time between successive polling of the button pin 
                                             // all times in milliseconds

typedef void (*callback_int)(int);
typedef void (*callback_int_int)(int, int);

class mdPushButton {
  public:
    mdPushButton(uint8_t pin, uint8_t active = LOW, bool useInternalPullResistor = true);  //constructor
    int32_t version;

    // Set attributes
    void setDebouncePressTime(uint16_t value);
    void setDebounceReleaseTime(uint16_t value);
    void setMultiClickTime(uint16_t value);
    void setHoldTime(uint16_t value);
    void setCheckInterval(uint16_t value);

 
    // status, number of clicks since last update
    // -1 = button held, 0 = button not pressed, 1, 2, ... number of times button pressed
    int status();

    // Set callback function to be called when the button has been pressed
    //
    // mdPushButton upButton = mdButton(5);
    // mdPushButton rightButton = mdButton(7);
    // mdPushButton leftButton = mdButton(11);
    // mdPushButton downButton = mdButton(13);
    //
    // void buttonClicked(int pin, int status) {
    //   if (status < 0) restart();
    //    if (status == 1) {
    //      switch (pin) {
    //         case 5: moveUp() break;
    //         case 7: moveRight() break;
    //         case 11: moveLeft() break;
    //         case 13: move Down() break;
    //      }
    //    }
    // }
    //
    // void setup(void) {
    //    upButton.OnButtonClicked(buttonClicked);
    //    rightButton.OnButtonClicked(buttonClicked);
    //    leftButton.OnButtonClicked(buttonClicked);
    //    downButton.OnButtonClicked(buttonClicked);
    //    ...
    // pbut.OnButtonClicke(buttonPressed);
    void OnButtonClicked(callback_int_int);

    // if there is only one push button, or if using separate click handlers
    void OnButtonClicked(callback_int);

  private:
    uint8_t _pin;    
    uint8_t _active;                   
    uint16_t debouncePressTime_   = DEFAULT_DEBOUNCE_PRESS_TIME; 
    uint16_t debounceReleaseTime_ = DEFAULT_DEBOUNCE_RELEASE_TIME;
    uint16_t multiClickTime_      = DEFAULT_MULTI_CLICK_TIME;  
    uint16_t holdTime_            = DEFAULT_HOLD_TIME;
    uint16_t checkInterval_       = DEFAULT_CHECK_INTERVAL;
  
    callback_int _OnClick1;
    callback_int_int _OnClick2;

    // State variables 
    unsigned long lastButtonCheck_;
    long eventTime_; 
    enum buttonState_t { AWAIT_PRESS, DEBOUNCE_PRESS, AWAIT_RELEASE, DEBOUNCE_RELEASE, AWAIT_MULTI_PRESS } buttonState_;
    int clicks_;
    int _update(void);
};

#endif
