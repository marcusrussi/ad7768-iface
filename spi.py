import time
import spidev

bus = 0
device = 0

def get_register(spi, reg):
    spi.xfer([0x80 | reg, 0x00], 500000, 0, 8)
    result = spi.xfer([0x80 | reg, 0x00], 500000, 0, 8)[1]

    return "{:08b}".format(result)

def set_register(spi, reg, val):
    spi.xfer2([0x00 | reg, val], 500000, 0, 8)

def reset_device(spi):
    set_register(spi, 0x06, 0x03)
    set_register(spi, 0x06, 0x02)
    print("Resetting...")
    time.sleep(10 * 1/1000) # sleep 4ms
    
    print("First two frames:")
    print(spi.xfer([0x80 | 0x00, 0x00], 500000, 0, 8))
    print(spi.xfer([0x80 | 0x00, 0x00], 500000, 0, 8))
    

spi = spidev.SpiDev()
spi.open(bus, device)
spi.max_speed_hz = 500000

print(get_register(spi, 0x00))
print(get_register(spi, 0x01))
print(get_register(spi, 0x02))
print(get_register(spi, 0x03))
print(get_register(spi, 0x04))
print(get_register(spi, 0x05))
print(get_register(spi, 0x06))
print(get_register(spi, 0x07))
print(get_register(spi, 0x08))

print("0x09 status register:")
print(get_register(spi, 0x09))
print(get_register(spi, 0x0a))
print(get_register(spi, 0x0b))
print(get_register(spi, 0x0c))
print(get_register(spi, 0x0d))
print(get_register(spi, 0x0e))
print(get_register(spi, 0x0f))
print(get_register(spi, 0x10))

reset_device(spi)

print(get_register(spi, 0x00))
print(get_register(spi, 0x01))
print(get_register(spi, 0x02))
print(get_register(spi, 0x03))
print(get_register(spi, 0x04))
print(get_register(spi, 0x05))
print(get_register(spi, 0x06))
print(get_register(spi, 0x07))
print(get_register(spi, 0x08))
print("0x09 status register:")
print(get_register(spi, 0x09))
print(get_register(spi, 0x0a))
print(get_register(spi, 0x0b))
print(get_register(spi, 0x0c))
print(get_register(spi, 0x0d))
print(get_register(spi, 0x0e))
print(get_register(spi, 0x0f))
print(get_register(spi, 0x10))
