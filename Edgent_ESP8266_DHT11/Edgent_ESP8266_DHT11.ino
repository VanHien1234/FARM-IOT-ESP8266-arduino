// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLUNUACnm4"
#define BLYNK_DEVICE_NAME "FarmIOT"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG
#define USE_NODE_MCU_BOARD
#include "BlynkEdgent.h"
#include "DHT.h"
#include <NTPtimeESP.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#define startWaterTime 8 
#define endWaterTime 16 
#define timeBTW 2 
#define waterLength 30 
#define timeSleep 10 

const String Ver = "v1.0.0";
const String dateup = "Mon, NOv 15,2021";

#define DHTPIN D5 //DHT11
#define Relay D7 // Relay (Pump)
#define DHTTYPE DHT11
bool processing = 0;

ESP8266WiFiMulti WiFiMulti;
WidgetTerminal terminal(V3);
BlynkTimer timer;
strDateTime dateTime;


#include "RTC_setting.h"
#include "Blynksetup.h"


unsigned long times=millis();

WidgetLED led(V5);
// V5 LED Widget is blinking
void blinkLedWidget(){
  if (led.getValue()) {
    led.off();
  } else {
    led.on();
  }

  if(processing) return;
  
  if (WiFi.status() != WL_CONNECTED) 
  {
    ESP.deepSleep(timeSleep * (long)1e6);
  }
    
  UpdateDHT(); 
  if(getTimeWatering()) AcessWater();// Water
}

void setup(){
  Serial.begin(115200);
  delay(100);
  BlynkEdgent.begin();

  pinMode(Relay, OUTPUT);
  digitalWrite(Relay, LOW);
  dht.begin();

  terminal.clear();
  Serial.println("Esp8266 IoT-Farm ");
  terminal.println(Ver);
  terminal.print("NT131.M11-Nhom19");
  Serial.print("Connected.");
  terminal.flush();
  
  timer.setInterval(1000L, blinkLedWidget);
}

void loop() {
  BlynkEdgent.run();
  timer.run();
  }
