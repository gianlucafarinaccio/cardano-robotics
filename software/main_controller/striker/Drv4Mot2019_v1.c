/* ========================================================================== */
/*                                                                            */
/*   Drv4Mot2019_v1.c                              Update to 24 Nov 2018      */
/*                                                                            */
/*   Description: Driver movimento motori Robot 4 ruote                       */
/*                                                                            */
/*                                                  (C)gf Monterotondo-Rm     */
/* ========================================================================== */
// Riferimenti schematic:  [1] motorDriverL6206D
//                         [2] MainBoard18/19, AnxaPic80 v4.1


//                          I 4 MOTORI del Robot
//    N.B. => Riferimento => Guardando il robot dalla parte posteriore
//*****************************************************************************

// MASX          1 - Motore Anteriore SX
//                Azionamento =>  Pin RE2 -- PWM10
//                Verso =>        Pin RF2
//                                    Avanti => RF2(HIGH) --- PWM COMPLEMENTATO
//                                    Indietro => RF2(LOW) -- PWM NORMALE
//                Enable =>       Pin RE0
//-----------------------------------------------------------------------------
// MPSX          1 - Motore Posteriore SX
//                Azionamento =>  Pin RE3 -- PWM9
//                Verso =>        Pin RF6
//                                    Avanti => RF6(HIGH) --- PWM COMPLEMENTATO
//                                    Indietro => RF6(LOW) -- PWM NORMALE
//                Enable =>       Pin RE1
//-----------------------------------------------------------------------------
// MPDX          1 - Motore Posteriore DX
//                Azionamento =>  Pin RE4 -- PWM8
//                Verso =>        Pin RF4
//                                    Avanti => RF4(HIGH) --- PWM COMPLEMENTATO
//                                    Indietro => RF4(LOW) -- PWM NORMALE
//                Enable =>       Pin RF3
//-----------------------------------------------------------------------------
// MADX          1 - Motore Anteriore DX
//                Azionamento =>  Pin RE5 -- PWM7
//                Verso =>        Pin RF7
//                                    Avanti => RF7(HIGH) --- PWM COMPLEMENTATO
//                                    Indietro => RF7(LOW) -- PWM NORMALE
//                Enable =>       Pin RF5
//-----------------------------------------------------------------------------
#DEFINE MASX_STOP  output_low(PIN_E0) //MASX
#DEFINE MPSX_STOP  output_low(PIN_E1) //MPSX
#DEFINE MPDX_STOP  output_low(PIN_F3) //MPDX
#DEFINE MADX_STOP  output_low(PIN_F5) //MADX

#DEFINE MASX_ON  output_high(PIN_E0) //MASX
#DEFINE MPSX_ON  output_high(PIN_E1) //MPSX
#DEFINE MPDX_ON  output_high(PIN_F3) //MPDX
#DEFINE MADX_ON  output_high(PIN_F5) //MADX

#DEFINE MASX_A   output_high(PIN_F2)
#DEFINE MASX_I   output_low(PIN_F2)
#DEFINE MPSX_A   output_high(PIN_F6)
#DEFINE MPSX_I   output_low(PIN_F6)
#DEFINE MPDX_A   output_high(PIN_F4)
#DEFINE MPDX_I   output_low(PIN_F4)
#DEFINE MADX_A   output_high(PIN_F7)
#DEFINE MADX_I   output_low(PIN_F7)


void MotorInit(){
  MASX_STOP;
  MPSX_STOP;
  MPDX_STOP;
  MADX_STOP;
}

//##########################################
void masx_Avanti(unsigned int16 speed){
//!  MPSX_STOP;
//!  MPDX_STOP;
//!  MADX_STOP;
  if(speed > 797){
      speed=797;
  }
  set_pwm10_duty((int16)797-speed);
  output_high(PIN_F2); //imposto il verso
  MASX_ON;
}

void masx_Indietro(unsigned int16 speed){
//!  MPSX_STOP;
//!  MPDX_STOP;
//!  MADX_STOP;
  if(speed > 797){
      speed=797;
  }
  set_pwm10_duty((int16)speed);
  output_low(PIN_F2); //imposto il verso
  MASX_ON;
}

//##########################################

void mpsx_Avanti(unsigned int16 speed){
//!  MASX_STOP;
//!  MPDX_STOP;
//!  MADX_STOP;
  if(speed > 797){
      speed=797;
  }
  set_pwm9_duty((int16)797-speed);
  output_high(PIN_F6); //imposto il verso
  MPSX_ON;
}

void mpsx_Indietro(unsigned int16 speed){
//!  MASX_STOP;
//!  MPDX_STOP;
//!  MADX_STOP;
//!
  if(speed > 797){
      speed=797;
  }
  set_pwm9_duty((int16)speed);
  output_low(PIN_F6); //imposto il verso
  MPSX_ON;
}

//##########################################

void mpdx_Avanti(unsigned int16 speed){
//!  MASX_STOP;
//!  MPSX_STOP;
//!  MADX_STOP;
  if(speed > 797){
      speed=797;
  }
  set_pwm8_duty((int16)797-speed);
  output_high(PIN_F4); //imposto il verso
  MPDX_ON;
}

void mpdx_Indietro(unsigned int16 speed){
//!  MASX_STOP;
//!  MPSX_STOP;
//!  MADX_STOP;
  if(speed > 797){
      speed=797;
  }
  set_pwm8_duty((int16)speed);
  output_low(PIN_F4); //imposto il verso
  MPDX_ON;
}

//##########################################

void madx_Avanti(unsigned int16 speed){
//!  MASX_STOP;
//!  MPSX_STOP;
//!  MPDX_STOP;
  if(speed > 797){
      speed=797;
  }
  set_pwm7_duty((int16)797-speed);
  output_high(PIN_F7); //imposto il verso
  MADX_ON;
}

void madx_Indietro(unsigned int16 speed){
//!  MASX_STOP;
//!  MPSX_STOP;
//!  MPDX_STOP;
  if(speed > 797){
      speed=797;
  }
  set_pwm7_duty((int16)speed);
  output_low(PIN_F7); //imposto il verso
  MADX_ON;
}

//##########################################

void robot_Avanti(unsigned int16 speed){
//!  MASX_STOP;
//!  MPSX_STOP;
//!  MPDX_STOP;
//!  MADX_STOP;

  //imposto i versi
  MASX_A;
  MPSX_A;
  MPDX_A;
  MADX_A;

  //imposto le velocita'
  set_pwm10_duty((int16)797-speed);
  set_pwm9_duty((int16)797-speed);
  set_pwm8_duty((int16)797-speed);
  set_pwm7_duty((int16)797-speed);

  //abilito i motori
  MASX_ON;
  MPSX_ON;
  MPDX_ON;
  MADX_ON;
}

void robot_Indietro(unsigned int16 speed){
//!  MASX_STOP;
//!  MPSX_STOP;
//!  MPDX_STOP;
//!  MADX_STOP;

  //imposto i versi
  MASX_I;
  MPSX_I;
  MPDX_I;
  MADX_I;

  //imposto le velocita'
  set_pwm10_duty((int16)speed);
  set_pwm9_duty((int16)speed);
  set_pwm8_duty((int16)speed);
  set_pwm7_duty((int16)speed);

  //abilito i motori
  MASX_ON;
  MPSX_ON;
  MPDX_ON;
  MADX_ON;
}

void MOT_A(signed int16 speed){
   if(speed==0){
      masx_Avanti(0);
   }

   if(speed > 0){
      masx_Indietro(speed);
   }
   else{
      masx_Avanti(-speed);
   }
}

void MOT_B(signed int16 speed){
   if(speed==0){
      mpsx_Avanti(0);
   }

   if(speed > 0){
      mpsx_Indietro(speed);
   }
   else{
      mpsx_Avanti(-speed);
   }
}

void MOT_C(signed int16 speed){
   if(speed==0){
      mpdx_Avanti(0);
   }

   if(speed > 0){
      mpdx_Avanti(speed);
   }
   else{
      mpdx_Indietro(-speed);
   }
}

void MOT_D(signed int16 speed){
   if(speed==0){
      madx_Avanti(0);
   }

   if(speed > 0){
      madx_Avanti(speed);
   }
   else{
      madx_Indietro(-speed);
   }
}
