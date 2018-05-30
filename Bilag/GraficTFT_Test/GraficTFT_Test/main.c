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
#include <avr/interrupt.h>
#include "uart.h"
#include "i2c.h"
#define ADDRESS 40

volatile char color;
volatile int COLORFLAG = 0;
volatile float countRed=0;
volatile float countGreen=0;
volatile float countBlue=0;

ISR(INT2_vect)
{
	//Make I2C call
	color = i2c_master_receive(ADDRESS);
	//Colorfalg
	COLORFLAG = 1;
	
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
	
	//Clears the display and displays the new result
	FillRectangle(0,0,320,240,31,63,31);
	writeString("Sorterer M&M's",10,10);
	writeString("Total:",200,10);
	drawTotal(countRed, countGreen, countBlue);

	DisplayOn();
	_delay_ms(2000);
}

int main(void)
{
	//init interrupt
	sei();
	initEXTInterrupt();
	//init Display
	DisplayInit();
	//Init I2C 
	i2c_master_init();

	//clear Display and set text
	FillRectangle(0,0,320,240,31,63,31);
	writeString("Sorterer M&M's",10,10);
	writeString("Total:",200,10);

    while (1) 
    {

    }
		
}
