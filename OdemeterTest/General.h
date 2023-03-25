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

#include "math.h"

//Main state
#define INIT 0
#define READY 1
#define SET 2
#define PLAY 3
//Head state
#define HEAD_INIT 0
#define SCAN_BALL 1
#define SCAN_FULL 2
#define SCAN_OBST 3
#define SCAN_GOAL 4
#define HEAD_LOCK 5
#define HEAD_DOWN 6
//Game state
#define COLOR_TEST 0
#define GAME_START 1
#define RIGHT_SHIFT 2
#define MOVE_BEHIND_BALL 3
#define MOVE_AROUND_BALL 4
#define DRIBBLE 5
#define DRIBBLE_OBST 6
#define SEARCH_OBSTACLE 7
#define DESIRE_SECTOR 8
#define SEARCH_GOAL 9
#define DESIRE_KICK 10
#define KICK_BALL 11
#define DESIRE_DEFEND 12
//Image state
#define BALL 0
#define OBSTACLE 1
#define GOAL 2
#define GOAL_KEEPER 3
//VIEW
#define VIEW_ANGLE 80

//MAP
#define field_width 900
#define field_height 600
#define line_offset 20

//HEAD UART
#define COM_PORT 2//2
#define BUNDRATE 1000000

//Camera info--------------------------
#define HEAD_ID_H 4
#define HEAD_ID_V 3
#define CAM_DEVICE 0

// Camera para-------------------------
#define P_VF 330
#define H_RO 118   //near 43
#define L_OF 17   //near 10

#define CAM_HOR_U_BOUDARY 3584
#define CAM_HOR_L_BOUDARY 512
#define CAM_VER_U_BOUDARY 2100//1869
#define CAM_VER_L_BOUDARY 2500

#define CAM_WIDTH 320
#define CAM_HEIGHT 240
#define CAM_HOR_CENTER 2048
#define CAM_VER_CENTER 2048

//general function----------------------------------
double Dist(double dx, double dy);
double Distance(CPoint pt1, CPoint pt2);
double Q_Dist(CvPoint pt1, CvPoint pt2);
void Q_Byte2Deg(double *hor, double *ver);
double Deg2Rad(double ang);
double Rad2Deg(double ang);
bool Estimate(int x, int y, double Head_hor, double Head_ver, int *rx, int *ry);
void One_Step_Filter(int input, double param, int &out);
int Round(double Get_Value);
double CosineAngle(double l1, double l2, double l3);

#endif // !defined(AFX_GENERAL_H__6F7129F5_4066_4EDC_AFE2_877C63BF47B6__INCLUDED_)
