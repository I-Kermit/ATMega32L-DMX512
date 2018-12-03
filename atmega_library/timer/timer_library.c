/*
 * timer.c
 *
 * Created: 01/12/2018 09:20:09
 * Author : PeterGoddard
 */ 

#include <assert.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/atomic.h>

#include "timer.h"

static timer_handler timer_callback = NULL;

/* Internal functions */
#if 0
/* Timer 1 count register 16 bit */
inline static uint16_t timer_read_16bit(void)
{
	uint16_t timer_tcnt1 = 0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		timer_tcnt1 = TCNT1;
	}

	return timer_tcnt1;
}

/* Timer 1 count register 16 bit */
inline static void timer_write_16bit(uint16_t timer_tcnt1)
{	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		TCNT1 = timer_tcnt1;
	}
}
#endif

/* API Functions */

timer_status_enum_t timer_initialise(void)
{
	timer_callback = NULL;

	/* Clear interrupt */
	TIFR  |= (1 << TOV1);

	return TIMER_SUCCESS;
}

timer_status_enum_t timer_set_callback(timer_handler callback)
{
	timer_status_enum_t status = TIMER_SUCCESS;

	assert(NULL != callback);

	if( NULL == callback)
	{
		status = TIMER_FAILURE;
	}
	else
	{
		timer_callback = callback;
	}
		
	return status;
}

timer_status_enum_t timer_run(uint16_t ticks)
{
	timer_status_enum_t status = TIMER_SUCCESS;

	assert(NULL != timer_callback);

	if( NULL == timer_callback)
	{
		status = TIMER_FAILURE;
	}
	else
	{
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			TCCR1A = 0;
			/*
			** CS12 CS11 CS10 Description
			** 0 0 0 No clock source (Timer/Counter stopped).
			** 0 0 1 clkI/O/1 (No pre-scaling)
			** 0 1 0 clkI/O/8 (From pre-scaler)
			** 0 1 1 clkI/O/64 (From pre-scaler)
			** 1 0 0 clkI/O/256 (From pre-scaler)
			** 1 0 1 clkI/O/1024 (From pre-scaler)
			** 1 1 0 External clock source on T1 pin. Clock on falling edge.
			** 1 1 1 External clock source on T1 pin. Clock on rising edge.
			*/
			TCCR1B = (0 << CS12) | (1 << CS11) | (1 << CS10);
			/* Timer counter */
			TCNT1  = ticks;
			TIMSK |= (1 << TOIE1);
		}
	}
	
	return status;
}

timer_status_enum_t timer_stop(void)
{
	TIMSK &= ~(1 << TOIE1);
	
	return TIMER_SUCCESS;
}

#if defined(DEBUG)
static volatile uint16_t debug_no_of_ticks = 0;
#endif

ISR(TIMER1_OVF_vect)
{
	/* Interrupt cleared automatically */

#if defined(DEBUG)
	debug_no_of_ticks++;
#endif

	assert(NULL != timer_callback);
	
	timer_callback();
}
