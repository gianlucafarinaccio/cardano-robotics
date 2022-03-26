#include <main.h>
#include <Drv4Mot2019_v1.c>
#include <RoboFunctions_v5p.c>


#INT_EXT2
void  EXT2_isr(void) {
   WlRilevata = True;
   disable_interrupts(INT_EXT2);

}

#INT_RDA2
void  RDA2_isr(void) {
   AllData = SerialRoutine();
}


void main() {
   MotorInit();
   RoboSetup();

   while(TRUE){

     if(WlRilevata){
        robo.Line.DataByte = Line_Read(0x18);
        if(robo.Line.DataByte==0 ||robo.Line.DataByte==255){
           Dir(0,0,VERSONORD);
           robo.Line.BallOut = False;
        } else{
           robo.Line.LineRoutineDir = LineRoutine(robo.Line.DataByte);
           Dir(1.35,robo.Line.LineRoutineDir,VERSONORD);
           robo.Line.BallOut = True;
        }
           WlRilevata = False;
           clear_interrupt(INT_EXT2);
           enable_interrupts(INT_EXT2);
     } else{
         robo.ToF.sud = ToF_Read(0x16);
         if(!robo.Line.BallOut){
               robo.Line.BallOut=False;
               if((robo.ToF.sud>25) && (robo.Goal.kick==False)){
                  OnGoal(1.35,robo.Goal.gdegrees);
               } else{
                  if(robo.Ball.bdegrees != 999 && robo.Line.BallOut==0){
                     if(((robo.Ball.bdegrees<20||robo.Ball.bdegrees>340)&&(robo.ToF.sud<210))||((robo.Ball.bdistance<40)&&(robo.Ball.bdegrees<20||robo.Ball.bdegrees>340))){
                        Dir(1.35,0,VERSONORD);
                        robo.Goal.kick=True;
                     } else{
                         robo.Goal.kick=False;
                         if(robo.Ball.bdegrees<=180){ Dir(1.3,90,VERSONORD); }
                         else{ Dir(1.3,270,VERSONORD); }
                       }
                  } else{
                     robo.Goal.kick=False;
                     if(robo.Goal.gdegrees>190||robo.Ball.bdegrees<170){
                        if(robo.Goal.gdegrees<=180){ Dir(1.35,90,VERSONORD); }
                        else{ Dir(1.35,270,VERSONORD); }
                     } else{
                          Dir(0,0,VERSONORD);
                     }
                  }
               }

         } else{
            if(robo.Goal.gdegrees>190||robo.Goal.gdegrees<170){
               robo.Line.BallOut=True;
               ReturnGoal(1.35,robo.Goal.gdegrees);
            } else{
                  robo.Line.BallOut=False;
                  Dir(0,0,0);
            }
         }




      }
   } //end loop
} //end main
