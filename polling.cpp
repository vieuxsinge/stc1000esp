#include "polling.h"

#include "stc1000p.h"
#include "mqtt.h"
#include "Poller.h"

const int POLL_DELAY = 5;

static auto setpointPoller = Poller<float>(POLL_DELAY, [] (float* data) {
  return stc1000p.readSetpoint(data);
}, [] (float data) {
  return mqttPublishF("setpoint", "%.1f", data);
});

static auto temperaturePoller = Poller<float>(POLL_DELAY, [] (float* data) {
  return stc1000p.readTemperature(data);
}, [] (float data) {
  return mqttPublishF("temperature", "%.1f", data);
});

static auto heatingPoller = Poller<bool>(POLL_DELAY, [] (bool* data) {
  return stc1000p.readHeating(data);
}, [] (bool data) {
  return mqttPublishF("heating", "%s", data?"true":"false");
});

static auto coolingPoller = Poller<bool>(POLL_DELAY, [] (bool* data) {
  return stc1000p.readCooling(data);
}, [] (bool data) {
  return mqttPublishF("cooling", "%s", data?"true":"false");
});

void polling_start() {
  setpointPoller.start();
  temperaturePoller.start();
  heatingPoller.start();
  coolingPoller.start();
}
