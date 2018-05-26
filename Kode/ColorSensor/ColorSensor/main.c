/*
 * ColorSensor.c
 *
 * Created: 19-04-2018 08:54:27
 * Author : stinu
 */ 

#include <avr/io.h>
#include "uart.h"
#include "I2C_slave.h"
#include <avr/interrupt.h>
#include <time.h>
#include <util/delay.h>
#include <stdlib.h>


#define F_CPU 16000000


volatile int newValue, oldValue, period, overflow = 0;
volatile int freq = 0;
volatile int FREQFLAG = 0, REDFLAG = 0, GREENFLAG = 0, BLUEFLAG = 0;
volatile char dataToSend;

ISR(TWI_vect){
	unsigned char transfering = 1;
	unsigned char i = 0;
	unsigned char data;
	
	if(i2c_slave_addressed())
	{
		
		switch(i2c_slave_status())
		{
	
			case 0x60:
			i2c_slave_ack();
			break;
			
			case 0x80:
			SendChar('A');
			data = TWDR;
			i2c_slave_ack();
			transfering = 0;
			break;
			
			case 0xA8:
			TWDR = dataToSend; //data send to master
			i2c_slave_ack();
			break;
			
			// Last byte sent by master
			case 0xC0:
			transfering = 0;
			i2c_slave_ack();
			break;
		}
	}
}


ISR(TIMER4_CAPT_vect)
{
	oldValue = newValue;
	newValue = ICR4;
	
	
	if(newValue < oldValue)
	{
		period = oldValue-newValue;
	}
	else
	{
		newValue + overflow;
		period = oldValue - newValue;
	}

	
	freq = F_CPU/period;
	FREQFLAG = 1;
			
}

ISR(TIMER4_OVF_vect)
{
	overflow = 65535;
}


int main (void)
{

	InitUART(9600,8,'N');
	initColorSensor();
	i2c_slave_init();
	sei();

	while(1)
	{
		dataToSend = getColor();
	}

}
