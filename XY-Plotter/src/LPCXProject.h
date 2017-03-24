/*
 * LPCXProject.h
 *
 *  Created on: 23 sept. 2016
 *      Author: basti_000
 */

#ifndef LPCXPROJECT_H_
#define LPCXPROJECT_H_

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here
#include "FreeRTOS.h"
#include "task.h"
#include "board_api.h"
#include "semphr.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctime"
#include "math.h"
#include "debugEvent.h"

#define XAxisTotalDistanceMm 310
#define YAxisTotalDistanceMm 350

int CalibrationX (void);

int CalibrationY (void);

int CoordinatesXStep (float fXmillimeters, int iTotalNbStep);

int CoordinatesYStep (float fYmillimeters, int iTotalNbStep);

float CoordinatesXMilli (int iNbSteps, int iTotalNbStep);

float CoordinatesYMilli (int iNbSteps, int iTotalNbStep);

void RIT_start(int count, int us);

int MoveXAxisAcceleration(int iStartPos, int iFinalPos,int MaxSpeed);

int MoveYAxisAcceleration(int iStartPos, int iFinalPos,int MaxSpeed);

bool MoveXYPlotter(float fXmillimeters,float fYmillimeters, int NbTotalStepX, int NbTotalStepY);

bool MoveXYPlotterLaser(float fXCoordinates,float fYCoordinates, int NbTotalStepX, int NbTotalStepY);

float getDistance(float futureX, float futureY);

#endif /* LPCXPROJECT_H_ */
