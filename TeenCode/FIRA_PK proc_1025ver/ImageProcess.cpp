// ImageProcess.cpp: implementation of the CImageProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FIRA_PK proc.h"
#include "ImageProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CImageProcess::CImageProcess()
{
	ColorScale = "ColorScale";
	BinaryScale = "BinaryScale";
 	cvNamedWindow(ColorScale, CV_WINDOW_AUTOSIZE);
 	cvNamedWindow(BinaryScale, CV_WINDOW_AUTOSIZE);

	ImgCatchFinish = false;
//	Img = cvLoadImage("Image[1].jpg", CV_LOAD_IMAGE_COLOR);

	Q_ball = false;
	Q_Obstacle = false;
	Q_GaolKeeper = false;
	if(OpenCapture())
	{
		Img = cvQueryFrame(Capture);
	}
	else
	{
		Img = 0;
	}

}

CImageProcess::~CImageProcess()
{
	cvDestroyAllWindows();
	cvReleaseCapture(&Capture);
}

void CImageProcess::ProcessProcedure(int state, int headstate, int catchbegin, CvPoint topleft, CvPoint downright)
{
	int i, j;
	if(catchbegin)
	{
		Img = cvQueryFrame(Capture);
		IplImage *resize_img = cvCreateImage(cvSize(CAM_WIDTH, CAM_HEIGHT), 8, 3);

		cvResize(Img, resize_img, CV_INTER_LINEAR);
		IplImage *gray_scale = cvCreateImage(cvGetSize(resize_img), 8, 1);
		cvZero(gray_scale);

		if(state == 1)
		{
			if(headstate < 4)
			{
				//just find obstacles location
				ObstacleRecognize(resize_img, gray_scale);
				cvErode(gray_scale, gray_scale, NULL, 1);
				cvDilate(gray_scale, gray_scale, NULL, 2);

				GetBottom(resize_img, gray_scale, headstate);

			}
			else if(headstate == 5)
			{
				FieldRecognize(resize_img, gray_scale);
				cvDilate(gray_scale, gray_scale, NULL, 2);

				int x = 0;
				int y = 0;
				int cnt = 0;
				for(i = 0; i < gray_scale->width; i++)
				{
					for(j = 40; j < 55; j++)
					{
						uchar *ptr = cvPtr2D(gray_scale, j, i);
						if(*ptr == 0)
						{
							x+=i;	y+=j;	cnt++;
						}
					}
				}
				if(cnt > 0)
				{
					x = int(x / (double)cnt);
					y = int(y / (double)cnt);
					GoalKeeper_center = cvPoint(x, y);
					Q_GaolKeeper = true;
				}
				cvCircle(resize_img, cvPoint(x, y), 3, cvScalar(0, 255, 255), -1);
				cvRectangle(resize_img, cvPoint(0, 40), cvPoint(320, 55), cvScalar(0, 255, 0), 1);
			}
			
		}
		else if(state > 1 && state != 5)
		{
			buttom_pts[0].RemoveAll();
			buttom_pts[1].RemoveAll();
 			buttom_pts[2].RemoveAll();
			//we have to find ball position and watch out obstacles

			//ObstacleRecognize(Img, gray_scale);
			BallRecognize(resize_img, gray_scale);
			cvErode(gray_scale, gray_scale, NULL, 1);
			//cvDilate(gray_scale, gray_scale, NULL, 3);
			FCC_ARRAY fcc_arr;
			int x, y, ballarea;
			FastCC(gray_scale, &fcc_arr);
			ObjectClassfied(resize_img, &fcc_arr, &x, &y, &ballarea);
			
			
			ObstacleRecognize(resize_img, gray_scale);
			cvRectangle(resize_img, topleft, downright, cvScalar(255, 0, 0), 1);
		}
		else if(state == 5)
		{

			ObstacleRecognize(resize_img, gray_scale);
			cvErode(gray_scale, gray_scale, NULL, 1);
			cvDilate(gray_scale, gray_scale, NULL, 2);

			double **bfr = CreateDynamicArray(gray_scale);	
			FastCC_Obst(bfr, gray_scale->height, gray_scale->width);
			FindObstacleCenter(resize_img, gray_scale, bfr);

			//release memories
			for(i = 0; i < gray_scale->width; i++)
			{
				delete bfr[i];
			}
			delete [] bfr;
			cvLine(resize_img, cvPoint(resize_img->width/2 - 30, 0), cvPoint(resize_img->width/2 - 30, resize_img->height), cvScalar(255, 0, 0), 1);
			cvLine(resize_img, cvPoint(resize_img->width/2 - 70, 0), cvPoint(resize_img->width/2 - 70, resize_img->height), cvScalar(255, 0, 0), 1);
			cvLine(resize_img, cvPoint(resize_img->width/2 + 30, 0), cvPoint(resize_img->width/2 + 30, resize_img->height), cvScalar(255, 0, 0), 1);
			cvLine(resize_img, cvPoint(resize_img->width/2 + 70, 0), cvPoint(resize_img->width/2 + 70, resize_img->height), cvScalar(255, 0, 0), 1);
			cvLine(resize_img, cvPoint(0, resize_img->height-90), cvPoint(resize_img->width, resize_img->height-90), cvScalar(255, 0, 0), 1);

		}
		//cvLine(resize_img, cvPoint(0, 60), cvPoint(resize_img->width, 60), cvScalar(255, 0, 0), 1);
		ImgCatchFinish = true;
		
		cvShowImage(ColorScale, resize_img);
		cvShowImage(BinaryScale, gray_scale);

		cvReleaseImage(&resize_img);
		cvReleaseImage(&gray_scale);
	}
}

bool CImageProcess::OpenCapture()
{
	Capture = cvCaptureFromCAM(CAM_DEVICE);
	if(Capture)
	{
		return true;
	}
	return false;
}

void CImageProcess::BinaryProcess(IplImage *img, IplImage *gray)
{
	
}

bool CImageProcess::ObstacleRecognize(IplImage *img, IplImage *gray)
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

			double a1 = Curve_Parameters_obstacle[0];
			double a2 = Curve_Parameters_obstacle[1];
			double a3 = Curve_Parameters_obstacle[2];
			double a4 = Curve_Parameters_obstacle[3];
			
			int y = int((a4*x3 + a3*x2 + a2*x + a1)*255);
			if(h > y-10 && h < y+10)
			{
				gray->imageData[j * gray->widthStep + i * gray->nChannels] = (unsigned char)255;
			}
		}
	}
	
	cvReleaseImage(&hsv);
	return false;
}

bool CImageProcess::BallRecognize(IplImage *img, IplImage *gray)
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

			double a1 = Curve_Parameters_ball[0];
			double a2 = Curve_Parameters_ball[1];
			double a3 = Curve_Parameters_ball[2];
			double a4 = Curve_Parameters_ball[3];
			
			int y = int((a4*x3 + a3*x2 + a2*x + a1)*255);
			if(h > y-10 && h < y+10)
			{
				gray->imageData[j * gray->widthStep + i * gray->nChannels] = (unsigned char)255;
			}
 		}
 	}
	
	cvReleaseImage(&hsv);
	return false;
}

void CImageProcess::GetBottom(IplImage *img, IplImage *gray, int headstate)
{
	cvDilate(gray, gray, NULL, 1);
	cvCanny(gray, gray, 30, 50, 3);

	int i, j;

	for(i = 0; i < gray->width; i++)
	{
		for(j = 0; j < gray->height; j++)
		{
			uchar *ptr = cvPtr2D(gray, j, i);
			if(*ptr == 255)
			{
				buttom_pts[headstate-1].Add(cvPoint(i, j));
			}
		}
	}

//	FieldFilter(img, headstate);
//	cvCircle(img, maxpt, 5, cvScalar(0, 0, 0), -1);
}

double **CImageProcess::CreateDynamicArray(IplImage *img)
{
	int i, j;
	double **darray = new double *[img->width];
	for(i = 0; i < img->width; i++)
	{
		darray[i] = new double [img->height];
	}
	
	for(i = 0; i < img->width; i++)
	{
		for(j = 0; j < img->height; j++)
		{
			darray[i][j] = (unsigned char)img->imageData[j*img->widthStep+i*img->nChannels];
		}
	}
	return darray;
}

void CImageProcess::FastCC(IplImage *img, FCC_ARRAY *fcc_arr)
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
		if(area[i] > 10)
		{
			FCC_OBJECT fcc_obj;
			fcc_obj.Area = area[i];

			CvMat* points = cvCreateMat(area[i], 1, CV_32FC2);
			for(j = 0; j < area[i]; j++)
			{
				if(point[i].GetAt(j).y > fcc_obj.Bottom.y)
					fcc_obj.Bottom = point[i].GetAt(j);
				CV_MAT_ELEM(*points, CvPoint2D32f, j, 0) = cvPoint2D32f(point[i].GetAt(j).x, point[i].GetAt(j).y); 
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

void CImageProcess::ObjectClassfied(IplImage *src, FCC_ARRAY *fcc_arr, int *x, int *y, int *Area)
{
	int size = fcc_arr->GetSize();
	int maxAreaIndex = 0;
	if(size > 0)
	{
		for(int i = 0; i < size; i++)
		{
			if(fcc_arr->GetAt(i).Bottom.y > fcc_arr->GetAt(maxAreaIndex).Bottom.y)
			{
				maxAreaIndex = i;
			}
		}
		FCC_OBJECT fcc_obj = fcc_arr->GetAt(maxAreaIndex);
		
		CvPoint2D32f pt;
		float r;
		//if(fcc_obj.Area > 0)
		{
			cvMinEnclosingCircle(fcc_obj.Points, &pt, &r);
			cvCircle(src, cvPoint((int)pt.x, (int)pt.y), (int)r, CV_RGB(255, 0, 0), 2, 8, 0);
			cvCircle(src, cvPoint((int)pt.x, (int)pt.y+int(r)), 1, CV_RGB(0, 0, 0), 5, 8, 0);
			cvCircle(src, cvPoint((int)pt.x, (int)pt.y), 1, CV_RGB(0, 255, 0), 5, 8, 0);
			*x = (int)pt.x;
			*y = (int)pt.y + (int)r;
			*Area = fcc_obj.Area;
			Pos_ball = cvPoint(*x, *y);
			Pos_ball_center = cvPoint((int)pt.x, (int)pt.y);
			Q_ball = true;
		}
	}
	else
	{
		Q_ball = false;
	}
}


void CImageProcess::Write2Pixel(IplImage *img, int x, int y, unsigned char val)
{
	int ptr;
	if(x > 0 && y > 0 && x < img->width && y < img->height)
	{
		ptr = y*img->widthStep + img->nChannels*x;			
		img->imageData[ptr] = (unsigned char)val;
	}
}

int CImageProcess::GetPixel(int x, int y, IplImage *img)
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

void CImageProcess::FindObstacleCenter(IplImage *img, IplImage *gray, double **darray/*IplImage *img, IplImage *gray*/)
{
	/*
	int i, j;
	int x = 0;
	int y = 0;
	int cnt = 0;

	for(i = 0; i < gray->width; i++)
	{
		for(j = 0; j < gray->height; j++)
		{
			uchar *ptr = cvPtr2D(gray, j, i);

			if(*ptr == 255)
			{
				x += i;
				y += j;
				cnt ++;
			}
		}
	}
	if(cnt > 0)
	{
		x = int(x / (double)cnt);
		y = int(y / (double)cnt);
		Q_Obstacle = true;
	}
	Obstacle_center = cvPoint(x, y);
	cvCircle(img, Obstacle_center, 3, cvScalar(0, 0, 255), -1);
	*/
	int i, j;
	
	int area[256] = {0};							//each gray scale's pixel number
	int index[1000] = {0};							//effective object's index
	int obj = 0;									//how many object's effective
	
	for(i = 0; i < gray->width; i++)
	{
		for(j = 0; j < gray->height; j++)
		{
			int label = (int) darray[i][j];			//get gray scale's label
			if (label != 0)							//this point's color is not black
			{
				area[label] =  area[label] + 1;		//save it into the same array and area plus 1
			}
			else
			{
				area[0]=  area[0] + 1;
			}
		}
	}
	for(i = 1; i < 256; i++)
	{
		if(area[i] > 100 && area[i] < 100000)		//if area is not too small and too large
		{
			index[obj] = i;							//record allow index
			obj = obj + 1;							//get 1 object
		}
	}	
	
	int cx = 0,	cy = 0;								//mesh's center point
	int total_num = 0;								//number of pixels
	
	for(int k = 0; k < obj; k++)					//totally object you have
	{
		for(i = 0; i < gray->width; i++)
		{
			for(j = 0; j < gray->height; j++)
			{
				if(int(darray[i][j]) == index[k])		//this pixel's color is same as buffer's value
				{
					cx+=i;	cy+=j;	total_num+=1;
				}
			}
		}
		if(total_num > 0)
		{
			Q_Obstacle = true;
			//calculate center point
			(int)cx = int(cx / (double)total_num);
			(int)cy = int(cy / (double)total_num);
			if(obj > 1)
			{
				Obstacle_center[k] = cvPoint(cx, cy);
			}
			else
			{
				Obstacle_center[k] = cvPoint(cx, cy);
				if(Obstacle_center[k].x < img->width/2)
				{
					Obstacle_center[1] = Obstacle_center[k];
					Obstacle_center[0] = cvPoint(0, 0);
				}
				else 
				{
					Obstacle_center[1] = cvPoint(0, 0);
					Obstacle_center[0] = Obstacle_center[k];
				}
			}
			cx = cy = total_num = 0;
		}
		else
		{
			Q_Obstacle = false;
		}
	}
	//0:right  , 1:left
	if(obj > 1)
	{
		if(Obstacle_center[0].x < Obstacle_center[1].x)
		{
			int tempx = Obstacle_center[0].x;
			int tempy = Obstacle_center[0].y;
			Obstacle_center[0] = Obstacle_center[1];
			Obstacle_center[1] = cvPoint(tempx, tempy);
		}
	}
	if(obj == 0)
	{
		Obstacle_center[0] = cvPoint(0, 0);
		Obstacle_center[1] = cvPoint(0, 0);
		Q_Obstacle = false;
	}

	
	cvCircle(img, Obstacle_center[0], 3, cvScalar(0, 0, 255), -1);
	cvCircle(img, Obstacle_center[1], 3, cvScalar(0, 255, 0), -1);
}

void CImageProcess::FastCC_Obst(double **src, int &imgHeight, int &imgWidth)
{
// = = = = = fast connected component = = = = = //
	// initialize tables	
	int count = 0;
	int	*rtable	=	new int	[imgHeight*imgWidth/4];
	int	*next	=	new int	[imgHeight*imgWidth/4];
	int	*tail	=	new int	[imgHeight*imgWidth/4];
	int i, j;
	for(i=0;i<imgHeight*imgWidth/4;i++){
		rtable[i]	= i;	// representative label table
		next[i]		= -1;
		tail[i]		= i;    
	}
	// processing
	int	label=1;	// 0 is background
	int	c1,c2,c3,c4,r1,r2,temp;	// t: temp
	for(j=0;j<imgHeight;j++){	// processing from the top-left corner
		for(i=0;i<imgWidth;i++){

			// adjustment
			double &c0=src[i][j];	// current point
			if(i-1>=0){
				c1=src[i-1][j];
			}else{
				c1=-1;
			}
			if(i-1>=0&&j-1>=0){
				c2=src[i-1][j-1];
			}else{
				c2=-1;
			}
			if(j-1>=0){
				c3=src[i][j-1];
			}else{
				c3=-1;
			}
			if(i+1<imgWidth&&j-1>=0){
				c4=src[i+1][j-1];
			}else{
				c4=-1;
			}

			if(c0>128){	// object

				if(c3>0){	// is labeled foreground
					c0=c3;
				}else if(c1>0){
					c0=c1;
					if(c4>0){
						// = = = = = resolve = = = = = //
						r1=rtable[c4];
						r2=rtable[c1];

						if(r1<r2){			
							temp=r2;
							while(temp!=-1){
								rtable[temp]=r1;	
								temp=next[temp];
							}
							next[tail[r1]]=r2;
							tail[r1]=tail[r2];

						}else if(r2<r1){	
							temp=r1;
							while(temp!=-1){
								rtable[temp]=r2;	
								temp=next[temp];
							}
							next[tail[r2]]=r1;
							tail[r2]=tail[r1];

						}

					}
				}else if(c2>0){
					c0=c2;
					if(c4>0){
						// = = = = = resolve = = = = = //
						r1=rtable[c2];
						r2=rtable[c4];

						if(r1<r2){			
							temp=r2;
							while(temp!=-1){
								rtable[temp]=r1;	
								temp=next[temp];
							}
							next[tail[r1]]=r2;
							tail[r1]=tail[r2];

						}else if(r2<r1){	
							temp=r1;
							while(temp!=-1){
								rtable[temp]=r2;	
								temp=next[temp];
							}
							next[tail[r2]]=r1;
							tail[r2]=tail[r1];

						}

					}
				}else if(c4>0){
					c0=c4;
				}else{
					c0=label;
					label+=10;
					count++;
				}
			}
		}
	}
	// refine
	for(i=0;i<imgWidth;i++){
		for(j=0;j<imgHeight;j++){
			if(src[i][j]>0){
				src[i][j]=rtable[(int)src[i][j]]+1;
			}
		}
	}
	// release space
	delete	[]	rtable;
	delete	[]	next;
	delete	[]	tail;
}

void CImageProcess::FieldRecognize(IplImage *img, IplImage *gray)
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
			
			double a1 = Curve_Parameters_field[0];
			double a2 = Curve_Parameters_field[1];
			double a3 = Curve_Parameters_field[2];
			double a4 = Curve_Parameters_field[3];
			
			int y = int((a4*x3 + a3*x2 + a2*x + a1)*255);
			if(h > y-10 && h < y+10)
			{
				gray->imageData[j * gray->widthStep + i * gray->nChannels] = (unsigned char)255;
			}
		}
	}
	
	cvReleaseImage(&hsv);
}

void CImageProcess::ViewCalibrate()
{
	Img = cvQueryFrame(Capture);
	IplImage *resize_img = cvCreateImage(cvSize(CAM_WIDTH, CAM_HEIGHT), 8, 3);
	
	cvResize(Img, resize_img, CV_INTER_LINEAR);

	cvLine(resize_img, cvPoint(0, 180), cvPoint(resize_img->width, 180), cvScalar(255, 0, 0), 1);
	cvLine(resize_img, cvPoint(160, 0), cvPoint(160, 240), cvScalar(255, 0, 0), 1);
	cvShowImage(ColorScale, resize_img);
	cvReleaseImage(&resize_img);
}
