#!/usr/bin/python3

""" Hawking Talking Demonstration. """
import time
import spidev

SECONDS_DELAY = 2

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

to_send = [0x00] * (512 + 1)

print(len(to_send))

print("Send")
#to_send = [0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00]
to_send[1] = 0xFF
to_send[9] = 0xFF
# print(to_send)
spi.writebytes(to_send)
# spi.xfer(to_send)
time.sleep(SECONDS_DELAY)

print("Send")
# to_send = [0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00]
to_send[1] = 0x00
to_send[2] = 0xFF
to_send[9] = 0xFF
spi.writebytes(to_send)
# spi.xfer(to_send)
time.sleep(SECONDS_DELAY)

print("Send")
# to_send = [0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00]
to_send[2] = 0x00
to_send[3] = 0xFF
to_send[9] = 0xFF
spi.writebytes(to_send)
# spi.xfer(to_send)
time.sleep(SECONDS_DELAY)

print("Send")
# to_send = [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]
to_send[3] = 0x00
to_send[9] = 0x00
spi.writebytes(to_send)
# spi.xfer(to_send)
time.sleep(SECONDS_DELAY)

spi.close()

print("Finished")

