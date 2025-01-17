# ColdEND32 - WiFi

This is a fork of the ColdEND32 firmware https://github.com/hausen8/ColdEND32.

This firmware works 100% the same as the original firmware, but adds the ability to control the input buttons and potentiometers using WiFi. If there is no wifi connection, the original buttons and potis can be used.
The buttons can be used in parallel, meaning you can switch them on the wifi panel or directly connected to the coldend32 at the same time.
The potis can only be used via wifi OR directly connected to the coldend32, not both at the same time. The software decides on its own which one to use. Meaning, if there is a wifi connection and the potis are connected to the esp32 panel, the potis directly connected will be ignored. It is also possible to only control e.g. the mist poti via wifi and the spit poti directly connected to the coldend32.

### How to use

To control the coldend32 via wifi check out the ESP32 Panel project: [ESP32 Panel Software](https://github.com/timo1235/-ocs2.x-esp32-panel-software-)

### Changes to the original firmware:

- Changed project structure for compatibility to platformio
- Added external libraries as platformio dependencies, they will be loaded automatically when building the project
- Added WiFi control of input buttons and potentiometers using esp now protocol

# Original ColdEND32 README

Firmware for ColdEND32 controller supporting various displays

Installation and configuration:

1. Download and import the following libraries depending on your display:

   **SSD1306 or SH1106**
   https://github.com/olikraus/U8g2_Arduino

   **HT16K33**
   https://github.com/adafruit/Adafruit_LED_Backpack

   **LCD 16x2 or 16x4**
   https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library

2. Uncomment (define) the appropriate display in config.h. Note that only one type of
   display at one time is allowed so don't forget to comment the other ones.
   For 7 segment LED displays, don't forget to jumper different I2C addresses for both
   displays and enter these values in config.h ("LED1_ADD" and "LED2_ADD").

3. Uncomment (define) MOMENTARY_SWITCH if you want to use monentary switches.
   Required library: https://github.com/thomasfredericks/Bounce2
   For solid state switches leave MOMENTARY_SWITCH commented.

4. Uncomment (define) DRAIN_SYSTEM if you want to have the option to drain the coolant
   pump and pipes anytime. To drain your system, turn the coolant pot all the way down
   and push the fast mode switch.

5. Uncomment (define) REMOTE_CTRL if you want to control your ColdEND32 system from the
   CNC controller with a 24V solid signal (must be connected to input #4). Please note
   that momentary switches are required!

6. Uncomment (define EXT_LED for external LEDs indicating active mist, air or spit mode.
   LEDs will be connected to first 3 pin pairs of JP5 (inner pin: +5V, outer Pin: GND).
   Please use appropriate resistors!

7. Flash your ColdEND controller and do a first check:

   | Problem                                          | Solution                                |
   | ------------------------------------------------ | --------------------------------------- |
   | Pump speed range does not match requirements     | Edit MIN_RPM and/or MAX_RPM             |
   | Flow direction of pump is wrong                  | Change FLOW_DIR                         |
   | Fast mode speed is too slow/fast                 | Edit FAST_RPM                           |
   | Spit mode speed is too slow/fast                 | Edit SPIT_RPM                           |
   | Spit time range dows not match requirements      | Edit MIN_SPIT_TIME and/or MAX_SPIT_TIME |
   | Spit mode does not start at times                | Slightly increase SWITCH_DEBOUNCE       |
   | Momentary switches do not work properly          | Slightly increase SWITCH_DEBOUNCE       |
   | 16x4 LCD rows 3 and 4 starting at wrong position | Uncomment FIX16X4                       |
   | Display seems to be too slow                     | Decrease REFRESH_TIME                   |
   | Mist or Spit pot values are jumping              | Try lower POT_FILTER values             |
   | Mist or Spit pot values are still jumping        | Increase POT_LOOP to 2 or 3             |

8. If any of these values have been altered, you need to flash the controller once again.

**HISTORY:**

- v1.2: Fix some 16x4 LCD row 3 and 4 starting at wrong positions

- v1.1: Added support for external LEDs indicating the active mode (mist, air or spit)

- v1.0: Initial firmware for ESP32 controlled pump board
