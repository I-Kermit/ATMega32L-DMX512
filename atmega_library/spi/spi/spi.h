/*
 * spi.h
 *
 * Created: 16/11/2018 12:42:47
 *  Author: PeterGoddard
 */ 


#ifndef SPI_H_
#define SPI_H_

typedef enum
{
	SPI_SUCCESS,
	SPI_FAILURE
}
spi_status_enum_t;

spi_status_enum_t spi_initialisation(void);
void spi_receive(uint8_t * data, size_t length);

#endif /* SPI_H_ */