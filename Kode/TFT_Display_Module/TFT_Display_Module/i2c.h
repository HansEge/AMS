#include <avr/io.h>
#define F_CPU 3686400
#include <avr/delay.h>


void i2c_master_init();
void i2c_master_wait();
unsigned char i2c_master_status();
void i2c_master_start();
void i2c_master_stop();
void i2c_master_ack();
void i2c_master_nack();
unsigned char i2c_master_receive(unsigned char address);
