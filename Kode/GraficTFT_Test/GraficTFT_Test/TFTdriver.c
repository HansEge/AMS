/*
 * TFTtest.c
 *
 * Created: 22-03-2018 09:37:12
 *  Author: Dalle Walle
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <avr/cpufunc.h>
#include <util/delay.h>
#include "TFTdriver.h"
#include "DotFactory.h"

//data port definitions:
#define DATA_PORT_HIGH PORTA
#define DATA_PORT_LOW PORTC

//Control port definitions:
#define WR_PORT PORTG
#define WR_BIT 2
#define DC_PORT PORTD
#define DC_BIT 7
#define CS_PORT PORTG
#define CS_BIT 1
#define RST_PORT PORTG
#define RST_BIT 1

int start_x = 0;
int checkX = 0;

void WriteCommand(unsigned int command){
	// Data set up (commands only use lower byte of the data bus)
	DATA_PORT_LOW = command;
	// DCX low
	// CSX low
	// WRX low
	DC_PORT &= ~(1<<DC_BIT);
	CS_PORT &= ~(1<<CS_BIT);
	WR_PORT &= ~(1<<WR_BIT);
	// twrl > 15ns
	_NOP();
	//WRX high
	WR_PORT |= (1<<WR_BIT);
	// twrh > 15ns
	_NOP();
}


 void WriteData(unsigned int data){
	 	// Data set up (commands only use lower byte of the data bus)
		DATA_PORT_HIGH = data >> 8;
	 	DATA_PORT_LOW = data;
	 	// DCX high
		// CSX low
		// WRX low
	 	DC_PORT |= (1<<DC_BIT);
	 	CS_PORT &= ~(1<<CS_BIT);
	 	WR_PORT &= ~(1<<WR_BIT);
	 	// twr0 > 15ns
	 	_NOP();
	 	//WR high
	 	WR_PORT |= (1<<WR_BIT);
	 	// twr0 > 15ns
	 	_NOP();
		 
 }
 
 DisplayInit(){
	 //Sets the control pins as output and set them high
	 //Sets data pis as output
	 DDRG |= 0b00000111;
	 DDRD |= 0b10000000;
	 DDRA = 0xFF;
	 DDRC = 0xFF;
	 PORTG |= 0b00000111;
	 PORTD |= 0b10000000;
	 
	 //Resest the display (RST low)
	 RST_PORT &= ~(1<<RST_BIT);
	 //Wait 300 ms
	 _delay_ms(300);
	 // REST high
	 RST_PORT |= (1<<RST_BIT);
	 _delay_ms(130);
	 
	 // Exit sleep mode.
	 SleepOut();
	 // Set display on
	 DisplayOn();
	 // set bit BGR (scanning direction)
	 MemoryAccessControl(0b00001000);
	 //16 bits (2bytes) per pixel
	 InterfacePixelFormat(0b00000101);
	 
 }
 
 void DisplayOff(){
	WriteCommand(0x28);
 }
 
 void DisplayOn(){
	WriteCommand(0x29); 
 }
 
 void SleepOut(){
	 WriteCommand(0x11);
 }
 
 void MemoryAccessControl(unsigned char parameter){
	 WriteCommand(0x36);
	 WriteData(parameter);
 }
 
 void InterfacePixelFormat(unsigned char parameter){
	 WriteCommand(0x3A);
	 WriteData(parameter);
 }
 
 void MemoryWrite(){
	 WriteCommand(0x2C);
 }
 
 //Red 0-31, Green 0-63, blue 0-31
 void WritePixel(unsigned char Red, unsigned char Blue, unsigned char Green){
	 WriteData((Red<<11)|(Green<<5)|Blue);
 }
 void SetColomnAddress(unsigned int Start,unsigned int End){
	 WriteCommand(0x2A);
	 WriteData(Start>>8);
	 WriteData(Start);
	 WriteData(End>>8);
	 WriteData(End);
 }
 
 void SetPageAddress(unsigned int Start,unsigned int End){
	 	 WriteCommand(0x2B);
	 	 WriteData(Start>>8);
	 	 WriteData(Start);
	 	 WriteData(End>>8);
	 	 WriteData(End);
 }
 
 void FillRectangle(unsigned int StartX, unsigned int StartY, unsigned int Width, unsigned int Height, unsigned char Red, unsigned char Blue, unsigned char Green){
	 SetPageAddress(StartX,StartX+Width);
	 SetColomnAddress(StartY,StartY+Height);
	 MemoryWrite();
	 for(unsigned long i=0; i<((unsigned long)Width*Height);i++)
		WritePixel(Red,Green,Blue);
 }
 
void getSymbolParameters(int length_of_symbol, int startX, int startY){
	//Makes sure to not override the previous letter.
	if (startX != checkX)
	{
		checkX = startX;
		start_x = startX;
	}
	//the first parameter is the length of the letter, the second is the offset. Therefore to get the length of the letter 
	// I have to multiply by 2. 
	length_of_symbol = length_of_symbol*2; //The parameters from array_carrier in the DotFactory.h
	
	// is the length of the chosen character in the DotFactory BitMap[]. 
	int length_count = symbol_carrier[length_of_symbol+3]-symbol_carrier[length_of_symbol+1];
	
	//is the offset of where in the DotFactory BitMap[] to read from.
	unsigned int offset_symbol = symbol_carrier[length_of_symbol+1];
	//A function to return the length of letter en whole byte
	int fixed_letter = lengthOfFixedSymbol(length_of_symbol); 	
	
	drawSymbol(symbols,fixed_letter,length_count,start_x,startY, offset_symbol);
	
	//prepares the offset for next letter.
	start_x = start_x+symbol_carrier[length_of_symbol]+1; 
		
}
int lengthOfFixedSymbol(int length_of_symbol){
	int fixed_letter = symbol_carrier[length_of_symbol]/8;
	int modulus_fixed_letter = symbol_carrier[length_of_symbol]%8;
	if(modulus_fixed_letter>0){
		fixed_letter++;
	} 
	return fixed_letter;
}


void drawSymbol(const uint8_t bitmap[],int length,int length_count,int startx,int starty, int letter){
	//Sets starts position on x-axis 
	int startX = startx;
	//Sets stop position on x-axis
	int stopX= startx+7;
	//Sets start position on y-axis
	int starY = starty;
	
	// this for loop runs trough the full byte length of the letter according to the bitmap from Dotfactory.
	for (int j=0;j<length_count;j++)
	{
		//Set page and colomn address
		SetPageAddress(startX,stopX);
		SetColomnAddress(starY,starY);
		MemoryWrite();
		
		// This for loop runs the byte through and checks if every single bit is high or low
		// if that bit is high a black dot is made, if the bit is low a withe dot is made.
		for (int i=0; i<8; i++)
		{
			if((bitmap[j+letter] & (0b10000000>>i)) != 0)
			{
				WritePixel(0,0,0);
			}
			else
			{
				WritePixel(31,31,63);
			}
		}
		
		//Set new startX to be ready for next byte. (+1 to not override the last bit in the last byte)
		startX = stopX+1;
		stopX = stopX+8;
		
		//this if loop keep track of the letters length in the x-axis direction. 
		//if that length is reached it goes the the next y-axis row.
		if(stopX >= length*8+startx)
		{
			starY++;
			startX = startx;
			stopX = startX+7;
		}
	}
	

}

void writeString(char str[],int startx, int starty){
	// temp is the maximum length of the input string 	
	int temp[99]={},i=0,j;
	
	//the while loop puts puts the input array into a new array that has the exact length of the string
	while(str[i]!='\0') 
	{
		temp[i]=str[i]; i++;
	}
	
	//this for loop runs through the temp array and displays one letter at the time.
	for(j=0;j<i;j++)
	{
		// temp[] -33 to make it ASCII so it fits the bitMap
		int space = temp[j]-33;
		if (space != -1)
		{
				getSymbolParameters(space,startx,starty);
		}
		else
		{
			//If space just move 6 bits on the x-axis
			start_x = start_x+6;
		}
	}
	return 0;
}

void writeInt(long int num, int startX, int startY){
	// sets num flag
	int num_flag = 0;
	
	//maximum size of a number (999999)
	int number[6] = {num % 10 , (num/10) % 10 , (num/100) % 10 , (num/1000) % 10 , (num/10000) % 10 , (num/100000) % 10};

	//This for loop displays the number as MSB first, if the number is 0 and the num falg hasn't been set
	//it ignores that number and moves on the the next.
	for (int i = 5; i >= 0; i--)
	{
		if (number[i] != 0 | num_flag == 1 )
		{
			//number[i]+15 to make it fir the bit map.
			getSymbolParameters(number[i]+15,startX,startY);
			num_flag = 1;
		}
	}
	start_x = startX;

}

void drawRed(int count, int height, int realHeight){
	int _realHeight = 139 - realHeight;
	writeInt(count,140,60+_realHeight);
	FillRectangle(130,100+_realHeight,50,height-_realHeight,31,31,63);
	FillRectangle(130,100+_realHeight,50,height-_realHeight,31,0,0);
}

void drawGreen(int count, int height, int realHeight){
	int _realHeight = 139 - realHeight;
	writeInt(count,40,60+_realHeight);
	FillRectangle(30,100+_realHeight,50,height-_realHeight,31,31,63);
	FillRectangle(30,100+_realHeight,50,height-_realHeight,0,31,0);
}

void drawBlue(int count, int height, int realHeight){
	int _realHeight = 139 - realHeight;
	writeInt(count,240,60+_realHeight);
	FillRectangle(230,100+_realHeight,50,height-_realHeight,31,31,63);
	FillRectangle(230,100+_realHeight,50,height-_realHeight,0,0,31);
}

void drawTotal(float Red, float Green, float Blue){
	int totalCount = Red + Green + Blue;
	
	int heightRed = (Red/totalCount)*139;
	int heightGreen = (Green/totalCount)*139;
	int heightBlue = (Blue/totalCount)*139;
	
	
	
	drawRed(Red, 139, heightRed);
	drawGreen(Green, 139, heightGreen);
	drawBlue(Blue, 139, heightBlue);
	
	writeInt(totalCount, 250, 10);
	
}







 