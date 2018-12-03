/*
 * dmx_library.c
 *
 * Created: 05/11/2018 12:23:26
 *  Author: PeterGoddard
 */ 

//#define USE_SOFTWARE_DELAY

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <util/atomic.h>

#include "dmx.h"

#define DMX_TX_PIN (PD1)

static uint8_t  dmx_data_buffer_1[DMX_SIZE];
static uint8_t  dmx_data_buffer_2[DMX_SIZE];
static uint8_t *dmx_data_in_use_p = NULL;

static dmx_uart_transmit_fp uart_transmit_fp = NULL;
static dmx_uart_disable_fp  uart_disable_fp  = NULL;
static dmx_uart_enable_fp   uart_enable_fp   = NULL;
static dmx_timer_run_fp     timer_run_fp     = NULL;
static dmx_timer_stop_fp    timer_stop_fp    = NULL;

#if defined(USE_SOFTWARE_DELAY)
/* ToDo: Common setting */
#define F_CPU 8000000UL
#include <util/delay.h>

#else

static volatile bool timer_callback_fired_flag = false;

static inline void poll_timer(void)
{
	/* Trial this for now */
	while(false == timer_callback_fired_flag)
	{
		/* Do nothing */
	}
	
	timer_callback_fired_flag = false;
}

/*
 * Fy = F_CPU/(prescaler * (timer + 1)
 * The timer counts up from here.
 */
#define _100US_DELAY (0xFFFF - 799)
#define _12US_DELAY  (0xFFFF - 95)

#endif


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

#if defined(USE_SOFTWARE_DELAY)
	_delay_us(100);
#else
	/* Trial this for now */
	timer_run_fp(_100US_DELAY);
	poll_timer();
#endif

	/* HIGH */
	PORTD |= (1 << DMX_TX_PIN);

#if defined(USE_SOFTWARE_DELAY)
	_delay_us(12);
#else
	/* Trial this for now */
	timer_run_fp(_12US_DELAY);
	poll_timer();
#endif

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
								 dmx_uart_disable_fp  disable_fp,
								 dmx_timer_run_fp     run_fp,
								 dmx_timer_stop_fp    stop_fp)
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
	assert(NULL != run_fp);
	assert(NULL != stop_fp);
	
	if( NULL != transmit_fp || NULL != disable_fp || NULL != enable_fp || NULL != run_fp || NULL != stop_fp)
	{
		uart_transmit_fp = transmit_fp;
		uart_disable_fp  = disable_fp;
		uart_enable_fp   = enable_fp;
		timer_run_fp     = run_fp;
		timer_stop_fp    = stop_fp;
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
	assert(DMX_SIZE >= length);

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

#if !defined(USE_SOFTWARE_DELAY)
void dmx_timer_callback(void)
{
	timer_stop_fp();
	timer_callback_fired_flag = true;
}
#endif
