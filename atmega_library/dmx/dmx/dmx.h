/*
 * dmx.h
 *
 * Created: 05/11/2018 12:20:48
 *  Author: PeterGoddard
 */ 

#ifndef DMX_H_
#define DMX_H_

//#include <stdbool.h>

// ToDo: Look at this
//#include "timer.h"

#define DMX_SIZE (512+1)

typedef enum
{
	DMX_SUCCESS,
	DMX_FAILURE,
	
} dmx_status_enum_t;

typedef void (*dmx_uart_transmit_fp)(uint8_t * data, size_t length);
typedef void (*dmx_uart_enable_fp)(void);
typedef void (*dmx_uart_disable_fp)(void);
// ToDo: Look at this
#include "../../timer/timer.h"
typedef timer_status_enum_t (*dmx_timer_run_fp)(uint16_t ticks);
typedef timer_status_enum_t (*dmx_timer_stop_fp)(void);

/* WIP */
void dmx_timer_callback(void);

dmx_status_enum_t dmx_initialise(dmx_uart_transmit_fp transmit_fp,
                                 dmx_uart_enable_fp   enable_fp,
								 dmx_uart_disable_fp  disable_fp,
								 dmx_timer_run_fp     timer_run_fp,
								 dmx_timer_stop_fp    timer_stop_fp);
dmx_status_enum_t dmx_load_data(uint8_t * data, size_t length);
dmx_status_enum_t dmx_send_data(void);

#endif /* DMX_H_ */
