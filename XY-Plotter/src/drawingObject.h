/*
 * drawingObject.h
 *
 *  Created on: 5.10.2016
 *      Author: Olli
 */

#ifndef DRAWINGOBJECT_H_
#define DRAWINGOBJECT_H_
#define pentool 1
#define lasertool 0
#include "chip.h"

class DrawingObject {
public:

	DrawingObject();
	~DrawingObject();


void init(LPC_SCT_T *pSCT1=LPC_SCT0, CHIP_SWM_PIN_MOVABLE_T  movable1=SWM_SCT0_OUT0_O,
		int cycleLenght1=50, bool tool1=pentool, int port=0, uint8_t pin=3);
	void write(uint8_t power);

	LPC_SCT_T *pSCT;
	CHIP_SWM_PIN_MOVABLE_T movable;
	int cycleLenght=50;
	float laserPower;
	int attenuationPower=0;
	bool tool=pentool;

private:

};



#endif /* DRAWINGOBJECT_H_ */
