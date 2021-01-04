/*
 * push_button_tune.ino
 * Version 0.2, 2020-01-24.
 * 
 * An Arduino sketch to "tune" the various delays used in the 
 * mdPushButton library <https://github.com/sigmdel/mdPushButton>
 * 
 * This sketch has a serial command line interpreter (to use in Arduino Serial Monitor)
 * with the following commands:
 * 
 *   help                           - displays the list of commands
 *   times                          - will display button time attributes
 *   debouncePressTime <value>      - will set button.debouncePressTime_ to <value>
 *   debounceReleaseTime <value>    - will set button.debounceReleaseTime_ to <value>
 *   debounceMulticlickTime <value> - will set button.debounceMultiClickTime_ to <value>
 *   debounceHoldTime <value>       - will set button.debounceHoldTime_ to <value>
 *   defaults                       - will reset all times to default values
 *   
 *      - <value> must be between 0 and 10000 (milliseconds)
 *      - the commands are not case sensitive
 *      
 * Tested with 
 *    Arduino UNO, ESP8266, ESP32, STM32F103 (bluePill), SAMD21 (Seeeduino XIAO)     
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

#ifndef BUTTON_PIN
  #define BUTTON_PIN 7
#endif

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
  mdPushButton button = mdPushButton(BUTTON_PIN, HIGH, true);
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
  mdPushButton button = mdPushButton(BUTTON_PIN);
#endif

typedef struct {
  const char * cmd;
  const char * help;
} cmd_t;

const cmd_t cmds [] {
  {"help", "                    - print this output"},
  {"times", "                   - list all button timing values"},
  {"debouncepresstime", " <n>   - set debounce time after button is pressed to <n> ms"},
  {"debouncereleasetime", " <n> - set debounce time after button is released to <n> ms"},
  {"multiclicktime", " <n>      - set time waiting for a button press in quick succession after the last press to <n> ms"},
  {"holdtime", " <n>            - set time to hold down the button for a long press to <n> ms"},
  {"defaults", "                - set default times"}
  #if (DEBUG_PUSH_BUTTON > 0)  
  ,
  {"setup", "                   - print button setup"}
  #endif
};  

const int cmdCount = sizeof(cmds)/sizeof(cmd_t);

void DumpButtonTimings(char * msg = NULL) {
  if (msg) Serial.println(msg);
  Serial.print("  Debounce press time: ");
  Serial.println(button.setDebouncePressTime(0xFFFF));

  Serial.print("  Debounce release time: ");
  Serial.println(button.setDebounceReleaseTime(0xFFFF));

  Serial.print("  Multiple click wait time: ");
  Serial.println(button.setMultiClickTime(0xFFFF));

  Serial.print("  Hold time: ");
  Serial.println(button.setHoldTime(0xFFFF));

  Serial.println("  All times in milliseconds");
}


void buttonModule(void) {
  int clicks = button.status();
  switch (clicks) {
    case -1: Serial.println("Long button press"); break;
    case  0: /* ignore this case */; break;
    case  1: Serial.println("Button pressed once"); break;
    default: Serial.print("Button pressed "); Serial.print(clicks); Serial.println(" times consecutively"); break;
  }
}

#define TOKENCOUNT 3
String token[TOKENCOUNT];

int parseString(const char *s) {

  for (int i = 0; i < TOKENCOUNT; i++) {
    token[i] = "";
  }
  int ndx = 0;
  int tokCounter = 0;
  int len = 0;

  // skip leading spaces
  while (s[ndx] == ' ') ndx++;

  while (1) {
    char inChar = s[ndx];
    ndx++;
    if ( (len > 0) && ((inChar == ' ') || (inChar == 0)) ) {
      tokCounter += 1;
      len = 0;
    }
    if ( (inChar == 0) || (tokCounter >= TOKENCOUNT) ) return tokCounter;
    if (inChar != ' ') {
      token[tokCounter] += inChar;
      len++;
    } else {
      // skip extra spaces following inChar == ' '
      while (s[ndx] == ' ') ndx++;
    }
  }
}

void printHelp() {
  Serial.println();
  Serial.println("commands:");
  for (int i = 0; i < cmdCount ; i++) {
      Serial.print("  ");
      Serial.print(cmds[i].cmd);
      Serial.println(cmds[i].help);
  }
}  

static String inputString = "";
boolean stringComplete = false;

void inputModule() {
  
  while (Serial.available() && !stringComplete) {
    char inChar = (char) Serial.read();
    if (inChar == '\b')  {  
      if (inputString.length() > 0) { 
        inputString.remove(inputString.length()-1); //backspace, remove last char in InputString
        Serial.write(" \b");  // and overwrite it with space on serial monitor
      }
    } else if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
  if (stringComplete) {
    int counter = parseString(inputString.c_str());
    inputString = "";
    stringComplete = false;
    long nvalue = 0;
    int cmd = -1;
    if (counter < 1) { return; } 
    token[0].toLowerCase();
    for (int i = 0; i < cmdCount; i++) {
      if (token[0].equals(cmds[i].cmd)) {
        cmd = i;
        break;
      }
    } 

    if (cmd < 0) {
      Serial.print("\"");
      Serial.print(token[0]);
      Serial.println("\" is unknown");
      return;
    }  

    if ( (cmd > 1 && cmd < 7 && counter != 2) || (cmd < 2 && counter != 1) ) {
      Serial.println("wrong number of parameters");
     return;
    }

    if (counter == 2) {
      nvalue = token[1].toInt();
      if ((nvalue < 0) || (nvalue > 10000)) {
        Serial.print(token[1]);
        Serial.println(" is out of range");
        return;
      }
    }
    
    int oldtime = 0;

    switch(cmd) {
      case 0: printHelp(); Serial.println("\nAll times values must be from 0 to 10,000 milliseconds (10 seconds)\n"); break;
      case 1: DumpButtonTimings(); break; 
      case 2: oldtime = button.setDebouncePressTime(nvalue); break;
      case 3: oldtime = button.setDebounceReleaseTime(nvalue); break;
      case 4: oldtime = button.setMultiClickTime(nvalue); break;
      case 5: oldtime = button.setHoldTime(nvalue); break;
      case 6: 
        button.setDebouncePressTime(DEFAULT_DEBOUNCE_PRESS_TIME);
        button.setDebounceReleaseTime(DEFAULT_DEBOUNCE_RELEASE_TIME);
        button.setMultiClickTime(DEFAULT_MULTI_CLICK_TIME);
        button.setHoldTime(DEFAULT_HOLD_TIME);
        DumpButtonTimings();
        break;
      #if (DEBUG_PUSH_BUTTON > 0)  
      case 7: button.printSetup(); break;        
      #endif
    }

    if (cmd > 1 && cmd < 6) {
      Serial.print("Previous value, ");
      Serial.print(oldtime);
      Serial.print(", replaced by ");
      Serial.print(nvalue);
      Serial.println(" ms");         
    }    
  }  
}


void setup() {
  Serial.begin(SERIAL_BAUD);
  while (!Serial) delay(10);
  #if defined(ESP8266)
    Serial.println("\n"); // skip boot garbage
  #endif   

  Serial.println("setup() starting...");

  #if (DEBUG_PUSH_BUTTON > 0)
  button.printSetup();
  #endif
 
  DumpButtonTimings("Initial push button times:");

  printHelp();
  inputString = "";
  Serial.println("setup() completed");
}

void loop() {
  buttonModule();
  inputModule(); 
}
