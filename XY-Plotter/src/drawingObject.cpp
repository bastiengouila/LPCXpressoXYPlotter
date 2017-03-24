/*
 * drawingObject.cpp
 *
 *  Created on: 5.10.2016
 *      Author: Olli
 */
#include "drawingObject.h"


DrawingObject::DrawingObject(){

}
DrawingObject::~DrawingObject(){

}
/*The object needs to be created after setting up the board
 * If drawing object isn't initialized pen will not work
 * if port and pin aren't defined it will just power the green board LED
 * takes in
 * 		*pSCT1		=	pointer of the SCT for example LPC_SCT0
 * 		movable1	=	Movable pinassign for example SWM_SCT0_OUT0_O
 * 		cycleLenght	=	cycle lenght as Hertz Hz
 * 		dutyCycle	=	dutyCycleLengt as percent %
 */

void DrawingObject::init(LPC_SCT_T *pSCT1, CHIP_SWM_PIN_MOVABLE_T  movable1,
		int cycleLenght1, bool tool1, int port, uint8_t pin){
		pSCT=pSCT1;
			movable =movable1;
			tool=tool1;
			cycleLenght=1000000/cycleLenght1;

			Chip_SCTPWM_Init(pSCT);

			pSCT->CONFIG |= (1 << 17); // two 16-bit timers, auto limit
			pSCT->CTRL_L |= (72 - 1) << 5; // set prescaler, SCTimer/PWM clock = 1 MHz

			Chip_SWM_MovablePortPinAssign(movable, (uint8_t)port, pin);

			pSCT->RES = (pSCT->RES & ~(3 << (0 << 1))) | (0x01 << (0 << 1));						//clear config resolution register

							/* Set and Clear do not depend on direction */

			pSCT->OUTPUTDIRCTRL = (pSCT->OUTPUTDIRCTRL & ~(3 << (0 << 1)));

				pSCT->MATCH[0].L = cycleLenght -1;
				pSCT->MATCHREL[0].L = cycleLenght -1;



				if(tool){//pen
					//Chip_SWM_DisableFixedPin(SWM_FIXED_ADC1_2 );
				pSCT->MATCHREL[1].L = cycleLenght*0.05;// at the start 95% duty cycle
				}
				else{//laser
					//Chip_SWM_DisableFixedPin(SWM_FIXED_DAC_OUT);
					pSCT->MATCHREL[1].L = 0;//laser is shut down
				}

				pSCT->EVENT[0].STATE = 0xFFFFFFFF; 			// event 0 happens in all states
				pSCT->EVENT[0].CTRL = (1 << 12); 			// match 0 condition only

				pSCT->EVENT[1].STATE = 0xFFFFFFFF; 			// event 1 happens in all states
				pSCT->EVENT[1].CTRL = (1 << 0) | (1 << 12); // match 1 condition only

				pSCT->OUT[0].SET = (1 << 0); 				// event 0 will set SCTx_OUT0
				pSCT->OUT[0].CLR = (1 << 1);				// event 1 will clear SCTx_OUT0

				pSCT->CTRL_L &= ~(1 << 2); 					// unhalt it by clearing bit 2 of CTRL reg

}

void DrawingObject::write(uint8_t power=0){
			laserPower=(float)power/255;
			laserPower=cycleLenght*laserPower;
	if(!tool){//using laser

		pSCT->MATCHREL[1].L = laserPower;
	}
	else{// pencil
		if(power==160){//up
		pSCT->MATCHREL[1].L = cycleLenght*0.05;
		}
		else if(power==90){//down
			pSCT->MATCHREL[1].L = cycleLenght*0.09;
		}
	}
}
