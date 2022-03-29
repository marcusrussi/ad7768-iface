all: spi
	
spi: spi.c
	gcc -o spi spi.c /usr/local/lib/arm-linux-gnueabihf/libmraa.so
