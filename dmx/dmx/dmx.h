/*
 * dmx.h
 *
 * Created: 05/11/2018 12:20:48
 *  Author: PeterGoddard
 */ 

#ifndef DMX_H_
#define DMX_H_

#define DMX_SIZE (512+1)

typedef enum
{
	DMX_SUCCESS,
	DMX_FAILURE,
	
} dmx_status_enum_t;

typedef void (*dmx_uart_transmit_fp)(uint8_t * data, size_t length);
typedef void (*dmx_uart_enable_fp)(void);
typedef void (*dmx_uart_disable_fp)(void);

dmx_status_enum_t dmx_initialise(dmx_uart_transmit_fp transmit_fp,
                                 dmx_uart_enable_fp   enable_fp,
								 dmx_uart_disable_fp  disable_fp);
dmx_status_enum_t dmx_load_data(uint8_t * data, size_t length);
dmx_status_enum_t dmx_send_data(void);

#endif /* DMX_H_ */
