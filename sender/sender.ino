/*
  Farm monitoring system.

  Program to read analog & digital inputs and send vector over Lora Protocol
  as well as diplay packets on OLED screen.
 
  by Cristian Alonso
*/
//################      Include libraries         ##################
#include "heltec.h"
#include "Arduino.h"
#include "USER_SENDER.h"

//################      Definitions & variables        ##################
#define OLED_UPDATE_INTERVAL 10
#define BAND 868E6 //LORA band e.g. 868E6,915E6
String payload;
int page = 0;
int sensorsPerPage = 5;
int numberOfSensors = sizeof(mySensorDictArr)/sizeof(sensorDict);

//################      Define functions         ##################
//Function to setup inputs
void setupInput(sensorDict *sensorX)
{
  if (sensorX->sensorType.equals(String("digital")))
  {
    pinMode(sensorX->pinNumber.toInt(),INPUT_PULLUP);
  }
  else if (sensorX->sensorType.equals(String("analog12")))
  {
    adcAttachPin(sensorX->pinNumber.toInt());
  }
}

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

// 
void IRAM_ATTR addPage() {
    page++;
    if (page > 1 ){
      page = 0;
      }
}


//Function to write dict to screen
void updateScreen(sensorDict *sensorX)
{
  Heltec.display->clear();
  Heltec.display->drawString(40, 0, String(user + "/" + facility ));
  int firstSensor = page*sensorsPerPage;
  int lastSensor = firstSensor + sensorsPerPage;
  if (lastSensor > numberOfSensors ){
    lastSensor = numberOfSensors;
    }
  
  for (int i = firstSensor; i < lastSensor; i++) {
    Heltec.display->drawString(0, 10 + 8* (i-firstSensor), String(mySensorDictArr[i].sensorName) + String(": ") + String(mySensorDictArr[i].value));
    //Heltec.display->drawString(0, 50, "Sending packet pin: " + String(mySensorDictArr[i].pinNumber) );
    Heltec.display->display();
  }
}



//################      Setup         ##################
void setup()
{ 
  // Settup pins as inputs
  for (int thisPin = 0; thisPin < numberOfSensors; thisPin++) {    
    setupInput(&mySensorDictArr[thisPin]); // Updates the vaules in screen
  }

  //Setup button ISR
  attachInterrupt(0, addPage, FALLING);

  // Setup board  
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  // Setup screen
  Heltec.display->init();
  Heltec.display->setContrast(255);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->flipScreenVertically();
  Heltec.display->clear();

  Heltec.display->drawString(40, 0, String(user + "/" + facility ));
  Heltec.display->display();

  // Setup serial
  Serial.begin(115200);

}


//################      Loop         ##################
void loop()
{
  for (int sensorNum = 0; sensorNum < numberOfSensors; sensorNum++){
    Serial.println(page);
    
    //Read inputs
    readInput(&mySensorDictArr[sensorNum]); // Updates de value of the sensor
    payload = String(user + "/" + facility + "/" + mySensorDictArr[sensorNum].sensorName + "/" +
                     mySensorDictArr[sensorNum].value);
  
     // send LORA packet
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
  
    //Update sensor value on sccreen
    updateScreen(&mySensorDictArr[sensorNum]); // Updates the vaules in screen
    //delay(1);
                   
    }
}
