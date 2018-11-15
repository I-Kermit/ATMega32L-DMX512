/*
 * dmx_library.c
 *
 * Created: 05/11/2018 12:23:26
 *  Author: PeterGoddard
 */ 

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include <util/atomic.h>

#include "dmx.h"

static uint8_t  dmx_data_buffer_1[DMX_SIZE];
static uint8_t  dmx_data_buffer_2[DMX_SIZE];
static uint8_t *dmx_data_in_use_p = NULL;

static dmx_uart_transmit_fp uart_transmit_fp = NULL;
static dmx_uart_disable_fp  uart_disable_fp  = NULL;
static dmx_uart_enable_fp   uart_enable_fp   = NULL;

/* Internal */

/* ToDo: Common setting */
#define F_CPU 8000000
#include <util/delay.h>
#define DMX_TX_PIN (PD1)

inline static void initialise_output(void)
{
	/* Set as output */
	DDRD |= (1 << DMX_TX_PIN);
}

inline static void add_break_and_mab(void)
{
	/* PD1 == TXD */

	/* HIGH */
	PORTD |= (1 << DMX_TX_PIN);
	uart_disable_fp();	
	/* LOW */
	PORTD &= ~(1 << DMX_TX_PIN);
	_delay_us(100);
	/* HIGH */
	PORTD |= (1 << DMX_TX_PIN);
	_delay_us(12);
	/* LOW */
	PORTD &= ~(1 << DMX_TX_PIN);
}

inline static void add_idle(void)
{
	/* HIGH */
	PORTD |= (1 << DMX_TX_PIN);
	uart_disable_fp();	
}

/* API functions */

dmx_status_enum_t dmx_initialise(dmx_uart_transmit_fp transmit_fp,
                                 dmx_uart_enable_fp   enable_fp,
								 dmx_uart_disable_fp  disable_fp)
{
	dmx_status_enum_t status = DMX_SUCCESS;

	#if 0
	/* Clear the data out. */
	uint16_t loop = DMX_SIZE;

	while(loop--)
	{
		dmx_data_buffer_1[loop] = 0;
		dmx_data_buffer_2[loop] = 0;
	}

	#else

	for(uint16_t loop=0; loop < DMX_SIZE; loop++)
	{
		dmx_data_buffer_1[loop] = 0;
		dmx_data_buffer_2[loop] = 0;
	}
	#endif

	dmx_data_in_use_p = dmx_data_buffer_2;

	assert(NULL != transmit_fp);
	assert(NULL != disable_fp);
	assert(NULL != enable_fp);
	
	if( NULL != transmit_fp || NULL != disable_fp || NULL != enable_fp)
	{
		uart_transmit_fp = transmit_fp;
		uart_disable_fp  = disable_fp;
		uart_enable_fp   = enable_fp;
	}
	else
	{
		status = DMX_FAILURE;
	}

	initialise_output();

	return status;
}

dmx_status_enum_t dmx_load_data(uint8_t * data, size_t length)
{
	assert(DMX_SIZE > length);

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		uint16_t loop = length;
		uint8_t *external_p = data;
		uint8_t *internal_p = NULL;

		if( dmx_data_in_use_p == dmx_data_buffer_1 )
		{
			internal_p = dmx_data_buffer_2;
			dmx_data_in_use_p = dmx_data_buffer_2;
		}
		else
		{
			internal_p = dmx_data_buffer_1;
			dmx_data_in_use_p = dmx_data_buffer_1;
		}

		while(loop--)
		{
			*internal_p++ = *external_p++;
		}
			
		loop = DMX_SIZE - length;

		while(loop--)
		{
			*internal_p++ = 0;
		}
	}

	return DMX_SUCCESS;
}

dmx_status_enum_t dmx_send_data(void)
{
	add_break_and_mab();
	uart_enable_fp();
	uart_transmit_fp(dmx_data_in_use_p, DMX_SIZE);
	add_idle();

	return DMX_SUCCESS;
}
