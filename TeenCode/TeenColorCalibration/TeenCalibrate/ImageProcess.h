#pragma once

#include "cv.h"
#include "highgui.h"

#include <afxtempl.h>

class CImageProcess
{
public:
	
	CvCapture *Capture;
	IplImage *Img;
	IplImage *ImgHSV;

	CArray<CvPoint, CvPoint>	 Boundspts;			//boundary points

	CArray<int, int>	 HSVsacle[3];			
	CArray <CvPoint, CvPoint> hs_pt;
	CArray <CvPoint, CvPoint> Boundary;

	double Curve_Parameters[4];
	int debug_y;

public:
	void PhotoRequired();
	void ImageProcess();
	void PlotSamples();
	void ColorScaleCollection(bool flag);
	void SetROI(IplImage *img, int x, int y, bool flag, bool finish);
	void ReleaseImage(IplImage *img);

	void ProcessProcedure();
	CImageProcess();
	virtual ~CImageProcess();
};
