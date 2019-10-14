#include "wifi.h"

#include <JustWifi.h>
#include "Settings.h"
#include "ShellHelpers.h"
#include "ShellSubCommands.h"

static SettingsParam ssidParam("wifi.ssid", 64);
static SettingsParam passphraseParam("wifi.passphrase", 128);

static void infoCallback(justwifi_messages_t code, char * parameter) {
  if (code == MESSAGE_TURNING_OFF) {
    Serial.printf("[WIFI] Turning OFF\r\r\n");
  }

  if (code == MESSAGE_TURNING_ON) {
    Serial.printf("[WIFI] Turning ON\r\n");
  }

  if (code == MESSAGE_SCANNING) {
    Serial.printf("[WIFI] Scanning\r\n");
  }

  if (code == MESSAGE_SCAN_FAILED) {
    Serial.printf("[WIFI] Scan failed\r\n");
  }

  if (code == MESSAGE_NO_NETWORKS) {
    Serial.printf("[WIFI] No networks found\r\n");
  }

  if (code == MESSAGE_NO_KNOWN_NETWORKS) {
    Serial.printf("[WIFI] No known networks found\r\n");
  }

  if (code == MESSAGE_FOUND_NETWORK) {
    Serial.printf("[WIFI] %s\r\n", parameter);
  }

  if (code == MESSAGE_CONNECTING) {
    Serial.printf("[WIFI] Connecting to %s\r\n", parameter);
  }

  if (code == MESSAGE_CONNECT_WAITING) {
    // too much noise
  }

  if (code == MESSAGE_CONNECT_FAILED) {
    Serial.printf("[WIFI] Could not connect to %s\r\n", parameter);
  }

  if (code == MESSAGE_CONNECTED) {
    uint8_t * bssid = WiFi.BSSID();

    Serial.printf("[WIFI] MODE STA -------------------------------------\r\n");
    Serial.printf("[WIFI] SSID  %s\r\n", WiFi.SSID().c_str());
    Serial.printf("[WIFI] BSSID %02X:%02X:%02X:%02X:%02X:%02X\r\n",
      bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]
    );
    Serial.printf("[WIFI] CH    %d\r\n", WiFi.channel());
    Serial.printf("[WIFI] RSSI  %d\r\n", WiFi.RSSI());
    Serial.printf("[WIFI] IP    %s\r\n", WiFi.localIP().toString().c_str());
    Serial.printf("[WIFI] MAC   %s\r\n", WiFi.macAddress().c_str());
    Serial.printf("[WIFI] GW    %s\r\n", WiFi.gatewayIP().toString().c_str());
    Serial.printf("[WIFI] MASK  %s\r\n", WiFi.subnetMask().toString().c_str());
    Serial.printf("[WIFI] DNS   %s\r\n", WiFi.dnsIP().toString().c_str());
    Serial.printf("[WIFI] HOST  %s\r\n", WiFi.hostname().c_str());
    Serial.printf("[WIFI] ----------------------------------------------\r\n");
  }

  if (code == MESSAGE_DISCONNECTED) {
    Serial.printf("[WIFI] Disconnected\r\n");
  }

  if (code == MESSAGE_ACCESSPOINT_CREATED) {
    Serial.printf("[WIFI] MODE AP --------------------------------------\r\n");
    Serial.printf("[WIFI] SSID  %s\r\n", jw.getAPSSID().c_str());
    Serial.printf("[WIFI] IP    %s\r\n", WiFi.softAPIP().toString().c_str());
    Serial.printf("[WIFI] MAC   %s\r\n", WiFi.softAPmacAddress().c_str());
    Serial.printf("[WIFI] ----------------------------------------------\r\n");
  }

  if (code == MESSAGE_ACCESSPOINT_DESTROYED) {
    Serial.printf("[WIFI] Disconnecting access point\r\n");
  }

  if (code == MESSAGE_ACCESSPOINT_CREATING) {
    Serial.printf("[WIFI] Creating access point\r\n");
  }

  if (code == MESSAGE_ACCESSPOINT_FAILED) {
    Serial.printf("[WIFI] Could not create access point\r\n");
  }
}

static void setNetwork() {
  jw.cleanNetworks();
  const char* ssid = SETTINGS.getString("wifi.ssid");
  const char* passphrase = SETTINGS.getString("wifi.passphrase");
  if( strcmp(ssid, "") != 0 ) {
    jw.addNetwork(ssid, passphrase);
  }
}

void wifi_init() {
  jw.subscribe(infoCallback);
  jw.enableAP(false);
  jw.enableAPFallback(true);
  jw.enableSTA(true);
  jw.enableScan(true);
  setNetwork();
}

void wifi_start() {
  setNetwork();
  jw.disconnect();
}

void wifi_loop() {
  jw.loop();
}

static auto subcommands = ShellSubCommands({

  ShellSubCommand("ssid", "Get/Set WiFi SSID", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<const char*>(shell, argc, argv, [](const char** v){
      *v = SETTINGS.getString("wifi.ssid");
      return true;
    }, [](const char* v){
      return SETTINGS.setString("wifi.ssid", v);
    });
  }),

  ShellSubCommand("passphrase", "Get/Set WiFi passphrase", [](Shell &shell, int argc, const char* const argv[]){
    return shellGetSet<const char*>(shell, argc, argv, [](const char** v){
      *v = SETTINGS.getString("wifi.passphrase");
      return true;
    }, [](const char* v){
      return SETTINGS.setString("wifi.passphrase", v);
    });
  })

});

ShellCommand(wifi, "WiFi config", [](Shell &shell, int argc, const ShellArguments &argv){
  subcommands.execute(shell, argc, argv.argv);
});

