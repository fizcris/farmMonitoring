/*
  This is a simple example show the Heltec.LoRa sended data in OLED.

  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  https://heltec.org
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/

#include "heltec.h"

#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6

unsigned int counter = 0;
unsigned int sensorNum = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;
String payload;

void logo()
{
  Heltec.display->clear();
  //Heltec.display->drawXbm(0,5,logo_width,logo_height,logo_bits);
  Heltec.display->display();
}


  typedef struct { 
  uint8_t sensorNumber;
  String sensorName;
  String value;
} sensorsDictionary;

const String user = "Lina";
const String facility = "Nave1";

sensorsDictionary mySensorDictArr[] {
    {0, "tempI1", "00"},
    {1, "tempI2", "00"},
    {2, "tempD1", "00"},
    {3, "tempD2", "00"}
};

void setup()
{
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(500);


  Heltec.display->drawString(0, 0, "Sensors Initial success!");
  Heltec.display->display();
  
  
  delay(1000);
}

void loop()
{
  mySensorDictArr[sensorNum].value = String(counter);
  payload = String(user + "/" + facility + "/" + mySensorDictArr[sensorNum].sensorName + "/" +
   mySensorDictArr[sensorNum].value );

   
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, String(counter));
  Heltec.display->drawString(0, 10, payload);
  Heltec.display->display();

  

  // send packet
  LoRa.beginPacket();
/*
 * LoRa.setTxPower(txPower,RFOUT_pin);
 * txPower -- 0 ~ 20
 * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
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
  counter++;
  if(sensorNum >= sizeof(mySensorDictArr)/sizeof(sensorsDictionary)) {sensorNum = 0;};
}
