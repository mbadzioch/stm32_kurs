/*
 * uart.h
 *
 *  Created on: 12 wrz 2016
 *      Author: Marcin
 */

#ifndef UART_H_
#define UART_H_


#define REC_BUF_SIZE 128

uint8_t uartPcBufReadyFlag;

void UartInit(void);
void PC_Debug(volatile char *s);
void Com_Get(char *s, char *flag);
void PC_Send(volatile char *s);
void PC_Get(char *s, char *flag);

#endif /* UART_H_ */
