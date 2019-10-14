#pragma once

#include <stdarg.h>

bool mqttPublish(const char* topic, const char* payload);
bool mqttPublishF(const char* topic, const char* format...);

void mqtt_init();
void mqtt_start();
