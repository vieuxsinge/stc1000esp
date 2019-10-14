#include "ShellHelpers.h"

#include <Shell.h>
#include <errno.h>

bool shellCheckArgs(Shell &s, int argc, const char* const argv[], int min, int max) {
  if(min > 0 && argc < min) {
    s.println("Error: too few arguments");
    return false;
  }
  if(max > 0 && argc > max) {
    s.println("Error: too many arguments");
    return false;
  }
  return true;
}

void shellPrint(Stream &s, float v) {
  s.printf("%.1f\r\n", v);
}

void shellPrint(Stream &s, int v) {
  s.println(v);
}

void shellPrint(Stream &s, bool v) {
  s.println(v?"true":"false");
}

void shellPrint(Stream &s, const char* v) {
  s.println(v);
}

bool shellParse(const char* str, float* v) {
  float val = strtof(str, NULL);
  if( errno < 0 ) {
    return false;
  }
  *v = val;
  return true;
}

bool shellParse(const char* str, int* v) {
  int val = strtol(str, NULL, 0);
  if( errno < 0 ) {
    return false;
  }
  *v = val;
  return true;
}

bool shellParse(const char* str, bool* v) {
  if( strcmp(str, "true") == 0 ) {
    *v = true;
    return true;
  }
  else if( strcmp(str, "false") == 0 ) {
    *v = false;
    return true;
  }
  return false;
}

bool shellParse(const char* str, const char** v) {
  *v = str;
  return true;
}

ShellCommand(reboot, "Reboot system", [](Shell &shell, int argc, const ShellArguments &argv){
  ESP.restart();
});

