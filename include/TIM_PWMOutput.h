/*
 * TIM_PWMOutput.h
 *
 *  Created on: 12 wrz 2017
 *      Author: Micha³
 */

#ifndef TIM_PWMOUTPUT_H_
#define TIM_PWMOUTPUT_H_
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx_conf.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
/* Private function prototypes -----------------------------------------------*/
void PWM_Config(void);
void TIM_Config(void);
/* Private functions ---------------------------------------------------------*/


#endif /* TIM_PWMOUTPUT_H_ */
