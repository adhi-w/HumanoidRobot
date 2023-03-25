// ImageProcessing.cpp: implementation of the ImageProcessing class.
//
//////////////////////////////////////////////////////////////////////


///////////----------Note------------/////////////////
//This Program is just used for Orange ball detection /////////
///////////////////


#include "stdafx.h"
#include "TeenVision.h"
#include "ImageProcessing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageProcessing::ImageProcessing()
{	
	Kalman = new CKalmanFilter();

	Window_Mouse = "MouseEvent";
	Window_ROI = "ROI";
	Window_Capture = "Capture";
	Window_Binary = "Field";

	cvNamedWindow(Window_Capture,CV_WINDOW_AUTOSIZE);	cvMoveWindow("capturing",0,0);
	cvNamedWindow(Window_Binary,CV_WINDOW_AUTOSIZE);	cvMoveWindow("Field",0,340);
	cvNamedWindow(Window_ROI,CV_WINDOW_AUTOSIZE);	cvMoveWindow("camera",480,0);

	cvNamedWindow(Window_Mouse,CV_WINDOW_AUTOSIZE);	

	cvSetMouseCallback(Window_Mouse, On_Mouse,this);

	if(ImageCapture())
	{
		streaming = cvQueryFrame(capture);
	}
	else
	{
		streaming = 0;
	}
	isGetPT = false;
	isGetBall =false;
	isStable = false;
	isObject = false;

	Pos_ball.x=0;	Pos_ball.y=0;
	GetPt.x = GetPt.y = 0;
	
}

ImageProcessing::~ImageProcessing()
{	
	cvDestroyAllWindows();
	cvReleaseCapture(&capture);
	delete Kalman;
}

void ImageProcessing::On_Mouse(int event, int x, int y, int flags, void *param)
{
	ImageProcessing *temp = (ImageProcessing*) param;
	IplImage *src = cvCreateImage(cvGetSize(temp->streaming),8,3);
	cvCopy(temp->streaming, src);
	CString str;
	CvFont font= cvFont(1,1);
	
	if(src)
	{
		IplImage *rimg = cvCreateImage(cvSize(camWidth,camHeight),8,3);
		cvResize(src,rimg);
		
		if(event == CV_EVENT_LBUTTONDOWN)
		{
//			temp->GetPt = cvPoint(x,y);
//			temp->isGetPT = true;
		}
		if(temp->isGetPT)
		{
			str.Format("%d, %d", temp->GetPt.x, temp->GetPt.y);
			cvPutText(rimg, str, cvPoint(0,20), &font, cvScalar(0,0,0));
		}
		cvCircle(rimg, temp->GetPt, 2, cvScalar(0,0,255), -1);
		
		cvShowImage(temp->Window_Mouse,rimg);
		cvReleaseImage(&rimg);
	}
	cvReleaseImage(&src);
}

bool ImageProcessing::ImageCapture()
{
	capture = cvCaptureFromCAM(camDevice); 
// 	capture= cvCaptureFromAVI("Video 13_Xvid.avi");

	if(capture)
	{
		return true;
	}
	return false;
}

void ImageProcessing::ImageProcedure(int capimage)
{
	//if(capimage)
	//{	
		streaming = cvQueryFrame(capture);
		IplImage *img = cvCreateImage(cvSize(camWidth, camHeight), 8, 3);
 		
		cvResize(streaming, img, CV_INTER_LINEAR);

		IplImage *thresh= cvCreateImage(cvGetSize(img), 8, 1);

		cvZero(thresh);		
	
		BallRecognize(img, thresh);
		//FieldRecognize(img, thresh);
		
		int k;

		if(isGetPT)
		{
			cvDilate(thresh,thresh,NULL,1);
			static int r=0;
			int range[2];
			range[0]=DefaultSize;
			range[1]=DefaultSize;
			
			CvPoint topleft, bottomright;

			if(!isGetBall || !isStable)
			{
				topleft = cvPoint(GetPt.x-range[0], GetPt.y-range[1]);
				bottomright = cvPoint(GetPt.x+range[0], GetPt.y+range[1]);
			}
			else
			{
				range[0] = range[1] = r+15;

				topleft = cvPoint(Virtual_pt.x-range[0], Virtual_pt.y-range[1]);
				bottomright= cvPoint(Virtual_pt.x+range[0], Virtual_pt.y+range[1]);
			}

			//boundary protected
			if(topleft.x < 0)						topleft.x = 0;
			if(topleft.y < 0)						topleft.y = 0;
			if(bottomright.x > camWidth)			bottomright.x = camWidth;
			if(bottomright.y > camHeight)			bottomright.y = camHeight;
			
			float kal_x_out;
			float kal_y_out;	
		
			if(isKalmanStable(Virtual_pt.x, Virtual_pt.y, Predict_pt.x, Predict_pt.y))
			{
				isStable = true;
				int width = abs(bottomright.x - topleft.x);
				int height = abs(bottomright.y - topleft.y);

				IplImage *ball_roi = cvCreateImage(cvSize(width,height), 8, 1);
				int i, j;
				int NumOfWhite = 0;
				
				for(i = topleft.x; i< bottomright.x; i++)
				{
					for(j = topleft.y; j< bottomright.y; j++)
					{
						uchar *ptr = cvPtr2D(thresh, j, i);
						ball_roi->imageData[(j-topleft.y)*ball_roi->widthStep + (i-topleft.x)*ball_roi->nChannels] = *ptr;
						if(*ptr == 255) NumOfWhite++;
					}
				}
				cvErode(ball_roi, ball_roi, NULL, 1);
				//cvDilate(gray_scale, gray_scale, NULL, 3);
				FCC_ARRAY fcc_arr;
				int x, y, ballarea;
				FastCC(ball_roi, &fcc_arr);
				ObjectClassfied(img, ball_roi, &fcc_arr, topleft, r,1);

				if(isObject)
				{
					//kalman filter predict points
					float x_in = float(Virtual_pt.x);
					float y_in = float(Virtual_pt.y);
					
					Kalman->KalmanFilterAlgorithm(x_in, y_in, kal_x_out, kal_y_out);
					Predict_pt = cvPoint(int(kal_x_out), int(kal_y_out));
				}

// 				cvErode(ball_roi,ball_roi,NULL,5);
// 									
// 					
// 				if(isCorrectObject(NumOfWhite, width, height))
// 				{	
// 					cvDilate(ball_roi, ball_roi, NULL, 2);				
// 
// 					FCC_ARRAY fcc_arr;
// 					FastCC(ball_roi, &fcc_arr);
// 					ObjectClassfied(img, ball_roi, &fcc_arr, topleft, r,1);
// 					if(!isObject)	isGetPT = false;

// 				}
				
				cvShowImage(Window_ROI, ball_roi);
				cvReleaseImage(&ball_roi);
				fcc_arr.RemoveAll();
			}

			else
			{
				//BallRecognition(thresh, topleft, bottomright);
				cvErode(thresh, thresh, NULL, 4);
				//cvDilate(gray_scale, gray_scale, NULL, 3);
				FCC_ARRAY fcc_arr;
				int x, y, ballarea;
				FastCC(thresh, &fcc_arr);
				ObjectClassfied(img, thresh, &fcc_arr, cvPoint(0, 0), r,1);
				
				if(isObject)
				{
					//kalman filter predict points;
					float x_in = float(Virtual_pt.x);
					float y_in = float(Virtual_pt.y);
					
					Kalman->KalmanFilterAlgorithm(x_in, y_in, kal_x_out, kal_y_out);
					Predict_pt = cvPoint(int(kal_x_out), int(kal_y_out));
				}

			}
			
			CvPoint v_topleft = cvPoint(kal_x_out-range[0], kal_y_out-range[1]);
			CvPoint v_bottomright = cvPoint(kal_x_out+range[0], kal_y_out+range[1]);
			cvCircle(img, cvPoint(kal_x_out, kal_y_out), 2, cvScalar(0, 255, 255), -1);
			cvRectangle(img, v_topleft, v_bottomright, cvScalar(0, 255, 255), 1);
			
			v_topleft = cvPoint(Virtual_pt.x-range[0], Virtual_pt.y-range[1]);
			v_bottomright = cvPoint(Virtual_pt.x+range[0], Virtual_pt.y+range[1]);
			cvCircle(img, Virtual_pt, 2, cvScalar(0, 0, 255), -1);
			cvRectangle(img, v_topleft, v_bottomright, cvScalar(255, 0, 0), 1);

			Pos_ball=Virtual_pt;
			

		}
		else if(!isGetPT)
		{
			cvErode(thresh,thresh,NULL,2);
			cvDilate(thresh,thresh,NULL,1);
			static int r;
			
			int range[2];
			range[0]=DefaultSize;
			range[1]=DefaultSize;

			FCC_ARRAY fcc_arr;
			FastCC(thresh, &fcc_arr);
			ObjectClassfied(img, thresh, &fcc_arr, cvPoint(0,0), r,2);

			float x_in = float(GetPt.x);
			float y_in = float(GetPt.y);
			float kal_x_out;
			float kal_y_out;
			
			Kalman->KalmanFilterAlgorithm(x_in, y_in, kal_x_out, kal_y_out);
			Predict_pt = cvPoint(int(kal_x_out), int(kal_y_out));


			CvPoint topleft = cvPoint(GetPt.x-range[0], GetPt.y-range[1]);
			CvPoint bottomright = cvPoint(GetPt.x+range[0], GetPt.y+range[1]);

			CvPoint v_topleft = cvPoint(kal_x_out-range[0], kal_y_out-range[1]);
			CvPoint v_bottomright = cvPoint(kal_x_out+range[0], kal_y_out+range[1]);
			cvCircle(img, cvPoint(kal_x_out, kal_y_out), 2, cvScalar(0, 255, 255), -1);
			cvRectangle(img, v_topleft, v_bottomright, cvScalar(0, 255, 255), 1);

			v_topleft = cvPoint(GetPt.x-range[0], GetPt.y-range[1]);
			v_bottomright = cvPoint(GetPt.x+range[0], GetPt.y+range[1]);
			cvCircle(img, GetPt, 2, cvScalar(0, 0, 255), -1);
			cvRectangle(img, v_topleft, v_bottomright, cvScalar(255, 0, 0), 1);
					
		}
		
		cvShowImage(Window_Capture,img); 
		cvShowImage(Window_Binary,thresh);	


		cvReleaseImage(&img);
		cvReleaseImage(&thresh);		

//	}
}

bool ImageProcessing::BallRecognize(IplImage *img, IplImage *thresh)
{
	IplImage *hsv = cvCreateImage(cvGetSize(img), 8, 3);
	cvCvtColor(img, hsv, CV_RGB2HSV);
	
	int i, j;
	for(i = 0; i < hsv->width; i++)
	{
		for(j = 0; j < hsv->height; j++)
		{
			uchar *ptr = cvPtr2D(hsv, j, i);
			int h = *ptr;
			int s = *(ptr+1);
			int v = *(ptr+2);
			
			double x_temp = s/255.0;
			
			double x3 = x_temp*x_temp*x_temp;
			double x2 = x_temp*x_temp;
			double x =  x_temp;
			
			double a1 = Ball_Parameter[0];
			double a2 = Ball_Parameter[1];
			double a3 = Ball_Parameter[2];
			double a4 = Ball_Parameter[3];
			
			int y = int((a4*x3 + a3*x2 + a2*x + a1)*255);
			if(h > y-10 && h < y+10)
			{
				thresh->imageData[j * thresh->widthStep + i * thresh->nChannels] = (unsigned char)255;
				//thresh->imageData[j * thresh->widthStep + i * thresh->nChannels+1] = (unsigned char)255;
			//	thresh->imageData[j * thresh->widthStep + i * thresh->nChannels+2] = (unsigned char)0;
			}				
		}
	}
	
	cvReleaseImage(&hsv);
	return false;	
}

bool ImageProcessing::FieldRecognize(IplImage *img, IplImage *thresh)
{
	IplImage *hsv = cvCreateImage(cvGetSize(img), 8, 3);
	cvCvtColor(img, hsv, CV_RGB2HSV);
	
	int i, j;
	for(i = 0; i < hsv->width; i++)
	{
		for(j = 0; j < hsv->height; j++)
		{
			uchar *ptr = cvPtr2D(hsv, j, i);
			int h = *ptr;
			int s = *(ptr+1);
			int v = *(ptr+2);
			
			double x_temp = s/255.0;
			
			double x3 = x_temp*x_temp*x_temp;
			double x2 = x_temp*x_temp;
			double x =  x_temp;
			
			double a1 = Field_Parameter[0];
			double a2 = Field_Parameter[1];
			double a3 = Field_Parameter[2];
			double a4 = Field_Parameter[3];
			
			int y = int((a4*x3 + a3*x2 + a2*x + a1)*255);
			if(h > y-10 && h < y+10)
			{
				thresh->imageData[j * thresh->widthStep + i * thresh->nChannels] = 0;
			}
			else	
				thresh->imageData[j * thresh->widthStep + i * thresh->nChannels] = (unsigned char)255;
		}
	}
	
	cvReleaseImage(&hsv);
	return false;	
}

void ImageProcessing::ImageReleasing(IplImage *img)
{	cvReleaseImage(&img);
}

void ImageProcessing::BallRecognition(IplImage *binary, CvPoint upt, CvPoint bpt)
{
	int i, j;
	int x_sum = 0;
	int y_sum = 0;
	int cnt = 0;
	
	for(i = upt.x; i < bpt.x; i++)
	{
		for(j = upt.y; j < bpt.y; j++)
		{
			uchar *ptr = cvPtr2D(binary, j, i);
			
			if(*ptr > 0)
			{
				x_sum += i;
				y_sum += j;
				cnt ++;
			}
		}
	}
	if(cnt > 0)			
	{
		x_sum = int(x_sum / (double)cnt);
		y_sum = int(y_sum / (double)cnt);
		Virtual_pt = cvPoint(x_sum, y_sum);
		isGetBall = true;
	}
}

void ImageProcessing::Circle(IplImage *src,IplImage *img, int &x, int &y)
{
	IplImage* canny=cvCreateImage(cvGetSize(src), 8, 1);
	CvMemStorage* storage = cvCreateMemStorage(0);
		
	cvCanny(src, canny,200,220,3);
	
	CvSeq* circles = cvHoughCircles(canny, storage, CV_HOUGH_GRADIENT,1,
		src->height/4, 200, 10, 5, 20);
		
	for (int i = 0; i < circles->total; i++) 
	{ 
		float* p = (float*)cvGetSeqElem(circles, 1); 
		cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])),3, CV_RGB(0,255,0), -1, 8, 0 ); 
		cvCircle( img, cvPoint(cvRound(p[0]),cvRound(p[1])),cvRound(p[2]), CV_RGB(255,0,0), 3, 8, 0 );
		CvPoint center = cvPoint(cvRound(p[0]),cvRound(p[1]));

		x=center.x;	y=center.y;
	}
	
	cvReleaseMemStorage(&storage);

}

void ImageProcessing::ObjectClassfied(IplImage *src, IplImage *binary, FCC_ARRAY *fcc_arr, CvPoint offset, int &range, int type)
{
	int size = fcc_arr->GetSize();
	int maxAreaIndex = 0;
	if(size > 0)
	{
		for(int i = 0; i < size; i++)
		{
			if(fcc_arr->GetAt(i).Area > fcc_arr->GetAt(maxAreaIndex).Area)
			{
				maxAreaIndex = i;
			}
		}
		FCC_OBJECT fcc_obj = fcc_arr->GetAt(maxAreaIndex);
		
		CvPoint2D32f pt;
		float r;
		if(fcc_obj.Area > 100)
		{
			cvMinEnclosingCircle(fcc_obj.Points, &pt, &r);


			range = cvRound(r);

			if(type == 1)
			{
				int x = (int)pt.x + offset.x;
				int y = (int)pt.y + offset.y;
				Virtual_pt = cvPoint(x, y);
				cvCircle(src, Virtual_pt, (int)r, CV_RGB(255, 0, 0), 2, 8, 0);
				cvCircle(src, cvPoint((int)Virtual_pt.x, (int)Virtual_pt.y+int(r)), 1, CV_RGB(0, 0, 0), 5, 8, 0);
				cvCircle(src, Virtual_pt, 1, CV_RGB(0, 255, 0), 5, 8, 0);
			}
			else
			{
				GetPt = cvPoint((int)pt.x, (int)pt.y);
				cvCircle(src, GetPt, (int)r, CV_RGB(255, 0, 0), 2, 8, 0);
				cvCircle(src, cvPoint((int)GetPt.x, (int)GetPt.y+int(r)), 1, CV_RGB(0, 0, 0), 5, 8, 0);
				cvCircle(src, GetPt, 1, CV_RGB(0, 255, 0), 5, 8, 0);
			}

			//Pos_ball = cvPoint((int)pt.x, (int)pt.y);
			//Pos_ball_center = cvPoint((int)pt.x, (int)pt.y);
			Q_ball = true;
			isObject = true;
			isGetPT = true;
			isGetBall = true;
			findBall = true;
		}
		else
		{
			Q_ball = false;
			isObject = false;
			isGetPT = false;
			isGetBall = false;
			findBall = false;
		}

	}
	else
	{
		isGetPT = false;
		isObject = false;
		Q_ball = false;
		isGetBall = false;
		findBall = false;
	}
//////////////////////////////////////////////////////////////////////////////////////
/*
	int obj = fcc_arr->GetSize();
	CArray <CvPoint, CvPoint> circle_info;
	int max_r = 0;
	
	if(obj > 0)
	{
		int i;
		for(i = 0; i < obj; i++)
		{
			int now_r = 0;
			int size = fcc_arr->GetAt(i).Points->rows;
			if(fcc_arr->GetAt(i).Area > 100)
			{
				now_r = 0;
				for(int k = fcc_arr->GetAt(i).Center.y; k > 0; k--)
				{
					uchar *ptr = cvPtr2D(binary, k, fcc_arr->GetAt(i).Center.x);
					if(*ptr == 0)
						break;
					else
						now_r += 1;
				}
				
				double realcircle;
				if(now_r > 0)	realcircle = fcc_arr->GetAt(i).Area / (double)(now_r*now_r);
				if(fabs(realcircle - 3.14159) < 3.0)
				{
					circle_info.Add(cvPoint(fcc_arr->GetAt(i).Area, i));
					max_r = now_r;
				}
				if(max_r <= now_r)
					max_r = now_r;
			}
		}
		int maxArea = 0;
		int maxAreaIndex = 0;
		int maxRadius = 0;
		int maxRadiusIndex = 0;
		
		if(circle_info.GetSize() > 0)
		{
			for(i = 0; i < circle_info.GetSize(); i++)
			{
				if(circle_info[i].x > maxArea)
				{
					maxArea = circle_info[i].x;
					maxAreaIndex = circle_info[i].y;
				}
			}
			if(type==1)
			{
				Virtual_pt.x = fcc_arr->GetAt(maxAreaIndex).Center.x + offset.x;
				Virtual_pt.y = fcc_arr->GetAt(maxAreaIndex).Center.y + offset.y;
				cvCircle(src, Virtual_pt, max_r, CV_RGB(0, 255, 0), 1);
			}	
			
			else
			{
				GetPt = fcc_arr->GetAt(maxAreaIndex).Center;
				cvCircle(src, GetPt, max_r, CV_RGB(0, 255, 0), 1);
			}
			
			
			range = max_r;
			isObject = true;
		}
		else
		{
			isObject = false;
			range = DefaultSize;
		}
		
		
	}
	else
	{
		isGetPT = false;
		isObject = false;
		range = DefaultSize;
	}
	fcc_arr->RemoveAll();
	circle_info.RemoveAll();
	//radius.RemoveAll();
*/
}

void ImageProcessing::FastCC(IplImage *img, FCC_ARRAY *fcc_arr)
{
	// = = = = = fast connected component = = = = = //
	BOOL FCC_DebugMode = false;
	// initialize tables	
	int i, j;
	//src matrix init
	unsigned int** src;
	src = new unsigned int *[img->width];
	for(i = 0; i < img->width; i++){
		src[i] = new unsigned int [img->height];
	}

	int	*rtable	=	new int	[img->height*img->width/4];
	int	*next	=	new int	[img->height*img->width/4];
	int	*tail	=	new int	[img->height*img->width/4];
	for(i = 0; i < img->height*img->width / 4; i++){
		rtable[i]	= i;	// representative label table
		next[i]		= -1;
		tail[i]		= i;    
	}
	// processing
	int min_height = 0;
	int max_height = img->height;
	int start = 1;
	int	label = start;	// 0 is background
	int	c1, c2, c3, c4, r1, r2, temp;	// t: temp
	for(j = min_height; j < max_height; j++){	// processing from the top-left corner
		for(i = 0; i < img->width; i++){
			// adjustment
			int c0 = GetPixel(i, j, img);	// current point
			if(c0 > start)
			{
				if(i-1 >= 0){
					c1 = src[i-1][j];
				}else{
					c1 = -1;
				}
				if(i-1 >= 0 && j-1 >= 0){
					c2 = src[i-1][j-1];
				}else{
					c2 = -1;
				}
				if(j-1 >= 0){
					c3 = src[i][j-1];
				}else{
					c3 = -1;
				}
				if(i+1 < img->width && j-1 >= 0){
					c4 = src[i+1][j-1];
				}else{
					c4 = -1;
				}
				// object
				if(c3 > 0){	// is labeled foreground
					src[i][j] = c3;
				}else if(c1 > 0)
				{
					src[i][j] = c1;
					if(c4 > 0)
					{	// = = = = = resolve = = = = = //
						r1 = rtable[c4];
						r2 = rtable[c1];
						if(r1 < r2)
						{			
							temp = r2;
							while(temp != -1){
								rtable[temp] = r1;	
								temp = next[temp];
							}
							next[tail[r1]] = r2;
							tail[r1] = tail[r2];
						}else if(r2 < r1)
						{	
							temp = r1;
							while(temp != -1){
								rtable[temp] = r2;	
								temp = next[temp];
							}
							next[tail[r2]] = r1;
							tail[r2] = tail[r1];
						}
					}
				}else if(c2 > 0)
				{
					src[i][j] = c2;
					if(c4 > 0){
						// = = = = = resolve = = = = = //
						r1 = rtable[c2];
						r2 = rtable[c4];
						if(r1 < r2){			
							temp = r2;
							while(temp != -1){
								rtable[temp] = r1;	
								temp = next[temp];
							}
							next[tail[r1]] = r2;
							tail[r1] = tail[r2];
						}else if(r2 < r1){
							temp = r1;
							while(temp != -1){
								rtable[temp] = r2;	
								temp = next[temp];
							}
							next[tail[r2]] = r1;
							tail[r2] = tail[r1];
						}
					}
				}else if(c4 > 0){
					src[i][j] = c4;
				}else{
					src[i][j] = label;
					label = label + 1;
				}
			}
		}
	}
	int *area = new int[label];
	for(i = 0; i < label; i++){
		area[i] = 0;
	}
	CArray<CvPoint, CvPoint> *point = new CArray<CvPoint, CvPoint>[label];
	// refine
	for(j = min_height; j < max_height; j++)
	{
		for(i = 0; i < img->width; i++)
		{
			int data = src[i][j];
			if(data > 0 && data < label){
				int val = rtable[(int)data];
				if(FCC_DebugMode)
					Write2Pixel(img, i, j, (unsigned char)(val+100));
				area[val]++;
				point[val].Add(cvPoint(i, j));
			}
		}
	}
	//加入FastCC物件
	for(i = 0; i < label; i++)
	{
		int x, y, cnt;
		x = y = cnt = 0;
		if(area[i] > 10)
		{
			FCC_OBJECT fcc_obj;
			fcc_obj.Area = area[i];

			CvMat* points = cvCreateMat(area[i], 1, CV_32FC2);
			for(j = 0; j < area[i]; j++)
			{
				x += point[i].GetAt(j).x;
				y += point[i].GetAt(j).y;
				cnt += 1;

				if(point[i].GetAt(j).y > fcc_obj.Bottom.y)
					fcc_obj.Bottom = point[i].GetAt(j);
				CV_MAT_ELEM(*points, CvPoint2D32f, j, 0) = cvPoint2D32f(point[i].GetAt(j).x, point[i].GetAt(j).y); 
			}
			if(cnt > 0)
			{
				fcc_obj.Center.x = int(x / (double)cnt);
				fcc_obj.Center.y = int(y / (double)cnt);
			}
			fcc_obj.Points = points;
			fcc_arr->Add(fcc_obj);
		}
	}
	// release space
	for(i = 0; i < img->width; i++){
		delete src[i];// = new unsigned int [img->height];
	}
	delete	[] src;
	delete	[]	area;
	for(i = 0; i < label; i++){
		point[i].RemoveAll();
	}
	delete	[]	point;
	delete	[]	rtable;
	delete	[]	next;
	delete	[]	tail;
	
}

void ImageProcessing::Write2Pixel(IplImage *img, int x, int y, unsigned char val)
{
	int ptr;
	if(x > 0 && y > 0 && x < img->width && y < img->height)
	{
		ptr = y*img->widthStep + img->nChannels*x;			
		img->imageData[ptr] = (unsigned char)val;
	}
}

int ImageProcessing::GetPixel(int x, int y, IplImage *img)
{
	if(x > 0 && y > 0 && x < img->width && y < img->height)
	{
		int data = 0;
		int ptr = y*img->widthStep + img->nChannels*x;			
		data = (unsigned char)img->imageData[ptr];
		
		return data;
	}else
	{
		return -1;
	}	
}

bool ImageProcessing::isKalmanStable(int rx, int ry, int kx, int ky)
{
	if(rx == 0 && ry == 0)
		return false;

	int x_dis = abs(rx-kx);
	int y_dis = abs(ry-ky);

	if(x_dis < 1*DefaultSize && y_dis < 1*DefaultSize)	return true;
	return false;
}

bool ImageProcessing::isCorrectObject(int NumOfWhite, int width, int height)
{
	int area = width*height;
	double ratio = NumOfWhite / (double)area;
	if(ratio > 0.1 && ratio < 0.8)	return true;
	return false;
}

