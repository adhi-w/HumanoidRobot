// ImageProcess.cpp: implementation of the CImageProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HuroEvolutionSystem.h"
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
	pKalman = new CKalmanFilter();
	ColorScale = "ColorScale";
	BinaryScale = "BinaryScale";
	Window_ROI = "WindowROI_Ball";
	Window_Mouse = "MouseEvent";

	cvNamedWindow(ColorScale, CV_WINDOW_AUTOSIZE);
 	cvNamedWindow(BinaryScale, CV_WINDOW_AUTOSIZE);
//	cvNamedWindow(Window_Mouse, CV_WINDOW_AUTOSIZE);
	cvNamedWindow(Window_ROI, 1);
	
	

	if(isCameraOpenning())
	{
		//cvSetMouseCallback(Window_Mouse,On_Mouse,this);
		isCameraOpen = true;
		Img = cvQueryFrame(Capture);
	}
	else
	{
		isCameraOpen = false;
		Img = 0;
	}
	GrabTime = 0;
	ImageMode = 0;

	isGetPT = false;
	isGetBall = false;
	Q_Ball = false;
	isBallGone = false;
	isImgDone = false;
	isZoomIn = false;

	L_kick_Region = cvPoint(130, 215);
	R_kick_Region = cvPoint(215, 215);
	RegionOfKick = 10;
	// Mask's parameters Initial
	SetMaskParameters();
}

CImageProcess::~CImageProcess()
{
	cvReleaseCapture(&Capture);
	cvDestroyAllWindows();
	delete pKalman;
}

void CImageProcess::Procedure(bool img_catch, int hor, int ver)
{
	if(img_catch)
	if(Img)
	{
		if(!isGetPT)
		{
			if(ver > 550)
				isZoomIn = true;
			else
				isZoomIn = false;
		}

		if(isZoomIn)
		{
			Cam_Width = CAM_ZOOMIN_WIDTH;
			Cam_Height = CAM_ZOOMIN_HEIGHT;
			L_kick_Region = cvPoint(130, 190);
			R_kick_Region = cvPoint(215, 190);
			RegionOfKick = 10;
		}
		else
		{
			Cam_Width = CAM_ZOOMOUT_WIDTH;
			Cam_Height = CAM_ZOOMOUT_HEIGHT;
			L_kick_Region = cvPoint(250, 405);
			R_kick_Region = cvPoint(425, 405);
			RegionOfKick = 20;
		}
		//=================Get Grab Time======================
		SYSTEMTIME T;
		GetLocalTime(&T);
		int t = T.wSecond*1000 + T.wMilliseconds;

		Img = cvQueryFrame(Capture);

		IplImage *resize_img = cvCreateImage(cvSize(Cam_Width, Cam_Height), 8, 3);
		
		cvResize(Img, resize_img, CV_INTER_LINEAR);
		IplImage *gray_scale = cvCreateImage(cvGetSize(resize_img), 8, 1);
		cvZero(gray_scale);

		GetLocalTime(&T);
		int Now_Time = T.wSecond*1000 + T.wMilliseconds;
		GrabTime = Now_Time - t;
		//=================Get Grab Time======================

		FieldRecognition(resize_img, gray_scale);
		if(!isGetPT && ver < 550)
		{
			for(int i = 0; i < resize_img->width; i++)
			{
				for(int j = resize_img->height-1; j > resize_img->height - 100; j--)
				{
					gray_scale->imageData[j*gray_scale->widthStep + i*gray_scale->nChannels] = (unsigned char)0;
				}
			}			
		}

		// Mask
		if(isZoomIn)
			ShoulderMask(gray_scale);
		cvDilate(gray_scale, gray_scale, NULL, 2);
		
		if(isGetPT)
		{
			ImageMode = 1;
			KalmanTracking(resize_img, gray_scale, ver);
		}
		else
		{
			if(gray_scale)
			{
				ImageMode = 2;
				cvErode(gray_scale, gray_scale, NULL, 10);
				cvDilate(gray_scale, gray_scale, NULL, 5);
				FCC_ARRAY fcc_arr;
				FastCC(gray_scale, &fcc_arr);
 				ObjectSelection(resize_img, gray_scale, &fcc_arr);
				isImgDone = true;
			}
		}
		L_Kick_Region_topleft = cvPoint(L_kick_Region.x - RegionOfKick, L_kick_Region.y - RegionOfKick);
		L_Kick_Region_downright = cvPoint(L_kick_Region.x + RegionOfKick, L_kick_Region.y + RegionOfKick);

		R_Kick_Region_topleft = cvPoint(R_kick_Region.x - RegionOfKick, R_kick_Region.y - RegionOfKick);
		R_Kick_Region_downright = cvPoint(R_kick_Region.x + RegionOfKick, R_kick_Region.y + RegionOfKick);

		cvRectangle(resize_img, L_Kick_Region_topleft, L_Kick_Region_downright, cvScalar(255, 100, 100), 1);
		cvRectangle(resize_img, R_Kick_Region_topleft, R_Kick_Region_downright, cvScalar(255, 100, 100), 1);

		cvShowImage(BinaryScale, gray_scale);
		cvShowImage(ColorScale, resize_img);
		cvReleaseImage(&gray_scale);
		cvReleaseImage(&resize_img);
	}	
}

void CImageProcess::FieldRecognition(IplImage *img, IplImage *gray)
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
			
			double a1 = Curve_parameters_field[0];
			double a2 = Curve_parameters_field[1];
			double a3 = Curve_parameters_field[2];
			double a4 = Curve_parameters_field[3];
		
			int y = int((a4*x3 + a3*x2 + a2*x + a1)*255);

			if(h > y-15 && h < y+15)
			{
				//gray->imageData[j * gray->widthStep + i * gray->nChannels] = (unsigned char)0;
			}
			else
			{
				gray->imageData[j * gray->widthStep + i * gray->nChannels] = (unsigned char)255;
			}
		}
	}


	
	
	cvReleaseImage(&hsv);
}

bool CImageProcess::isCameraOpenning()
{	
	Capture = cvCaptureFromCAM(CAM_DEVICE);
	//Capture = cvCaptureFromAVI("Video 13_Xvid.avi");
	if(Capture)
	{
		return true;
	}
	return false;
}

void CImageProcess::On_Mouse(int event, int x, int y, int flags, void *param)
{
	CImageProcess *temp = (CImageProcess*) param;
	IplImage *src = cvCreateImage(cvGetSize(temp->Img), 8, 3);
	cvCopy(temp->Img, src);
	CString str;
	CvFont font= cvFont(1,1);

	if(src)
	{
		IplImage *rimg = cvCreateImage(cvSize(CAM_ZOOMIN_WIDTH, CAM_ZOOMIN_HEIGHT), 8, 3);
		cvResize(src, rimg);
		if(event == CV_EVENT_LBUTTONDOWN)
		{
			temp->GetPt = cvPoint(x, y);
			temp->isGetPT = true;
			//cvDestroyWindow(temp->Window_Mouse);
		}
		if(temp->isGetPT)
		{
			str.Format("%d, %d", temp->GetPt.x, temp->GetPt.y);
			cvPutText(rimg, str, cvPoint(0, 20), &font, cvScalar(0, 0, 255));
		}

		cvCircle(rimg, temp->GetPt, 2, cvScalar(0, 0, 255), -1);

		cvShowImage(temp->Window_Mouse, rimg);
		cvReleaseImage(&rimg);
	}
	cvReleaseImage(&src);
}

void CImageProcess::BallRecognition(IplImage *binary, CvPoint upt, CvPoint bpt)
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
	}
}

bool CImageProcess::isKalmanStable(int rx, int ry, int kx, int ky)
{
	int x_dis = abs(rx - kx);
	int y_dis = abs(ry - ky);
	double rate = 1.0;

	if(!isZoomIn)
		rate = 3.0;

	if(x_dis < rate*DEFAULT_SIZE && y_dis < rate*DEFAULT_SIZE)
		return true;
	return false;
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
	//CArray<CvPoint, CvPoint> point[256];// = new CArray<CvPoint, CvPoint>[label];
	std::vector < CvPoint>  point[1000];
	// refine
	
	for(j = min_height; j < max_height; j++)
	{
		for(i = 0; i < img->width; i++)
		{
			int data = src[i][j];
			if(data > 0 && data < label)
			{
				int val = rtable[(int)data];
 				area[val]++;
 				//point[val].Add(cvPoint(i, j));
				point[val].push_back(cvPoint(i, j));
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
				x += point[i].at(j).x;
				y += point[i].at(j).y;
				cnt += 1;

				if(point[i].at(j).y > fcc_obj.Bottom.y)
					fcc_obj.Bottom = point[i].at(j);
				CV_MAT_ELEM(*points, CvPoint2D32f, j, 0) = cvPoint2D32f(point[i].at(j).x, point[i].at(j).y); 
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
	
	for(i = 0; i < 256; i++){
		point[i].clear();
	}
//	delete	[]	point;
	
	delete	[]	rtable;
	delete	[]	next;
	delete	[]	tail;
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

void CImageProcess::Write2Pixel(IplImage *img, int x, int y, unsigned char val)
{
	int ptr;
	if(x > 0 && y > 0 && x < img->width && y < img->height)
	{
		ptr = y*img->widthStep + img->nChannels*x;			
		img->imageData[ptr] = (unsigned char)val;
	}
}

void CImageProcess::ObjectClassfied(IplImage *src, IplImage *binary, FCC_ARRAY *fcc_arr, CvPoint offset, int &range)
{
	int obj = fcc_arr->GetSize();
	//CArray <CvPoint, CvPoint> circle_info;
	std::vector <CvPoint> circle_info;
	int max_r = 0;
	int i;

	int area_limit = 3000;
	if(!isZoomIn)
		area_limit = 14000;

	if(obj > 0)
	{
		for(i = 0; i < obj; i++)
		{
			int now_r = 0;
			if(fcc_arr->GetAt(i).Area < area_limit && fcc_arr->GetAt(i).Area > 300)
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
					circle_info.push_back(cvPoint(fcc_arr->GetAt(i).Area, i));
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
		for(i = 0; i < circle_info.size(); i++)
		{
			if(circle_info.at(i).x > maxArea)
			{
				maxArea = circle_info.at(i).x;
				maxAreaIndex = circle_info.at(i).y;
			}
		}
		CvFont font = cvFont(1, 1);
		CString str;
	
		str.Format("Area = %d", fcc_arr->GetAt(maxAreaIndex).Area);
	 	cvPutText(src, str, cvPoint(0, 20), &font, cvScalar(0, 255, 255));

		Virtual_pt.x = fcc_arr->GetAt(maxAreaIndex).Center.x + offset.x;
		Virtual_pt.y = fcc_arr->GetAt(maxAreaIndex).Center.y + offset.y;
		int by = fcc_arr->GetAt(maxAreaIndex).Center.y + offset.y + max_r;
		cvCircle(src, Virtual_pt, max_r, cvScalar(0, 255, 0), 1);
		cvCircle(src, cvPoint(Virtual_pt.x, by), 3, cvScalar(0, 0, 0), -1);
		Ball_pos = cvPoint(Virtual_pt.x, Virtual_pt.y);
		range = max_r;
	}

	else
	{
		isGetPT = false;
	}
	for(i = 0; i < obj; i++)
	{
		FCC_OBJECT fcc_obj = fcc_arr->GetAt(i);
		cvReleaseMat(&(fcc_obj.Points));
	}
	fcc_arr->RemoveAll();
	circle_info.clear();


}

bool CImageProcess::isCorrectObject(int NumOfWhite, int width, int height)
{
	int area = width*height;
	double ratio = NumOfWhite / (double)area;
	if(ratio < 0.95)
		return true;
	return false;
}

void CImageProcess::ObjectSelection(IplImage *src, IplImage *binary, FCC_ARRAY *fcc_arr)
{
	int size = fcc_arr->GetSize();
	int maxAreaIndex = 0;
	int i;
	int area_limit = 5000;
	int width = CAM_ZOOMIN_WIDTH;
	int height = CAM_ZOOMIN_HEIGHT;
	CString str;
	CvFont font= cvFont(1,1);

	if(!isZoomIn)
	{
		area_limit = 10000;
		width = CAM_ZOOMOUT_WIDTH;
		height = CAM_ZOOMOUT_HEIGHT;
	}

	if(size > 0)
	{		
		for(i = 0; i < size; i++)
		{
			if(fcc_arr->GetAt(i).Area < area_limit && fcc_arr->GetAt(i).Area > 500)
			{
				int dy = fcc_arr->GetAt(i).Bottom.y;
				int dis = abs(dy - fcc_arr->GetAt(i).Center.y);
				CvPoint vtopleft = cvPoint(fcc_arr->GetAt(i).Center.x - dis, fcc_arr->GetAt(i).Center.y - dis);
				CvPoint vdownright = cvPoint(fcc_arr->GetAt(i).Center.x + dis, fcc_arr->GetAt(i).Center.y + dis);
				if(vtopleft.x < 0)						vtopleft.x = 0;
				if(vtopleft.y < 0)						vtopleft.y = 0;
				if(vdownright.x > width)				vdownright.x = width;
				if(vdownright.y > height)				vdownright.y = height;
				
				
				int window_width = abs(vdownright.x - vtopleft.x);
 				int window_height = abs(vdownright.y - vtopleft.y);
// 				cvRectangle(src, vtopleft, vdownright, cvScalar(255, 0, 0), 1);
// 				str.Format("%d, %d", window_width, window_height);
//  				cvPutText(src, str, cvPoint(0, 40), &font, cvScalar(0, 0, 255));
				
				if(window_width > 0 && window_height > 0)
				{
					IplImage *sobel = cvCreateImage(cvSize(window_width, window_height), 8, 1);
					cvZero(sobel);
					
					ColorPatternMatch(src, binary, sobel, vtopleft, vdownright, fcc_arr->GetAt(i).Center, dis);
					
					
					cvCircle(src, fcc_arr->GetAt(i).Center, 2, cvScalar(255, 0, 0), -1);
					cvRectangle(src, vtopleft, vdownright, cvScalar(255, 0, 0), 1);
					
					cvReleaseImage(&sobel);
				}
			
			}
			//else
			//	break;
			
		}
	}

	for(i = 0; i < size; i++)
	{
		
		FCC_OBJECT fcc_obj = fcc_arr->GetAt(i);
		cvReleaseMat(&(fcc_obj.Points));
	}
	fcc_arr->RemoveAll();
}

void CImageProcess::ColorPatternMatch(IplImage *src, IplImage *binary, IplImage *sobel, CvPoint top, CvPoint bottom, CvPoint cp, int r)
{
	CString str;
	CvFont font= cvFont(1,1);
	int i, j, theta;

	for(i = top.x; i < bottom.x; i++)
	{
		for(j = top.y; j < bottom.y; j++)
		{
			unsigned char ptr = binary->imageData[j*binary->widthStep + i*binary->nChannels];
			sobel->imageData[(j - top.y)*sobel->widthStep + (i - top.x)*sobel->nChannels] = ptr;
		}
	}
 	IplImage *gray = cvCreateImage(cvGetSize(sobel), 8, 1);
 	cvCanny(sobel, gray, 30, 50, 3);


	double err_sum = 0.0;
	WhiteComponent = 0;

	for(i = 0; i < sobel->width; i++)
	{
		for(j = 0; j < sobel->height; j++)
		{
			uchar *ptr = cvPtr2D(sobel, j, i);
			
			if(*ptr > 0)
			{
				CvPoint pt = cvPoint(i+top.x, j+top.y);
				int dx = pt.x - cp.x;
				int dy = pt.y - cp.y;
				int dis = cvRound(Dist(dx, dy));
				double ang = atan2(dy, dx);
				
				double unit = 9.0;
				int piece = cvRound(dis/unit);

				for(int k = 0; k <= unit; k++)
				{
					int px = cp.x + cvRound(piece * k * cos(ang));
					int py = cp.y + cvRound(piece * k * sin(ang));
					
					if(px > 0 && py > 0 && px < src->width && py < src->height)
					{
 						uchar *cptr = cvPtr2D(src, py, px);
 						int r = *(cptr + 2);
 						int g = *(cptr + 1);
 						int b = *(cptr);

						if(r > 150 && g > 150 && b > 150)
						{
							WhiteComponent += 1;
						}
					}
					//cvCircle(src, cvPoint(px, py), 1, cvScalar(0, 255, 255), -1);
				}
			}
			
		}
	}
	
	str.Format("%d", WhiteComponent);
	cvPutText(src, str, cvPoint(0, 40), &font, cvScalar(0, 0, 255));

	int whitethreshold = 10;
	if(!isZoomIn)
		whitethreshold = 0;

	if(WhiteComponent > whitethreshold)
	{
		GetPt = cp;
		Virtual_pt = cp;
		isGetPT = true;
	}

	cvReleaseImage(&sobel);
	cvReleaseImage(&gray);
	
}

double CImageProcess::CircleFunction(CvPoint cp, CvPoint pt, int radius)
{
	int dx = cp.x - pt.x;
	int dy = cp.y - pt.y;

	double error = dx*dx + dy*dy - radius*radius;
	return fabs(error);
}

void CImageProcess::SaveImage()
{
	CString str;
	static int num = 0;

	if(Img)
	{
		IplImage *resize_img = cvCreateImage(cvSize(CAM_ZOOMIN_WIDTH, CAM_ZOOMIN_HEIGHT), 8, 3);
		cvResize(Img, resize_img);
		IplImage *binary_img = cvCreateImage(cvGetSize(resize_img), 8, 1);
		cvZero(binary_img);
		FieldRecognition(resize_img, binary_img);
		cvDilate(binary_img, binary_img, NULL, 2);
		cvErode(binary_img, binary_img, NULL, 8);

		
		str.Format("ImageFile_[%d].jpg", num);
		cvSaveImage(str, resize_img);
		str.Format("BinaryFile_[%d].jpg", num);
		cvSaveImage(str, binary_img);
		cvReleaseImage(&resize_img);
		cvReleaseImage(&binary_img);
		num += 1;
	}
}

void CImageProcess::ShoulderMask(IplImage *gray_scale)
{
	double Head_hor_1 = Head_hor;
	double Head_hor_2 = Head_hor * Head_hor;
	double Head_ver_1 = Head_ver;
	double Head_ver_2 = Head_ver * Head_ver;
	if (Head_hor <= 512 && Head_hor >= 350)
	{
		double Pixel_ru_x = 319.0;
		double Pixel_ru_y = (P_ru00 + P_ru10*Head_hor_1 + P_ru01*Head_ver_1 + P_ru20*Head_hor_2 + P_ru11*Head_hor_1*Head_ver_1 + P_ru02*Head_ver_2) - 50;
		double Pixel_rd_x = (P_rd00 + P_rd10*Head_hor_1 + P_rd01*Head_ver_1 + P_rd20*Head_hor_2 + P_rd11*Head_hor_1*Head_ver_1 + P_rd02*Head_ver_2) - 50;
		double Pixel_rd_y = 239.0;
		double Slope_r = (Pixel_ru_y - Pixel_rd_y) / (Pixel_ru_x - Pixel_rd_x);
		double Intercept_r = Pixel_ru_y - (Slope_r * Pixel_ru_x);//b = y - mx
		for(int i = Pixel_rd_x; i <= Pixel_ru_x; i++)
		{
			for(int j = Pixel_ru_y; j <= Pixel_rd_y; j++)
			{
				if(((j - (Slope_r * i)) >= Intercept_r) && (i <= 319) && (j <= 239) && (i >= 0) && (j >= 0))
					gray_scale->imageData[j * gray_scale->widthStep + i * gray_scale->nChannels] = (unsigned char)0;
			}
		}
	}
	// Mask Left
	if (Head_hor <= 665 && Head_hor >= 512)
	{
		double Pixel_lu_x = 0.0;
		double Pixel_lu_y = (P_lu00 + P_lu10*Head_hor_1 + P_lu01*Head_ver_1 + P_lu20*Head_hor_2 + P_lu11*Head_hor_1*Head_ver_1 + P_lu02*Head_ver_2) - 50;
		double Pixel_ld_x = (P_ld00 + P_ld10*Head_hor_1 + P_ld01*Head_ver_1 + P_ld20*Head_hor_2 + P_ld11*Head_hor_1*Head_ver_1 + P_ld02*Head_ver_2) + 50;
		double Pixel_ld_y = 239.0;
		double Slope_l = (Pixel_lu_y - Pixel_ld_y) / (Pixel_lu_x - Pixel_ld_x);
		double Intercept_l = Pixel_lu_y - (Slope_l * Pixel_lu_x);//b = y - mx
		for(int i = Pixel_lu_x; i <= Pixel_ld_x; i++)
		{
			for(int j = Pixel_lu_y; j <= Pixel_ld_y; j++)
			{
				if(((j - (Slope_l * i)) >= Intercept_l) && (i <= 319) && (j <= 239) && (i >= 0) && (j >= 0))
					gray_scale->imageData[j * gray_scale->widthStep + i * gray_scale->nChannels] = (unsigned char)0;
			}
		}
	}
}

void CImageProcess::KalmanTracking(IplImage *resize_img, IplImage *gray_scale, int ver)
{
	static int r = 0;
	int range[2];	//(x, y)axis
	range[0] = DEFAULT_SIZE;
	range[1] = DEFAULT_SIZE;

	int width = CAM_ZOOMIN_WIDTH;
	int height = CAM_ZOOMIN_HEIGHT;

	int torlence = 40;
	if(!isZoomIn)
	{
		torlence = 40;
		width = CAM_ZOOMOUT_WIDTH;
		height = CAM_ZOOMOUT_HEIGHT;
	}

	if(!isGetBall)
	{
		topleft = cvPoint(GetPt.x-range[0], GetPt.y-range[1]);
		bottomright = cvPoint(GetPt.x+range[0], GetPt.y+range[1]);
	}
	else
	{
		range[0] = range[1] = r+torlence;
		
		topleft = cvPoint(Virtual_pt.x-range[0], Virtual_pt.y-range[1]);
		bottomright = cvPoint(Virtual_pt.x+range[0], Virtual_pt.y+range[1]);
	}
	
	//boundary protected
	if(topleft.x < 0)						topleft.x = 0;
	if(topleft.y < 0)						topleft.y = 0;
	if(bottomright.x > width)			bottomright.x = width;
	if(bottomright.y > height)			bottomright.y = height;
	
	float kal_x_out;
	float kal_y_out;
	
	if(isKalmanStable(Virtual_pt.x, Virtual_pt.y, Predict_pt.x, Predict_pt.y))
	{	
		int width = abs(bottomright.x - topleft.x);
		int height = abs(bottomright.y - topleft.y);
		IplImage *ball_roi = cvCreateImage(cvSize(width, height), 8, 1);
		int i, j;
		int NumOfWhite = 0;
		if(width > 0 && height > 0)
		{
			for(i = topleft.x; i < bottomright.x; i++)
			{
				for(j = topleft.y; j < bottomright.y; j++)
				{
					unsigned char ptr = gray_scale->imageData[j*gray_scale->widthStep + i*gray_scale->nChannels];
					ball_roi->imageData[(j-topleft.y)*ball_roi->widthStep + (i-topleft.x)*ball_roi->nChannels] = ptr;
					if(ptr == 255)	NumOfWhite ++;
				}
			}
			if(isCorrectObject(NumOfWhite, width, height))
			{
				isBallGone = false;
				if(ver < 550)
				{
					cvErode(ball_roi, ball_roi, NULL, 8);
					cvDilate(ball_roi, ball_roi, NULL, 3);
				}
				else
				{
					cvErode(ball_roi, ball_roi, NULL, 13);
					cvDilate(ball_roi, ball_roi, NULL, 3);
				}
				FCC_ARRAY fcc_arr;
				FastCC(ball_roi, &fcc_arr);
				ObjectClassfied(resize_img, ball_roi, &fcc_arr, topleft, r);
				
				
				//kalman filter predict points
				float x_in = float(Virtual_pt.x);
				float y_in = float(Virtual_pt.y);
				
				pKalman->KalmanFilterAlgorithm(x_in, y_in, kal_x_out, kal_y_out);
				Predict_pt = cvPoint(int(kal_x_out), int(kal_y_out));
			}
			else
			{
				isBallGone = true;
				isGetPT = false;
			}
			
			cvShowImage(Window_ROI, ball_roi);
 			cvReleaseImage(&ball_roi);
		}
		
	}
	else
	{
		BallRecognition(gray_scale, topleft, bottomright);
		isGetBall = true;
		
		//kalman filter predict points
		float x_in = float(Virtual_pt.x);
		float y_in = float(Virtual_pt.y);
		
		pKalman->KalmanFilterAlgorithm(x_in, y_in, kal_x_out, kal_y_out);
		Predict_pt = cvPoint(int(kal_x_out), int(kal_y_out));
	}
	
	CvPoint v_topleft = cvPoint(kal_x_out-range[0], kal_y_out-range[1]);
	CvPoint v_bottomright = cvPoint(kal_x_out+range[0], kal_y_out+range[1]);
	cvCircle(resize_img, cvPoint(kal_x_out, kal_y_out), 2, cvScalar(0, 255, 255), -1);
	cvRectangle(resize_img, v_topleft, v_bottomright, cvScalar(0, 255, 255), 1);
	
	v_topleft = cvPoint(Virtual_pt.x-range[0], Virtual_pt.y-range[1]);
	v_bottomright = cvPoint(Virtual_pt.x+range[0], Virtual_pt.y+range[1]);
	cvCircle(resize_img, Virtual_pt, 2, cvScalar(0, 0, 255), -1);
	cvRectangle(resize_img, v_topleft, v_bottomright, cvScalar(255, 0, 0), 1);
}


void CImageProcess::SetMaskParameters()
{
	// Right side
	// Right up point y parameters(x = 319)
	P_ru00 = -4282;
	P_ru01 = 5.366;
	P_ru02 = -0.001174;
	P_ru10 = 14.15;
	P_ru11 = -0.01297;
	P_ru20 = -0.005751;
	// Right down point x parameters(y = 239)
	P_rd00 = -2981;
	P_rd01 = 6.558;
	P_rd02 = -0.0008346;
	P_rd10 = 8.181;
	P_rd11 = -0.01939;
	P_rd20 = 0.005537;
	// Left side
	// Left up point y parameters(x = 0)
	P_lu00 = 5267;
	P_lu01 = -10.77;
	P_lu02 = 0.0005825;
	P_lu10 = -3.469;
	P_lu11 = 0.01498;
	P_lu20 = -0.006037;
	// Left down point x parameters(y = 239)
	P_ld00 = -11930;
	P_ld01 = 10.09;
	P_ld02 = 0.004099;
	P_ld10 = 26.24;
	P_ld11 = -0.02038;
	P_ld20 = -0.01063;
}
