/*
 * GraficTFT_Test.c
 *
 * Created: 22-03-2018 09:34:40
 * Author : Liver
 */ 
#include <avr/io.h>
#define F_CPU 16000000
#include <avr/cpufunc.h>
#include <util/delay.h>
#include "TFTdriver.h"
#include "uart.h"
#include "i2c.h"
#define ADDRESS 40

volatile char color;
volatile int COLORFLAG = 0;

ISR(INT3_vect)
{
	color = i2c_master_receive(ADDRESS);
	COLORFLAG = 1;
}

int main(void)
{
    /* Replace with your application code */
	
	 
	initEXTInterrupt();
	DisplayInit();
	i2c_master_init();
	int i = 0;
	//long int i=0;
	//int count = 0;
	FillRectangle(0,0,320,240,31,63,31);
	//int size = sizeof(array)/sizeof(array[0]);
	float countRed=0;
	float countGreen=0;
	float countBlue=0;
	writeString("Sort M&M's",10,10);
	writeString("Total:",190,10);

	_delay_ms(1000);
    while (1) 
    {
		FillRectangle(0,0,320,240,31,63,31);
		writeString("Sort M&M's",10,10);
		writeString("Total:",190,10);
		
		if (color == 'R' && COLORFLAG == 1)
		{
			countRed++;
			COLORFLAG = 0;
		}
		else if (color == 'G' && COLORFLAG == 1)
		{
			countGreen++;
			COLORFLAG = 0;
		}
		else if (color == 'B' && COLORFLAG == 1)
		{
			countBlue++;
			COLORFLAG = 0;
		}
		
		drawTotal(countRed, countGreen, countBlue);
		//countBlue(count);
		//countRed(count);

		//writeString("12",50,100);
		
		countRed++;
		countRed++;
		countBlue++;
		countGreen++;
	
		_delay_ms(1000);
		DisplayOn();		
    }
		
}
