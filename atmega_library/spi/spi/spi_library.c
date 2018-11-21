/*
 * spi.c
 *
 * Created: 16/11/2018 12:40:30
 * Author : PeterGoddard
 */ 

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "spi.h"

static          uint8_t  *spi_data_p       = NULL;
static          uint16_t  spi_data_max     = 0;
static volatile uint16_t  spi_data_index   = 0;
static bool spi_interrupt_flag = false;

static void reset_data(void)
{
	spi_data_p     = NULL;
	spi_data_index = 0;
	spi_data_max   = 0;
	spi_interrupt_flag = false;
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

spi_status_enum_t spi_slave_initialise(void)
{
	spi_status_enum_t status = SPI_SUCCESS;

	/* Set MISO (PB6) output, all others input */
	DDRB = (1 << PB6);
	/* Enable SPI, SPI interrupt */
	SPCR = (1 << SPE) | (1 << SPIE);
	
	reset_data();

	return status;
}

bool spi_interrupt_fired(void)
{
	return spi_interrupt_flag;
}

void spi_interrupt_reset(void)
{
	spi_interrupt_flag = false;
}

/* Polling version */
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

/* SPI Reception ISR */
#if defined(DEBUG)
static volatile uint16_t debug_no_of_ints = 0;
static volatile uint16_t debug_no_of_spi_frames = 0;
#endif

ISR(SPI_STC_vect)
{
#if defined(DEBUG)
	debug_no_of_ints++;
#endif

	spi_data_p[spi_data_index++] = SPDR;

	if( spi_data_index == spi_data_max )
	{
		spi_data_index = 0;
		spi_interrupt_flag = true;

		#if defined(DEBUG)
			debug_no_of_spi_frames++;
		#endif
	}
}
