/*
 * USER_SENDER.h
 * 
 * User data
 * 
 * (c) 2020 Cristian Alonso. All Rights Reserved.
 */

const String user = "Lina";
const String facility = "Nave1";

typedef struct
{
  uint8_t sensorNumber;
  String sensorName;
  String sensorType;
  String pinNumber;
  String value;
} sensorDict;


sensorDict mySensorDictArr[]{
    {0, "Alarm", "digital", "17", "00"},
    {1, "Alarm2", "digital", "23", "00"},
    {2, "windowFR", "analog12", "36", "00"},
    {3, "windowFL", "analog12", "37", "00"},
    {4, "windowRR", "analog12", "38", "00"},
    {5, "windowRL", "analog12", "39", "00"},
    {6, "tempFR", "analog12", "12", "00"},
    {7, "tempFL", "analog12", "13", "00"},
    {8, "tempRR", "analog12", "2", "00"},
    {9, "tempRL", "analog12", "25", "00"},
};
