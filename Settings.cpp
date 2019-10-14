#include "Settings.h"

#include <Arduino.h>
#include <EEPROM.h>

Settings SETTINGS = Settings();

static const unsigned long MAGIC = 223456789;

SettingsParam::SettingsParam(const char* key, size_t size):
  key(key),
  size(size),
  next(NULL) {
  SETTINGS.addParam(this);
}

Settings::Settings() {
  _prefixSize = sizeof(MAGIC);
}

bool Settings::begin() {
  EEPROM.begin(_prefixSize + _dataSize);
  
  unsigned long magic;
  EEPROM.get(0, magic);
  if( magic != MAGIC ) {
    memset(&EEPROM[0], 0, EEPROM.length());
    EEPROM.put(0, MAGIC);
    EEPROM.commit();
    return true;
  }
  
  return false;
}

void Settings::addParam(SettingsParam* param) {
  param->next = params;
  params = param;
  param->index = _dataSize;
  _dataSize += param->size;
}

int Settings::getInt(const char* key) {
  return *((int*)getBytes(key));
}

bool Settings::getBool(const char* key) {
  return *((bool*)getBytes(key));
}

const char* Settings::getString(const char* key) {
  return (const char*)getBytes(key);
}

const byte* Settings::getBytes(const char* key) {
  SettingsParam* param = getParam(key);
  if( param == NULL ) {
    return NULL;
  }
  return &EEPROM[_prefixSize + param->index];
}

bool Settings::setInt(const char* key, int value) {
  return setBytes(key, (byte*)&value);
}

bool Settings::setBool(const char* key, bool value) {
  return setBytes(key, (byte*)&value);
}

bool Settings::setString(const char* key, const char* value) {
  SettingsParam* param = getParam(key);
  if( param == NULL ) {
    return false;
  }
  strncpy((char*)&EEPROM[_prefixSize + param->index], value, param->size);
  EEPROM.commit();
  return true;
}

bool Settings::setBytes(const char* key, const byte* value) {
  SettingsParam* param = getParam(key);
  if( param == NULL ) {
    return false;
  }
  memcpy(&EEPROM[_prefixSize + param->index], value, param->size);
  EEPROM.commit();
  return true;
}

SettingsParam* Settings::getParam(const char* key) {
  for( SettingsParam* p = params ; p != NULL ; p = p->next ) {
    if( strcmp(p->key, key) == 0 ) {
      return p;
    }
  }
  return NULL;
}

