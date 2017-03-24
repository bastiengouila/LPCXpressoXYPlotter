/*
 * LPCXProjectFunctions.cpp
 *
 *  Created on: 23 sept. 2016
 *      Author: basti_000
 */

#include "LPCXProject.h"
#define fAccuracy  0.01
volatile uint32_t RIT_count;
xSemaphoreHandle sbRIT = xSemaphoreCreateBinary();
bool bDirection = false;
bool bXMotor = false;
bool bYMotor = false;
int const NormalPps = 12500;
bool bAxisX = false;
bool bAxisY = false;
float fSavedMilliXaxis = 0;
float fSavedMilliYaxis = 0;

//Count the number of steps between switches and put the block at the origin of the X axis
int CalibrationX (void){
	int iPps = 15000;
	int iCount = 0;
	int i = 0;
	int iMargin =0;
	bool bDir = false;
	bool bSwitch1 = false;
	bool bSwitch2 = false;
	bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 29);//Xmin
	bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 9);//Xmax

	bAxisX = true;

	while(bSwitch1 == true){
		Chip_GPIO_SetPinState(LPC_GPIO, 1, 0, false);
		bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 29);//Xmin
		bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 9);//Xmax
		RIT_start(4,iPps);
	}
	while(bSwitch1 == false){
		Chip_GPIO_SetPinState(LPC_GPIO, 1, 0, true);
		bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 29);//Xmin
		bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 9);//Xmax
		RIT_start(4,iPps);
		iMargin+=4;
	}
	iMargin += 200;
	bDir = true;

	while(i<2){

		bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 29);//Xmin
		bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 9);//Xmax

		if(bSwitch1 == false && bSwitch2 == false){
			while(bSwitch1 == false || bSwitch2 == false){
				vTaskDelay(configTICK_RATE_HZ);
				bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 29);//Xmin
				bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 9);//Xmax
			}
		}

		else if((bSwitch1 == false && bDir == false)||(bSwitch2 == false && bDir == true)){
			bDir = !bDir;
			i++;
		}

		else if (bSwitch1 == true || bSwitch2 == true){
			if(bDir == false){
				Chip_GPIO_SetPinState(LPC_GPIO, 1, 0, bDir); // X Dir
				RIT_start(4,iPps);
				iCount += 4;
			}
			else{
				Chip_GPIO_SetPinState(LPC_GPIO, 1, 0, bDir);// X Dir
				RIT_start(4,iPps);
				iCount += 4;
			}
		}
	}
	iCount = (iCount - 2*iMargin)/i;

	if(!bSwitch1){bDirection = true;}
	if(!bSwitch2){bDirection = false;}
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 0, bDirection);// X Dir

	for (int i = 0;i< iMargin;i+=4){
		RIT_start(4,iPps);
	}

	fSavedMilliXaxis = 0;
	bAxisX = false;
	return iCount;
}

//Count the number of steps between switches and put the block at the origin of the Y axis
int CalibrationY (void){
	int iPps = 15000;
	int iCount = 0;
	int i = 0;
	int iMargin =0;
	bool bDir = false;
	bool bSwitch1 = false;
	bool bSwitch2 = false;
	bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 1, 3);//Ymin
	bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 0);//Ymax

	bAxisY = true;

	while(bSwitch1 == true){
		Chip_GPIO_SetPinState(LPC_GPIO, 0, 28, false); // YDir
		bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 1, 3);//Ymin
		bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 0);//Ymax
		RIT_start(4,iPps);
	}
	while(bSwitch1 == false){
		Chip_GPIO_SetPinState(LPC_GPIO, 0, 28, true); // YDir
		bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 1, 3);//Ymin
		bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 0);//Ymax
		RIT_start(4,iPps);
		iMargin+=4;
	}
	iMargin += 200;
	bDir = true;

	while(i<2){

		bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 1, 3);//Ymin
		bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 0);//Ymax

		if(bSwitch1 == false && bSwitch2 == false){
			while(bSwitch1 == false || bSwitch2 == false){
				vTaskDelay(configTICK_RATE_HZ);
				bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 1, 3);//Ymin
				bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 0);//Ymax
			}
		}

		else if((bSwitch1 == false && bDir == false)||(bSwitch2 == false && bDir == true)){
			bDir = !bDir;
			i++;
		}

		else if (bSwitch1 == true || bSwitch2 == true){
			if(bDir == false){
				Chip_GPIO_SetPinState(LPC_GPIO, 0, 28, bDir); // YDir
				RIT_start(4,iPps);
				iCount += 4;
			}
			else{
				Chip_GPIO_SetPinState(LPC_GPIO, 0, 28, bDir); // YDir
				RIT_start(4,iPps);
				iCount += 4;
			}
		}
	}
	iCount = (iCount - 2*iMargin)/i;

	if(!bSwitch1){bDirection = true;}
	if(!bSwitch2){bDirection = false;}
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 28, bDirection); // YDir

	for (int i = 0;i<iMargin;i+=4){
		RIT_start(4,iPps);
	}
	fSavedMilliYaxis = 0;
	bAxisY = false;
	return iCount;
}


//Move the block on the X axis and return the new position
int MoveXAxisAcceleration(int iStartPos, int iFinalPos, int MaxSpeed){

	int iCurrentStepX = iStartPos;
	int iStepMove = iFinalPos - iStartPos;
	int iPps = MaxSpeed;
	bool bSwitch1 = false;
	bool bSwitch2 = false;

	bAxisX = true;

	if(iStepMove > 0){
		bDirection = true;
	}
	else if (iStepMove < 0){
		bDirection = false;
	}
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 0, bDirection);// X Dir

	while(iCurrentStepX != iFinalPos){

		bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 29);//Xmin
		bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 9);//Xmax
		if (bSwitch1 == true && bSwitch2 == true){

			//Occurs when an error happen
			if(/*iCurrentStepX >= iNbTotalStep || */iCurrentStepX <= 0){
				break;
			}

			//Going to the left
			else if(bDirection == false){

				RIT_start(1,iPps);
				iCurrentStepX = iCurrentStepX - 1;
			}

			//Going to the right
			else if(bDirection == true){

				RIT_start(1,iPps);
				iCurrentStepX = iCurrentStepX + 1;
			}
		}
	}
	bAxisX = false;
	return iCurrentStepX;
}

//Move the block on the Y axis and return the new position
int MoveYAxisAcceleration(int iStartPos, int iFinalPos,int MaxSpeed){

	int iCurrentStepY = iStartPos;
	int iStepMove = iFinalPos - iStartPos;
	int iPps = MaxSpeed;
	bool bSwitch1 = false;
	bool bSwitch2 = false;

	bAxisY = true;

	if(iStepMove > 0){
		bDirection = true;
	}
	else if (iStepMove < 0){
		bDirection = false;
	}
	Chip_GPIO_SetPinState(LPC_GPIO, 0 , 28, bDirection);//YDir

	while(iCurrentStepY != iFinalPos){

		bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 1, 3);//Ymin
		bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 0);//Ymax
		if (bSwitch1 == true && bSwitch2 == true){
			//Occurs when an error happen
			if(/*iCurrentStepY >= iNbTotalStep || */iCurrentStepY <= 0){
				break;
			}
			//Going to the left
			else if(bDirection == false){

				RIT_start(1,iPps);
				iCurrentStepY = iCurrentStepY - 1;
			}
			//Going to the right
			else if(bDirection == true){

				RIT_start(1,iPps);
				iCurrentStepY = iCurrentStepY + 1;
			}
		}
	}
	bAxisY = false;
	return iCurrentStepY;
}

//The function converts the millimeters coordinates into steps coordinates for the X axis
int CoordinatesXStep (float fXmillimeters, int iTotalNbStep){
	int iCoord = 0;
	float fCoeff = iTotalNbStep / XAxisTotalDistanceMm;
	iCoord = fXmillimeters*fCoeff;
	if(fXmillimeters*fCoeff - iCoord >= 0.5){
		iCoord++;
	}
	return iCoord;
}

//The function converts the millimeters coordinates into steps coordinates for the Y axis
int CoordinatesYStep (float fYmillimeters, int iTotalNbStep){
	int iCoord = 0;
	float fCoeff = iTotalNbStep / YAxisTotalDistanceMm;
	iCoord = fYmillimeters*fCoeff;
	if(fYmillimeters*fCoeff - iCoord >= 0.5){
		iCoord++;
	}
	return iCoord;
}

//The function converts the steps coordinates into millimeters coordinates for the X axis
float CoordinatesXMilli (int iNbSteps, int iTotalNbStep){
	float fCoord = 0;
	float fCoeff = iTotalNbStep / XAxisTotalDistanceMm;
	fCoord = iNbSteps/fCoeff;
	return fCoord;
}

//The function converts the steps coordinates into millimeters coordinates for the Y axis
float CoordinatesYMilli (int iNbSteps, int iTotalNbStep){
	float fCoord = 0;
	float fCoeff = iTotalNbStep / YAxisTotalDistanceMm;
	fCoord = iNbSteps/fCoeff;
	return fCoord;
}

bool MoveXYPlotter(float fXCoordinates,float fYCoordinates, int NbTotalStepX, int NbTotalStepY){
	bool bRetVal = true;
	float dx = fXCoordinates - fSavedMilliXaxis;
	float dy = fYCoordinates - fSavedMilliYaxis;
	float Y = 0;
	int iSpeed = NormalPps;
	float distance = sqrt((dx*dx)+(dy*dy));
	float distanceRemain = distance;
	float YPrevious = fSavedMilliYaxis;
	int iXCheck = 0;
	int iYCheck = 0;

	if(dx > 0){
		for (float X = fSavedMilliXaxis + 0.01 ; X <= fXCoordinates ;X = X + 0.01 ) {
			Y = fSavedMilliYaxis + dy * (X - fSavedMilliXaxis) / dx;//calculate the new Y coordinates every 0.01mm on X
			//distance remaining
			distanceRemain = sqrt( (((X-0.01)-fXCoordinates)*((X-0.01)-fXCoordinates)) + ((YPrevious-fYCoordinates)*(YPrevious-fYCoordinates)) );
			//decrease the speed while the drawing block is under 5mm from the end or from the beginning
			if(distanceRemain <= 5 || distanceRemain >= distance-5){
				if(iSpeed <=NormalPps){
					iSpeed = NormalPps;
				}
				else{
					iSpeed /=1.001;
				}
			}
			//increase the speed
			else{
				if(iSpeed >=NormalPps*5){
					iSpeed = NormalPps*5;//Maximum 5*12500
				}
				else{
					iSpeed *=1.001;
				}
			}
			//Move X and Y and remember the previous Y value for the newt cycle
			iXCheck = MoveXAxisAcceleration(CoordinatesXStep((X-0.01),NbTotalStepX),CoordinatesXStep(X,NbTotalStepX),iSpeed);
			iYCheck = MoveYAxisAcceleration(CoordinatesYStep(YPrevious,NbTotalStepY),CoordinatesYStep(Y,NbTotalStepY),iSpeed);
			YPrevious = Y;
		}
	}
	else if(dx < 0){
		for (float X = fSavedMilliXaxis - 0.01 ; X >= fXCoordinates ;X = X - 0.01 ) {
			Y = fSavedMilliYaxis + dy * (X - fSavedMilliXaxis) / dx;

			distanceRemain = sqrt( (((X+0.01)-fXCoordinates)*((X+0.01)-fXCoordinates)) + ((YPrevious-fYCoordinates)*(YPrevious-fYCoordinates)) );

			if(distanceRemain <= 5 || distanceRemain >= distance-5){
				if(iSpeed <=NormalPps){
					iSpeed = NormalPps;
				}
				else{
					iSpeed /=1.001;
				}
			}
			else{
				if(iSpeed >=NormalPps*5){
					iSpeed = NormalPps*5;
				}
				else{
					iSpeed *=1.001;
				}
			}

			iXCheck = MoveXAxisAcceleration(CoordinatesXStep((X+0.01),NbTotalStepX),CoordinatesXStep(X,NbTotalStepX),iSpeed);
			iYCheck = MoveYAxisAcceleration(CoordinatesYStep(YPrevious,NbTotalStepY),CoordinatesYStep(Y,NbTotalStepY),iSpeed);
			YPrevious = Y;
		}
	}
	else{
		if(dy>0){
			for(float j = fSavedMilliYaxis + 0.01; j <= fYCoordinates; j = j + 0.01){
				distanceRemain = sqrt((j-0.01-fYCoordinates)*(j-0.01-fYCoordinates));

				if(distanceRemain <= 5 || distanceRemain >= distance-5){
					if(iSpeed <=NormalPps){
						iSpeed = NormalPps;
					}
					else{
						iSpeed /=1.001;
					}
				}
				else{
					if(iSpeed >=NormalPps*5){
						iSpeed = NormalPps*5;
					}
					else{
						iSpeed *=1.001;
					}
				}
				iYCheck = MoveYAxisAcceleration(CoordinatesYStep(j-0.01,NbTotalStepY),CoordinatesYStep(j,NbTotalStepY),iSpeed);
			}
		}
		else if(dy<0){
			for(float j = fSavedMilliYaxis - 0.01; j>= fYCoordinates; j = j - 0.01){
				distanceRemain = sqrt((j+0.01-fYCoordinates)*(j+0.01-fYCoordinates));

				if(distanceRemain <= 5 || distanceRemain >= distance-5){
					if(iSpeed <=NormalPps){
						iSpeed = NormalPps;
					}
					else{
						iSpeed /=1.001;
					}
				}
				else{
					if(iSpeed >=NormalPps*5){
						iSpeed = NormalPps*5;
					}
					else{
						iSpeed *=1.001;
					}
				}
				iYCheck = MoveYAxisAcceleration(CoordinatesYStep(j+0.01,NbTotalStepY),CoordinatesYStep(j,NbTotalStepY),iSpeed);
			}
		}
	}

	fSavedMilliXaxis = fXCoordinates;
	fSavedMilliYaxis = fYCoordinates;
	return bRetVal;
}

bool MoveXYPlotterLaser(float fXCoordinates,float fYCoordinates, int NbTotalStepX, int NbTotalStepY){
	bool bRetVal = true;
	float dx = fXCoordinates - fSavedMilliXaxis;
	float dy = fYCoordinates - fSavedMilliYaxis;
	float Y = 0;
	int iSpeed = 4000;
	float distance = sqrt((dx*dx)+(dy*dy));
	float distanceRemain = distance;
	float YPrevious = fSavedMilliYaxis;
	int iXCheck = 0;
	int iYCheck = 0;

	if(dx > 0){
		for (float X = fSavedMilliXaxis + 0.01 ; X <= fXCoordinates ;X = X + 0.01 ) {
			Y = fSavedMilliYaxis + dy * (X - fSavedMilliXaxis) / dx;

			iXCheck = MoveXAxisAcceleration(CoordinatesXStep((X-0.01),NbTotalStepX),CoordinatesXStep(X,NbTotalStepX),iSpeed);
			iYCheck = MoveYAxisAcceleration(CoordinatesYStep(YPrevious,NbTotalStepY),CoordinatesYStep(Y,NbTotalStepY),iSpeed);
			YPrevious = Y;
		}
	}
	else if(dx < 0){
		for (float X = fSavedMilliXaxis - 0.01 ; X >= fXCoordinates ;X = X - 0.01 ) {
			Y = fSavedMilliYaxis + dy * (X - fSavedMilliXaxis) / dx;

			iXCheck = MoveXAxisAcceleration(CoordinatesXStep((X+0.01),NbTotalStepX),CoordinatesXStep(X,NbTotalStepX),iSpeed);
			iYCheck = MoveYAxisAcceleration(CoordinatesYStep(YPrevious,NbTotalStepY),CoordinatesYStep(Y,NbTotalStepY),iSpeed);
			YPrevious = Y;
		}
	}
	else{
		if(dy>0){
			for(float j = fSavedMilliYaxis + 0.01; j <= fYCoordinates;j+0.01){

				iYCheck = MoveYAxisAcceleration(CoordinatesYStep(j-0.01,NbTotalStepY),CoordinatesYStep(j,NbTotalStepY),iSpeed);
			}
		}
		else if(dy<0){
			for(float j = fSavedMilliYaxis - 0.01; j>= fYCoordinates;j-0.01){

				iYCheck = MoveYAxisAcceleration(CoordinatesYStep(j+0.01,NbTotalStepY),CoordinatesYStep(j,NbTotalStepY),iSpeed);
			}
		}
	}

	fSavedMilliXaxis = fXCoordinates;
	fSavedMilliYaxis = fYCoordinates;
	return bRetVal;
}

float getDistance(float futureX, float futureY){
	float dx1= futureX-fSavedMilliXaxis;
	float dy1= futureY-fSavedMilliYaxis;
	return sqrt((dx1*dx1)+(dy1*dy1));
}

extern "C" {
void RIT_IRQHandler(void)
{

	// This used to check if a context switch is required
	portBASE_TYPE xHigherPriorityWoken = pdFALSE;
	// Tell timer that we have processed the interrupt.
	// Timer then removes the IRQ until next match occurs
	Chip_RIT_ClearIntStatus(LPC_RITIMER); // clear IRQ flag
	if(RIT_count > 0) {
		RIT_count--;
		// do something useful here...
		//bool bSwitch1 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 27);
		//bool bSwitch2 = Chip_GPIO_GetPinState(LPC_GPIO, 0, 28);
		//if(bSwitch1 || bSwitch2){

		
		if(bAxisX == true){
			Chip_GPIO_SetPinState(LPC_GPIO, 0, 24, bXMotor);// X Motor
			bXMotor = !bXMotor;
		}
		
		if(bAxisY == true){
			Chip_GPIO_SetPinState(LPC_GPIO, 0, 27, bYMotor);// Y Motor
			bYMotor = !bYMotor;
		}
		//}
	}
	else {
		Chip_RIT_Disable(LPC_RITIMER); // disable timer
		// Give semaphore and set context switch flag if a higher priority task was woken up
		xSemaphoreGiveFromISR(sbRIT, &xHigherPriorityWoken);
	}
	// End the ISR and (possibly) do a context switch
	portEND_SWITCHING_ISR(xHigherPriorityWoken);
}
}

void RIT_start(int count, int pps)
{
	uint64_t cmp_value;
	// Determine approximate compare value based on clock rate and passed interval
	cmp_value = (uint64_t) Chip_Clock_GetSystemClockRate()/ (uint64_t)pps;
	// disable timer during configuration
	Chip_RIT_Disable(LPC_RITIMER);
	RIT_count = count;
	// enable automatic clear on when compare value==timer value
	// this makes interrupts trigger periodically
	Chip_RIT_EnableCompClear(LPC_RITIMER);
	// reset the counter
	Chip_RIT_SetCounter(LPC_RITIMER, 0);
	Chip_RIT_SetCompareValue(LPC_RITIMER, cmp_value);
	// start counting
	Chip_RIT_Enable(LPC_RITIMER);
	// Enable the interrupt signal in NVIC (the interrupt controller)
	NVIC_EnableIRQ(RITIMER_IRQn);
	// wait for ISR to tell that we're done
	if(xSemaphoreTake(sbRIT, portMAX_DELAY) == pdTRUE) {
		// Disable the interrupt signal in NVIC (the interrupt controller)
		NVIC_DisableIRQ(RITIMER_IRQn);
	}
	else {
		// unexpected error
	}
}
