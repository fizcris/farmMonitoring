/*
 * DisplayUI.cpp
 * 
 * Helper functions for the display UI.
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.
 */

#include <HAL.h>
#include <WString.h>
#include <Arduino.h>

void displaySpreadFactor(int sf) {
    displayString(0, 1, "SF:            ");
    displayString(3, 1, String(sf).c_str());    
}

void displayRssi(int rssi) {
  displayString(0, 4, "RSSI:         ");
  displayString(6, 4, String(rssi, DEC).c_str());
}
