#pragma once

#include <Arduino.h>
#include <Ticker.h>

template <typename T>
class Poller {
  public:
    Poller(int delay, bool (*read)(T* data), bool (*notify)(T data));
    void start();
    void stop();
  
  private:
    Ticker _ticker;
    T _value;
    bool _initialized = false;
    int _delay;
    bool (*_read)(T* data);
    bool (*_notify)(T data);
    void _poll();
};

template <typename T>
Poller<T>::Poller(int delay, bool (*read)(T* data), bool (*notify)(T data)):
  _delay(delay),
  _read(read),
  _notify(notify) {
}

template <typename T>
void Poller<T>::start() {
  _ticker.attach(_delay, [this] () {
    this->_poll();
  });
}

template <typename T>
void Poller<T>::stop() {
  _ticker.detach();
  _initialized = false;
}

template <typename T>
void Poller<T>::_poll() {
  T v;
  if( _read(&v) ) {
    if( _initialized && v == _value ) {
      return;
    }
    _value = v;
    bool notified = _notify(_value);
    if( notified && !_initialized ) {
      _initialized = true;
    }
  }
}

