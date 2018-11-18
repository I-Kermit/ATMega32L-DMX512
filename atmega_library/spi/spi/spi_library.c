/*
 * spi.c
 *
 * Created: 16/11/2018 12:40:30
 * Author : PeterGoddard
 */ 

#include <assert.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "spi.h"

static uint8_t  *spi_data_p     = NULL;
static uint16_t  spi_data_max   = 0;
static uint16_t  spi_data_index = 0;
spi_buffer_full_fp buffer_full_callback_fp = NULL;

static void reset_data(void)
{
	spi_data_p     = NULL;
	spi_data_index = 0;
	spi_data_max   = 0;
}

/* API functions */

void spi_initialise_data(uint8_t *data_p, uint16_t data_max)
{
	spi_data_p   = data_p;
	spi_data_max = data_max;
}

/*
 * n.b.
 * PB7 SCK  (SPI Bus Serial Clock)
 * PB6 MISO (SPI Bus Master Input/Slave Output)
 * PB5 MOSI (SPI Bus Master Output/Slave Input)
 * PB4 SS   (SPI Slave Select Input)
*/

spi_status_enum_t spi_slave_initialise(spi_buffer_full_fp buffer_full_fp)
{
	spi_status_enum_t status = SPI_SUCCESS;

SPDR |= 0;

	/* Set MISO (PB6) output, all others input */
	DDRB = (1 << PB6);
	/* Enable SPI, SPI interrupt */
	SPCR = (1 << SPE) | (1 << SPIE);
	
	reset_data();

	assert(NULL != buffer_full_fp);
	
	if( NULL != buffer_full_fp )
	{
		buffer_full_callback_fp = buffer_full_fp;
	}
	else
	{
		status = SPI_FAILURE;
	}

	return status;
}

/* Polling version */
#if 0
void spi_slave_receive(uint8_t *data, size_t length)
{
	for(uint16_t loop=0; loop<length; loop++)
	{
		/* Wait for reception to complete */
		while(!(SPSR & (1 << SPIF)))
		{
			/* Deliberately empty */	
		}
			
		data[loop] = SPDR;
	}
}
#endif

/* SPI Transmission/reception ISR */
static volatile uint16_t no_of_ints;
static volatile uint8_t dummy_read;
ISR(SPI_STC_vect)
{
	volatile static uint8_t spi_in = 0;
	spi_in = SPDR;
dummy_read = SPSR;
no_of_ints++;

	if( spi_data_index < spi_data_max )
	{
		spi_data_p[spi_data_index++] = spi_in; //SPDR;
	}
	else
	{
		spi_data_index = 0;
		// ToDo: Find out whether this takes too long.
	}
}
