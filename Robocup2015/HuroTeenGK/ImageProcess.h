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

#define CAM_DEVICE 0
#define CAM_WIDTH 320
#define CAM_HEIGHT 240

#define DEFAULT_SIZE 50 

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
	void Procedure(bool img_catch, int hor, int ver);
	CImageProcess();
	virtual ~CImageProcess();

public:
	void BallRecognition(IplImage *binary, CvPoint upt, CvPoint bpt);
	static void On_Mouse(int event, int x, int y, int flags , void* param);

	bool isCameraOpenning();
	CvCapture *Capture;
	IplImage *Img;
	
	double Curve_parameters_field[4];
	double Curve_parameters_white[4];
	bool isCameraOpen;
	bool Q_Ball;
	bool isBallGone;

	// Mask's parameters
	// Right side
	// Right up point y parameters(x = 319)
	double A_ru_y, B_ru_y, C_ru_y, D_ru_y, E_ru_y, F_ru_y;
	// Right down point x parameters(y = 239)
	double A_rd_x, B_rd_x, C_rd_x, D_rd_x, E_rd_x, F_rd_x;
	// Left side
	// Left up point y parameters(x = 0)
	double A_lu_y, B_lu_y, C_lu_y, D_lu_y, E_lu_y, F_lu_y;
	// Left down point x parameters(y = 239)
	double A_ld_x, B_ld_x, C_ld_x, D_ld_x, E_ld_x, F_ld_x;
	// Get Head Position use for mask
	short Head_hor, Head_ver;
	short WhiteComponent;

public:
	void KalmanTracking(IplImage *resize_img, IplImage *gray_scale, int ver);
	void ShoulderMask(IplImage *gray_scale);
	void SaveImage();
	double CircleFunction(CvPoint cp, CvPoint pt, int radius);
	void ColorPatternMatch(IplImage *src, IplImage *binary, IplImage *sobel, CvPoint top, CvPoint bottom, CvPoint cp, int r);
	void ObjectSelection(IplImage *src, IplImage *binary, FCC_ARRAY *fcc_arr);
	bool isCorrectObject(int NumOfWhite, int width, int height);
	void ObjectClassfied(IplImage *src, IplImage *binary, FCC_ARRAY *fcc_arr, CvPoint offset, int &range);
	void Write2Pixel(IplImage *img, int x, int y, unsigned char val);
	int GetPixel(int x, int y, IplImage *img);
	void FastCC(IplImage *img, FCC_ARRAY *fcc_arr);
	bool isKalmanStable(int rx, int ry, int kx, int ky);
	
	CvPoint Virtual_pt;
	CvPoint Predict_pt;
	CvPoint Ball_pos;
	CvPoint topleft, bottomright;
	bool isGetBall;
	CvPoint GetPt;
	bool isGetPT;
	bool isImgDone;

private:

	
	char *ColorScale;
	char *BinaryScale;
	char *Window_ROI;
	char *Window_Mouse;
	char *ImageFile;
	

	CKalmanFilter *pKalman;
};

#endif // !defined(AFX_IMAGEPROCESS_H__E81CE6BF_7585_48EA_A959_8946E2F10A1B__INCLUDED_)
