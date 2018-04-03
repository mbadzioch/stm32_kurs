/*
 * main_set.h
 *
 *  Created on: 21 sty 2017
 *      Author: Marcin
 */

#ifndef MAIN_SET_H_
#define MAIN_SET_H_


#define UART_DEBUG 1

/*
 *	Timery HW
 *
 *	Wykorzystano:
 *					- TIM2 w module timer
 */


typedef struct{
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}rtc;

#endif /* MAIN_SET_H_ */
