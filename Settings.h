#pragma once

#include <Arduino.h>

class SettingsParam {
  public:
    SettingsParam(const char* key, size_t size);
    int index = 0;
    const char* key;
    size_t size;
    SettingsParam* next;
};

class Settings {
  public:
    Settings();
    bool begin();
    void addParam(SettingsParam* param);
    int getInt(const char* key);
    bool getBool(const char* key);
    const char* getString(const char* key);
    const byte* getBytes(const char* key);
    SettingsParam* getParam(const char* key);
    bool setInt(const char* key, int value);
    bool setBool(const char* key, bool value);
    bool setString(const char* key, const char* value);
    bool setBytes(const char* key, const byte* value);
    SettingsParam* params;

  private:
    size_t _dataSize;
    size_t _prefixSize;
};

extern Settings SETTINGS;

