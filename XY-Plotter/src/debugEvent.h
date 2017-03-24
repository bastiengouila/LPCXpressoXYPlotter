/*
 * debugEvent.h
 *
 *  Created on: 10.10.2016
 *      Author: Olli
 */

#ifndef DEBUGEVENT_H_
#define DEBUGEVENT_H_


/*
 * Xplane			=	The position where to move on the X-axis
 * Yplane			=	The position where to move on the Y-axis
 * Gcode			=	The three numbered G-code command everything starting with 9 is G everything starting with 1 is M
 * 						For Example 901 = G1 and 110 = M10
 * Acode 			=	Dont really know what's this for yet but it's at the end of the G-code command
 * echo				=	The received command as it was we really don't need it at functional project
 * 						but it's good for testing
 */
struct debugEvent{
		float Xplane;
		float Yplane;
		int Gcode;
		int Acode;
		const char* echo;
	};

#endif /* DEBUGEVENT_H_ */
