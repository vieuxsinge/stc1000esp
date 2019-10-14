#include "mqtt.h"

#include <AsyncMqttClient.h>
#include <JustWifi.h>
#include <Ticker.h>

#include "Settings.h"
#include "ShellHelpers.h"
#include "ShellSubCommands.h"

static SettingsParam hostParam("mqtt.host", 64);
static SettingsParam portParam("mqtt.port", sizeof(int));
static SettingsParam secureParam("mqtt.secure", sizeof(bool));
static SettingsParam prefixParam("mqtt.prefix", 64);
static SettingsParam serverFingerprintParam("mqtt.serverfingerprint", 20);
static SettingsParam usernameParam("mqtt.username", 64);
static SettingsParam passwordParam("mqtt.password", 64);

static AsyncMqttClient client;
static Ticker reconnectTimer;

static void prefix(char* topic, size_t size, const char* suffix) {
  const char* sep = "";
  const char* p = SETTINGS.getString("mqtt.prefix");
  if( strcmp(p, "") != 0 ) {
    sep = "/";
  }
  
  snprintf(topic, size, "%s%s%s", p, sep, suffix);
}

static void onConnect(bool sessionPresent) {
  Serial.println("[MQTT] Connected");

  char topic[128];
  prefix(topic, sizeof(topic), "status");
  client.setWill(topic, 1, true, "offline");
  client.publish(topic, 1, true, "online");
}

static void onDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("[MQTT] Disconnected");

  if (jw.connected()) {
    Serial.println("[MQTT] Reconnecting...");
    reconnectTimer.once(5, [] {
      client.connect();
    });
  }
}

static void wifiCallback(justwifi_messages_t code, char * parameter) {
  if (code == MESSAGE_ACCESSPOINT_CREATED || code == MESSAGE_CONNECTED) {
    client.connect();
  }
}

bool mqttPublish(const char* t, const char* payload) {
  char topic[128];
  prefix(topic, sizeof(topic), t);
  
  if( !client.publish(topic, 0, false, payload) ) {
    Serial.print("[MQTT] Failed to publish ");
    Serial.print(topic);
    Serial.print("=");
    Serial.println(payload);
    return false;
  }
  
  Serial.print("[MQTT] Published ");
  Serial.print(topic);
  Serial.print("=");
  Serial.println(payload);
  return true;
}

bool mqttPublishF(const char* topic, const char* format...) {
  va_list args;
  va_start(args, format);
  
  char payload[256];
  vsnprintf(payload, sizeof(payload)-1, format, args);
  bool res = mqttPublish(topic, payload);
  
  va_end(args);
  
  return res;
}

void mqtt_init() {
  jw.subscribe(wifiCallback);
  client.onConnect(onConnect);
  client.onDisconnect(onDisconnect);
}

void mqtt_start() {
  client.setServer(SETTINGS.getString("mqtt.host"), SETTINGS.getInt("mqtt.port"));
  client.setSecure(SETTINGS.getBool("mqtt.secure"));
  client.addServerFingerprint(SETTINGS.getBytes("mqtt.serverfingerprint"));

  if( strcmp(SETTINGS.getString("mqtt.username"), "") != 0 ) {
    client.setCredentials(SETTINGS.getString("mqtt.username"), SETTINGS.getString("mqtt.password"));
  }

  if (jw.connected()) {
    client.connect();
  }
}

static void mqtt_stop() {
  client.disconnect();
}

static auto subcommands = ShellSubCommands({

  ShellSubCommand("host", "Get/Set MQTT Host", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<const char*>(shell, argc, argv, [](const char** v){
      *v = SETTINGS.getString("mqtt.host");
      return true;
    }, [](const char* v){
      return SETTINGS.setString("mqtt.host", v);
    });
  }),

  ShellSubCommand("port", "Get/Set MQTT Port", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<int>(shell, argc, argv, [](int* v){
      *v = SETTINGS.getInt("mqtt.port");
      return true;
    }, [](int v){
      return SETTINGS.setInt("mqtt.port", v);
    });
  }),

  ShellSubCommand("secure", "Get/Set MQTT Secure mode", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<bool>(shell, argc, argv, [](bool* v){
      *v = SETTINGS.getBool("mqtt.secure");
      return true;
    }, [](bool v){
      return SETTINGS.setBool("mqtt.secure", v);
    });
  }),

  ShellSubCommand("prefix", "Get/Set MQTT topics prefix", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<const char*>(shell, argc, argv, [](const char** v){
      *v = SETTINGS.getString("mqtt.prefix");
      return true;
    }, [](const char* v){
      return SETTINGS.setString("mqtt.prefix", v);
    });
  }),

  ShellSubCommand("serverfingerprint", "Get/Set MQTT Server fingerprint", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<std::array<byte, 20>>(shell, argc, argv, [](std::array<byte, 20>* v){
      const byte* bytes = SETTINGS.getBytes("mqtt.serverfingerprint");
      std::copy(bytes, bytes+20, v->begin());
      return true;
    }, [](std::array<byte, 20>& v){
      return SETTINGS.setBytes("mqtt.serverfingerprint", v.data());
    });
  }),

  ShellSubCommand("username", "Get/Set MQTT Username", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<const char*>(shell, argc, argv, [](const char** v){
      *v = SETTINGS.getString("mqtt.username");
      return true;
    }, [](const char* v){
      return SETTINGS.setString("mqtt.username", v);
    });
  }),

  ShellSubCommand("password", "Get/Set MQTT Password", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<const char*>(shell, argc, argv, [](const char** v){
      *v = SETTINGS.getString("mqtt.password");
      return true;
    }, [](const char* v){
      return SETTINGS.setString("mqtt.password", v);
    });
  })

});

ShellCommand(mqtt, "MQTT config", [](Shell &shell, int argc, const ShellArguments &argv){
  subcommands.execute(shell, argc, argv.argv);
});

