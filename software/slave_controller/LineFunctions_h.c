#include <na_I2CLcd4x20_v2.c>

#define USR_BUTTON PIN_C0
#define USR_LED    PIN_C1

#define ALERT      PIN_A4
#define A          PIN_B3
#define B          PIN_B4
#define C          PIN_B5

void NORD_OUT(void){    output_low(PIN_B3);    output_low(PIN_B4);}  //00
void SUD_OUT(void){     output_low(PIN_B3);    output_high(PIN_B4);} //01
void EST_OUT(void){     output_high(PIN_B3);   output_low(PIN_B4);}  //10
void OVEST_OUT(void){   output_high(PIN_B3);   output_high(PIN_B4);} //11

unsigned int16 LineValue=0,LineValueLow=0,LineValueHigh=0;
int8 DataByte=0,i=0;

void SensorCalibration(){
   unsigned int16 adc_value=0;

  set_adc_channel(3);
  delay_us(10);

  while(input(USR_BUTTON));
  output_high(USR_LED);
  while(!input(USR_BUTTON)){
      adc_value=read_adc(); 
  }
  output_low(USR_LED);  
  write_eeprom(0,(adc_value/10));
  write_eeprom(1,(adc_value%10));
}


int8 SensorRead(){
  unsigned int16 value=0;
  int8 OutByte=0;
 
  //nord
  set_adc_channel(0);
  delay_us(5);
  value = read_adc(); 
  if(value <= LineValue) { OutByte = 1; }

  //est
  set_adc_channel(12);
  delay_us(5);
  value = read_adc();   
  if(value <= LineValue) { OutByte+=2; }  
 
  //ovest 
  set_adc_channel(17);
  delay_us(5);
  value = read_adc();   
  if(value <= LineValue) { OutByte+=4; }   

  //sud 
  set_adc_channel(3);
  delay_us(5);
  value = read_adc();   
  if(value <= LineValue) { OutByte+=8; }  

  //nord interno
  set_adc_channel(1);
  delay_us(5);
  value = read_adc(); 
  if(value <= LineValue) { OutByte+=16; }

  //est interno
  set_adc_channel(4);
  delay_us(5);
  value = read_adc();   
  if(value <= LineValue) { OutByte+=32; }  
 
  //ovest interno 
  set_adc_channel(14);
  delay_us(5);
  value = read_adc();   
  if(value <= LineValue) { OutByte+=64; }   

  //sud interno
  set_adc_channel(2);
  delay_us(5);
  value = read_adc();   
  if(value <= LineValue) { OutByte+=128; }

   return OutByte;
}



