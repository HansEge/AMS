/*
 * Source.h
 *
 * Created: 19-04-2018 09:05:49
 *  Author: stinu
 */ 
	
#include <avr/io.h>
#include "uart.h"
#include <avr/interrupt.h>
#include <time.h>
#include <util/delay.h>
#include <stdlib.h>

/*	
*************************
S0 = PB4
S1 = PB5
S2 = PB6
S3 = PB7
FO = PL1
*************************
*/

#ifndef COLORSENSOR_H_
#define COLORSENSOR_H_

void initColorSensor();
char getColor();
char getMeanColor();

#endif /* COLORSENSOR_H_ */