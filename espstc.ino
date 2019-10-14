#include <Arduino.h>
#include <ArduinoOTA.h>
#include <JustWifi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>
#include <Stc1000p.h>
#include <Shell.h>
#include <errno.h>
#include <stdarg.h>

#include "Settings.h"
#include "wifi.h"
#include "telnet.h"
#include "mqtt.h"
#include "stc1000p.h"
#include "polling.h"

void setup() {
  Serial.begin(115200);
  
  if( SETTINGS.begin() ) {
    Serial.println("[SETTINGS] Initialize memory");
  }

  wifi_init();
  telnet_init();
  mqtt_init();

  wifi_start();
  ArduinoOTA.begin();
  mqtt_start();
  polling_start();
}

void loop() {
  wifi_loop();
  ArduinoOTA.handle();
  telnet_loop();
}

