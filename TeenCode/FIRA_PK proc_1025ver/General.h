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
#define COM_PORT 3//2
#define BUNDRATE 1000000

//BODY UART---------------------------
#define B_COM_PORT 6//1
#define B_BUNDRATE 115200

//Camera info--------------------------
#define CAM_Hor_ID 4
#define CAM_Ver_ID 3
#define CAM_DEVICE 0

// Camera para-------------------------
#define P_VF 330
#define H_RO 118   //near 43
#define L_OF 17   //near 10

#define CAM_HOR_U_BOUDARY 2650
#define CAM_HOR_L_BOUDARY 1300
#define CAM_VER_U_BOUDARY 2100//1869
#define CAM_VER_L_BOUDARY 2500

#define CAM_WIDTH 320
#define CAM_HEIGHT 240
#define CAM_HOR_CENTER 2048
#define CAM_VER_CENTER 2048

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

//Decision(robot motion)-------------------------------
#define STAND 201
#define Forward 202
#define TurnRight 203
#define TurnLeft 204
#define ForwardSLOW 205
#define ShiftLeft 206
#define ShiftRight 207
#define Backward 208
#define StepIt 209
#define TurnRightS 213
#define TurnLeftS 214
#define SurroundBallRight 220
#define SurroundBallLeft 221
#define SMALLKICKRIGHT 250
#define SMALLKICKLEFT 251
#define KICKRIGHT 252
#define KICKLEFT 253
#define KICKLEFT45 254
#define KICKRIGHT45 255
#define VectorMode 300


//general function----------------------------------
double Dist(double dx, double dy);
double Q_Dist(CvPoint pt1, CvPoint pt2);
void Q_Byte2Deg(double *hor, double *ver);
double Deg2Rad(double ang);
double Rad2Deg(double ang);
bool Estimate(CvPoint pt, double hor, double ver, int *rx, int *ry);
void One_Step_Filter(int input, double param, int &out);
#endif