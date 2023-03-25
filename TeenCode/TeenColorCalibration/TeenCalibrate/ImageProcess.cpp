#include "StdAfx.h"
#include "ImageProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CImageProcess::CImageProcess(void)
{	Capture = cvCaptureFromCAM(0);
}

CImageProcess::~CImageProcess(void)
{
}

void CImageProcess::PhotoRequired()
{
	Img = cvLoadImage("Picture 197.jpg", CV_LOAD_IMAGE_COLOR);
	ImgHSV = cvCreateImage(cvGetSize(Img), 8, 3);
	cvCvtColor(Img, ImgHSV, CV_RGB2HSV);
}

void CImageProcess::ProcessProcedure()
{
//	Img = cvQueryFrame(Capture);
//	cvSmooth(Img, Img, CV_GAUSSIAN, 9, 9);
	Img = cvLoadImage("Picture 197.jpg", CV_LOAD_IMAGE_COLOR);
	ImgHSV = cvCreateImage(cvGetSize(Img), 8, 3);

	cvCvtColor(Img, ImgHSV, CV_RGB2HSV);
}

void CImageProcess::ReleaseImage(IplImage *img)
{
	cvReleaseImage(&img);
}

void CImageProcess::SetROI(IplImage *img, int x, int y, bool flag, bool finish)
{
	if(img)
	{
		if(!finish)
		{
			if(!flag)																						//left button mode
			{	
				Boundspts.Add(cvPoint(x, y));																//save pts to array
				for(int i = 0; i < Boundspts.GetSize(); i++)
				{
					cvCircle(img, cvPoint(Boundspts[i].x, Boundspts[i].y), 5, cvScalar(0, 0, 0), -1);		//draw circle
				}
			}//endif if(!elimate_flag)
		}//endif if(!finish)
		else
		{
			cvRectangle(img, cvPoint(Boundspts[0].x, Boundspts[0].y), cvPoint(Boundspts[1].x, Boundspts[1].y), cvScalar(0, 0, 0), 1);
		}
	}
}

void CImageProcess::ColorScaleCollection(bool flag)
{
	int i, j;
	if(flag)
	{
		for(i = Boundspts[0].x; i <= Boundspts[1].x; i++)
		{
			for(j = Boundspts[0].y; j <= Boundspts[1].y; j++)
			{
				uchar *ptr = cvPtr2D(ImgHSV, j, i);
				int v = *(ptr+2);
				int s = *(ptr+1);
				int h = *ptr;

				if(v > 0)
					HSVsacle[2].Add(v);
				if(s > 0)
					HSVsacle[1].Add(s);
				if(h > 0)
					HSVsacle[0].Add(h);
			}
		}
		IplImage *temp_draw_gray = cvCreateImage(cvSize(300, 300), 8, 3);
		cvZero(temp_draw_gray);

		if(HSVsacle[0].GetSize() > 0)
		{
			for(i = 0; i < HSVsacle[0].GetSize(); i++)
			{
				int yp = HSVsacle[0][i];
				int xp = HSVsacle[1][i];

				temp_draw_gray->imageData[yp*temp_draw_gray->widthStep + xp*temp_draw_gray->nChannels+2] = (unsigned char)255;
			}
		}
		//cvErode(temp_draw_gray, temp_draw_gray, NULL, 1);
		cvDilate(temp_draw_gray, temp_draw_gray, NULL, 3);
		for(i = 0; i < temp_draw_gray->width; i++)
		{
			for(j = 0; j < temp_draw_gray->height; j++)
			{
				uchar *ptr = cvPtr2D(temp_draw_gray, j, i);
				int val = *(ptr+2);
				if(val == 255)
				{
					hs_pt.Add(cvPoint(i, j));
				}
			}
		}
		cvReleaseImage(&temp_draw_gray);
	}
}

void CImageProcess::PlotSamples()
{
	int i, j;
	IplImage *histogram_gray = cvCreateImage(cvSize(300, 300), 8, 3);
	cvZero(histogram_gray);

	
	if(hs_pt.GetSize() > 0)
	{
		for(i = 0; i < hs_pt.GetSize(); i++)
		{
			double x_temp = hs_pt[i].x/255.0;
			
			double x3 = x_temp*x_temp*x_temp;
			double x2 = x_temp*x_temp;
			double x =  x_temp;
			
			//int py = cvRound(-0.000002*x3 + 0.0008*x2 - 0.2151*x + 41.036);
			int py;
			int py1, py2;
			py = (Curve_Parameters[3]*x3 + Curve_Parameters[2]*x2 + Curve_Parameters[1]*x + Curve_Parameters[0])*255;
			py1 = py + 10;
			py2 = py - 5;

			cvCircle(histogram_gray, cvPoint(hs_pt[i].x, hs_pt[i].y), 1, cvScalar(0, 0, 255), 1);
			cvCircle(histogram_gray, cvPoint(x*255, py), 1, cvScalar(0, 255, 0), 1);
// 				cvCircle(histogram_gray, cvPoint(x*255, py1[k]), 1, cvScalar(0, 255, 0), 1);
// 				cvCircle(histogram_gray, cvPoint(x*255, py2[k]), 1, cvScalar(0, 255, 0), 1);
		}
		
	}


	cvNamedWindow("Histogram_gray", CV_WINDOW_AUTOSIZE);
	cvShowImage("Histogram_gray", histogram_gray);

	cvReleaseImage(&histogram_gray);
}

void CImageProcess::ImageProcess()
{
	IplImage *img1 = cvCreateImage(cvSize(320, 240), 8, 3);
	IplImage *img_hsv = cvCreateImage(cvSize(320, 240), 8, 3);
	cvResize(Img, img1);
	cvResize(ImgHSV, img_hsv);

	int i, j;
	for(i = 0; i < img_hsv->width; i++)
	{
		for(j = 0; j < img_hsv->height; j++)
		{
			uchar *ptr = cvPtr2D(img_hsv, j, i);
			int h = *ptr;
			int s = *(ptr+1);
			int v = *(ptr+2);

			double x_temp = s/255.0;

			double x3 = x_temp*x_temp*x_temp;
			double x2 = x_temp*x_temp;
			double x =  x_temp;

			int y1 = int((Curve_Parameters[3]*x3 + Curve_Parameters[2]*x2 + Curve_Parameters[1]*x + Curve_Parameters[0])*255);
			//printf("%d\n", y1);
			
			if(h > y1-10 && h < y1+10)
			{
				img1->imageData[img1->widthStep*j + img1->nChannels*i + 0] = (unsigned char)255;
				
				img1->imageData[img1->widthStep*j + img1->nChannels*i + 1] = (unsigned char)255;
				
				img1->imageData[img1->widthStep*j + img1->nChannels*i + 2] = (unsigned char)0;
				//cvCircle(img1, cvPoint(i, j), 1, cvScalar(0, 255, 255), -1);
 			}	

		}
	}	
	cvNamedWindow("Image", CV_WINDOW_AUTOSIZE);
	cvShowImage("Image", img1);
	cvReleaseImage(&img1);
	cvReleaseImage(&img_hsv);
}


