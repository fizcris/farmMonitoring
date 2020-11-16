/*
 * USER_RECEIVER.h
 * 
 * User data
 * 
 * (c) 2020 Cristian Alonso. All Rights Reserved.
 */

// WiFi credentials
#define WIFI_SSID "+++++++"
#define WIFI_PASSWORD "+++++"

// MQTT Broker info
// Define either an IP address...
#define MQTT_SERVER IPAddress(192, 168, 0, 5)
// ...or a hostname
//#define MQTT_SERVER   "Mac-mini"

// Topic for MQTT
static const char *_topic = "allNodes";