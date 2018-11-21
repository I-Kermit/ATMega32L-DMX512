# ATMega32L-DMX512 demonstrator

The following project implements DMX512 on an ATmega32L microcontroller. It uses data received on the interrupt driven SPI interface to send to the UART. The UART output is converted to RS485 and is transmitted to a Studio Due NanoLED fixture. The SPI data is provided by a Raspberry Pi via a Python script. It is unusual to use SPI in this way and was chosen as way of demonstration.
An AVR Dragon was used to debug and flash the microcontroller. 

The library project (atmega_library) implements SPI, UART and DMX. 
The main application (dmx_app) uses the library APIs.

A demonstration is on YouTube…
https://www.youtube.com/watch?v=OgjfR1OaRTE

![alt text](https://github.com/I-Kermit/ATMega32L-DMX512/blob/master/ATMEGA_DMX.png "Schematic")
