import spidev

bus = 0, device = 0

def register_val(spi, reg):
    spi.xfer2([reg, 0x00], 500000, 0, 8)
    result = spi.xfer2([reg, 0x00], 500000, 0, 8)[1])

    return "{:08b}".format(result)

spi = spidev.SpiDev()
spi.open(bus, device)
