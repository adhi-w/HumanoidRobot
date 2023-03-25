// ImageProcess.h: interface for the CImageProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESS_H__7E3BF6C3_BEAD_458C_8A88_6609402F87E5__INCLUDED_)
#define AFX_IMAGEPROCESS_H__7E3BF6C3_BEAD_458C_8A88_6609402F87E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "General.h"

#include "cv.h"
#include "highgui.h"
#include "math.h"

#include "afxtempl.h"

class CImageProcess  
{

private:

	char *ColorScale;
	char *BinaryScale;

public:
	
	CvCapture *Capture;
	IplImage *Img;

	double Curve_Parameters_ball[4];
	double Curve_Parameters_obstacle[4];
	double Curve_Parameters_field[4];

	bool ImgCatchFinish;

public:
	
	struct FCC_OBJECT
	{
		CvPoint Center;
		CvPoint Bottom;
		int Area;	
		CvMat* Points;	
	};

public:

	CArray<CvPoint, CvPoint> buttom_pts[3];

	CvPoint Pos_ball;
	CvPoint Pos_ball_center;
	CvPoint Obstacle_center[2];
	CvPoint GoalKeeper_center;
	bool Q_ball;
	bool Q_Obstacle;
	bool Q_GaolKeeper;
	
typedef CArray<FCC_OBJECT, FCC_OBJECT> FCC_ARRAY;
public:
	void ViewCalibrate();
	void FieldRecognize(IplImage *img, IplImage *gray);
	void FastCC_Obst(double **src, int &imgHeight,int &imgWidth);
	void FindObstacleCenter(IplImage *img, IplImage *gray, double **darray);
	int GetPixel(int x, int y, IplImage *img);
	void Write2Pixel(IplImage *img, int x, int y, unsigned char val);

	void ObjectClassfied(IplImage *src, FCC_ARRAY *fcc_arr, int *x, int *y, int *Area);

	void FastCC(IplImage *img, FCC_ARRAY *fcc_arr);
	double **CreateDynamicArray(IplImage *img);
	void GetBottom(IplImage *img, IplImage *gray, int headstate);
	bool BallRecognize(IplImage *img, IplImage *gray);
	bool ObstacleRecognize(IplImage *img, IplImage *gray);
	void BinaryProcess(IplImage *img, IplImage *gray);
	bool OpenCapture();
	void ProcessProcedure(int state, int headstate, int catchbegin, CvPoint topleft, CvPoint downright);
	CImageProcess();
	virtual ~CImageProcess();

};

#endif // !defined(AFX_IMAGEPROCESS_H__7E3BF6C3_BEAD_458C_8A88_6609402F87E5__INCLUDED_)
