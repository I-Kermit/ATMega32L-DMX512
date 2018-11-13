/*
 * uart.c
 *
 * Created: 06/11/2018 16:39:01
 * Author : PeterGoddard
 */ 

#include <stdio.h>
#include <avr/io.h>

#include "uart.h"

/* ToDo: Common setting */
#define F_CPU 8000000

/* API functions */

inline void uart_enable_tx(void)
{
	UCSRB |= (1 << TXEN);	
}

inline void uart_disable_tx(void)
{
	UCSRB &= ~(1 << TXEN);
}

uart_status_enum_t uart_initialisation(void)
{
	#undef BAUD  // avoid compiler warning
	#define BAUD 250000
	#include <util/setbaud.h>
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	#if USE_2X
	UCSRA |= (1 << U2X);
	#else
	UCSRA &= ~(1 << U2X);
	#endif
	
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL) | (1<<USBS) | (3<<UCSZ0);

	return UART_SUCCESS;
}

/* API - UART Tx function */
/* Polling version */
void uart_transmit(uint8_t * data, size_t length)
{
	uart_enable_tx();

	for(uint16_t loop=0; loop<length; loop++)
	{
		/* Wait for empty transmit buffer */
		while ( !( UCSRA & (1<<UDRE)) )
		;

		/* Put data into buffer, sends the data */
		UDR = data[loop];
	}
}
