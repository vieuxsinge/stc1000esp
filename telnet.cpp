#include "telnet.h"

#include <ESPAsyncTCP.h>
#include <Shell.h>
#include <SyncClient.h>
#include <JustWifi.h>

static AsyncServer server(23);

static Shell shell;
static SyncClient client;
static bool clientConnected = false;

static void newClient(void* arg, AsyncClient* c) {
  if (client.connected()) {
  	Serial.printf("[TELNET] Reject client: %s\n", c->remoteIP().toString().c_str());
    c->close(true);
  	return;
  }
  
	Serial.printf("[TELNET] New client connected: %s\n", c->remoteIP().toString().c_str());
	client = SyncClient(c);
	shell.begin(client, 5);
}

static void begin() {
  server.begin();
  Serial.println("[TELNET] Listening");
}

static void wifiCallback(justwifi_messages_t code, char * parameter) {
  if (code == MESSAGE_ACCESSPOINT_CREATED || code == MESSAGE_CONNECTED) {
    begin();
  }
}

void telnet_init() {
  jw.subscribe(wifiCallback);
	server.onClient(newClient, NULL);
}

void telnet_loop() {
  shell.loop();
  if (!clientConnected && client.connected()) {
    clientConnected = true;
  }
  else if (clientConnected && !client.connected()) {
    clientConnected = false;
	  Serial.println("[TELNET] Client disconnected");
  }
}

