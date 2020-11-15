/*
  Farm monitoring system.

  Program to read nalog & digital inputs and send vector over Lora Protocol
 
  by Cristian Alonso
*/

#include "heltec.h"
#include "Arduino.h"
#include "src/ReadInputs/ReadInputs.h"

#define OLED_UPDATE_INTERVAL 500   
#define BAND    868E6 //LORA band e.g. 868E6,915E6


 typedef struct { 
  uint8_t sensorNumber;
  String sensorName;
  String sensorType;
  String pinNumber;
  String value;
} sensorsDictionary;


int sensorNum;
String payload;
const String user = "Lina";
const String facility = "Nave1";


sensorsDictionary mySensorDictArr[] {
    {0, "Alarm",    "digital", "17", "00"},
    {1, "windowFR", "analog12","36", "00"},
    {2, "windowFL", "analog12","37", "00"},
    {3, "windowRR", "analog12","38", "00"},
    {4, "windowRL", "analog12","39", "00"},
    {5, "tempFR",   "analog12","12", "00"},
    {6, "tempFL",   "analog12","13", "00"},
    {7, "tempRR",   "analog12","2",  "00"},
    {8, "tempRL",   "analog12","0",  "00"},
};

double* outputArray; // Pointer to output array


//Iport library to read inputs
ReadInputs readInputs; 

void setup()
{
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  
  Heltec.display->init();
  Heltec.display->setContrast(255);
  Heltec.display->setFont(ArialMT_Plain_10);  
  
  Serial.begin(115200);
   
}

void loop() {
  outputArray = readInputs.ReadAll();
  mySensorDictArr[sensorNum].value = outputArray[sensorNum];
  
  payload = String(user + "/" + facility + "/" + mySensorDictArr[sensorNum].sensorName + "/" +
  mySensorDictArr[sensorNum].value );

  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(sensorNum  ));
  Heltec.display->drawString(0, 10, payload);
  Heltec.display->display();

  // send packet
  LoRa.beginPacket();
  /*
  * LoRa.setTxPower(txPower,RFOUT_pin);
  * txPower -- 0 ~ 20
  *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
  *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
  */
  LoRa.setTxPower(20,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print(payload);
  LoRa.endPacket();


  digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
  delay(100);


  sensorNum++;
  if(sensorNum >= sizeof(mySensorDictArr)/sizeof(sensorsDictionary)) {sensorNum = 0;};
}
