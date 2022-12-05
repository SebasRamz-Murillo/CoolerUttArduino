#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883        
/*************************** Sketch Code ************************************/
void MQTT_connect();

char Data[6];
int a=0,b=0,c=0,d=0,e=0,f=0;
unsigned long lastrecorded = 0;
char json;
void setup() {
  Serial.begin(115200);
  Serial.println(F("Iniciando..."));

  do{
    StaticJsonDocument<200> doc;
    if(Serial.available()) { json = Serial.read();}
    
  }while(!espera);
  DeserializationError error = deserializeJson(doc, json.readStringUntil('?'));

  char WLAN_SSID=json["nombredered"];
  char WLAN_PASS=json["contraseñared"];
  char AIO_USERNAME=json["usuarioadafruit"];
  char AIO_KEY=json["iokey"];
  /************ Global State (you don't need to change this!) ******************/
  WiFiClient client;
  Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
  /****************************** Feeds ***************************************/

  Adafruit_MQTT_Publish Humi = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Humidity");
  Adafruit_MQTT_Publish Temp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/Temperature");

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");

  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi connected with: ");Serial.println(WLAN_SSID);
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  delay(1000);
}


void loop() { // run over and over

  MQTT_connect();
  unsigned long now = millis();
  if(Serial.available()) {
   if(now > lastrecorded + 15000 ){    
      lastrecorded = now;
    }
  }
}

void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  while ((ret = mqtt.connect()) != 0) { 
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();

  uint8_t retries = 3;
       delay(5000);  
       retries--;
       if (retries == 0) {
         while (1);
       }
  }    
  Serial.println("MQTT Connected!");
 }