#include "delay.h"




void Delay_Init(void){
	if (SysTick_Config(SystemCoreClock / 1000)){
		while (1);
	}
}

void TimingDelay_Decrement(void){
	if (TimingDelay != 0){
		TimingDelay--;
	}
}

void Delay_ms(__IO uint32_t nTime){
	TimingDelay = nTime;

	while(TimingDelay != 0);
}


void SysTick_Handler(void){
  TimingDelay_Decrement();
}
