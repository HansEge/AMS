/*
 * I2C_slave.c
 *
 * Created: 22-05-2018 14:06:57
 *  Author: Stinus
 */ 

#include "I2C_slave.h"
#include "uart.h"

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>


#define ADDRESS 40

void i2c_slave_init()
{
	TWAR = (ADDRESS << 1);
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN) | (1 << TWIE);
	
	DDRD = 0xFF;
	PORTD = 0x00;
}

void i2c_slave_wait(){
		unsigned char i = 0;
		while (!(TWCR & (1<<TWINT)) && i < 100) i++;
}

unsigned char i2c_slave_status()
{
	return (TWSR & 0xF8);
}

void i2c_slave_clear()
{
	TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);
}

unsigned char i2c_slave_addressed()
{
	return (TWCR & (1 << TWINT));
}

void i2c_slave_ack()
{
	TWCR = ((1 << TWEA) | (1 << TWEN) | (1 << TWINT)| (1 << TWIE));
}

void i2c_slave_nack()
{
	TWCR = (1 << TWEN) | (1 << TWINT);
}
