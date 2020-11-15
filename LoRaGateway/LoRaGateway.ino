/*
 * LoRaGateway.ino 
 * 
 * Implements a minimal, single-channel LoRa to MQTT gateway
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.
 */

#include "Arduino.h"
#include <PubSubClient.h>

#include "HAL.h"
#include "LoRaInterface.h"

// WiFi credentials
#define WIFI_SSID "+++++++"
#define WIFI_PASSWORD "+++++"

// MQTT Broker info
// Define either an IP address...
#define MQTT_SERVER IPAddress(192, 168, 0, 26)
// ...or a hostname
//#define MQTT_SERVER   "Mac-mini"

// checkAndForwardPackets()
// This is the core function that checks for received LoRa packets and forwards the contents on to MQTT
//

char data[MAX_LORA_PAYLOAD + 1] = "";
const char delim[2] = "/";
static void checkAndForwardPackets()
{
  Serial.println("checkAndForwardPackets");
  // check for received data
  String *rxPacketString = checkRxBuffer();
  if (rxPacketString)
  {
    const char *msg = rxPacketString->c_str();
    strcpy(data, msg);

    int init_size = strlen(data);
    char *ptr = strtok(data, delim);

    while (ptr != NULL)
    {
      Serial.println(ptr);
      ptr = strtok(NULL, delim);
    }

    Serial.println();

    publishMQTT(msg);

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
  Serial.println("setup()");

  pinMode(LED_BUILTIN, OUTPUT);

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

  Serial.println("Wifi connected");
  Serial.println("Trying to connect to MQTT server");
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

  delay(100);
}
