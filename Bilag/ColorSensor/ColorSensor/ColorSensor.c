/*
 * Source.c
 *
 * Created: 19-04-2018 09:05:26
 *  Author: stinu
 */ 

#include "ColorSensor.h"


extern int newValue, oldValue, period, overflow;
extern int freq;
extern int FREQFLAG, REDFLAG, GREENFLAG, BLUEFLAG;



void initColorSensor()
{
		InitUART(9600,8,'N');
		
		//Enable interrupts
		sei();
		TIMSK4 |= (1<<ICIE4);
		
		//Rising edge, no prescaler
		TCCR4B |= (1<<ICES4);
		TCCR4B |= (1<<CS40);
		TCCR4A = 0;
		
		//Set PORTL to input
		PORTL = 0xFF;
		DDRB = 0xFF;

}

char getColor()
{
	int redFreq = 0, greenFreq = 0, blueFreq = 0;
	

	//Check for all colors
	while((REDFLAG == 0) || (GREENFLAG == 0) || (BLUEFLAG == 0))
	{
		if((REDFLAG == 0) && (FREQFLAG == 1))
		{
			PORTB = 0b00100000; //Red, 2% frequency scaling
			_delay_ms(50);
			redFreq = abs(freq);
			FREQFLAG = 0;
			REDFLAG = 1;
			SendInteger(redFreq);
			SendString(" Red \r\n");
		}

		if((GREENFLAG == 0) && (FREQFLAG == 1))
		{
			PORTB = 0b11100000; //Green, 2% frequency scaling
			_delay_ms(50);
			greenFreq = abs(freq);
			FREQFLAG = 0;
			GREENFLAG = 1;
			SendInteger(greenFreq);
			SendString(" Green \r\n");
		}

		if((BLUEFLAG == 0) && (FREQFLAG == 1))
		{
			PORTB = 0b10100000; //Blue, 2% frequency scaling
			_delay_ms(50);
			blueFreq = abs(freq);
			FREQFLAG = 0;
			BLUEFLAG = 1;
			SendInteger(blueFreq);
			SendString(" Blue \r\n");
		}
	}
	REDFLAG = 0;
	GREENFLAG = 0;
	BLUEFLAG = 0;

	if((redFreq > greenFreq) && (redFreq > blueFreq))
	{
		return 'R';
	}
	if((greenFreq > redFreq) && (greenFreq > blueFreq))
	{
		return 'G';
	}
	if((blueFreq > greenFreq) && (blueFreq > redFreq))
	{
		return 'B';
	}
	else
		
	return 'E'; // error
}

char getMeanColor()
{
	char meanColor[50];
	int redNumber = 0, greenNumber = 0, blueNumber = 0;
	int i = 0;

	while(FREQFLAG == 1)
	{
		
		for(i = 0; i < 50; i++)
		{
			meanColor[i] = getColor();
		}
	}

	cli();

	i = 0;
	for(i = 0; i < 50; i++)
	{
		if(meanColor[i] == 'R')
		redNumber++;

		if(meanColor[i] == 'G')
		greenNumber++;

		if(meanColor[i] == 'B')
		blueNumber++;
	}
	
	SendInteger(redNumber);
	SendString(" \r\n");
	SendInteger(greenNumber);
	SendString(" \r\n");
	SendInteger(blueNumber);
	SendString(" \r\n");

	if((redNumber > greenNumber) && (redNumber > blueNumber))
	{
		sei();
		return 'R';
	}
	if((greenNumber > redNumber) && (greenNumber > blueNumber))
	{
		sei();
		return 'G';
	}
	if((blueNumber > greenNumber) && (blueNumber > redNumber))
	{
		sei();
		return 'B';
	}
	
	
		
}