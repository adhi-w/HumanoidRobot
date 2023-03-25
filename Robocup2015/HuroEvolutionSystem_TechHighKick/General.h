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
#define COM_PORT 2//2
#define BUNDRATE 1000000

//BODY UART---------------------------
#define B_COM_PORT 9//5//3//7//3//1
#define B_BUNDRATE 115200

//COMPASS UART------------------------
#define COMPASS_COM_PORT 5
#define COMPASS_BUNDRATE 115200

//Camera info--------------------------
#define CAM_Hor_ID 21
#define CAM_Ver_ID 20
#define CAM_DEVICE 0

// Camera para-------------------------
#define P_VF 391.242
#define H_RO 78.5-6
#define L_OF 6.5   

#define CAM_HOR_U_BOUDARY 612
#define CAM_HOR_U_CHANGE 545
#define CAM_HOR_L_BOUDARY 412
#define CAM_HOR_L_CHANGE 495
#define CAM_VER_U_BOUDARY 600//1869
#define CAM_VER_L_BOUDARY 745
#define CAM_VER_CHANGE 735

#define CAM_ZOOMIN_WIDTH 320
#define CAM_ZOOMIN_HEIGHT 240
#define CAM_ZOOMOUT_WIDTH 640
#define CAM_ZOOMOUT_HEIGHT 480
#define CAM_HOR_CENTER 512
#define CAM_VER_CENTER 512

//Game State------------------------------
#define INIT 0
#define READY 1
#define SET 2
#define PLAY 3

//Robot Behavior--------------------------
#define Stop 201
#define Forward 202
#define TurnRight 203
#define TurnLeft 204
#define ShiftLeft 206
#define ShiftRight 207
#define Backward 208
#define StepIt 209
#define KICKLEFT 210
#define KICKLEFT45 211
#define KICKRIGHT 212
#define KICKRIGHT45 213
#define FALLDOWNLEFT 214
#define FALLDOWNRUGHT 215
#define STANDUP 216
#define SurrandRight 217
#define SurrandLeft 218


//general function----------------------------------
double Dist(double dx, double dy);
double Q_Dist(CvPoint pt1, CvPoint pt2);
void Q_Byte2Deg(double *hor, double *ver);
double Deg2Rad(double ang);
double Rad2Deg(double ang);
bool Estimate(CvPoint pt, double hor, double ver, int *rx, int *ry, int width, int height);
void One_Step_Filter(int input, double param, int &out);
#endif