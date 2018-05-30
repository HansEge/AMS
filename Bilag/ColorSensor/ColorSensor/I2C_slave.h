/*
 * I2C_slave.h
 *
 * Created: 22-05-2018 14:07:09
 *  Author: stinu
 */ 


#ifndef I2C_SLAVE_H_
#define I2C_SLAVE_H_

void i2c_slave_init();
void i2c_slave_wait();
unsigned char i2c_slave_status();
void i2c_slave_clear();
unsigned char i2c_slave_addressed();
void i2c_slave_ack();
void i2c_slave_nack();


#endif /* I2C_SLAVE_H_ */