""" SPI to ATMega32 DMX Demonstration. """
import microbit
import time

# Press any button to repeat the test.
microbit.display.show(microbit.Image.SAD)

while not microbit.button_a.is_pressed() and not microbit.button_b.is_pressed():
    pass

microbit.spi.init(baudrate=500000, bits=8, mode=0, sclk=microbit.pin13, mosi=microbit.pin15, miso=microbit.pin14)

DELAY = 2000
SECONDS_DELAY = 2

to_send = [0x00] * (512 + 1)
pad = [0x00] * (len(to_send) - 13)

while True:

    # to_send = [0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00]
    to_send[1] = 0xFF
    to_send[9] = 0xFF
    # microbit.spi.write(bytes(to_send))
    microbit.spi.write(b'\x00\xFF\x00\x00\x00\x00\x00\x00\x00\xFF\x00\x00\x00')
    microbit.spi.write(bytes(pad))
    microbit.display.show(1, delay=DELAY, wait=True, loop=False, clear=False)
    time.sleep(SECONDS_DELAY)

    # to_send = [0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00]
    to_send[1] = 0x00
    to_send[2] = 0xFF
    to_send[9] = 0xFF
    # microbit.spi.write(bytes(to_send))
    microbit.spi.write(b'\x00\x00\xFF\x00\x00\x00\x00\x00\x00\xFF\x00\x00\x00')
    microbit.spi.write(bytes(pad))
    microbit.display.show(2, delay=DELAY, wait=True, loop=False, clear=False)
    time.sleep(SECONDS_DELAY)

    # to_send = [0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00]
    to_send[2] = 0x00
    to_send[3] = 0xFF
    to_send[9] = 0xFF
    # microbit.spi.write(bytes(to_send))
    microbit.spi.write(b'\x00\x00\x00\xFF\x00\x00\x00\x00\x00\xFF\x00\x00\x00')
    microbit.spi.write(bytes(pad))
    microbit.display.show(3, delay=DELAY, wait=True, loop=False, clear=False)
    time.sleep(SECONDS_DELAY)

    # to_send = [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]
    to_send[3] = 0x00
    to_send[9] = 0x00
    # microbit.spi.write(bytes(to_send))
    microbit.spi.write(b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00')
    microbit.spi.write(bytes(pad))
    microbit.display.show(4, delay=DELAY, wait=True, loop=False, clear=False)
    time.sleep(SECONDS_DELAY)

    microbit.display.show(microbit.Image.HAPPY)

    # Press any button to repeat the test.
    while not microbit.button_a.is_pressed() and not microbit.button_b.is_pressed():
        pass
