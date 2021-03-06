/*
 * spi.h
 *
 * Created: 16/11/2018 12:42:47
 *  Author: PeterGoddard
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdbool.h>

typedef enum
{
	SPI_SUCCESS,
	SPI_FAILURE
}
spi_status_enum_t;

spi_status_enum_t spi_slave_initialise(void);
bool spi_interrupt_fired(void);
void spi_interrupt_reset(void);
void spi_initialise_data(uint8_t *data_p, uint16_t data_max);
void spi_slave_receive(uint8_t * data, size_t length);

#endif /* SPI_H_ */
