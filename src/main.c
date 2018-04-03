
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f4xx_conf.h"
#include "main_set.h"
#include "delay.h"
#include "timer.h"
#include "kom.h"
#include "spi.h"
#include "TIM_PWMOutput.h"
#include "tm_stm32f4_usb_vcp.h"
#include "main.h"
#include "sinus.h"
/* kontroler PHY jest wewnêtrzny i zewnêtrzny, stosuje siê zabezpieczenia w postaci diod TVS (clamp diodes)
 * w stm zewnêtrzny PHY jest na interfejsie ULPI po to by uzyskac USB High Speed 60MB/s*/

/* STM USB Stack, posiada stos kolejno od do³u Obs³uga PHY/USB CORE/Klasa np*/

int8_t datax, datay, dataz;
char BUFOR [50];


//const uint16_t Sine12bit[64] = {
//		2048,2248,2447,2642,2831,3013,3185,3346,3495,3630,3750,3853,3939,4007,4056,4085,4095,4085,4056,4007,3939,3853,3750,3630,3495,3346,3185,3013,2831,2642,2447,2248,
//		2048,1847,1648,1453,1264,1082,910,749,600,465,345,242,156,88,39,10,0,10,39,88,156,242,345,465,600,749,910,1082,1264,1453,1648,1847};

int main(void)
{
	SystemInit();

	fir_8 filt;
	uint8_t cnt=0;
	uint16_t delay=0;
	//enables GPIO clock for PortD
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	codec_init();
	codec_ctrl_init();

	I2S_Cmd(CODEC_I2S, ENABLE);

	initFilter(&filt);
	SIN_Config();

    while(1)
    {

    	if (SPI_I2S_GetFlagStatus(CODEC_I2S, SPI_I2S_FLAG_TXE))
    	{
    		SPI_I2S_SendData(CODEC_I2S, sample);

    		//only update on every second sample to insure that L & R ch. have the same sample value
    		if (sampleCounter & 0x00000001)
    		{
    			sawWave += NOTEFREQUENCY;
    			if (sawWave > 1.0)
    				sawWave -= 2.0;

    			filteredSaw = updateFilter(&filt, sawWave);

    			sample = (int16_t)(NOTEAMPLITUDE*filteredSaw);
    		}
    		sampleCounter++;
    	}

    	if (sampleCounter==48000)
    	{
    		LED_BLUE_OFF;

    	}
    	else if (sampleCounter == 96000)
    	{
    		LED_BLUE_ON;
    		sampleCounter = 0;
    	}

    }
}

// a very crude FIR lowpass filter
float updateFilter(fir_8* filt, float val)
{
	uint16_t valIndex;
	uint16_t paramIndex;
	float outval = 0.0;

	valIndex = filt->currIndex;
	filt->tabs[valIndex] = val;

	for (paramIndex=0; paramIndex<8; paramIndex++)
	{
		outval += (filt->params[paramIndex]) * (filt->tabs[(valIndex+paramIndex)&0x07]);
	}

	valIndex++;
	valIndex &= 0x07;

	filt->currIndex = valIndex;

	return outval;
}

void initFilter(fir_8* theFilter)
{
	uint8_t i;

	theFilter->currIndex = 0;

	for (i=0; i<8; i++)
		theFilter->tabs[i] = 0.0;

	theFilter->params[0] = 0.01;
	theFilter->params[1] = 0.05;
	theFilter->params[2] = 0.12;
	theFilter->params[3] = 0.32;
	theFilter->params[4] = 0.32;
	theFilter->params[5] = 0.12;
	theFilter->params[6] = 0.05;
	theFilter->params[7] = 0.01;
}
