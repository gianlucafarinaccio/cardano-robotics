#include <18F87K22.h>
#device ADC=12

#FUSES NOWDT                    //No Watch Dog Timer

#use delay(internal=32MHz)
#use rs232(baud=115200,parity=N,xmit=PIN_G1,rcv=PIN_G2,bits=8,stream = openmv)
#use rs232(baud=115200,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream = Bluetooth)
#use i2c(Master,Fast=200000,sda=PIN_C4,scl=PIN_C3)
