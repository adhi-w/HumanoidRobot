// FIRA_PK procView.h : interface of the CFIRA_PKprocView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIRA_PKPROCVIEW_H__3BBA3A73_405F_4EEE_8734_66BCE8A8238E__INCLUDED_)
#define AFX_FIRA_PKPROCVIEW_H__3BBA3A73_405F_4EEE_8734_66BCE8A8238E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageProcess.h"

#include "HeadControl.h"
#include "Decision.h"

#include "Serial.h"
#include "cv.h"
#include "highgui.h"


#define WM_THREAD (WM_USER + 20)

class CFIRA_PKprocView : public CFormView
{
protected: // create from serialization only
	CFIRA_PKprocView();
	DECLARE_DYNCREATE(CFIRA_PKprocView)

public:
	//{{AFX_DATA(CFIRA_PKprocView)
	enum { IDD = IDD_FIRA_PKPROC_FORM };
	CButton	m_button_pathplaning;
	CButton	m_button_loadparameters;
	CButton	m_button_selfturnleft;
	CButton	m_button_selfturnright;
	CButton	m_button_stop;
	CButton	m_button_leftkick;
	CButton	m_button_rightkick;
	CButton	m_button_backward;
	CButton	m_button_shiftright;
	CButton	m_button_shiftleft;
	CButton	m_button_forward;
	int		m_edit_headstate;
	int		m_edit_gamestate;
	int		m_edit_posx;
	int		m_edit_posy;
	int		m_edit_horpos;
	int		m_edit_verpos;
	int		m_edit_odometerx;
	int		m_edit_odometery;
	int		m_edit_odometertheta;
	int		m_edit_distance;
	int		m_edit_targetmode;
	int		m_edit_obstaclecondition;
	int		m_edit_qobstacle;
	int		m_edit_robstaclex;
	int		m_edit_robstacley;
	int		m_edit_lobstaclex;
	int		m_edit_lobstacley;
	//}}AFX_DATA

// Attributes
public:

	struct Img_thread
	{
		CWnd *wnd;
		bool isTerminate;
		HANDLE hThread;

		CImageProcess *pImageProcess;
	};

	struct Odometer
	{
		int x, tx;
		int y, ty;
		int theta, ttheta;
		bool flag_stop;
		bool flag_clear;
	};
public:

	//Map informations

	CPoint Robot_pos;
	CPoint Robot_error;
	CPoint Robot_offset;
	CPoint Origin_pos;
	CPoint Target_pos;
	CPoint L_obst_pos[2];
	CPoint ML_obst_pos[2];
	CPoint MR_obst_pos[2];
	CPoint R_obst_pos[2];
	CPoint Ball_pos;
	CPoint Sensorline[2];
	CPoint GoalKeeper;

	CArray <CPoint, CPoint>Path_pos[2];
	
public:

	bool isPlaningpath;
	
	bool flag_obstacle;
	bool flag_ball;
	bool flag_localize;
	bool flag_endcalibrate;
	bool flag_start;


	bool isGetPath;
	bool isObstacle[3];
	bool isFirstGet;
	bool isRotate;
	bool isGoalkeeper;

	bool isSimulate;
	bool isRobotInit;
	short Simulate_mode;
	short Test_mode;
	short Target_mode;//1:left, 2:right
	short Wrongside;
	

	short Gamestate;

public:

	bool HeadtrajStart;
	bool Img_catchstart;
	bool Img_processfinish;

	short Head_state;

public:
	CFIRA_PKprocDoc* GetDocument();
	
	CSerial m_Serial;

	Img_thread m_Imgthread;
	Odometer m_Odemeter_info;

	CDecision *pDecision;
	CHeadControl *pHeadcontrol;

	FILE *Fp1;
	FILE *Fp2;
	FILE *Fp3;

	CvPoint TopLeft;
	CvPoint DownRight;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFIRA_PKprocView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnKickingBall();
	void OnSelfturnrihgt();
	void OnSelfturnleft();
	void rCloseBall();
	bool rObstacleAvoidance();
	bool rCheckBallPos();
	bool lCheckBallPos();
	bool rDribbling();
	void OnStandWalk();
	bool lCloseBall();
	bool lObstacleAvoidance();
	void OnDribblingBall();
	bool lDribbling();
	void PassThrough();
	void ReceiveData();
	bool DribbleAndPassby();
	void GameState();
	void PathPlanning(CPoint robot, CPoint target, CPoint l_obst[2], CPoint ml_obst[], CPoint mr_obst[], CPoint r_obst[2]);
	afx_msg LRESULT OnThread(WPARAM wParam, LPARAM lParam);
	static UINT DoThread(LPVOID pParam);
	virtual ~CFIRA_PKprocView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFIRA_PKprocView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonForward();
	afx_msg void OnButtonShiftleft();
	afx_msg void OnButtonShiftright();
	afx_msg void OnButtonBackward();
	afx_msg void OnButtonRightkick();
	afx_msg void OnButtonLeftkick();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonSelfturnright();
	afx_msg void OnButtonSelfturnleft();
	afx_msg void OnButtonLoadparameters();
	afx_msg void OnButtonPathplaning();
	afx_msg void OnButtonHeadtrajectory();
	afx_msg void OnButtonSimulate();
	afx_msg void OnButtonOmniLeft();
	afx_msg void OnButtonOmniRight();
	afx_msg void OnModeTest1();
	afx_msg void OnButtonModeTest2();
	afx_msg void OnButtonCalibrateView();
	afx_msg void OnButtonGoalleft();
	afx_msg void OnButtonGoalright();
	afx_msg void OnButtonWrongside();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in FIRA_PK procView.cpp
inline CFIRA_PKprocDoc* CFIRA_PKprocView::GetDocument()
   { return (CFIRA_PKprocDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIRA_PKPROCVIEW_H__3BBA3A73_405F_4EEE_8734_66BCE8A8238E__INCLUDED_)
