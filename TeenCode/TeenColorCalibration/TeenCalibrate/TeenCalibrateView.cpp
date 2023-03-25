// TeenCalibrateView.cpp : implementation of the CTeenCalibrateView class
//

#include "stdafx.h"
#include "TeenCalibrate.h"

#include "TeenCalibrateDoc.h"
#include "TeenCalibrateView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTeenCalibrateView

IMPLEMENT_DYNCREATE(CTeenCalibrateView, CFormView)

BEGIN_MESSAGE_MAP(CTeenCalibrateView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CTeenCalibrateView::OnButtonMeasureHSV)
	ON_BN_CLICKED(IDC_BUTTON2, &CTeenCalibrateView::OnButtonSaveDATA)
	ON_BN_CLICKED(IDC_BUTTON4, &CTeenCalibrateView::OnButtonLoadfile)
	ON_BN_CLICKED(IDC_BUTTON5, &CTeenCalibrateView::OnButtonCalculate)
	ON_BN_CLICKED(IDC_BUTTON6, &CTeenCalibrateView::OnButtonWrite2File)
	ON_BN_CLICKED(IDC_BUTTON3, &CTeenCalibrateView::OnButtonPlot2D)
	ON_BN_CLICKED(IDC_BUTTON9, &CTeenCalibrateView::OnButtonVideoCapture)
	ON_BN_CLICKED(IDC_BUTTON8, &CTeenCalibrateView::OnButtonLoadsamples)
	ON_BN_CLICKED(IDC_BUTTON7, &CTeenCalibrateView::OnButtonLoadCurveParameters)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_TIMER()
	ON_MESSAGE(WM_THREAD, OnThread)
END_MESSAGE_MAP()

// CTeenCalibrateView construction/destruction

CTeenCalibrateView::CTeenCalibrateView()
	: CFormView(CTeenCalibrateView::IDD)
{
	// TODO: add construction code here
	pImageProcess	= new CImageProcess();

	isMeasureHSV = false;
	start_flag = false;

	//=============Set pure window's boundarys for process image===========//
	m_WindowsBoundary.leftupboundx = 40;
	m_WindowsBoundary.leftupboundy = 70;
	m_WindowsBoundary.rightdownboundx = 405;
	m_WindowsBoundary.rightdownboundy = 330;
}

CTeenCalibrateView::~CTeenCalibrateView()
{
	delete pImageProcess;
}

void CTeenCalibrateView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_button_measurehsv);
	DDX_Control(pDX, IDC_BUTTON2, m_button_savedata);
	DDX_Control(pDX, IDC_BUTTON3, m_button_plot);
	DDX_Control(pDX, IDC_BUTTON4, m_button_loadfile);
	DDX_Control(pDX, IDC_BUTTON5, m_button_calculate);
	DDX_Control(pDX, IDC_BUTTON6, m_button_writetofile);
	DDX_Control(pDX, IDC_BUTTON7, m_button_curveparameters);
	DDX_Control(pDX, IDC_BUTTON8, m_button_loadsamples);
}

BOOL CTeenCalibrateView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CTeenCalibrateView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CallConsole();
}


// CTeenCalibrateView diagnostics

#ifdef _DEBUG
void CTeenCalibrateView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTeenCalibrateView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTeenCalibrateDoc* CTeenCalibrateView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTeenCalibrateDoc)));
	return (CTeenCalibrateDoc*)m_pDocument;
}
#endif //_DEBUG


// CTeenCalibrateView message handlers
LRESULT CTeenCalibrateView::OnThread(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

UINT CTeenCalibrateView::DoThread(LPVOID pParam)
{
	return 0;
}

void CTeenCalibrateView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	pImageProcess->ProcessProcedure();
 	ShowDiagrame(pImageProcess->ImgHSV, IDC_IMG_HSV_STATIC);
// 	
 	pImageProcess->ImageProcess();

 	pImageProcess->ReleaseImage(pImageProcess->Img);
  	pImageProcess->ReleaseImage(pImageProcess->ImgHSV);
	CFormView::OnTimer(nIDEvent);
}

void CTeenCalibrateView::OnButtonMeasureHSV()
{
	// TODO: Add your control notification handler code here
	//	SetTimer(1, 10, NULL);

  	pImageProcess->PhotoRequired();
  	ShowDiagrame(pImageProcess->ImgHSV, IDC_IMG_HSV_STATIC);
	
	isMeasureHSV = true;

// 	pImageProcess->ReleaseImage(pImageProcess->Img);
// 	pImageProcess->ReleaseImage(pImageProcess->ImgHSV);
}


void CTeenCalibrateView::ShowDiagrame(IplImage *img, UINT ID)
{
	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	SetRect(rect, rect.left, rect.top, rect.right, rect.bottom);
	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}

void CTeenCalibrateView::CallConsole()
{
	AllocConsole();
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)handle, _O_TEXT);
	FILE *hf = _fdopen(hCrt, "w");
	*stdout = *hf;
}

void CTeenCalibrateView::Arr2Mat(CvMat *mat1, double Arr1[], int row, int col)
{
	int i, j, k=0;
	for(i = 0; i < row; i++)
	{
		for(j = 0; j < col; j++)
		{
			CV_MAT_ELEM(*mat1, double, i, j) = Arr1[k];
			k++;
		}
	}
}

void CTeenCalibrateView::OnButtonSaveDATA()
{
	// TODO: Add your control notification handler code here
	int i;
	int min_size = 0;

	//CString m_data = "C:\\BALL";
	fp = fopen("E:\\Lab Project\\TeenSize\\TeenCalibrate\\Ball.txt","w");

	
	for(i = 0; i < pImageProcess->hs_pt.GetSize(); i++)
	{
		fprintf(fp, "%d, %d\n", pImageProcess->hs_pt[i].y, pImageProcess->hs_pt[i].x);
	}
	
	fclose(fp);
	m_button_savedata.EnableWindow(false);
}

void CTeenCalibrateView::OnButtonLoadfile()
{
	// TODO: Add your control notification handler code here
	fp = fopen("E:\\Lab Project\\TeenSize\\TeenCalibrate\\BALL.txt", "r");
	
	int i;
	for(i = 0; i < 10000; i++)
	{
		int h, s;
		if(fscanf(fp,"%d, %d\n", &h, &s) != EOF)
		{
			Xlsx_Data.Add(cvPoint(s, h));
		}
		else
			break;
	}
	for(i = 0; i < Xlsx_Data.GetSize(); i++)
	{
		printf("%d, %d\n", Xlsx_Data[i].x, Xlsx_Data[i].y);
	}
	m_button_loadfile.EnableWindow(false);
	fclose(fp);
}

void CTeenCalibrateView::OnButtonWrite2File()
{
	// TODO: Add your control notification handler code here
	//CString m_data = "C:\\Parameters";
	fp = fopen("E:\\Lab Project\\TeenSize\\TeenCalibrate\\Parameters.txt","w");
	
	int i, j;
	for(i = 0; i < 4; i++)
	{
		fprintf(fp, "%f\n", pImageProcess->Curve_Parameters[i]);
	}
	
	fclose(fp);
	m_button_writetofile.EnableWindow(false);
}

void CTeenCalibrateView::OnButtonLoadsamples()
{
	// TODO: Add your control notification handler code here
	fp = fopen("E:\\Lab Project\\TeenSize\\TeenCalibrate\\BALL.txt", "r");
	
	int i;
	for(i = 0; i < 10000; i++)
	{
		int h, s;
		if(fscanf(fp,"%d, %d\n", &h, &s) != EOF)
		{
			pImageProcess->hs_pt.Add(cvPoint(s, h));
		}
		else
			break;
	}
	for(i = 0; i < pImageProcess->hs_pt.GetSize(); i++)
	{
		printf("%d, %d\n", pImageProcess->hs_pt[i].x, pImageProcess->hs_pt[i].y);
	}
	m_button_loadsamples.EnableWindow(false);
	fclose(fp);
}

void CTeenCalibrateView::OnButtonLoadCurveParameters()
{
	// TODO: Add your control notification handler code here
	// 	fp = fopen("BALL.txt", "r");
	fp = fopen("E:\\Lab Project\\TeenSize\\TeenCalibrate\\Parameters.txt", "r");
	

	int i;
	for(i = 0; i < 10; i++)
	{
		float temp;
		if(fscanf(fp,"%f\n", &temp) != EOF)
		{
			pImageProcess->Curve_Parameters[i] = double(temp);
		}
		else
			break;
	}

	for(i = 0; i < 4; i++)
	{
		printf("%f\n", pImageProcess->Curve_Parameters[i]);
	}
	

	fclose(fp);
	m_button_curveparameters.EnableWindow(false);
}

void CTeenCalibrateView::OnButtonCalculate()
{
	// TODO: Add your control notification handler code here
	int i, j;
	CArray <double, double> x_data, y_data;
	for(j = 0; j < 4; j++)
	{
		for(i = 0; i < Xlsx_Data.GetSize(); i++)
		{
			double x_norm = Xlsx_Data[i].x / 255.0;
			double y_norm = Xlsx_Data[i].y / 255.0;
			x_data.Add(x_norm);
			y_data.Add(y_norm);
		}
	}


// 	for(i = 0; i < x_data.GetSize(); i++)
// 	{
// 		printf("%f, %f\n", x_data[i], y_data[i]);
// 	}

	double summation_x[6] = {0};			//each index refer to power exponent times

	for(i = 0; i < x_data.GetSize(); i++)
	{
		summation_x[0] += x_data[i];
		summation_x[1] += x_data[i]*x_data[i];
		summation_x[2] += x_data[i]*x_data[i]*x_data[i];
		summation_x[3] += x_data[i]*x_data[i]*x_data[i]*x_data[i];
		summation_x[4] += x_data[i]*x_data[i]*x_data[i]*x_data[i]*x_data[i];
		summation_x[5] += x_data[i]*x_data[i]*x_data[i]*x_data[i]*x_data[i]*x_data[i];
	}
/*
	for(j = 0; j < 4; j++)
	{
		for(i = 0; i < 6; i++)
		{
			printf("%f\n", summation_x[j][i]);
		}
		printf("\n");
	}
*/

	double summation_yx[4] = {0};

	for(i = 0; i < y_data.GetSize(); i++)
	{
		summation_yx[0] += y_data[i];
		summation_yx[1] += y_data[i]*x_data[i];
		summation_yx[2] += y_data[i]*(x_data[i]*x_data[i]);
		summation_yx[3] += y_data[i]*(x_data[i]*x_data[i]*x_data[i]);
	}
/*
	for(j = 0; j < 4; j++)
	{
		for(i = 0; i < 4; i++)
		{
			printf("%f\n", summation_yx[j][i]);
		}
		printf("\n");
	}
*/

	
	int m = x_data.GetSize();


	double x_array[] = {m             , summation_x[0], summation_x[1], summation_x[2],
				        summation_x[0], summation_x[1], summation_x[2], summation_x[3],
						summation_x[1], summation_x[2], summation_x[3], summation_x[4],
					    summation_x[2], summation_x[3], summation_x[4], summation_x[5]};

	CvMat *x_matrix;
	x_matrix = cvCreateMat(4, 4, CV_64FC1);
 	Arr2Mat(x_matrix, x_array, 4, 4);
 	CvMat *InverseMatrix;
	InverseMatrix = cvCreateMat(4, 4, CV_64FC1);
 	cvInvert(x_matrix, InverseMatrix, CV_LU);

	double y_array[] = {summation_yx[0], summation_yx[1], summation_yx[2], summation_yx[3]};
	CvMat *y_matrix;
	y_matrix = cvCreateMat(4, 1, CV_64FC1);
	Arr2Mat(y_matrix, y_array, 4, 1);

	Parameters_a = cvCreateMat(4, 1, CV_64FC1);
 	cvMatMul(InverseMatrix, y_matrix, Parameters_a);

	for(i = 0; i < 4; i++)
	{
		printf("%f\n", CV_MAT_ELEM(*Parameters_a, double, i, 0));
	}

	for(j = 0; j < 4; j++)
	{
		for(i = 0; i < 4; i++)
		{
			pImageProcess->Curve_Parameters[i] = CV_MAT_ELEM(*Parameters_a, double, i, 0);
		}
	}

	
// 	for(i = 0; i < 4; i++)
// 	{
// 		printf("%f\n", pImageProcess->Curve_Parameters[i]);
//    	}
	m_button_calculate.EnableWindow(false);
}

void CTeenCalibrateView::OnButtonPlot2D()
{
	// TODO: Add your control notification handler code here
	pImageProcess->PlotSamples();
	UpdateData(0);
}

void CTeenCalibrateView::OnButtonVideoCapture()
{
	// TODO: Add your control notification handler code here
	SetTimer(1, 10, NULL);
}

void CTeenCalibrateView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CFormView::OnLButtonDown(nFlags, point);
}

void CTeenCalibrateView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(isMeasureHSV)
	{
		if(point.x > m_WindowsBoundary.leftupboundx && point.y > m_WindowsBoundary.leftupboundy &&
			point.x < m_WindowsBoundary.rightdownboundx && point.y < m_WindowsBoundary.rightdownboundy)
		{
			int region_width = m_WindowsBoundary.rightdownboundx - m_WindowsBoundary.leftupboundx;		//region width
			int region_height = m_WindowsBoundary.rightdownboundy - m_WindowsBoundary.leftupboundy;		//region height
			
			//Map window coordinate to image coordinate
			int offsetx = m_WindowsBoundary.leftupboundx;
			int offsety = m_WindowsBoundary.leftupboundy;
			int correct_ptx = point.x - offsetx;
			int correct_pty = point.y - offsety;
			
			//Get image coordinate
			m_GetWindowsBoundary.leftupboundx = int(correct_ptx * IMAGEWIDTH / double(region_width));
			m_GetWindowsBoundary.leftupboundy = int(correct_pty * IMAGEHEIGHT / double(region_height));
			
			
			pImageProcess->SetROI(pImageProcess->ImgHSV, m_GetWindowsBoundary.leftupboundx, m_GetWindowsBoundary.leftupboundy,
				0, 0);
			ShowDiagrame(pImageProcess->ImgHSV, IDC_IMG_HSV_STATIC);
		}
	}

	CFormView::OnLButtonUp(nFlags, point);
}

void CTeenCalibrateView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CFormView::OnMouseMove(nFlags, point);
}

void CTeenCalibrateView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(point.x > m_WindowsBoundary.leftupboundx && point.y > m_WindowsBoundary.leftupboundy &&
	point.x < m_WindowsBoundary.rightdownboundx && point.y < m_WindowsBoundary.rightdownboundy)
	{
		pImageProcess->SetROI(pImageProcess->ImgHSV, m_GetWindowsBoundary.leftupboundx, m_GetWindowsBoundary.leftupboundy,
			0, 1);
		ShowDiagrame(pImageProcess->ImgHSV, IDC_IMG_HSV_STATIC);
	}

	isMeasureHSV = false;
	start_flag = true;
	pImageProcess->ColorScaleCollection(start_flag);

	pImageProcess->Boundspts.RemoveAll();

//	m_button_measurehsv.EnableWindow(false);

	CFormView::OnRButtonUp(nFlags, point);
}

