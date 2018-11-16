/*
 * dmx_app.c
 *
 * Created: 09/11/2018 22:50:03
 * Author : PeterGoddard
 */ 

/* ToDo: Common setting */
#define F_CPU 8000000

#include <assert.h>
#include <avr/io.h>
#include <util/delay.h>

#include "dmx.h"
#include "uart.h"
#include "spi.h"

#define DMX_START (0x00) // Start word.

static uint8_t test_data_1[] = {DMX_START, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00};
static uint8_t test_data_2[] = {DMX_START, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00};

static dmx_uart_transmit_fp uart_transmit_fp = uart_transmit;
static dmx_uart_disable_fp  uart_disable_fp  = uart_disable_tx;
static dmx_uart_enable_fp   uart_enable_fp   = uart_enable_tx;
static dmx_status_enum_t  dmx_status;
static uart_status_enum_t uart_status;

static void one_second_delay(void)
{
	_delay_ms(1000.0);
}

int main(void)
{
	uart_status = uart_initialisation();
	assert(UART_SUCCESS == uart_status);

	dmx_status = dmx_initialise(uart_transmit_fp, uart_enable_fp, uart_disable_fp);
	assert(DMX_SUCCESS == dmx_status);

    while (1) 
    {
		uint32_t iterations = 0;

		dmx_status = dmx_load_data(test_data_1, sizeof(test_data_1));
		assert(DMX_SUCCESS == dmx_status);
		for(iterations = 0; iterations < 10; iterations++)
		{
			dmx_status = dmx_send_data();
			assert(DMX_SUCCESS == dmx_status);
			one_second_delay();
		}
		
		dmx_status = dmx_load_data(test_data_2, sizeof(test_data_2));
		assert(DMX_SUCCESS == dmx_status);		
		for(iterations = 0; iterations < 10; iterations++)
		{
			dmx_status = dmx_send_data();
			assert(DMX_SUCCESS == dmx_status);
			one_second_delay();		
		}
    }
	
	/* Should never get here! */
	return 0;
}
