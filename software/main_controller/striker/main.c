#include <main.h>
#include <Drv4Mot2019_v1.c>
#include <RoboFunctions_v7.c>


#INT_EXT2
void  EXT2_isr(void) {
   WlRilevata = True;
   disable_interrupts(INT_EXT2);
}

#INT_RDA2
void  RDA2_isr(void) {
   AllData = SerialRoutine();
}

#INT_RTCC
void  RTCC_isr(void) {
   output_toggle(ROLLER);
   level==1? set_rtcc(25535+tHigh):set_rtcc(25535+tLow);
   level = !level;
   initRoller<100? initRoller++: 100;
}

#INT_TIMER1
void  TIMER1_isr(void) {
   static int8 kickStep=0;

   if(kickStep==0){
      output_low(KICKER);
      kickStep+=1;
      set_timer1(65536-(msOverflow*1000));
   } else{
      if(kickStep>0 && kickStep<10){
         set_timer1(65536-(msOverflow*1000));
         kickStep+=1;
      } else{
         kickAvaible = 1;
         kickStep = 0;
         disable_interrupts(INT_TIMER1);
      }
   }
}

void main() {
   MotorInit();
   RoboSetup();
     /* SETUP ESC FOR ROLLER */
    setup_timer_0(RTCC_INTERNAL|RTCC_DIV_4);      //32,7 ms overflow //0.5us
    setup_esc(500);
    enable_interrupts(INT_RTCC);
    enable_interrupts(GLOBAL);
    while(initRoller<20);
    setup_esc(500);

   while(TRUE){

     if(WlRilevata){
        robo.Line.DataByte = Line_Read(0x18);
        if(robo.Line.DataByte==0 ||robo.Line.DataByte==255){
           Dir(0,0,VERSONORD);
           robo.Line.BallOut = False;
        } else{

           robo.Line.LineRoutineDir = LineRoutine(robo.Line.DataByte);
           Dir(1.2,robo.Line.LineRoutineDir,VERSONORD);
           robo.Line.BallOut = True;
        }
           WlRilevata = False;
           //clear_interrupt(INT_EXT2);
           enable_interrupts(INT_EXT2);
     } else{

       control_roller();
       control_kicker();

       if(!robo.Line.BallOut){
          robo.Line.BallOut = False;
          if(robo.goal.gdegrees!=999 && robo.ball.bvalue>=5){
              OnGoal(1.2,robo.goal.gdegrees);
          } else{
              OnBall(1.2,robo.Ball.bdegrees);
          }

       } else{
             if((!BallOutRange(robo.Line.OutLineAngle))&&(robo.Ball.bdegrees!=999)){
                robo.Line.BallOut = True;
                followOutBall(robo.Line.OutLineAngle);
             } else{ //una delle condizioni sopra non e' verificata
                   robo.Line.BallOut = False;
                   OnBall(1.2,robo.Ball.bdegrees);
             }
        }
     }

   } //end loop
} //end main
