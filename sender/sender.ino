/*
  Farm monitoring system.

  Program to read analog & digital inputs and send vector over Lora Protocol
  as well as diplay packets on OLED screen.
 
  by Cristian Alonso
*/

#include "heltec.h"
#include "Arduino.h"

#include "USER_SENDER.h"

#define OLED_UPDATE_INTERVAL 500
#define BAND 868E6 //LORA band e.g. 868E6,915E6



int sensorNum;
int row;
String payload;
double *outputArray; // Pointer to output array

//Function to increase analogread() accuracy of ESP32
double readVoltage(byte pin)
{
  double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  if (reading < 1 || reading >= 4095)
  {
    //return 0;
    // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
    return -0.000000000000016 * pow(reading, 4) + 0.000000000118171 * pow(reading, 3) - 0.000000301211691 * pow(reading, 2) + 0.001109019271794 * reading + 0.034143524634089;
  } // Added an improved polynomial, use either, comment out as required
}

//Function to read inputs
void readInput(sensorDict *sensorX)
{
  if (sensorX->sensorType.equals(String("digital")))
  {
    sensorX->value = digitalRead(sensorX->pinNumber.toInt());
  }
  else if (sensorX->sensorType.equals(String("analog12")))
  {
    sensorX->value = readVoltage(sensorX->pinNumber.toInt());
  }
}

void setup()
{
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  Heltec.display->init();
  Heltec.display->setContrast(255);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->flipScreenVertically();
  Heltec.display->clear();

  Heltec.display->drawString(40, 0, String(user + "/" + facility ));
  Heltec.display->display();

  Serial.begin(115200);
}

void loop()
{
  readInput(&mySensorDictArr[sensorNum]); // Updates de value of the sensor
  payload = String(user + "/" + facility + "/" + mySensorDictArr[sensorNum].sensorName + "/" +
                   mySensorDictArr[sensorNum].value);

  Heltec.display->drawString(0, 10 + 8*row, String(mySensorDictArr[sensorNum].sensorName) + String(": ") + String(mySensorDictArr[sensorNum].value));
  //Heltec.display->drawString(0, 50, "Sending packet pin: " + String(mySensorDictArr[sensorNum].pinNumber) );
  Heltec.display->display();

  // send packet
  LoRa.beginPacket();
  /*
  * LoRa.setTxPower(txPower,RFOUT_pin);
  * txPower -- 0 ~ 20
  *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
  *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
  */
  LoRa.setTxPower(20, RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(payload);
  LoRa.endPacket();

  delay(10);
  
  sensorNum++;
  row++;
  if (row>5){
    row=0;
    Heltec.display->clear();
    Heltec.display->drawString(40, 0, String(user + "/" + facility ));
    }
  if (sensorNum >= sizeof(mySensorDictArr) / sizeof(sensorDict))
  {
    sensorNum = 0;
  };
}
