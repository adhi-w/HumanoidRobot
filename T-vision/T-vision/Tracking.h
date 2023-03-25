#include <highgui.h> 
#include <cxcore.h> 
#include <cv.h> 
#include <math.h> 
#include <stdlib.h>
#include "BlobResult.h"

#define rad2deg (180/(4*atan((double)1)))

CvFont font;
int *x_m, *y_m, *x_c, *y_c, *x_y, *y_y; int x_o; int y_o;
long int a_target, dist;

void tulis(char q, int a, int b)
{	
	/*if(q=='M') { x_m=a; y_m=b; }
	else if(q=='C') { x_c=a; y_c=b; }
	else if(q=='Y') { x_y=a; y_y=b; }
	else */
		if(q=='O') { x_o=a; y_o=b; }
}

IplImage* detect(IplImage* img)
{
	FILE *fp;
	double Curve_Parameters[4];

	fp = fopen("E:\\Lab Project\\TeenSize\\TeenCalibrate\\Parameters.txt", "r");
	
	IplImage* hsv = cvCreateImage(cvGetSize(img), 8, 3);
    cvCvtColor(img, hsv, CV_RGB2HSV);

	int a;
	for(a = 0; a < 10; a++)
	{
		float temp;
		if(fscanf(fp,"%f\n", &temp) != EOF)
		{
			Curve_Parameters[a] = double(temp);
		}
		else
			break;
	}
	fclose(fp);
		
	for(int i = 0; i < hsv->width; i++)
	{
		for(int j = 0; j < hsv->height; j++)
		{
			uchar *ptr = cvPtr2D(hsv, j, i);
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
				img->imageData[img->widthStep*j + img->nChannels*i + 0] = (unsigned char)255;				
				img->imageData[img->widthStep*j + img->nChannels*i + 1] = (unsigned char)255;				
				img->imageData[img->widthStep*j + img->nChannels*i + 2] = (unsigned char)0;
				//cvCircle(img1, cvPoint(i, j), 1, cvScalar(0, 255, 255), -1);
 			}
		}		
	}	
	cvSmooth(img,img, CV_GAUSSIAN, 5, 5 );
	cvReleaseImage(&hsv);
	return img;
}

IplImage* ThreshImage(IplImage* image,int hmax, int smax, int vmax, int hmin,int smin, int vmin)
{
	CvSize size = cvGetSize(image);

	IplImage* HSV = cvCreateImage(cvGetSize(image), 8, 3);
    cvCvtColor(image, HSV, CV_BGR2HSV);

	IplImage* Threshold = cvCreateImage(cvGetSize(image), 8, 1);
	CvScalar hsv_min = cvScalar(hmin, smin, vmin,0);
	CvScalar hsv_max = cvScalar(hmax, smax, vmax,0);
	cvInRangeS(HSV, hsv_min, hsv_max,  Threshold); 
	
	//cvSmooth(Threshold,Threshold, CV_MEDIAN,1,1 );
	//	cvAdaptiveThreshold(Threshold, Threshold, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, 101, 1);

	cvThreshold(Threshold,Threshold,150,255,CV_THRESH_BINARY);
	cvEqualizeHist(Threshold,Threshold);
	cvSmooth(Threshold,Threshold, CV_GAUSSIAN, 5, 5 );

	//morphological opening (remove small objects from the foreground)
	cvErode(Threshold,Threshold,NULL,4);
	cvDilate(Threshold,Threshold,NULL,4);

	//morphological closing (fill small holes in the foreground)
	cvDilate(Threshold,Threshold,NULL,4);
	cvErode(Threshold,Threshold,NULL,4);
//	cvCanny(Threshold,Threshold,60,80,3);
	
	cvReleaseImage(&HSV);
    return Threshold; 
}

void tracking(IplImage* image, IplImage* image2,char z)
{	
	CBlobResult blobs;    
	CBlob *currentBlob;   
	CvPoint pt1, pt2;  
	CvRect cvRect;  
	int midX[30],midY[30],mX,mY;
	char label[20];

	CvPoint p1,p2;	
	int data,largest=0;

	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, .8, .8, 0, 1, 8);

	//cvNot(image,image);
	// Detect the white blobs from the black background  
            blobs = CBlobResult( image, NULL, 0 );    
      
            // Exclude white blobs smaller than the given value (10)    
            // The bigger the last parameter, the bigger the blobs need    
            // to be for inclusion    
            blobs.Filter( blobs,B_EXCLUDE,CBlobGetArea(),B_LESS,10 ); 
			      
            // Attach a bounding rectangle for each blob discovered  
            int num_blobs = blobs.GetNumBlobs();

            for ( int i = 0; i < num_blobs; i++ )    
            {                 
                currentBlob = blobs.GetBlob( i ); 
                cvRect = currentBlob->GetBoundingBox();  
      
                pt1.x = cvRect.x;  
                pt1.y = cvRect.y;  
                pt2.x = cvRect.x + cvRect.width;  
                pt2.y = cvRect.y + cvRect.height; 

				midX[i]=cvRect.width*0.5+pt1.x;
				midY[i]=cvRect.height*0.5+pt1.y;

				data= cvRect.width + cvRect.height;
				
				if(data>largest)
				{
					largest=data;
					p1=pt1;
					p2=pt2;
					mX=cvRect.width*0.5+pt1.x;
					mY=cvRect.height*0.5+pt1.y;
				}
				
                // Attach bounding rect to blob in orginal video input  
				// if(z=='M'||z=='C')
				//	cvRectangle( image2, pt1, pt2, cvScalar(0, 0, 255, 0),1, 8,0 );
				
				/*cvCircle( image2, cvPoint(midX[i],midY[i]),3, CV_RGB(0,255,0), -1, 8, 0 );
				sprintf(label,"%c.%d",z,i);
				cvPutText(image2, label, cvPoint(midX[i],midY[i]),&font,CV_RGB(0,255,0));*/
			}	
			if(z=='O'||z=='Y')
			{	
				cvRectangle( image2, p1, p2, cvScalar(0, 128, 200, 0),1, 8,0 );
				cvCircle( image2, cvPoint(mX,mY),3, CV_RGB(0,255,0), -1, 8, 0 );
				sprintf(label,"%c,%d,%d",z,mX,mY);
				cvPutText(image2, label, cvPoint(mX,mY),&font,CV_RGB(0,255,0));
			}
			tulis(z,mX,mY);
			/*cvLine(image2, cvPoint(160,116), cvPoint(160,124),cvScalar(255,0,0),2);
			cvLine(image2, cvPoint(156,120), cvPoint(164,120),cvScalar(255,0,0),2);*/

			cvLine(image2, cvPoint(240,156), cvPoint(240,164),cvScalar(255,0,0),2);
			cvLine(image2, cvPoint(236,160), cvPoint(244,160),cvScalar(255,0,0),2);
}

void circle(IplImage* image, IplImage* image2)
{
	IplImage* canny=cvCreateImage(cvGetSize(image), 8, 1);
	CvMemStorage* storage = cvCreateMemStorage(0);
	
	cvCanny(image, canny,200,220,3);

	CvSeq* circles = cvHoughCircles(image, storage, CV_HOUGH_GRADIENT,1,
				image->height/4, 200, 50, 20, 80);
	for (int i = 0; i < circles->total; i++) 
	{ 
		float* p = (float*)cvGetSeqElem( circles, 1 ); 
		cvCircle( image2, cvPoint(cvRound(p[0]),cvRound(p[1])),3, CV_RGB(0,255,0), -1, 8, 0 ); 
		cvCircle( image2, cvPoint(cvRound(p[0]),cvRound(p[1])),cvRound(p[2]), CV_RGB(255,0,0), 3, 8, 0 );
	//	 CvPoint center = cvPoint(cvRound(p[0]),cvRound(p[1]));
	 //    CvScalar val = cvGet2D(thresholded, center.y, center.x);
	  //   if (val.val[0] < 1) continue;
       //      cvCircle(image2,  center, 3,             CV_RGB(0,255,0), -1, CV_AA, 0);
        //     cvCircle(image2,  center, cvRound(p[2]), CV_RGB(255,0,0),  3, CV_AA, 0);
	}
	cvReleaseMemStorage(&storage);
}

void shape(IplImage* image, IplImage* image2)
{
	CvSeq* contours;  
	CvSeq* result;  
	CvMemStorage *storage = cvCreateMemStorage(0); 

	//finding all contours in the image
	cvFindContours(image, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

	//iterating through each contour
	while(contours)
	{
		//obtain a sequence of points of contour, pointed by the variable 'contour'
		result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0);
		
		//if there are 4 vertices in the contour(It should be a quadrilateral)
		if(result->total==4 )
		{
		//iterating through each point
		CvPoint *pt[4];
		for(int i=0;i<4;i++){
		pt[i] = (CvPoint*)cvGetSeqElem(result, i);
		}

		//drawing lines around the quadrilateral
		cvLine(image2, *pt[0], *pt[1], cvScalar(0,255,0),4);
		cvLine(image2, *pt[1], *pt[2], cvScalar(0,255,0),4);
		cvLine(image2, *pt[2], *pt[3], cvScalar(0,255,0),4);
		cvLine(image2, *pt[3], *pt[0], cvScalar(0,255,0),4);
		}

		//obtain the next contour
		contours = contours->h_next;

	}
	
	 cvReleaseMemStorage(&storage);
}

void distance(float alpha_cam, float hWindow, float y) 
{	
	long int dy, h=hWindow/2;		
	float Hrobot=100;
	
	dy=rad2deg*tan(alpha_cam)*Hrobot; 	//distance robot to target	//center of x image
	dist = dy*(hWindow-y)/h;				//actual distance between robot & ball/other target
}

void set_camera_target(float w0,float h0, float xt, float yt)
{
	double px = fabs(xt-(w0/2));		//position robot towards center of camera
	double py = fabs(yt-h0);
	
	long int angle;

	angle = rad2deg*atan2(py,px);
	//a_target=angle;
	if (xt<=(w0/2)) a_target=90-angle;	// kuadran I
	else if (xt>(w0/2)) a_target=-angle;	// kuadran II	//Heading Robot
}