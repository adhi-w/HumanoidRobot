// TeenCalibrateView.h : interface of the CTeenCalibrateView class
//


#pragma once
#include "afxwin.h"

#include "cv.h"
#include "highgui.h"
#include "math.h"
#include <io.h>
#include <fcntl.h>

#include "ImageProcess.h"

#define IMAGEWIDTH 1920
#define IMAGEHEIGHT 1080

#define WM_THREAD (WM_USER + 20)

class CTeenCalibrateView : public CFormView
{
protected: // create from serialization only
	CTeenCalibrateView();
	DECLARE_DYNCREATE(CTeenCalibrateView)

public:
	enum{ IDD = IDD_TEENCALIBRATE_FORM };

// Attributes
public:
	CTeenCalibrateDoc* GetDocument() const;
	
	CImageProcess *pImageProcess;
	
	FILE *fp; 

public:	
	struct Boundarys
	{
		int leftupboundx;
		int leftupboundy;
		int rightdownboundx;
		int rightdownboundy;
	};

	Boundarys m_WindowsBoundary;
	Boundarys m_GetWindowsBoundary;			//Get pure region boundarys for image process

	bool isMeasureHSV;
	bool start_flag;

	CArray <CvPoint, CvPoint> Xlsx_Data;
	CvMat *Parameters_a;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	void Arr2Mat(CvMat *mat1, double Arr1[], int row, int col);
	void CallConsole();
	void ShowDiagrame(IplImage *img, UINT ID);
	static UINT DoThread(LPVOID pParam);
	afx_msg LRESULT OnThread(WPARAM wParam, LPARAM lParam);
	virtual ~CTeenCalibrateView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	CButton m_button_measurehsv;
	CButton m_button_savedata;
	CButton m_button_plot;
	CButton m_button_loadfile;
	CButton m_button_calculate;
	CButton m_button_writetofile;
	CButton m_button_curveparameters;
	CButton m_button_loadsamples;
	afx_msg void OnButtonMeasureHSV();
	afx_msg void OnButtonSaveDATA();
	afx_msg void OnButtonLoadfile();
	afx_msg void OnButtonCalculate();
	afx_msg void OnButtonWrite2File();
	afx_msg void OnButtonPlot2D();
	afx_msg void OnButtonVideoCapture();
	afx_msg void OnButtonLoadsamples();
	afx_msg void OnButtonLoadCurveParameters();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // debug version in TeenCalibrateView.cpp
inline CTeenCalibrateDoc* CTeenCalibrateView::GetDocument() const
   { return reinterpret_cast<CTeenCalibrateDoc*>(m_pDocument); }
#endif

