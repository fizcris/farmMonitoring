/*
 * MQTT.h 
 * 
 * Implements all MQTT-related functions.
 * 
 * (c) 2019 Lee Dowthwaite. All Rights Reserved.
 */

// Topic for MQTT
//
static const char *_topic = "allNodes";

// connect to MQTT server using IP address and port
void connectToMQTTServer(IPAddress addr, uint16_t port);
// connect to MQTT server using host and port
void connectToMQTTServer(const char *host, uint16_t port);

// Helper connect to MQTT server using host and port
void connectMQTT();

// MQTT housekeeping
void updateMQTT();

// Publish a message on a topic
void publishMQTT(const char *topic, const char *msg);
void publishMQTT(const char *msg);

