/*
 * spi.c
 *
 *  Created on: 13 wrz 2017
 *      Author: Micha³
 */

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx_conf.h"
#include "spi.h"


static void	SpiGPIO_Init();
static void	Spi_Init();
static void Akcel_Init();
//static void SPI_Config(void);

void Acc_Init(void){
	SpiGPIO_Init();
	Spi_Init();
	Akcel_Init();
}
int8_t Spi_GetReg(uint8_t reg)
{
	uint16_t data;

	data=(reg|0x80)<<8;

    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, data);
//	    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==0);
//	    SPI_I2S_ReceiveData(SPI1);
//	    SPI_I2S_SendData(SPI1, 0xff);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    data = SPI_I2S_ReceiveData(SPI1);
    GPIO_SetBits(GPIOE, GPIO_Pin_3);

    return data;
}
void Spi_Send(uint8_t reg, uint8_t data)
{
	uint16_t temp;

	temp = reg;
	temp = temp << 8;
	temp |= data;
	temp &= ~0x8000;

    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPI1, temp);
//	    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==0);
//	    SPI_I2S_ReceiveData(SPI1);
//	    SPI_I2S_SendData(SPI1, 0xff);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(SPI1);
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
}
void Spi_Read(int8_t* x, int8_t* y, int8_t* z){


	*x = Spi_GetReg(0x29);
	*y = Spi_GetReg(0x2B);
	*z = Spi_GetReg(0x2D);
}

static void	SpiGPIO_Init(){

	GPIO_InitTypeDef  GPIO_InitStructure;

	  /* GPIOG Peripheral clock enable */
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	  /* Configure PG6 and PG8 in output pushpull mode */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5  | GPIO_Pin_6 | GPIO_Pin_7;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
	  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

	  /* GPIOG Peripheral clock enable */
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	  /* Configure PG6 and PG8 in output pushpull mode */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(GPIOE, &GPIO_InitStructure);

	  GPIO_SetBits(GPIOE, GPIO_Pin_3);
}

static void	Spi_Init(){

		SPI_InitTypeDef  SPI_InitStructure;
//	  NVIC_InitTypeDef NVIC_InitStructure;

	  /* Peripheral Clock Enable -------------------------------------------------*/
	  /* Enable the SPI clock */

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

	  /* SPI configuration -------------------------------------------------------*/
	  SPI_I2S_DeInit(SPI1);
	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	  SPI_InitStructure.SPI_CRCPolynomial = 7;


  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable the SPI peripheral */
  SPI_Cmd(SPI1, ENABLE);

}

static void Akcel_Init(){
	Spi_Send(0x20,0x47);

}
//    aRxBuffer[ubRxIndex++] = SPI_I2S_ReceiveData(SPIx);

//    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
//    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//    SPI_I2S_SendData(SPI1, 0x2047);
////	    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==0);
////	    SPI_I2S_ReceiveData(SPI1);
////	    SPI_I2S_SendData(SPI1, 0xff);
//    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//    *x = SPI_I2S_ReceiveData(SPI1);
//    GPIO_SetBits(GPIOE, GPIO_Pin_3);
//
//
//
//    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
//    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//    SPI_I2S_SendData(SPI1, 0xA900);
////	    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==0);
////	    SPI_I2S_ReceiveData(SPI1);
////	    SPI_I2S_SendData(SPI1, 0xff);
//    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//    *x = SPI_I2S_ReceiveData(SPI1);
//    GPIO_SetBits(GPIOE, GPIO_Pin_3);
//
//
//
//    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
//    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//    SPI_I2S_SendData(SPI1, 0xAB00);
////	    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==0);
////	    SPI_I2S_ReceiveData(SPI1);
////	    SPI_I2S_SendData(SPI1, 0xff);
//    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//    *y = SPI_I2S_ReceiveData(SPI1);
//    GPIO_SetBits(GPIOE, GPIO_Pin_3);
//
//
//
//
//    GPIO_ResetBits(GPIOE, GPIO_Pin_3);
//    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//    SPI_I2S_SendData(SPI1, 0xAD00);
////	    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==0);
////	    SPI_I2S_ReceiveData(SPI1);
////	    SPI_I2S_SendData(SPI1, 0xff);
//    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//    *z = SPI_I2S_ReceiveData(SPI1);
//    GPIO_SetBits(GPIOE, GPIO_Pin_3);

//	    uint8_t spi_sendrecv(uint8_t byte)
//	    {
//	     // poczekaj az bufor nadawczy bedzie wolny
//	     while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
//	     SPI_I2S_SendData(SPI1, byte);
//
//	     // poczekaj na dane w buforze odbiorczym
//	     while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
//	     return SPI_I2S_ReceiveData(SPI1);
//	    }


