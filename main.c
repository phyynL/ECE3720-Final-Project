#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

unsigned int noteperiod=45866;//starting at c4
unsigned int half=22933;
/*      REFERENCE TABLE FOR NOTES
        freq        period count=12MHz/f
                   //max is 65535
c4=     261.63      45866
c#/db4= 277.18      43293
d4=     293.66      40864
d#/eb4= 311.13      38569
e4=     329.63      36404
f4=     349.23      34361
f#/gb4= 369.99      32433
g4=     392.00      30612
g#/ab4= 415.30      28895
a4=     440.00      27273
a#/bb4= 466.16      25742
b4=     493.88      24297
c5=     523.25      22934

*/
//this runs at 160 bpm btw
//can get more octaves if we mult by 2 or div by 2

//might need pt5 jumper like lab
void initialize(void){
  PLLCTL|=0X40;
  CLKSEL&=0X7F;           //from lab
  SYNR=0X02;
  REFDV=0X00;
  while(!(CRGFLG&0x08));
  CLKSEL|=0X80;
  
  TSCR1=0X80;/* TIMER ENABLE */
  TSCR2=0X01; //PRESCALAR=2
  TIOS|=0X60;//CHANNEL 5 OUTPUT COMPARE+channel 6 delay output compare
  TCTL1&=~0X0C;//DISCONNECT pt5 PIN (SILENT)  
}

void setnote(unsigned int period) {       //sets the note that is playing
    noteperiod=period;
    half=period/2;
    TFLG1=0X20;
    TC5=TCNT+half;
    TCTL1|=0x04;
}

void speakerun(void) {                    //runs the note 
    if(TFLG1&0X20){ //was flag set?
      TFLG1=0X20;  //clear flag
      TC5+=half;   //toggles in half, creating the note period
    }
}

void delay_ms(unsigned int ms) {
  while (ms>0) {
    TC6=TCNT+12000;  //1MS AT 12MHZ
    TFLG1=0X40;     //CLEAR CHANNEL 6
    while (!(TFLG1&0X40)) {
      speakerun();
      _FEED_COP(); 
    }
    ms--;
  }
}

#define quarter 375
#define halfnote 750
#define whole 1500
#define eighth 188    //16th would be 94 if needed

#define NotelowC    45866
#define NoteCs      43293
#define NoteD       40864
#define NoteDs      38569
#define NoteE       36404
#define NoteF       34361
#define NoteFs      32433
#define NoteG       30612
#define NoteGs      28895
#define NoteA       27273
#define NoteAs      25742
#define NoteB       24297
#define NotehighC   22934
#define rest    0

#define gap 20


void play (unsigned int note, unsigned int length) {
  if (note==rest) {
    TCTL1&=~0X0C;
    delay_ms(length);
  } 
  else {
    setnote(note);
    delay_ms(length-gap);
    TCTL1&=~0X0C;
    delay_ms(gap);
  }
}

void demo (void) {
  play(NotelowC, halfnote);
  play(NoteD, quarter);
  play(NoteE, quarter);
  play(NoteF, quarter);
  play(NoteG, quarter);
  play(NoteA, quarter);
  play(NoteB, quarter);
  play(NotehighC, halfnote);
  play(NoteB, quarter);
  play(NoteA, quarter);
  play(NoteG, quarter);
  play(NoteF, quarter);
  play(NoteE, quarter);
  play(NoteD, quarter);
  
  play(NotelowC, halfnote);
  play(NoteE, halfnote);  
  play(NoteG, halfnote);  
  play(NotehighC, halfnote);  
  play(NoteG, halfnote);  
  play(NoteE, halfnote);  
  play(NotelowC, whole);
  
  play(NoteD, halfnote);
  play(NoteF, halfnote);
  play(NoteA, halfnote);
  play(NotehighC, halfnote);
  play(NoteA, halfnote);
  play(NoteF, halfnote);
  play(NoteD, whole);
  
  play(NoteE, halfnote);  
  play(NoteG, halfnote);  
  play(NoteB, halfnote);  
  play(NotehighC, halfnote);  
  play(NoteB, halfnote);  
  play(NoteG, halfnote);  
  play(NoteE, whole);
  
  play(NoteD, halfnote);
  play(NoteF, halfnote);
  play(NoteA, halfnote);
  play(NotehighC, halfnote);
  play(NoteA, halfnote);
  play(NoteF, halfnote);
  play(NoteD, whole);  

  play(NotelowC, halfnote);
  play(NoteE, halfnote);  
  play(NoteG, halfnote);  
  play(NotehighC, whole);  
    
  TCTL1&=~0X0C;//turns off speaker at end
}

void main(void) {
  /* put your own code here */
  initialize();


	EnableInterrupts;
	play(NotelowC, whole); //check tune
  demo();

  for(;;) {
    speakerun();
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
