#pragma once
/*

  ColdEND32 v1.2 Minimum Quantity Lubrication
  https://www.end-cnc-shop.de

  Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License

  Written by Tilman, 2021-12-11
  Last edited by Tilman, 2021-12-29

*/

extern DATA_TO_COLDEND dataToControl;

void switchStat();

void setValves();

void potVals();

void switchLEDs();

void pumpControl();

void IRAM_ATTR stepPulse();
