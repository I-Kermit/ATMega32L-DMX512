/*
 * dmx_app.c
 *
 * Created: 09/11/2018 22:50:03
 * Author : PeterGoddard
 */ 

//#define USE_SOFTWARE_DELAY
#define USE_SPI_DATA

#define ITERATIONS (10)

/* ToDo: Common setting */
#define F_CPU 8000000UL

#include <assert.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "dmx.h"
#include "uart.h"
#include "spi.h"
#include "timer.h"

#if defined(USE_SPI_DATA)

static uint8_t spi_data[DMX_SIZE];

#else

/* Start word. */
#define DMX_START (0x00)

/* StudioDue NanoLed test data */
static uint8_t test_data_1[] = {DMX_START, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00};
static uint8_t test_data_2[] = {DMX_START, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00};

#endif

static dmx_status_enum_t  dmx_status;
static uart_status_enum_t uart_status;

#if !defined(USE_SOFTWARE_DELAY)
static timer_status_enum_t timer_status;
#endif

#if defined(USE_SPI_DATA)

static spi_status_enum_t  spi_status;
static uint32_t delay=0;

#else

#include <util/delay.h>
static void one_second_delay(void)
{
	_delay_ms(1000.0);
}

#endif

int main(void)
{
	dmx_status = dmx_initialise(uart_transmit, uart_enable_tx, uart_disable_tx, timer_run, timer_stop);
	assert(DMX_SUCCESS == dmx_status);

	uart_status = uart_initialise();
	assert(UART_SUCCESS == uart_status);

#if !defined(USE_SOFTWARE_DELAY)
	timer_status = timer_initialise();
	assert(TIMER_SUCCESS == timer_status);
	timer_status = timer_set_callback(dmx_timer_callback);
	assert(TIMER_SUCCESS == timer_status);
#endif

#if defined(USE_SPI_DATA)

	spi_status = spi_slave_initialise();
	assert(SPI_SUCCESS == spi_status);

	spi_initialise_data(spi_data, DMX_SIZE);

	sei();

	for(;;)
	{
		if( 0 == delay )
		{
			dmx_status = dmx_send_data();
			assert(DMX_SUCCESS == dmx_status);
		}

		if( delay >= 0x1000)
		{
			delay = 0;
		}
		else
		{
			delay++;	
		}
			
		if( spi_interrupt_fired() )
		{
			dmx_status = dmx_load_data(spi_data, DMX_SIZE);
			spi_interrupt_reset();
		}
	}
	
#else

	for(;;) 
    {
		dmx_status = dmx_load_data(test_data_1, sizeof(test_data_1));
		assert(DMX_SUCCESS == dmx_status);
		for(uint32_t iterations = 0; iterations < ITERATIONS; iterations++)
		{
			dmx_status = dmx_send_data();
			assert(DMX_SUCCESS == dmx_status);
			one_second_delay();
		}
		
		dmx_status = dmx_load_data(test_data_2, sizeof(test_data_2));
		assert(DMX_SUCCESS == dmx_status);		
		for(uint32_t iterations = 0; iterations < ITERATIONS; iterations++)
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
