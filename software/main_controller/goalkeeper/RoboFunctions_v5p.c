/*
   RoboFunctions_v2.c
   Gianluca Farinaccio, 2019
*/

#include <RoboCommunication_v5p.c>
#include <math.h>

void kick();
void setup_esc(unsigned int16 timeHigh);
void control_roller();
void control_kicker();
int mod(int a);
void RoboSetup();
void Dir(float speed,float degrees, int reference);
void OnBall(float speed,float bdegrees);
void OnGoal(float speed,float gdegrees);
void ReturnGoal(float speed, float gdegrees);
unsigned int16 LineRoutine(int8 n_data);
int1 BallOutRange(unsigned int16 out_angle);


/******************************************************************************
   Autore: Gianluca Farinaccio
   Descrizione: Attivazione routine kicker
******************************************************************************/
void kick(){
  if(kickAvaible){
     output_high(KICKER);
     output_high(USR_LED);
     set_timer1(65536-(msOverflow*1000));
     enable_interrupts(INT_TIMER1);
     kickAvaible = 0;
  }
}

/******************************************************************************
   Autore: Gianluca Farinaccio
   Descrizione: Imposta velocita per roller. Accetta parametri tra 200 e 2000us
******************************************************************************/
void setup_esc(unsigned int16 timeHigh){
   #define start_esc OUTPUT_HIGH(ROLLER)
   if(timeHigh <200){ timeHigh=200; }
   tHigh=(20000-timeHigh)/0.5;
   tLow=timeHigh/0.5;
   start_esc;
   level=0;
   set_rtcc(25535+tHigh);
}

/******************************************************************************
 Autore: Gianluca Farinaccio
 Descrizione: Decide se attivare o meno il roller in base a varie info
 ******************************************************************************/
void control_roller(){

    if((robo.ball.bdegrees!=999)&&(robo.ball.bdegrees>330||robo.ball.bdegrees<30)&&(robo.ball.bdistance<45)){
        setup_esc(700);
        robo.ball.bvalue+=1;
        output_high(LD2);
        if(robo.Ball.bvalue>=5){
            robo.Line.BallOut = 0;
        }

    } else{
        setup_esc(500);
        robo.ball.bvalue = 0;
        output_low(LD2);
    }
}

/******************************************************************************
 Autore: Gianluca Farinaccio
 Descrizione: Decide se attivare o meno il kicker in base a varie info
 ******************************************************************************/
 void control_kicker(){
    if((robo.goal.gdegrees!=999)&&(robo.goal.gdegrees>330||robo.Goal.gdegrees<30)&&(robo.goal.gdistance < 130)&&(robo.ball.bvalue>=10)){
        kick();
    }
    else{
      output_low(USR_LED);
    }
 }

/******************************************************************************
   Autore: Gianluca Farinaccio
   Descrizione: Formatta un valore in 0-360
******************************************************************************/
int mod(int a){
    signed int r = a % 360;
    return r<0 ? r+360 : r;
}

/******************************************************************************
   Autore: Gianluca Farinaccio
   Descrizione: Routine setup iniziale del robot
******************************************************************************/
void RoboSetup(){

    /* SET CCP MODULE FOR MOTOR */
    setup_timer_2(T2_DIV_BY_4,199,1);      //100 us overflow, 100 us interrupt
    setup_ccp7(CCP_PWM);
    setup_ccp8(CCP_PWM);
    setup_ccp9(CCP_PWM);
    setup_ccp10(CCP_PWM);
    set_pwm7_duty((int16)0);
    set_pwm8_duty((int16)0);
    set_pwm9_duty((int16)0);
    set_pwm10_duty((int16)0);

 /* SET VIRTUAL NORD */
    output_high(USR_LED);
    Compass_Read(0);
    robo.Compass.VirtualNord = robo.Compass.VirtualDegrees;

    /* SET WHITELINE INTERRUPTS AND PULLUP */
    port_b_pullups(0x04);
    ext_int_edge(2,H_TO_L);
    enable_interrupts(INT_EXT2);
    WlRilevata = False;
    robo.Line.BallOut = False;

    /* SET SERIAL INTERRUPTS */
    enable_interrupts(INT_RDA2);
    AllData = False;

    /* SETUP ROBOT END */
    enable_interrupts(GLOBAL);
    output_low(USR_LED);

}

/******************************************************************************
   Autore: Gianluca Farinaccio
   Descrizione: Funzione movimenti ominidirezionale
   Parametri: Velocita (0 - 1.3), Angolo di direzione
******************************************************************************/
void Dir(float speed, float degrees, int reference){
   float speedOUT[4],x,y;
   int8 kp=4;
   signed int16 errore=0,delta=0;

   degrees+=90; //trasformo il mio angolo in angolo trigonometrico
   degrees = (3.14 * degrees)/180;
   x = -cos(degrees) * speed; //componente x
   y = sin(degrees) * speed; //componente y

   speedOUT[0] = ((x * -0.707) + (y * 0.707))*797; //madx
   speedOUT[1] = ((x * 0.707 ) + (y * 0.707))*797; //mpdx
   speedOUT[2] = -speedOUT[0]; //mpsx
   speedOUT[3] = -speedOUT[1]; //masx

    if(reference == 0){
        Compass_Read(1);
        delta = robo.Compass.DeltaDegrees;
        kp = 8;
    }
    else{
        if(robo.Goal.gdegrees>180){ delta = -(robo.Goal.gdegrees-360); }
        else{ delta = -robo.Goal.gdegrees; }
        kp = 12;
    }

    errore = kp * delta;
    MOT_A(speedOUT[3] + errore);
    MOT_B(speedOUT[2] + errore);
    MOT_C(speedOUT[1] + errore);
    MOT_D(speedOUT[0] + errore);
}

/******************************************************************************
   Autore: Gianluca Farinaccio
   Descrizione: Funzione movimenti ominidirezionali verso la palla
   Parametri: Velocita (0 - 1.3), gradi palla
******************************************************************************/
void OnBall(float speed,float bdegrees){

   float risultato=0,angolo_opposto=0;

  if(bdegrees != 999){ //palla presente
      if(bdegrees>350||bdegrees<10){ risultato=0; } //cono centrale
      else{
         if(bdegrees <= 180){ // palla dx
            if(bdegrees<=45){ risultato=bdegrees*2.0; }
            else{ risultato=bdegrees+65; }
         } //palla dx end
         else{ //palla sx
            angolo_opposto= -(bdegrees-360);
            if(bdegrees>=315){ angolo_opposto=360-(angolo_opposto*2.0); }
            else{ angolo_opposto=360-(angolo_opposto+65); }
            risultato = angolo_opposto;
         } //palla sx end
      }
      Dir(speed,risultato,VERSONORD);
   } //palla presente end
   else{
       Dir(0,0,VERSONORD);
   }
}

/******************************************************************************
   Autore: Gianluca Farinaccio
   Descrizione: Funzione movimenti ominidirezionali verso porta
   Parametri: Velocita (0 - 1.3), gradi porta
******************************************************************************/
void OnGoal(float speed,float gdegrees){
    float angolo=0;

    if(gdegrees != 999){
      if(robo.ball.bdegrees>90 && robo.Ball.bdegrees<270){
          OnBall(speed,robo.Ball.bdegrees);
      } else{
           if(gdegrees<150 || gdegrees>210){
               if(gdegrees<=180){
                   angolo = gdegrees-(gdegrees*0.5);
                   if(angolo < 0){ angolo = angolo + 360;}
               } else{
                   angolo = -(gdegrees-360);
                   angolo = gdegrees+(angolo*0.5);
                   if(angolo>359){ angolo = angolo-360; }
               }
           }
           else{
               angolo = gdegrees;
           }
         Dir(speed,angolo,VERSONORD);
        }
    } else{
         Dir(speed,180,VERSONORD);
    }

}

/******************************************************************************
   Autore: Gianluca Farinaccio
   Descrizione: Funzione movimenti ominidirezionali verso porta
   Parametri: Velocita (0 - 1.3), gradi porta
******************************************************************************/
void ReturnGoal(float speed, float gdegrees){
  float angolo=0;
  
  if(gdegrees != 999){
       if(gdegrees<150||gdegrees>210){
           if(gdegrees<=180){
               angolo = gdegrees-(gdegrees*0.5);
               if(angolo < 0){ angolo = angolo + 360;}
           } else{
               angolo = -(gdegrees-360);
               angolo = gdegrees+(angolo*0.5);
               if(angolo>359){ angolo = angolo-360; }
           }
       }
       else{
           angolo = gdegrees;
       }
       Dir(speed,angolo,VERSONORD);
  } else{
       Dir(0,0,VERSONORD);
  }
}


/******************************************************************************
   Autore: Gianluca Farinaccio
   Descrizione: Funzione calcolo movimento rientro in campo
   Parametri: Byte dei sensori         i=interno | e=esterno
                                     (Si-Oi-Ei-Ni-Se-Oe-Ee-Ne)
                                      (128-64-32-16-8-4-2-1)
   Ritorno: Grado di direzione (0-359)
******************************************************************************/
unsigned int16 LineRoutine(int8 n_data){
                                            //  NORD
   switch(n_data){

      case 1:           //                 �      |    �
      case 7:           //                        |  �   �
      case 16:
      case 17:
      case 112:
            robo.Line.OutLineAngle=0;
            return 180;
            break;

      case 8:           //                        |  �   �
      case 14:          //                 �      |    �
      case 128:
      case 136:
      case 224:
            robo.Line.OutLineAngle=180;
            return 0;
         break;

      case 4:           //                        |    �
      case 13:          //               �        |  �
      case 64:         //                         |    �
      case 208:
      case 68:
         robo.Line.OutLineAngle=270;
         return 90;
         break;

      case 2:           //                        |    �
      case 11:          //                   �    |      �
      case 32:          //                        |    �
      case 176:
      case 34:
         robo.Line.OutLineAngle=90;
         return 270;
         break;

      case 5:
      case 80:
         robo.Line.OutLineAngle=315;
         return 135; // ex 135
         break;

      case 12:
      case 132:
      case 192:
      case 72:
         robo.Line.OutLineAngle=225;
         return 45; //ex 45
         break;

      case 10:
      case 160:
         robo.Line.OutLineAngle=135;
         return 315; //ex 315
         break;

      case 3:
      case 48:
         robo.Line.OutLineAngle=45;
         return 225; //ex 225
         break;

      default:
         return robo.Line.LineRoutineDir;
//!         if(robo.Ball.bdegrees<=180){
//!            return (robo.Ball.bdegrees+180);
//!         }
//!         else{
//!            return (robo.Ball.bdegrees-180);
//!         }
   }
}

/******************************************************************************
   Autore: Gianluca Farinaccio
   Descrizione: Mascheramento palla quando linea rilevata
   Parametri: angolo uscita linea
******************************************************************************/
int1 BallOutRange(unsigned int16 out_angle){
   switch(out_angle){

     case 0:
       if((robo.ball.bdegrees<105)||(robo.Ball.bdegrees>255)){
          return 0;
       }
       else{ return 1; }
       break;

     case 180:
       if((robo.ball.bdegrees<270)&&(robo.Ball.bdegrees>90)){
          return 0;
       }
       else{ return 1; }
       break;

     case 90:
       if((robo.ball.bdegrees>345)||(robo.Ball.bdegrees<195)){
          return 0;
       }
       else{ return 1; }
       break;

     case 270:
       if((robo.ball.bdegrees<15)||(robo.Ball.bdegrees>165)){
          return 0;
       }
       else{ return 1; }
       break;

     case 45:
       if((robo.ball.bdegrees>270)||(robo.Ball.bdegrees<200)){
          return 0;
       }
       else{ return 1; }
       break;

     case 135:
       if((robo.ball.bdegrees>345)||(robo.Ball.bdegrees<270)){
          return 0;
       }
       else{ return 1; }
       break;

     case 225:
       if((robo.ball.bdegrees<15)||(robo.Ball.bdegrees>90)){
          return 0;
       }
       else{ return 1; }
       break;

     case 315:
       if((robo.ball.bdegrees<105)||(robo.Ball.bdegrees>195)){
          return 0;
       }
       else{ return 1; }
       break;

   }
}
