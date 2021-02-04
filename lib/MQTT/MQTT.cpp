/*
 * MQTT.cpp 
 * 
 * Implements all MQTT-related functions.
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.
 */
#include <config.h>


#include <WiFi.h>
#include <MQTT.h>
#include <PubSubClient.h>
#include <HAL.h>



// Instantiate the MQTT PubSubClient (requires a WiFiClient which is not used anywhere else)
static WiFiClient _wifiClient;
static PubSubClient _mqttClient(_wifiClient);
char str[100];

// MQTT Setup

// Helper connect to MQTT server using host and port
//
void connectMQTT()
{
  // Wait until we're connected
  while (!_mqttClient.connected())
  {
    // Create a unique MQTT client ID of form LoRa-Gateway-XXXX, where the XXXX is the Heltec chip ID.
    String clientId = "LoRa-Gateway-";
    clientId += String((uint32_t)(_chipID >> 32));
    clientId += String((uint32_t)_chipID);
    Serial.printf("MQTT connecting as client %s\n", clientId.c_str());
    // Attempt to connect

    if (_mqttClient.connect(clientId.c_str()))
    {
      clearDisplay();
      displayString(0, 5, "MQTT connected");
      // Once connected, publish an announcement...
      _mqttClient.publish(_topic, String(clientId + " connected").c_str());
      clearDisplay();
    }
    else
    {
      Serial.printf("MQTT failed, state %i, retrying...\n", _mqttClient.state());
      // Wait before retrying
      delay(2500);
    }
  }
}

// connect to MQTT server using IP address and port
//
void connectToMQTTServer(IPAddress addr, uint16_t port)
{
  _mqttClient.setServer(addr, port);
  sprintf(str, "Trying to connecto to MQTT server on:");
  displayString(0, 5, str);
  sprintf(str, "%s : %d", addr.toString().c_str(), port);
  displayString(0, 6, str);
  connectMQTT();
}


// connect to MQTT server using host and port
//
void connectToMQTTServer(const char *host, uint16_t port)
{
  _mqttClient.setServer(host, port);
  clearDisplay();
  sprintf(str, "MQTT Host: %s Port: %d", host, port);
  displayString(0, 5, str);
  connectMQTT();
}

// MQTT housekeeping
//
void updateMQTT()
{
  if (!_mqttClient.connected())
  {
    connectMQTT();
  }
  // this is ESSENTIAL!
  _mqttClient.loop();
}

// Publish a message on a topic
//
void publishMQTT(const char *topic, const char *msg)
{
  _mqttClient.publish(topic, msg);
}

void publishMQTT(const char *msg)
{
  _mqttClient.publish(_topic, msg);
}
