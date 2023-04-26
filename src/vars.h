#pragma once

/*

  ColdEND32 v1.2 Minimum Quantity Lubrication
  https://www.end-cnc-shop.de

  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License

  Written by Tilman, 2021-12-11
  Last edited by Tilman, 2021-12-29

*/

// Create hardware timer
extern hw_timer_t *stepTimer;
extern portMUX_TYPE timerMux0;

// Potentiometers
extern float mist_pot_val;            // Mist value in milliliter per minute
extern float spit_pot_val;            // Spit value in seconds
extern float mist_pot_old;            // Previous mist pot value for exponential smoothing
extern float spit_pot_old;            // Previous spit pot value for exponential smoothing
extern unsigned long prev_pot_read;   // Previous pot read loop
extern unsigned long curr_pot_read;   // Current pot read loop
extern float max_spit;                // Convert MAX_SPIT_TIME to float
extern float exp_scale;               // Exponential flow scaling to match a 12bit analogue value

// Relais states
extern bool coolant_valve;   // Coolant valve state (on/off)
extern bool air_valve;       // Air valve state (on/off)

// Switch states
extern bool spit_mode;      // Spit mode state
extern bool spit_stat;      // Flag indicating whether spit mode has been executed or not
extern byte fast_mode;      // Fast mode state
extern byte mist_stat;      // Mist switch state
extern byte air_stat;       // Air switch state
extern byte rem_stat;       // Remote mist control state
extern byte rem_stat_old;   // Last remote state

// WiFi button states
extern bool wifi_mist_last_state;   // WiFi button state
extern bool wifi_air_last_state;    // WiFi button state

// Stepper
extern volatile byte pulse;        // Step pulse state
extern float rpm;                  // Stepper RPM
extern unsigned long spit_start;   // Timestamp for spit start

// Display
extern unsigned long prev_refresh;   // Previous display refresh time
extern unsigned long curr_refresh;   // Current display refresh time
extern float mist_val;               // Displayed mist value
extern int spit_val;                 // Displayed spit value
extern float spit_sub;               // Subtracted from spit time during spit mode countdown
extern int start_pos;                // Variable start position to fix some 16x4 LCD
extern int cursor_pos;               // Variable cursor position
extern int digits;                   // Mist value decimal places
extern float mist_old;               // Values from previous refresh
extern int spit_old;
extern byte coolant_old;
extern byte air_old;

//