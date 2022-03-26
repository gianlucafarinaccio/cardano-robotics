#include <main.h>
#include <LineFunctions_h.c>

#INT_SSP2
void  SSP2_isr(void) {
   unsigned int8 state = 0;

   state=i2c_isr_state(I2CSlave);
   if(state >= 0x80){
      i2c_write(I2CSlave,DataByte); // Byte out 
   }
}

void main(){
   setup_adc_ports(sAN0|sAN1|sAN2|sAN3|sAN4|sAN12|sAN14|sAN17);
   setup_adc(ADC_CLOCK_INTERNAL|ADC_TAD_MUL_0);
   output_low(USR_LED);  
   delay_ms(1500);
   if(!input(USR_BUTTON)){
      output_high(USR_LED);
      while(!input(USR_BUTTON));
      output_low(USR_LED);      
      SensorCalibration();
   }
   
   LineValueHigh = read_eeprom(0);
   LineValueLow = read_eeprom(1);   
   LineValue = ((LineValueHigh*10)+LineValueLow)+0;

   for(i=0;i<4;i++){
      output_toggle(USR_LED);
      delay_ms(100);
   }
   output_low(USR_LED);     
   output_high(ALERT);
   DataByte=0;
   enable_interrupts(INT_SSP2);
   enable_interrupts(GLOBAL);   
   
   while(TRUE){
        DataByte = SensorRead();

        if(DataByte != 0){
            output_low(ALERT);
            output_high(USR_LED);         
            delay_ms(2);
            output_high(ALERT);     
        }
        else{
            output_high(ALERT);         
            output_low(USR_LED);      
        }  

   }

}
