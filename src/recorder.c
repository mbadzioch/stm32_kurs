/*
 * recorder.c
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
#include "kom.h"
#include "recorder.h"

/*
 * Działanie nagrywania:
 * 	- w momencie załączenia nagrywania (flaga status w strukturze settings) zapisywany jest znacznik czasu, względem którego odmierzamy milisekundy.
 * 	- milisekundy są zapisywane w osobnej zmiennej w momencie odczytu przyjścia fotonu
 *
 * 	! w razie przepełnienia bufora na rekordy należy powiadomić oprogramowanie!
 *
 * 	? czy w razie odczytu przez PC rekordu kasować go z pamięci?
 */

typedef struct{
	uint32_t msec;  // Znacznik czasowy rekordu względem początku pomiaru
	uint8_t val;	// Liczba fotonów w danym oknie czasowym
}recordS;

struct{

};

recordS recTab[1000]; // Bufor kołowy na rekordy

int8_t Rec_GetRecords(char* buf,uint8_t flag)
{
	return 0;
}

void Rec_Init(void)
{
	/*
	 * Inicjalizacja timera odmierzającego okno pomiarowe
	 */
}
void Rec_Main(void)
{
	/*
	 *	Obsługa pamięci rekordów (czyszczenia etc)
	 */
}
