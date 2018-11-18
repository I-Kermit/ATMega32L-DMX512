/*
 * dmx_app.c
 *
 * Created: 09/11/2018 22:50:03
 * Author : PeterGoddard
 */ 

#define USE_SPI_DATA

/* ToDo: Common setting */
#define F_CPU 8000000UL

#include <assert.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "dmx.h"
#include "uart.h"
#include "spi.h"

#define DMX_START (0x00) // Start word.

#if defined(USE_SPI_DATA)
static uint8_t spi_data[DMX_SIZE];

volatile static uint16_t no_of_callbacks;
static void spi_buffer_full(void)
{
no_of_callbacks++;

	dmx_status_enum_t dmx_status = DMX_FAILURE;
	/* Only 13 for now */
	dmx_status = dmx_load_data(spi_data, 13);
	assert(DMX_SUCCESS == dmx_status);
}

static spi_buffer_full_fp buffer_full_fp = spi_buffer_full;
#else
/* StudioDue NanoLed test data */
static uint8_t test_data_1[] = {DMX_START, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00};
static uint8_t test_data_2[] = {DMX_START, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00};
#endif

static dmx_uart_transmit_fp uart_transmit_fp = uart_transmit;
static dmx_uart_disable_fp  uart_disable_fp  = uart_disable_tx;
static dmx_uart_enable_fp   uart_enable_fp   = uart_enable_tx;
static dmx_status_enum_t  dmx_status;
static uart_status_enum_t uart_status;
static spi_status_enum_t  spi_status;

static void one_second_delay(void)
{
#if 0
	_delay_ms(1000.0);
#else
	//volatile static uint64_t delay;

	for(volatile uint64_t delay=0UL; delay<0x10000UL; delay++)
	{
		/* EMPTY */
	}
#endif
}

int main(void)
{
	dmx_status = dmx_initialise(uart_transmit_fp, uart_enable_fp, uart_disable_fp);
	assert(DMX_SUCCESS == dmx_status);

	uart_status = uart_initialise();
	assert(UART_SUCCESS == uart_status);

#if defined(USE_SPI_DATA)

	spi_status = spi_slave_initialise(buffer_full_fp);
	assert(SPI_SUCCESS == spi_status);

	spi_initialise_data(spi_data, 13 /*DMX_SIZE*/);


#if 0
for(volatile uint8_t l=0;l<13;l++)
{
	//SPDR = 0x99;
	while(!(SPSR & (1 << SPIF)));

	spi_data[l] = SPDR;
}

for(volatile uint8_t l=100;l<113;l++)
{
	//SPDR = 0x99;
	while(!(SPSR & (1 << SPIF)));

	spi_data[l] = SPDR;
}


while(1)
{
	
}
#endif


	sei();

    while(1)
    {
		if( 1 )
		{
			dmx_status = dmx_send_data();
			assert(DMX_SUCCESS == dmx_status);
			one_second_delay();
		}
	}
	
#else

    while (1) 
    {
		//uint32_t iterations = 0;

		dmx_status = dmx_load_data(test_data_1, sizeof(test_data_1));
		assert(DMX_SUCCESS == dmx_status);
		for(uint32_t iterations = 0; iterations < 10; iterations++)
		{
			dmx_status = dmx_send_data();
			assert(DMX_SUCCESS == dmx_status);
			one_second_delay();
		}
		
		dmx_status = dmx_load_data(test_data_2, sizeof(test_data_2));
		assert(DMX_SUCCESS == dmx_status);		
		for(uint32_t iterations = 0; iterations < 10; iterations++)
		{
			dmx_status = dmx_send_data();
			assert(DMX_SUCCESS == dmx_status);
			one_second_delay();		
		}
    }

#endif

	/* Should never get here! */
	return 0;
}
