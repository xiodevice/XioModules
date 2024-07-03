all:
	gcc -Wall -g -o main.exe main.c src/system.c src/configurator.c src/mqtt.c src/log.c src/modules/module.c src/modules/mdi8.c src/modules/mdo8r1.c src/modules/chips/i2c.c src/modules/chips/pcf8574.c src/libs/frozen.c -pthread