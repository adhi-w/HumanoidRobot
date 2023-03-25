/*
				*** header file ***
		
	  define all variables for all of class 1~6
	
	  and the parameters is seldom be changed

	//*************************************\\
*/
#ifndef __GENERAL_H__
#define __GENERAL_H__

#include "cv.h"
#include "highgui.h"


//HEAD UART---------------------------
#define H_comPort 4
#define H_Baudrate 1000000

//BODY UART---------------------------
#define B_comPort 7
#define B_Baudrate 115200

//Camera info--------------------------
#define panHead_ID 23
#define tiltHead_ID 24
#define camDevice 1

// Camera para-------------------------
#define P_VF 330
#define H_RO 104   //near 43
#define L_OF 6   //near 10

#define hor_U	785
#define hor_L	239
#define ver_L	388
#define ver_U	524

#define camWidth	320
#define camHeight	240
#define DefaultSize	50

#define hor_init 512
#define ver_init 240

//Object------------------------------
#define BALL 0
#define GOAL 1
#define FIELD 2

//visualstate------------------------------
#define STOP 100
#define Ball_Searching 101
#define Ball_Tracking 102
#define BALL_Lock 103
#define Goal_Scan 104
#define Goal_Tracking 105 
#define Goal_Lock 106
#define Goal_Check 107
// 
// //Decision(robot motion)-------------------------------
// #define STAND 201
// #define Forward 202
// #define TurnRight 203
// #define TurnLeft 204
// #define ForwardSLOW 205
// #define ShiftLeft 206
// #define ShiftRight 207
// #define Backward 208
// #define StepIt 209
// #define TurnRightS 213
// #define TurnLeftS 214
// #define SurroundBallRight 220
// #define SurroundBallLeft 221
// #define SMALLKICKRIGHT 250
// #define SMALLKICKLEFT 251
// #define KICKRIGHT 252
// #define KICKLEFT 253
// #define KICKLEFT45 254
// #define KICKRIGHT45 255
// #define VectorMode 300


//general function----------------------------------
double Dist(double dx, double dy);
double Q_Dist(CvPoint pt1, CvPoint pt2);
void Q_Byte2Deg(double *hor, double *ver);
double Deg2Rad(double ang);
double Rad2Deg(double ang);
bool Estimate(CvPoint pt, double hor, double ver, int *rx, int *ry);
void One_Step_Filter(int input, double param, int &out);
#endif