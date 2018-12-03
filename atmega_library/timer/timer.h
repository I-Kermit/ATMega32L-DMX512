/*
 * timer.h
 *
 * Created: 01/12/2018 09:20:46
 *  Author: PeterGoddard
 */ 


#ifndef TIMER_H_
#define TIMER_H_

typedef enum
{
	TIMER_SUCCESS,
	TIMER_FAILURE
}
timer_status_enum_t;

typedef void (*timer_handler)(void);

timer_status_enum_t timer_initialise(void);
timer_status_enum_t timer_set_callback(timer_handler callback);
timer_status_enum_t timer_run(uint16_t ticks);
timer_status_enum_t timer_stop(void);

#endif /* TIMER_H_ */
