/*
 * GParser.cpp
 *
 *  Created on: 7.10.2016
 *      Author: Olli
 */
#include "GParser.h"
/*
 * example of an gcode line
 * G1 X7.50 Y107.50 A0
 * M4 137
 * G1 X157.50 Y107.50 A0
 */

GParser::GParser(){

}
bool GParser::takeCommand(int commandChar,int &num){

	if(commandChar!=10){
		if(num >= maxCommandLenght){
			num=maxCommandLenght-1;
		}
		if(commandChar!=77 && num == 0){//sometimes it sends a command like 0 0 0M10 aka. {48,32,48,32,48,77,49,48}
			if(commandChar !=71 && num==0){
				num=0;
				return false;//the first character was neither G or M so we just return
			}
		}
		command[num]=commandChar;

		for(int i=0;i < maxCommandLenght; i++){
		commando[i]=command[i];
		}
		db.echo=commando;
		num++;
	}
	else{
		Board_UARTPutSTR("\n\r");

		if(num==0){//someone fucking up with keyboard and onlu enter has been pressed
			num=0;
			return false;
		}
		for(int a=0;a<dataI;a++){
			for(int b=0; b< dataJ; b++){
				data[a][b]=-1;
			}
		}
		separate();
		parse();

		for(int b=0; b< maxCommandLenght; b++){
			command[b]=0;
		}
		num=0;

		return true;
	}
	//TEMPORARY STUFF!!!
	return false;
}
void GParser::separate(){

	int i=0,b=0;

	for(int a=0; a < dataI;a++){
		while(command[i] != 0 && command[i]!=32){
			if(b > dataJ){
				i++;
			}
			else{
				data[a][b]=command[i];
				b++;
				i++;
			}
		}
		if(command[i] == 0 || command[i]==32){
			i++;
		}
		b=0;

	}

}
void GParser::parse(){
	int a;
	int divider=1;
	int d=0;
	double Xplane=0;
	double Yplane=0;
	db.Xplane=0;
	db.Yplane=0;
	db.Gcode=0;
	db.Acode=0;
	if(data[0][0]=='G'){
		db.Gcode=data[0][1]-48;
		if(data[0][2]!=-1){
			db.Gcode=db.Gcode*10;
			db.Gcode+=data[0][2]-48;
		}
		db.Gcode+=900;
		divider=100;
	}
	else if(data[0][0]=='M'){
			db.Gcode=data[0][1]-48;
			if(data[0][2]!=-1){
						db.Gcode=db.Gcode*10;
						db.Gcode+=data[0][2]-48;
					}
			db.Gcode+=100;
	}
	if(data[3][0]=='A'){
		switch (data[3][1]){
		case 48:
			db.Acode=A0;
			break;
		case 49:
			db.Acode=A1;
			break;
		}
	}
	if(data[1][0] > 45){
		for(int j = dataJ ; j >= 0 ; j--){
			if(data[1][j] >=48 && data[1][j]< 58){
				Xplane+=(data[1][j]-48)*pow(10,d);
				d++;
			}
		}

	}
	d=0;
	if(data[1][1]==45){
		Xplane=-Xplane;
		}
	if(data[2][0] > 45){
		for(int j =dataJ;j >= 0;j--){
			if(data[2][j] >=48 && data[2][j]< 58){
				Yplane+=(data[2][j]-48)*pow(10,d);
				d++;
			}
		}

	}
	if(data[2][1]==45){
		Yplane=-Yplane;
}

		d=0;

		db.Xplane= Xplane/divider;//divider = 1 if M-code 100 if Gcode
			db.Yplane=Yplane/divider;//divider = 1 if M-code 100 if Gcode
}

debugEvent GParser::getDebugEvent(){
	return db;
}
const char* GParser::getJoku(){
	return joku;
}






