# STC1000esp

This is an ESP8266 firmware to add Wifi abilities to your STC1000 thermostat.

1. Flash your STC1000 with [STC1000+ COM firmware](https://github.com/matsstaff/stc1000p)
1. Connect the STC1000 to a ESP8266 board
1. Flash the ESP8266 board with ESP-STC

## Features

- WiFi config: Provides a WiFi Access Point to help during the configuration phase
- OTA: Over The Air reprogrammation using Arduino OTA method
- Telnet Shell: Configure all parameters through shell and access STC1000+
- MQTT: Publish STC1000 data to an MQTT broker

You could then plug this to a grafana server for instance, in order to have
supervision. We [made a script](https://github.com/vieuxsinge/supervision)
to help with this.

## Tooling

Once you have these thermostat installed, you might want to use these tools to
use them. We've made a few scripts to help :

If you want to retrieve the list of thermostats :

```bash
./scripts/list-ota.sh
```

## Getting started

Using ESP8266 + STC1000 is not a straight-forward setup to make a WiFi thermostat.
You could rather use relays, display and buttons directly connected to an ESP board.

We use this setup because:

1. We already had STC1000 installed
1. It is a really cheap setup (less than 30€)

If you consider all parts of the setup, STC1000 provides for ~15€:

- 1 x Power transformation from 220V to 5V
- 2 x 220V relays (heat and cool)
- 1 x Numeric display
- 4 x Buttons
- 1 x Temperature sensor

You then only need to add a ESP8266 board like NodeMCU for ~10€ and you get
a WiFi thermostat for less than 30€ without complex electronic design and soldering.

### Prerequisites

The first step is to flash your STC1000 with the excellent STC1000+ firmware.
STC1000+ comes in multiple variants and you must use the "COM" variant to enable
one-wire communication on the STC1000.

Be carefull: not all STC1000 are compatible with STC1000+ firmware.

STC1000 modification are explained on the [STC1000+ project.](https://github.com/matsstaff/stc1000p)

### Electronics setup

Once your STC1000 is upgraded to STC1000+ using the single-wire COM variant, you are ready to plug the
ESP8266 board to the STC1000.

We are using the NodeMCU board, but the project should work with any ESP8266 based board with few changes.

Pin attribution on NodeMCU:

STC1000 | NodeMCU
------- | -------
ICSPDAT | D0
ICSPCLK | Unused
GND     | GND
5V      | VIN
mCLR    | Unused

Pin D0 is used because it provides a PULLDOWN mode and it is used directly without any pulldown resistor
on the connection.

### Build and upload

First install your Arduino environment. We use [ESP Makefile](https://github.com/plerup/makeEspArduino)
to build the project, but it should work with the Arduino IDE with few changes.

Make sure you have all dependencies installed:

* [xoseperez/justwifi](https://github.com/xoseperez/justwifi)
* [marvinroger/async-mqtt-client](https://github.com/marvinroger/async-mqtt-client)
* [fredericsureau/arduino-stc1000p](https://github.com/vieuxsinge/arduino-stc1000p)
* [fredericsureau/arduino-shell](https://github.com/vieuxsinge/arduino-shell)

Connect the NodeMCU using USB port.

```
make
make upload
```

That's it!

For OTA you can use:

```
OTA_ADDR=<ip_address> make ota
```

You can send telnet commands in batch using:

```
cd scripts
./telnet.sh <ip_address> commands.txt
```

## License

This project is licensed under the GPLv3 License - see the [LICENSE](LICENSE) file for details

## Acknowledgments

This project use a lot of great Arduino/ESP librairies, thanks!

* [xoseperez/justwifi](https://github.com/xoseperez/justwifi) - Simple WiFi
* [marvinroger/async-mqtt-client](https://github.com/marvinroger/async-mqtt-client) - Asynchronous MQTT client for ESP
* [fredericsureau/arduino-stc1000p](https://github.com/vieuxsinge/arduino-stc1000p) - Arduino communication with the STC1000+
* [fredericsureau/arduino-shell](https://github.com/vieuxsinge/arduino-shell) - Shell for arduino
