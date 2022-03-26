/*
   RoboGlobal_v4.c
   Gianluca Farinaccio, 2019
*/


/* Define Hardware of robot
   Reference: AnxaPic80 v4.1, MainBoard_18/19
*/
#define LD2         PIN_D0 //LD2, AnxaPic80 v4.1
#define LD3         PIN_D1 //LD3, AnxaPic80 v4.1
#define BUZZER      PIN_D2 //BUZZER, AnxaPic80 v4.1
#define BUTTON      PIN_B0 //INT0, AnxaPic80 v4.1

#define USR_LED      PIN_J0 //D5, MainBoard_18/19
#define USR_BUTTON   PIN_B4 //BUTTON1, MainBoard_18/19

#define ALERT        PIN_B2 //MainBoard_18/19
#define OUT1         PIN_H0 //MainBoard_18/19
#define OUT2         PIN_H1 //MainBoard_18/19
#define OUT3         PIN_H2 //MainBoard_18/19

#define BALL_ANALOG  PIN_H3 //MainBoard_18/19
#define BALL_DIGITAL PIN_B3 //MainBoard_18/19
#define ROLLER       PIN_A3
#define KICKER       PIN_J3

/* Define data structure of robot */
typedef struct {
  unsigned int16 nord;
  unsigned int16 sud;
  unsigned int16 est;
  unsigned int16 ovest;
}D;

typedef struct {
  unsigned int16 bdegrees;
  unsigned int16 bdistance;
  unsigned int16 bvalue;
}B;

typedef struct {
  unsigned int16 gdegrees;
  unsigned int16 gdistance;
}G;

typedef struct {
  unsigned int16 VirtualNord;
  unsigned int16 SavedVirtualNord;
  unsigned int16 VirtualDegrees;
  signed int16 DeltaDegrees;
}C;

typedef struct{
  int8 DataByte;
  unsigned int16 LineRoutineDir;
  unsigned int16 OutLineAngle;
  int1           BallOut;
  int8           BallOutCycle;
  int8           ReturnFieldCycle;
}W;

typedef struct{
  D ToF;
  B Ball;
  G Goal;
  C Compass;
  W Line;
}data;
data robo;
/* Esempio di scrittura nella struct (robo.Ball.bdegrees) */

/*  System function */
void Beep(){
  OUTPUT_HIGH(BUZZER);
  delay_ms(50);
  OUTPUT_LOW(BUZZER);
  delay_ms(50);
}

void DoubleBeep(){
  OUTPUT_HIGH(BUZZER);
  delay_ms(50);
  OUTPUT_LOW(BUZZER);
  delay_ms(50);

  OUTPUT_HIGH(BUZZER);
  delay_ms(50);
  OUTPUT_LOW(BUZZER);
  delay_ms(50);
}

/* General Purpose Variables */
int1 AllData = False;
int1 WlRilevata = False;
#define VERSOPALLA 2
#define VERSOPORTA 1
#define VERSONORD  0

/* Roller Variables */
int1 level = 0;
unsigned int16 tHigh = 0, tLow = 0, initRoller = 0;

/* Kicker Variables */
#define msOverflow 50
int1 kickAvaible = 1;
