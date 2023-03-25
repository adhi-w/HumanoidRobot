// HuroTeenGKView.h : interface of the CHuroTeenGKView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUROTEENGKVIEW_H__3B7781D2_A031_4FC8_9B8B_34A8777BFA47__INCLUDED_)
#define AFX_HUROTEENGKVIEW_H__3B7781D2_A031_4FC8_9B8B_34A8777BFA47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cv.h"
#include "highgui.h"

#include "HeadControl.h"
#include "RobotBehavior.h"
#include "ImageProcess.h"
#include "Serial.h"
#include "General.h"

#define WM_THREAD (WM_USER + 20)

class CHuroTeenGKView : public CFormView
{
protected: // create from serialization only
	CHuroTeenGKView();
	DECLARE_DYNCREATE(CHuroTeenGKView)

public:
	//{{AFX_DATA(CHuroTeenGKView)
	enum { IDD = IDD_HUROTEENGK_FORM };
	CButton	m_button_bodystart;
	CButton	m_button_manualcontrol;
	int		m_edit_processtime;
	double	m_edit_Pix_ballx;
	double	m_edit_Pix_bally;
	double	m_edit_Real_bally;
	double	m_edit_Real_ballx;
	int		m_edit_Head_horpos;
	int		m_edit_Head_verpos;
	int		m_edit_isGetball;
	int		m_edit_Ballgone;
	float	m_edit_vball;
	float	m_edit_odometeryX;
	CString	m_edit_decision;
	CString	m_falldown_show;
	//}}AFX_DATA

// Attributes
public:
	CHuroTeenGKDoc* GetDocument();

	CHeadControl	*pHeadControl;
	CRobotBehavior	*pRobot;

// Operations
public:
	struct ThreadInfo
	{
		CWnd *wnd;
		bool isTerminate;
		HANDLE hThread;
		
		CImageProcess *pImageProcess;
	};
	
	ThreadInfo m_Thread_img;
	FILE *FilePointer;

public:
	int ProcessTime;
	int Ctrl_Mode;
	
	bool isHeadStart;
	bool isBodyStart;
	bool isManulControl;
	bool isCatchImg;
	bool isImgDone;
	
	int Real_posx;
	int Real_posy;

	CString decision;

	float velocity;
	FILE* vData;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHuroTeenGKView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnMovedOmni(int x, int y, int theta);
	
	void RobotState();
	void UpdateInfo();
	void HeadState();

	void LoadParameter();

	afx_msg LRESULT OnThread(WPARAM wParam, LPARAM lParam);
	static UINT DoThread(LPVOID pParam);
	virtual ~CHuroTeenGKView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHuroTeenGKView)
	afx_msg void OnButtonHeadStart();
	afx_msg void OnButtonBodyStart();
	afx_msg void OnButtonSaveImage();
	afx_msg void OnButtonEmergencyStop();
	afx_msg void OnButtonManualControl();
	afx_msg void OnButtonBodyStop();
	afx_msg void OnButtonBodyForward();
	afx_msg void OnButtonBodyBackward();
	afx_msg void OnButtonBodyShiftRight();
	afx_msg void OnButtonBodyShiftLeft();
	afx_msg void OnButtonBodyStepIt();
	afx_msg void OnButtonBodyTurnRight();
	afx_msg void OnButtonBodyTurnLeft();
	afx_msg void OnButtonViewLeft();
	afx_msg void OnButtonViewRight();
	afx_msg void OnButtonViewDown();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonBodyLeftKick();
	afx_msg void OnButtonBodyRightKick();
	afx_msg void OnButtonBodyFallLeft();
	afx_msg void OnButtonBodyFallRight();
	afx_msg void OnChangeEdit13();
	afx_msg void OnChangeEdit12();
	afx_msg void OnButton21();
	afx_msg void OnButton22();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in HuroTeenGKView.cpp
inline CHuroTeenGKDoc* CHuroTeenGKView::GetDocument()
   { return (CHuroTeenGKDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HUROTEENGKVIEW_H__3B7781D2_A031_4FC8_9B8B_34A8777BFA47__INCLUDED_)
