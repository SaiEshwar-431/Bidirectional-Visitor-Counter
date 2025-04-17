#include <LPC21xx.h>
#include "types.h"
#include "delay.h"
#include "i2c_defines.h"

void init_i2c(void) {
    // Configure I/O pin for SCL & SDA functions using PINSEL0
    // PINSEL0 = 0x00000050;
    PINSEL0 |= SCL_EN | SDA_EN;

    // Configure Speed for I2C Serial Communication using I2CSCLL & I2CSCLH
    I2SCLL = 75;
    I2SCLH = 75;

    // I2C Peripheral Enable for Communication
    I2CONSET = 1 << I2EN_BIT;
}

void i2c_start(void) {
    // Start condition
    I2CONSET = 1 << STA_BIT;

    // Wait for start bit status
    while (((I2CONSET >> SI_BIT) & 1) == 0);

    // Clear start condition
    I2CONCLR = 1 << STA_BIT;
}

void i2c_restart(void) {
    // Start condition
    I2CONSET = 1 << STA_BIT;

    // Clear SI_BIT
    I2CONCLR = 1 << SI_BIT;

    // Wait for SI bit status
    while (((I2CONSET >> SI_BIT) & 1) == 0);

    // Clear start condition
    I2CONCLR = 1 << STA_BIT;
}

void i2c_write(u8 dat) {
    // Put data into I2DAT
    I2DAT = dat;

    // Clear SI_BIT
    I2CONCLR = 1 << SI_BIT;

    // Wait for SI bit status
    while (((I2CONSET >> SI_BIT) & 1) == 0);
}

void i2c_stop(void) {
    // Issue stop condition
    I2CONSET = 1 << STO_BIT;

    // Clear SI bit status
    I2CONCLR = 1 << SI_BIT;

    // Stop will be cleared automatically
}

u8 i2c_nack(void) {
    I2CONSET = 0x00; // Assert Not of Ack
    I2CONCLR = 1 << SI_BIT;

    while (((I2CONSET >> SI_BIT) & 1) == 0);

    return I2DAT;
}

u8 i2c_masterack(void) {
    I2CONSET = 0x04; // Assert Ack
    I2CONCLR = 1 << SI_BIT;

    while (((I2CONSET >> SI_BIT) & 1) == 0);

    I2CONCLR = 0x04; // Clear Assert Ack

    return I2DAT;
}
