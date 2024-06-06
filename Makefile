all:
	gcc -Wall -g -o main.exe main.c src/system.c src/configurator.c src/module_io.c src/mqtt.c src/log.c src/chips/pcf8574chip.c src/chips/pcf8575chip.c src/libs/frozen.c -pthread