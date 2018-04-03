/*
 * spi.h
 *
 *  Created on: 13 wrz 2017
 *      Author: Micha³
 */

#ifndef SPI_H_
#define SPI_H_

//#include <stdio.h>
//#include <stdlib.h>
//#include "stm32f4xx_conf.h"

void Acc_Init(void);

void Spi_Read(int8_t* x, int8_t* y, int8_t* z);

#endif /* SPI_H_ */
