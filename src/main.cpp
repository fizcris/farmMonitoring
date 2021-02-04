/*
 * LoRaGateway.cpp
 * 
 * Implements a minimal, single-channel LoRa to MQTT gateway
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.
 */

#include <config.h>

#include <Arduino.h>
#include <HAL.h>
#include <PubSubClient.h>
#include <heltec.h>



#include <DisplayUI.h>
#include <LoRaInterface.h>
#include <MQTT.h>
#include <WiFiSupport.h>


// checkAndForwardPackets()
// This is the core function that checks for received LoRa packets and forwards the contents on to MQTT

char data[MAX_LORA_PAYLOAD + 1] = "";
char topic[12] = "allTopics";
char senVal[12] = "";
const char delim[2] = "/";

static void splitMsgData(){
    //int init_size = strlen(data);
    char *ptr = strtok(data, delim);
    int callNum = 0;

    while (ptr != NULL)
    {
        if(callNum <1){
            strcpy(topic,ptr);
        } else if (callNum <2) {
            strcat(topic,"/");
            strcat(topic,ptr);
        } else if (callNum <3) {
            strcpy(senVal,ptr);
        } else if (callNum <4) {
            strcat(senVal,"/");
            strcat(senVal,ptr);
        }
      ptr = strtok(NULL, delim);
      callNum++;
    }
}


static void checkAndForwardPackets()
{
  displayString(0, 5, "Waiting for LoRA packets");
  // check for received data
  String *rxPacketString = checkRxBuffer();
  if (rxPacketString)
  {
    const char *msg = rxPacketString->c_str();
    strcpy(data, msg);

    // Parse msg to split topic and data
    splitMsgData();

    Serial.println();

    publishMQTT(topic, senVal);

    Serial.print("rx packet: msg: ");
    Serial.println(msg);

    clearDisplay();
    displayString(0, 0, "received msg: ");
    displayString(0, 1, msg);
    displayRssi(rssi());
  }
}

// Arduino main hooks
void setup()
{
  // initialise the board
  configureBoard();

  Serial.begin(115200);

  clearDisplay();
  displayString(0, 0, "Initialising Gateway...");

  // Initialise wifi connection

  while (!isWiFiConnected())
  {
    initWiFi(WIFI_SSID, WIFI_PASSWORD);
    delay(100);
  }

  // Configure LoRa interface
  configureLoRa();
  // Connect to MQTT server
  connectToMQTTServer(MQTT_SERVER, 1883);
  
  Serial.println("setup() done");
}

void loop()
{

  // ensure WiFi stays connected
  checkWiFiStatus();

  // Perform packet forwarding
  checkAndForwardPackets();

  // MQTT housekeeping
  updateMQTT();

  delay(500);
}