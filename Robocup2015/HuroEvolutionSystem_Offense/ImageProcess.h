// ImageProcess.h: interface for the CImageProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESS_H__E81CE6BF_7585_48EA_A959_8946E2F10A1B__INCLUDED_)
#define AFX_IMAGEPROCESS_H__E81CE6BF_7585_48EA_A959_8946E2F10A1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cv.h"
#include "highgui.h"
#include "KalmanFilter.h"
#include "General.h"
#include <vector>

#define DEFAULT_SIZE 50
#define MAX_AREA 25000

class CImageProcess  
{
public:
	
	struct FCC_OBJECT
	{
		CvPoint Center;
		CvPoint Bottom;
		int Area;	
		CvMat* Points;	
	};
	typedef CArray<FCC_OBJECT, FCC_OBJECT> FCC_ARRAY;

public:
	void FieldRecognition(IplImage *img, IplImage *gray);
	void Procedure(bool img_catch, int hor, int ver, bool iskickmotion);
	CImageProcess();
	virtual ~CImageProcess();

public:
	void BallRecognition(IplImage *binary, CvPoint upt, CvPoint bpt);
	static void On_Mouse(int event, int x, int y, int flags , void* param);

	bool isCameraOpenning();
	CvCapture *Capture;
	IplImage *Img;
	
	int GrabTime;
	int Cam_Width;
	int Cam_Height;
	int ImageMode;

	double Curve_parameters_field[4];
	double Curve_parameters_white[4];
	bool isCameraOpen;
	bool Q_Ball;
	bool isBallGone;
	bool isKalmanTracking;

	// Mask's parameters
	// Right side
	// Right up point y parameters(x = 319)
	double P_ru00, P_ru01, P_ru02;
	double P_ru10, P_ru11;
	double P_ru20;
	// Right down point x parameters(y = 239)
	double P_rd00, P_rd01, P_rd02;
	double P_rd10, P_rd11;
	double P_rd20;
	// Left side
	// Left up point y parameters(x = 0)
	double P_lu00, P_lu01, P_lu02;
	double P_lu10, P_lu11;
	double P_lu20;
	// Left down point x parameters(y = 239)
	double P_ld00, P_ld01, P_ld02;
	double P_ld10, P_ld11;
	double P_ld20;
	// Get Head Position use for mask
	short Head_hor, Head_ver;
	short WhiteComponent;

public:
	void SetMaskParameters();
	void KalmanTracking(IplImage *resize_img, IplImage *gray_scale, int ver);
	void ShoulderMask(IplImage *gray_scale);
	void SaveImage();
	double CircleFunction(CvPoint cp, CvPoint pt, int radius);
	void ColorPatternMatch(IplImage *src, IplImage *binary, IplImage *sobel, CvPoint top, CvPoint bottom, CvPoint cp, int r);
	void ObjectSelection(IplImage *src, IplImage *binary, FCC_ARRAY *fcc_arr);
	bool isCorrectObject(IplImage* ball_roi, int NumOfWhite, int width, int height);
	void ObjectClassfied(IplImage *src, IplImage *binary, FCC_ARRAY *fcc_arr, CvPoint offset, int &range);
	void Write2Pixel(IplImage *img, int x, int y, unsigned char val);
	int GetPixel(int x, int y, IplImage *img);
	void FastCC(IplImage *img, FCC_ARRAY *fcc_arr);
	bool isKalmanStable(int rx, int ry, int kx, int ky);
	
	CvPoint GetPt;
	CvPoint Virtual_pt;
	CvPoint Predict_pt;
	CvPoint Ball_pos;
	CvPoint topleft, bottomright;
	CvPoint L_kick_Region;
	CvPoint R_kick_Region;

	CvPoint L_Kick_Region_topleft;
	CvPoint L_Kick_Region_downright;

	CvPoint R_Kick_Region_topleft;
	CvPoint R_Kick_Region_downright;

	int RegionOfKick;
	int BallRadius;


	bool isGetBall;	
	bool isGetPT;
	bool isImgDone;	

	int BallGone_cont;

	CKalmanFilter *pKalman;

private:

	
	char *ColorScale;
	char *BinaryScale;
	char *Window_ROI;
	char *Window_Mouse;
	char *ImageFile;
	


};

#endif // !defined(AFX_IMAGEPROCESS_H__E81CE6BF_7585_48EA_A959_8946E2F10A1B__INCLUDED_)
