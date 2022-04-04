all: spi
	
spi: spi.c log.c log.h
	gcc -DLOG_USE_COLOR -o spi spi.c /usr/local/lib/arm-linux-gnueabihf/libmraa.so.2 log.c
