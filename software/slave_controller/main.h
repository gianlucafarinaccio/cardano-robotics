#include <18F26K22.h>
#device ADC=10

#FUSES NOWDT                    //No Watch Dog Timer

#use delay(internal=32MHz)
#use i2c(I2C2,Slave,Fast,sda=PIN_B2,scl=PIN_B1,force_hw,address=0x18,stream=I2CSlave)
#use i2c(I2C1,Master,Fast,sda=PIN_C4,scl=PIN_C3,force_hw,stream=Debug)
