#pragma once

#include <Arduino.h>
#include <Shell.h>
#include <errno.h>

bool shellCheckArgs(Shell &s, int argc, const char* const argv[], int min = 0, int max = 0);

void shellPrint(Stream &s, float v);
void shellPrint(Stream &s, int v);
void shellPrint(Stream &s, bool v);
void shellPrint(Stream &s, const char* v);

bool shellParse(const char* str, float* v);
bool shellParse(const char* str, int* v);
bool shellParse(const char* str, bool* v);
bool shellParse(const char* str, const char** v);

template <size_t N>
bool shellPrint(Stream &s, std::array<byte, N> v) {
  for (int i=0 ; i<N ; i++) {
    s.printf("%02x", v[i]);
  }
  s.println();
  return true;
}

template <size_t N>
bool shellParse(const char* str, std::array<byte, N>* v) {
  if( strlen(str) != N*2 ) {
    return false;
  }

  std::array<byte, N> bytes;
  char hexstr[3] = {'\0'};
  for (int i=0 ; i<N ; i++) {
    hexstr[0] = str[i*2];
    hexstr[1] = str[i*2+1];
    bytes[i] = strtol(hexstr, NULL, 16);
    if(errno < 0) {
      return false;
    }
  }

  *v = bytes;
  return true;
}

template <typename T, typename ReadFunc>
bool shellGet(Shell &shell, int argc, const char* const argv[], ReadFunc read) {
  if( !shellCheckArgs(shell, argc, argv, 1, 1) ) {
    return false;
  }

  T v;
  if( !read(&v) ) {
    shell.println("Error: failed to get value");
    return false;
  }
  
  shellPrint(shell, v);
  return true;
}

template <typename T, typename ReadFunc, typename WriteFunc>
bool shellGetSet(Shell &shell, int argc, const char* const argv[], ReadFunc read, WriteFunc write) {
  if( !shellCheckArgs(shell, argc, argv, 1, 2) ) {
    return false;
  }

  if( argc == 1 ) {
    return shellGet<T, ReadFunc>(shell, argc, argv, read);
  }
  
  T v;
  if( !shellParse(argv[1], &v) ) {
    shell.println("Error: failed to parse value");
    return false;
  }
  if( !write(v) ) {
    shell.println("Error: failed to set value");
    return false;
  }

  return true;
}

