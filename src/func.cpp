#include <includes.h>

#ifdef MOMENTARY_SWITCH
    #include <Bounce2.h>   // Required library: https://github.com/thomasfredericks/Bounce2
Bounce btnMist = Bounce(IN_MIST, SWITCH_DEBOUNCE);
Bounce btnAir  = Bounce(IN_AIR, SWITCH_DEBOUNCE);
#endif

void switchStat() {
    // Read fast mode momentary switch
    fast_mode = digitalRead(IN_FAST);
#ifdef USE_WIFI
    if (dataToColdEnd.command.setInFast) fast_mode |= dataToColdEnd.in_fast;
#endif

// Either read momentary switches or solid state switches
#ifdef MOMENTARY_SWITCH
        // Read remote mist control
        // There is no priority between remote control and momentary switches
        // All signals can be overwritten by each other
    #ifdef REMOTE_CTRL
    rem_stat = digitalRead(IN_REM);
    if (rem_stat != rem_stat_old) {
        mist_stat    = rem_stat;   // Switch mist stat
        air_stat     = LOW;        // switch off air stat
        rem_stat_old = rem_stat;
    }
    #endif

    // Read momentary mist switch
    btnMist.update();
    if (btnMist.rose()) {
        mist_stat = !mist_stat;   // Toggle between mist on and off
        air_stat  = LOW;          // switch off air stat
    }

    // Read momentary air switch
    btnAir.update();
    if (btnAir.rose()) {
        air_stat  = !air_stat;   // Toggle between air on and off
        mist_stat = LOW;         // switch off mist stat
    }

    #ifdef USE_WIFI
    if (dataToColdEnd.command.setInMist) {
        if (wifi_mist_last_state != dataToColdEnd.in_mist) {
            // Only change inputs when button changed from off to on
            if (wifi_mist_last_state == false) {
                mist_stat = !mist_stat;   // Toggle between mist on and off
                air_stat  = LOW;          // switch off air stat
            }

            wifi_mist_last_state = dataToColdEnd.in_mist;
        }
    }
    if (dataToColdEnd.command.setInMist) {
        if (wifi_air_last_state != dataToColdEnd.in_air) {
            // Only change inputs when button changed from off to on
            if (wifi_air_last_state == false) {
                air_stat  = !air_stat;   // Toggle between air on and off
                mist_stat = LOW;         // switch off mist stat
            }
            wifi_air_last_state = dataToColdEnd.in_air;
        }
    }

    #endif

#else
    mist_stat = digitalRead(IN_MIST);
    air_stat  = digitalRead(IN_AIR);
    #ifdef USE_WIFI
    if (dataToColdEnd.command.setInMist) mist_stat |= dataToColdEnd.in_mist;
    if (dataToColdEnd.command.setInAir) air_stat |= dataToColdEnd.in_air;
    #endif
#endif
}

void setValves() {
    // Switch control relays for solenoid valves and set flags
    if (fast_mode == HIGH || mist_stat == HIGH) {
        digitalWrite(OUT_COOLANT, HIGH);
        coolant_valve = true;
    } else {
        digitalWrite(OUT_COOLANT, LOW);
        coolant_valve = false;
    }
    if (mist_stat == HIGH || air_stat == HIGH) {
        digitalWrite(OUT_AIR, HIGH);
        air_valve = true;
    } else {
        digitalWrite(OUT_AIR, LOW);
        air_valve = false;
    }
#ifdef USE_WIFI
    dataToClient.airValve     = air_valve;
    dataToClient.coolantValve = coolant_valve;
#endif
}

void potVals() {
    curr_pot_read = millis();
    if (curr_pot_read - prev_pot_read >= POT_LOOP) {
        uint16_t mist_poti_value;
#ifdef USE_WIFI
        if (dataToColdEnd.command.setPotMist) {
            mist_poti_value = dataToColdEnd.pot_mist;
        } else {
            mist_poti_value = analogRead(POT_MIST);
        }
#else
        mist_poti_value = analogRead(POT_MIST);
#endif
        mist_pot_old       = mist_pot_val;
        float mist_pot_raw = exp(log(MIN_RPM) + mist_poti_value * exp_scale);               // Map mist pot range to exponential
                                                                                            // RPM range (MIN_RPM to MAX_RPM)
        mist_pot_val       = POT_FILTER * mist_pot_raw + (1 - POT_FILTER) * mist_pot_old;   // Denoise value with exponential filter
        mist_val           = round(mist_pot_val * 10) / 10;                                 // Round displayed value to one decimal
                                                                                            // place to avoid unwanted refreshes
        if (mist_val >= 10) {
            mist_val = round(mist_val);   // Round greater values here to avoid
                                          // position conflicts between value and unit
        }
        uint16_t spit_poti_value;
#ifdef USE_WIFI
        if (dataToColdEnd.command.setPotSpit) {
            spit_poti_value = dataToColdEnd.pot_spit;
        } else {
            spit_poti_value = analogRead(POT_SPIT);
        }
#else
        spit_poti_value = analogRead(POT_SPIT);
#endif
        spit_pot_old       = spit_pot_val;
        float spit_pot_raw = (spit_poti_value * max_spit) / 4095;                           // Map spit pot range to spit time (0 to MAX_SPIT_TIME)
        spit_pot_val       = POT_FILTER * spit_pot_raw + (1 - POT_FILTER) * spit_pot_old;   // Denoise value with exponential filter
        spit_val           = round(spit_pot_val);                                           // Round displayed value to int to
                                                                                            // avoid unwanted refreshes

        prev_pot_read      = curr_pot_read;

#ifdef USE_WIFI
        dataToClient.mist_val = mist_val;
        if (!spit_mode) dataToClient.spit_val = spit_val;
#endif
    }
}

void switchLEDs() {
    if (spit_pot_val >= MIN_SPIT_TIME) {
        digitalWrite(OUT_SPIT_LED, HIGH);
    } else {
        digitalWrite(OUT_SPIT_LED, LOW);
    }
#ifdef EXT_LED
    digitalWrite(OUT_5V_1, mist_stat);
    digitalWrite(OUT_5V_2, air_stat);
    digitalWrite(OUT_5V_3, spit_mode ? HIGH : LOW);
#endif
}

void pumpControl() {
    if (coolant_valve == true) {
#ifdef DRAIN_SYSTEM
        if (fast_mode == HIGH && mist_pot_val <= MIN_RPM + 0.01) {
            digitalWrite(OUT_DIR,
                         !FLOW_DIR);   // Reverse flow direction in fast mode when mist
                                       // is turned all the way down to drain the system
        } else {
            digitalWrite(OUT_DIR, FLOW_DIR);
        }
#endif

        if (spit_pot_val >= MIN_SPIT_TIME && mist_stat == HIGH && spit_stat == false) {
            if (spit_mode == false) {
                spit_start = millis();   // Set timestamp for spit start
            }
            if (millis() < spit_start + (spit_pot_val * 1000)) {
                spit_mode = true;   // Set spit mode true as long as time is
                                    // below timestamp + spit time
            } else {
                spit_mode = false;   // Unset spit mode
                spit_stat = true;    // Set flag spit mode = executed
            }
        }

        if (spit_mode == true) {
            rpm = SPIT_RPM;   // Select RPM source
        } else if (fast_mode == HIGH) {
            rpm = FAST_RPM;
        } else {
            rpm = mist_pot_val;
        }
        digitalWrite(OUT_ENABLE, LOW);                       // Enable stepper driver
        int edge = round(1000000 / (rpm / 60 * 6400 * 2));   // Pulse time calculation for stepper to get
                                                             // the time in µs for a rising or falling edge
        timerAlarmWrite(stepTimer, edge, true);              // Set timer alarm to pulse length/2, autoreload = true
        timerAlarmEnable(stepTimer);
    } else {
        timerAlarmDisable(stepTimer);   // Stop timers and reset all states
        digitalWrite(OUT_ENABLE, HIGH);
        spit_stat = false;
        spit_mode = false;
    }
#ifdef USE_WIFI
    dataToClient.spitMode = spit_mode;
#endif
}

void IRAM_ATTR stepPulse() {
    // Step pulse generated by hardware timer
    portENTER_CRITICAL_ISR(&timerMux0);
    pulse = !pulse;
    digitalWrite(OUT_STEP, pulse);
    portEXIT_CRITICAL_ISR(&timerMux0);
}

//