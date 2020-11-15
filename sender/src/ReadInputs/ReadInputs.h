/*
  ReadInputs.h - Library for reading several Inputs at once in Heltec ESP32.
  Created by Cristian A. Mellis, November 11, 2020.
  Released into the public domain.
*/

#ifndef ReadInputs_h
#define ReadInputs_h

#include "Arduino.h"

// Define Digital read pins

#define alarmPin 17
// #define availablePin 23

// Define Analog read pins

#define windowFRPin 36
#define windowFLPin 37
#define windowRRPin 38
#define windowRLPin 39
#define tempFRPin 12
#define tempFLPin 13
#define tempRRPin 2
#define tempRLPin 0

// Global variable used to iterate over PINs

int const pinsToRead[] = {alarmPin, windowFRPin, windowFLPin, windowRRPin, windowRLPin, tempFRPin, tempFLPin, tempRRPin, tempRLPin};
int const sizeInputArray = (sizeof(pinsToRead) / sizeof(pinsToRead[0]));

class ReadInputs
{
public:
  ReadInputs();
  double *ReadAll(); // Returns a pointer to an array of size sizeInputArray
  
private:
  double ReadVoltage(byte pin);
};

#endif