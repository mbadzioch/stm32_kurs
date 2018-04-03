/*
 * uart.c
 *
 *  Created on: 12 wrz 2016
 *      Author: Marcin
 */
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx_conf.h"
#include "uart.h"


volatile uint8_t recBufA[REC_BUF_SIZE],recBufACnt=0,recBufAOVF=0,recBufB[REC_BUF_SIZE],recBufBCnt=0,recBufBOVF=0;

/*
 * Konfiguracja UART:
 *
 * 	USART1: Komunikacja między sterownikami
 *
 * 	PA: 9,10
 *
 * 	9600,8,1,0,0
 *
 * 	USART6: Debug
 *
 * 	PC: 6,7
 *
 *  115200,8,1,0,0
 */
void UartInit()
{
	USART_InitTypeDef UART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	// GPIO Config

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

	// UART Config

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	USART_StructInit(&UART_InitStructure);
	UART_InitStructure.USART_BaudRate = 115200;
	UART_InitStructure.USART_WordLength = USART_WordLength_8b;
	UART_InitStructure.USART_StopBits = USART_StopBits_1;
	UART_InitStructure.USART_Parity = USART_Parity_No;
	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART_InitStructure.USART_Mode = USART_Mode_Tx;

	USART_Init(USART2,&UART_InitStructure);
	USART_Cmd(USART2, ENABLE);

	USART_StructInit(&UART_InitStructure);
	UART_InitStructure.USART_BaudRate = 115200;
	UART_InitStructure.USART_WordLength = USART_WordLength_8b;
	UART_InitStructure.USART_StopBits = USART_StopBits_1;
	UART_InitStructure.USART_Parity = USART_Parity_No;
	UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART6,&UART_InitStructure);
	USART_Cmd(USART6, ENABLE);


	// NVIC Config

	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

 	NVIC_Init(&NVIC_InitStructure);
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

    uartPcBufReadyFlag=0;
}
/*
 * 	Nadawanie przez UART
 */
void PC_Debug(volatile char *s)
{
	while(*s){
	 // wait until data register is empty
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET); // Wait for Empty

		    USART_SendData(USART2, *s++); // Send Char
	 }
}
void PC_Send(volatile char *s)
{
	while(*s){
	 // wait until data register is empty
		while(USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET); // Wait for Empty

		    USART_SendData(USART6, *s++); // Send Char
	 }
}
/*
 * Odbiór z UART
 */

void Com_Get(char *s, char *flag)
{
	for(uint8_t i=0;i<REC_BUF_SIZE;i++){
		*s++=recBufA[i];
	}
	*flag = recBufAOVF;
	recBufACnt=0;
	recBufAOVF=0;
}
void PC_Get(char *s, char *flag)
{
	for(uint8_t i=0;i<recBufBCnt;i++){
		*s++=recBufB[i];
	}
	*flag = recBufBOVF;
	recBufBCnt=0;
	recBufBOVF=0;
}
/*
 * Com:
 */
void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET){
		if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE)){
			if(recBufACnt < REC_BUF_SIZE){
				recBufA[recBufACnt] = USART_ReceiveData(USART1);
				recBufACnt++;
			}
			else{
				//USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
				recBufACnt=0;
				recBufAOVF=1;
			}
		}
	}
}
/*
 * PC:
 */
void USART6_IRQHandler(void)
{
	if (USART_GetITStatus(USART6, USART_IT_RXNE) == SET){
		if (USART_GetFlagStatus(USART6, USART_FLAG_RXNE)){
			if(recBufBCnt < REC_BUF_SIZE){
				recBufB[recBufBCnt] = USART_ReceiveData(USART6);
				if(recBufB[recBufBCnt] == '\r'){
					uartPcBufReadyFlag=recBufBCnt;
				}
				recBufBCnt++;
			}
			else{
				//USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);
				recBufBCnt=0;
				recBufBOVF=1;
			}
		}
	}
}
