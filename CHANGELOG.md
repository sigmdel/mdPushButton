
## Unreleased (development)


## Released


### 0.1.3

- Removed setCheckInterval from keywords.txt
- Updated version number in library.properties and library.json
- Set a default 0 value for the DEBUG_PUSH_BUTTON macro
- Removed Version section in README.md and 
- Added this file instead


### 0.1.2  96edee9

- Added button release debounce on long button presses.
- Removed the _checkInterval` and setCheckInterval. The functionality was not implemented.
- Improved the debugging functions in the mdPushButton class.
- Added the push_button_tune.ino sketch.
- Moved VERSION macro to mdPushButton.cpp from header file to avoid conflicts
- Fixed errors in README.md


### 0.1.1  9770957

- Changed time setters in mdPushButton
- Improved examples
- Updated README.md


### 0.1.0  46ba24a

- Initial commit to GitHub with the improvements listed below
- Added support for internal pull-down resistors with active HIGH buttons if available 
- Added support for active HIGH buttons 
- Added callback functions.
