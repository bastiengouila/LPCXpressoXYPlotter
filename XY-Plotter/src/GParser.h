/*
 * GParser.h
 *
 *  Created on: 7.10.2016
 *      Author: Olli
 */

#ifndef GPARSER_H_
#define GPARSER_H_
#include "FreeRTOS.h"
#include "task.h"
#include "chip.h"
#include "board.h"
#include <math.h>
#include "debugEvent.h"

#define G0 900
#define G1 901
#define G2 902
#define G28 928
#define A0 400
#define A1 401
#define M10 110
#define M4  104

#define dataI 4
#define dataJ 7
#define maxCommandLenght 25

class GParser{
public:

	GParser();
	bool takeCommand(int commandChar,int &num);
	void parse();
	void separate();
	debugEvent getDebugEvent();
	const char* getJoku();

	debugEvent db;
	char command[maxCommandLenght];
	char commando[maxCommandLenght];
	const char* joku;

	char* part;
private:
	int data[dataI][dataJ];
};


#endif /* GPARSER_H_ */
