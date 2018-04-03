/*
 * kom.c
 *
 *  Created on: 29 cze 2017
 *      Author: Marcin
 */


#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx_conf.h"
#include "main_set.h"
#include "delay.h"
#include "timer.h"
#include "tm_stm32f4_usb_vcp.h"
#include "kom.h"
#include "my_strings.h"
#include "recorder.h"


//----------------------------------------------------------------------------------------------------------------------

const char* commands [] = { "help" , "IDN?" , "CNT?" , "REC?" , "REC_NEW?" , "SET_HV?" , "SET_HV" , "SET_REF?" ,
							"SET_REF" , "SHUTTER?" , "SHUTTER" , "PHOTO?" , "PHOTO" , "SOUND?" , "SOUND" ,
							"START" , "STOP" , "STATE?" , "DATE" , "DATE?" , "WINDOW?" , "WINDOW" , "ALEDON", "ALEDOFF"
			  };
const int command_count = 24;
extern uint8_t USB_Tx_State;
const char* IDN_string = "Bioluminescencja v1.0\r\n";

uint8_t testbuf[1024];
//----------------------------------------------------------------------------------------------------------------------

#define COMMAND_BUF_LEN 64
char command_buf [COMMAND_BUF_LEN];

//----------------------------------------------------------------------------------------------------------------------
void process_command ();


/*
 * green pd12
 *
 * red pd14
 *
 */
void led()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_14 | GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}
void Led_Green(uint8_t state)
{
	if(!state){
		GPIO_ResetBits(GPIOD,GPIO_Pin_12);
	}
	else{
		GPIO_SetBits(GPIOD,GPIO_Pin_12);
	}
}
void Led_Red(uint8_t state)
{
	if(!state){
		GPIO_ResetBits(GPIOD,GPIO_Pin_14);
	}
	else{
		GPIO_SetBits(GPIOD,GPIO_Pin_14);
	}
}
void USART1_send_string(char *s)
{
	TM_USB_VCP_Puts(s);
}

void Kom_Init(void)
{
	for(uint16_t i=0;i<1000;i++){
		testbuf[i]=(i%10)+'0';
	}
	testbuf[998]='\r';
	testbuf[999]='\n';
	led();
	TM_USB_VCP_Init();
	Led_Green(1);
}

void Test(void){
	uint16_t i = 5;
	char BUFOR [50];
	sprintf(BUFOR, "moja zmienna to: %d \r\n", i);
	TM_USB_VCP_Puts(BUFOR);

}

void Kom_Main(void)
{
	static TM_USB_VCP_Result lastStat,actStat;
	uint8_t i;
	uint16_t strLen;

	strLen=TM_USB_VCP_Gets(command_buf,COMMAND_BUF_LEN);
	if(strLen>0){
		process_command();

	    for (i = 0 ; i < COMMAND_BUF_LEN ; i ++) command_buf [i] = 0;
	}

	actStat=TM_USB_VCP_GetStatus();
	if (actStat != lastStat){
		if(actStat == TM_USB_VCP_CONNECTED){
			Led_Red(0);
			Led_Green(1);
		}
		else{
			Led_Green(0);
			Led_Red(1);
		}
		lastStat=actStat;
	}

}
void process_command () {
    int i , command_num = -1;
    char t_s[64];
    uint32_t t_i;

    for (i = 0 ; i < command_count ; i++)
	if ( my_strcmp ((char*)command_buf , (char*) commands [i]) ) {
	    command_num = i;
	    break;
	}

    switch (command_num) {
	case 0: { // command help
	    for (i = 0 ; i < command_count ; i++) {
		USART1_send_string ((char*)commands[i]);
		USART1_send_string ("\r\n");
	    }
	};break;

	case 1 : { // command IDN
		USART1_send_string ((char*)IDN_string);
	};break;

	case 2: { // command CNT?
		my_itoa(settings.cnt,t_s);
		USART1_send_string(t_s);
		USART1_send_string ("\r\n");
	};break;

	case 3: { // command REC?
		//while(Rec_GetRecords(t_s,0)==1)USART1_send_string(t_s);
		USART1_send_string ("21-13-04-32-123 1,21-13-05-12-032 5");
		USART1_send_string ("\r\n");
	};break;

	case 4: { // command REC_NEW?
		//while(Rec_GetRecords(t_s,1)==1)USART1_send_string(t_s);
		USART1_send_string ("0");
		USART1_send_string ("\r\n");
	};break;

	case 5: { // command SET_HV
		my_itoa(settings.hv,t_s);
		USART1_send_string(t_s);
		USART1_send_string ("\r\n");
	};break;

	case 6: { // command SET_HV?
		my_strcut ((char*)command_buf , ' ' , 1 , t_s , 64);
		t_i = my_atoi (t_s);
		settings.hv=t_i;
		USART1_send_string ("OK\r\n");
	};break;

	case 7: { // command SET_REF
		my_itoa(settings.ref,t_s);
		USART1_send_string(t_s);
		USART1_send_string ("\r\n");
	};break;

	case 8: { // command SET_REF?
		my_strcut ((char*)command_buf , ' ' , 1 , t_s , 64);
		t_i = my_atoi (t_s);
		settings.ref=t_i;
		USART1_send_string ("OK\r\n");
	};break;

	case 9: { // command SHUTTER
		my_itoa(settings.shutter,t_s);
		USART1_send_string(t_s);
		USART1_send_string ("\r\n");
	};break;


	case 10: { // command SHUTTER?
		my_strcut ((char*)command_buf , ' ' , 1 , t_s , 64);
		t_i = my_atoi (t_s);
		settings.shutter=t_i;
		USART1_send_string ("OK\r\n");
	};break;

	case 11: { // command PHOTO
		my_itoa(settings.photo,t_s);
		USART1_send_string(t_s);
		USART1_send_string ("\r\n");
	};break;

	case 12: { // command PHOTO?
		my_strcut ((char*)command_buf , ' ' , 1 , t_s , 64);
		t_i = my_atoi (t_s);
		settings.photo=t_i;
		USART1_send_string ("OK\r\n");
	};break;

	case 13: { // command SOUND
		my_itoa(settings.sound,t_s);
		USART1_send_string(t_s);
		USART1_send_string ("\r\n");
	};break;

	case 14: { // command SOUND?
		my_strcut ((char*)command_buf , ' ' , 1 , t_s , 64);
		t_i = my_atoi (t_s);
		settings.sound=t_i;
		USART1_send_string ("OK\r\n");
	};break;

	case 15: { // command START
		settings.state=1;
		for(uint32_t i=0;i<1;i++){
			VCP_DataTx(testbuf,1000);
		}
		//USART1_send_string ("OK\r\n");
	};break;

	case 16: { // command STOP
		settings.state=0;
		USART1_send_string ("OK\r\n");
	};break;

	case 17: { // command STATE?
		my_itoa(settings.state,t_s);
		USART1_send_string(t_s);
		USART1_send_string ("\r\n");
	};break;

	case 18: { // command DATE

		USART1_send_string ("OK\r\n");
	};break;

	case 19: { // command DATE?
		USART1_send_string ("28-06-17-13-02-43");
		USART1_send_string ("\r\n");
	};break;

	case 20: { // command WINDOW
		my_itoa(settings.window,t_s);
		USART1_send_string(t_s);
		USART1_send_string ("\r\n");
	};break;

	case 21: { // command WINDOW?
		my_strcut ((char*)command_buf , ' ' , 1 , t_s , 64);
		t_i = my_atoi (t_s);
		settings.window=t_i;
		USART1_send_string ("OK\r\n");
	};break;

	case 22: { // command ALEDON

		GPIO_SetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		USART1_send_string ("OK\r\n");
	};break;

	case 23: { // command ALEDOFF

		GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		USART1_send_string ("OK\r\n");
		Test();
	};break;

	case -1:USART1_send_string ("Unknown command\r\n");break;
    }


}
