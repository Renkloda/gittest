#include <LPC21xx.H>

#include "timer_interrupts.h"
#include "led.h"
#include "keyboard.h"
#define   DETECTOR_bm (1<<10)

void Delay(int iDelayMseconds){
	
	unsigned int uiDelayCntr;
	unsigned int uiMsecondsCntr;
	
	for (uiMsecondsCntr = 0; uiMsecondsCntr < iDelayMseconds; uiMsecondsCntr++ ){
		for (uiDelayCntr = 0; uiDelayCntr < 7500; uiDelayCntr++ ){

		}
	}
}

enum ServoState {CALLIB, IDLE, IN_PROGRESS};

struct Servo
{
	enum ServoState eState;
	unsigned int uiCurrentPosition;
	unsigned int uiDesiredPosition;
};

struct Servo sServo;

void DetectorInit(void){
	
	IO0DIR = IO0DIR &(~(DETECTOR_bm)); 
}

enum DetectorState {ACTIVE, INACTIVE};

enum DetectorState eReadDetector(void){
	if ((IO0PIN & DETECTOR_bm) == 0){
		return ACTIVE;
  }
	else {
		return INACTIVE;
	}

	
}




void Automat(void){
	
		switch(sServo.eState){
			case IDLE:
				if(sServo.uiCurrentPosition != sServo.uiDesiredPosition){
					sServo.eState = IN_PROGRESS;
				}
				else{
					sServo.eState = IDLE;
				}
				break;
			case IN_PROGRESS:
				if(sServo.uiCurrentPosition > sServo.uiDesiredPosition){
					sServo.uiCurrentPosition--;
					sServo.eState = IN_PROGRESS;
					LedStepLeft();
				}
				else if(sServo.uiCurrentPosition<sServo.uiDesiredPosition){
				
					sServo.uiCurrentPosition++;
					sServo.eState = IN_PROGRESS;
					LedStepRight();
				}
				else {
					sServo.eState = IDLE;
				}
				break;
			
			case CALLIB:
				if(eReadDetector()==INACTIVE){
					LedStepRight();
				}else{
					sServo.eState=IDLE;
					sServo.uiCurrentPosition=0;
					sServo.uiDesiredPosition=0;
				}
				break;
			default:
				break;
		}

}



void ServoCallib(void){
	
	sServo.eState=CALLIB;
	while(eReadDetector()==INACTIVE);
} 

void ServoInit(unsigned int uiServoFrequency){
	
	Timer0Interrupts_Init((1000000 / uiServoFrequency), &Automat);
	ServoCallib();
	
	LedInit();
	
} 

void ServoGoTo(unsigned int uiPosition){
	
	sServo.eState = IN_PROGRESS;
	sServo.uiDesiredPosition = uiPosition;
	
}
//sssssssssssssssss
int main (){
	
	DetectorInit();
	KeyboardInit();
	ServoInit(50);

	while(1){
	 	switch (eKeyboardRead()){
			case BUTTON_0:
				ServoCallib();
				break;
			case BUTTON_1:
				ServoGoTo(12);
				break;
			case BUTTON_2:
				ServoGoTo(24);
				break;
			case BUTTON_3:
				ServoGoTo(36);
				break;
			case RELEASED:
					break;
			default:
				break;
		}
	}
}
