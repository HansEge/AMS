/*
 * i2c.c
 *
 * Created: 22-05-2018 14:14:31
 *  Author: Liver
 */ 
#include "i2c.h"
#define ADDRESS 40


void i2c_master_init()
{
	// Set prescaler value to 1
	TWSR &= ~(1 << TWPS1) & ~(1 << TWPS0);
	// Set Bit Rate Register to 2 (clock division factor)
	TWBR = 0x2;
}

void i2c_master_wait()
{
	// Count to prevent infinite loop
	unsigned char i = 0;
	// Wait until interrupt flag has been cleared (set to 1 by hardware)
	while(!(TWCR & (1 << TWINT)) && i < 100)
	{
		i++;
	}
	
}

unsigned char i2c_master_status()
{
	// Set three last bits to zero and return status register value
	return (TWSR & 0xF8);
}

void i2c_master_start()
{
	// Enable interrupt and start bits
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	i2c_master_wait();
}

void i2c_master_stop()
{
	// Set stop bit
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2c_master_ack()
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
}

void i2c_master_nack()
{
	TWCR = (1 << TWINT) | (1 << TWEN);
}

unsigned char i2c_master_receive(unsigned char address)
{
	unsigned char data = 0;
	unsigned char address_r = (address << 1) | 0x01;
	unsigned char i = 0;
	unsigned char transfer = 1;
	
	i2c_master_start();
	while(transfer && i < 250)
	{
		//usart_transmit(twi_master_status());
		switch (i2c_master_status())
		{
			// Start condition has been transmitted
			case 0x08:
			// Contact slave and enter master transmitter mode
			TWDR = address_r;
			i2c_master_ack();
			i2c_master_wait();
			break;
			
			// SLA+R has been transmitted and acked
			case 0x40:
			i2c_master_nack();
			i2c_master_wait();
			break;
			
			// Data byte has been received and acked
			case 0x50:
			//SendChar('A');
			data = TWDR;
			transfer = 0;
			break;
			
			// Data byte has been received and nacked
			case 0x58:
			//SendChar('B');
			data = TWDR;
			transfer = 0;
			break;
		}
		i++;
	}
	i2c_master_stop();
	if(i < 250) return data;
	else return 0xEE;
}
