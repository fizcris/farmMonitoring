/*
  ReadInputs.cpp - Library for reading several Inputs at once in Heltec ESP32.
  Created by Cristian A. Mellis, November 11, 2020.
  Released into the public domain.
*/

#include "Arduino.h"
#include "ReadInputs.h"

ReadInputs::ReadInputs()
{
    Serial.println("Constructor");
}

double *ReadInputs::ReadAll()
{
    double static outputArray[sizeInputArray] = {};
    for (int i = 0; i < sizeInputArray; i++)
    {
        outputArray[i] = ReadVoltage(pinsToRead[i]);
    }
    return outputArray;
}

double ReadInputs::ReadVoltage(byte pin)
{
    double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
    if (reading < 1 || reading >= 4095)
        //return 0;
        // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
        return -0.000000000000016 * pow(reading, 4) + 0.000000000118171 * pow(reading, 3) - 0.000000301211691 * pow(reading, 2) + 0.001109019271794 * reading + 0.034143524634089;
} // Added an improved polynomial, use either, comment out as required
