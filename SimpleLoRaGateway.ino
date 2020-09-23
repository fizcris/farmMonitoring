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
#define WIFI_SSID "vodafone3B00"
#define WIFI_PASSWORD "CasaAlonso"

// MQTT Broker info
// Define either an IP address...
#define MQTT_SERVER IPAddress(192, 168, 0, 26)
// ...or a hostname
//#define MQTT_SERVER   "Mac-mini"

// checkAndForwardPackets()
// This is the core function that checks for received LoRa packets and forwards the contents on to MQTT
//
static void checkAndForwardPackets()
{
  // check for received data
  String *rxPacketString = checkRxBuffer();
  if (rxPacketString)
  {
    // forward packet content to MQTT
    const char *msg = rxPacketString->c_str();

    publishMQTT(msg);

    Serial.print("rx packet: msg: ");
    Serial.println(msg);

    clearDisplay();
    displayString(0, 0, "received msg: ");
    displayString(8, 0, msg);
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
  initWiFi(WIFI_SSID, WIFI_PASSWORD);

  // Configure LoRa interface
  configureLoRa();

  if (isWiFiConnected())
  {
    connectToMQTTServer(MQTT_SERVER, 1883);
  }
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
