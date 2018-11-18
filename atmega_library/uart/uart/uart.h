/*
 * IncFile1.h
 *
 * Created: 12/11/2018 15:20:33
 *  Author: PeterGoddard
 */ 


#ifndef UART_H_
#define UART_H_

typedef enum
{
	UART_SUCCESS,
	UART_FAILURE	
}
uart_status_enum_t;

uart_status_enum_t uart_initialise(void);
void uart_enable_tx(void);
void uart_disable_tx(void);
void uart_transmit(uint8_t * data, size_t length);

#endif /* UART_H_ */
