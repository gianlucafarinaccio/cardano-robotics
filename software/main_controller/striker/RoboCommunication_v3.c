/* v1.1
   RoboCommunication_v3.c
   Gianluca Farinaccio, 2019
*/

#include <RoboGlobal_v4.c>
#include <stdlib.h>
//#include <na_I2CLcd4x20_v2.c>


                          /* Define and Variables */

/* RS232 */
#define BUFFER_SIZE  15
char str[BUFFER_SIZE + 1];
int8 index = 0;
void str_decode();
int1 SerialRoutine();

/* I2C */
#define ToF_NORD_ADDR  0x10
#define ToF_SUD_ADDR   0x12
#define ToF_EST_ADDR   0x14
#define ToF_OVEST_ADDR 0x16
#define LINE_ADDR      0x18

#define CMPS12_ADDR         0xC0
#define MSB_HEADING_ADDR    0x02
#define LSB_HEADING_ADDR    0x03
#define CALIBRATION_ADDR    0x1E

int1  I2C_Check();
int8  I2C_Read_Device(int8 DEVICE_ADDR, REGISTER_ADDR);
void Compass_Read(int1 mode);
unsigned int16  ToF_Read(int8 DEVICE_ADDR);
int8 Line_Read(DEVICE_ADDR);



                            /* RS232 Functions */

/* Decodifica dei dati ricevuti via RS232 dall'openmv. */
void str_decode(){
   char data[4];

   data[0] = str[0];
   data[1] = str[1];
   data[2] = str[2];
   robo.Ball.bdegrees = atol(data);

   data[0] = str[3];
   data[1] = str[4];
   data[2] = str[5];
   robo.Ball.bdistance = atol(data);

   data[0] = str[6];
   data[1] = str[7];
   data[2] = str[8];
   robo.Goal.gdegrees = atol(data);

   data[0] = str[9];
   data[1] = str[10];
   data[2] = str[11];
   robo.Goal.gdistance = atol(data);

}

/* Routine Seriale per la ricezione dei dati ricevuti via RS232.  */
int1 SerialRoutine(){
  unsigned int8 serialChar;    //Carattere seriale ricevuto
   serialChar = getch();     //Carattere ricevuto
   if(serialChar == '\r')  { //Ricevuto un CR
     str[index] = '\0';  //Fine stringa
     index=0;   //Reset indice array per fase successiva
     str_decode();
     output_toggle(LD3);
     return 1;
   }
   else  {
     str[index] = serialChar;  //Immagazzino il carattere ricevuto
     index++;                //Incremento indice array
     if(index > BUFFER_SIZE){ //Controllo se ho ricevuto piu' di def caratteri
        index=0;   //Reset indice array
     }
     return 0;
   }
}


                            /* I2C Functions */

int8 I2C_Read_Device(int8 DEVICE_ADDR, REGISTER_ADDR){
  int8 data = 0;
     i2c_start();
     i2c_write(DEVICE_ADDR);
     i2c_write(REGISTER_ADDR);

     i2c_start();
     i2c_write(DEVICE_ADDR + 1);
     data = i2c_read(0);
     i2c_stop();
     return data;
}

void Compass_Read(int1 mode){
   int8 data1, data2;

   disable_interrupts(INT_EXT2);
   data1 = I2C_Read_Device(CMPS12_ADDR, MSB_HEADING_ADDR);
   data2 = I2C_Read_Device(CMPS12_ADDR, LSB_HEADING_ADDR);
   robo.Compass.VirtualDegrees = make16(data1, data2);
   robo.Compass.VirtualDegrees = robo.Compass.VirtualDegrees / 10;
   clear_interrupt(INT_EXT2);
   enable_interrupts(INT_EXT2);

   if(mode){
      signed int16 degrees;
      degrees = robo.Compass.VirtualDegrees - robo.Compass.VirtualNord;
      if(degrees < 0){
         degrees = degrees + 359;
      }

      if(degrees>180){
         degrees = degrees-359;
      }
      else{
         degrees = degrees;
      }
      robo.Compass.DeltaDegrees = degrees;
      }
}


unsigned int16  ToF_Read(int8 DEVICE_ADDR){
   int8 dataLow=0,dataHigh=0;
   unsigned int16 distance = 0;
   i2c_start();
   i2c_write(DEVICE_ADDR+1);
   dataLow=i2c_read();   //Legge dato con attesa Ack
   dataHigh=i2c_read(0);   //Legge dato senza attesa Ack
   i2c_stop();
   delay_ms(1);
   distance = make16(dataHigh,dataLow);
   return distance/10;
}


int8 Line_Read(DEVICE_ADDR){
   int8 data=0;
   i2c_start();
   i2c_write(DEVICE_ADDR+1);
   data=i2c_read(0);   //Legge dato senza attesa Ack
   i2c_stop();
   return data;
}
