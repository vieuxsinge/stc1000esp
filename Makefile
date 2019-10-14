BOARD = nodemcuv2
BUILD_DIR = build
BUILD_EXTRA_FLAGS = -DASYNC_TCP_SSL_ENABLED=1
LWIP_VARIANT = hb2f

include /usr/share/arduino/makeEspArduino.mk

