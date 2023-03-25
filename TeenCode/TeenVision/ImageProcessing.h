// ImageProcessing.h: interface for the ImageProcessing class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSING_H__8CA33703_FD1F_457E_B652_B6FE30E26FB4__INCLUDED_)
#define AFX_IMAGEPROCESSING_H__8CA33703_FD1F_457E_B652_B6FE30E26FB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cv.h"
#include "highgui.h"
#include "math.h"
#include "KalmanFilter.h"
#include "afxtempl.h"
#include "General.h"


class ImageProcessing  
{
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
		bool Q_ball;
		bool findBall;

		CvFont font;
		
		typedef CArray<FCC_OBJECT, FCC_OBJECT> FCC_ARRAY;

public:
	void ImageProcedure(int capimage);
	void ImageReleasing(IplImage *img);

	bool ImageCapture();
	bool BallRecognize(IplImage *img, IplImage *gray);
	bool FieldRecognize(IplImage *img, IplImage *gray);

	int GetPixel(int x, int y, IplImage *img);
	void Write2Pixel(IplImage *img, int x, int y, unsigned char val);

	void ObjectClassfied(IplImage *src, IplImage *binary, FCC_ARRAY *fcc_arr, CvPoint offset, int &range, int type);
	void Circle(IplImage *src, IplImage *img, int &x, int &y);
	void BallRecognition(IplImage *binary, CvPoint upt, CvPoint bpt);

	void FastCC(IplImage *img, FCC_ARRAY *fcc_arr);
	double **CreateDynamicArray(IplImage *img);

	bool isKalmanStable(int rx, int ry, int kx, int ky);
	bool isCorrectObject(int NumOfWhite, int width, int height);

	static void On_Mouse(int event, int x, int y, int flags , void* param);

public:
	CvPoint Virtual_pt;
	CvPoint Predict_pt;
	bool isGetBall;
	bool isStable;
	bool isGetPT;
	bool isObject;
	
	ImageProcessing();
	virtual ~ImageProcessing();

public:
	CvCapture *capture;
	IplImage* streaming;
	double Ball_Parameter[4];
	double Field_Parameter[4];




private:
	char *Window_Mouse;
	char *Window_ROI;
	char *Window_Capture;
	char *Window_Binary;
	
	CvPoint GetPt;
	

	CKalmanFilter *Kalman;
};

#endif // !defined(AFX_IMAGEPROCESSING_H__8CA33703_FD1F_457E_B652_B6FE30E26FB4__INCLUDED_)
