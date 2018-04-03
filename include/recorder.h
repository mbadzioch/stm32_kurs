/*
 * recorder.h
 *
 *  Created on: 29 cze 2017
 *      Author: Marcin
 */

#ifndef RECORDER_H_
#define RECORDER_H_

struct {
	uint32_t cnt;
	uint16_t hv;
	uint16_t ref;
	uint8_t  shutter;
	uint8_t  photo;
	uint8_t  sound;
	uint8_t  state;
	uint16_t window;
}settings;

/*
 * Daje dostęp do wszystkich próbek w pamięci, w każdym wywołaniu podaje jedną w standardowym formacie
 *
 * Argumenty:
 * 			buf - bufor na pojedynczy rekord
 * 			flag: 0 - odczyt wszystkich rekordów
 * 				  1 - odczyt tylko nowych rekordów
 * Zwraca:
 * 			0 - Brak nieodczytanych próbek w pamięci
 * 			1 - Są nieodczytane próbki w pamięci
 */
int8_t Rec_GetRecords(char* buf,uint8_t flag);

void Rec_Init(void);
void Rec_Main(void);


#endif /* RECORDER_H_ */
