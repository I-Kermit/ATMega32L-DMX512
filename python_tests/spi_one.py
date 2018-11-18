#!/usr/bin/python3

""" Hawking Talking Demonstration. """
import spidev

spi = spidev.SpiDev()
bus = 0
device = 0

spi.open(bus, device)

def spi_diagnostics():
    print("SPi diagnostics")
    print("max_speed_hz: " + str(spi.max_speed_hz))
    print("mode: " + str(spi.mode))
    print("bits_per_word: " + str(spi.bits_per_word))
    print("cshigh: " + str(spi.cshigh))
    print("loop: " + str(spi.loop))
    print("lsbfirst: " + str(spi.lsbfirst))
    print("threewire: " + str(spi.threewire))

spi_diagnostics()

# ATMega 8MHz/32
spi.max_speed_hz = 500000

spi.mode = 0

spi_diagnostics()

print("Send")

# print("Lots of data")
to_send = [0x99]
#spi.xfer(to_send)
#spi.xfer2(to_send)
spi.writebytes(to_send)

spi.close()

print("Finished")

