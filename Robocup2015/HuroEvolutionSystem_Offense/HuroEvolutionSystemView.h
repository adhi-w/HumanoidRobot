// HuroEvolutionSystemView.h : interface of the CHuroEvolutionSystemView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUROEVOLUTIONSYSTEMVIEW_H__AAE275CF_E651_41C9_AC25_0F93F4A4DFB8__INCLUDED_)
#define AFX_HUROEVOLUTIONSYSTEMVIEW_H__AAE275CF_E651_41C9_AC25_0F93F4A4DFB8__INCLUDED_

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
#include "MySocket.h"
#include "StdAfx.h"


#define WM_THREAD (WM_USER + 20)

class CHuroEvolutionSystemView : public CFormView
{
protected: // create from serialization only
	CHuroEvolutionSystemView();
	DECLARE_DYNCREATE(CHuroEvolutionSystemView)

public:
	//{{AFX_DATA(CHuroEvolutionSystemView)
	enum { IDD = IDD_HUROEVOLUTIONSYSTEM_FORM };
	CButton	m_button_bodystart;
	CButton	m_button_manulcontrol;
	CButton	m_button_LoadParameter;
	double	m_edit_Pix_ballx;
	double	m_edit_Pix_bally;
	double	m_edit_Real_ballx;
	double	m_edit_Real_bally;
	int		m_edit_Head_horpos;
	int		m_edit_Head_verpos;
	int		m_edit_processtime;
	int		m_edit_isGetball;
	int		m_edit_GrabTime;
	int		m_edit_ballmode;
	int		m_edit_odometery_y;
	int		m_edit_odometery_x;
	double	m_edit_compass;
	int		m_edit_closeball;
	int		m_edit_KickMode;
	CString	m_edit_GameState;
	CString	m_edit_Behavior;
	int		m_edit_isGoalSide;
	int		m_edit_CompassInit;
	int		m_edit_CompassNorth;
	int		m_edit_CompassWest;
	int		m_edit_CompassSouth;
	int		m_edit_CompassEast;
	int		m_edit_Gamebegining;
	int		m_edit_DecisionRand;
	int		m_edit_odemtheta;
	int		m_edit_robotface;
	int		m_edit_gamebox_kickoff;
	int		m_edit_gamebox_penalty;
	int		m_edit_gamebox_time;
	int		m_edit_Timewait;
	int		m_edit_RobotStep;
	//}}AFX_DATA

// Attributes
public:
	CHuroEvolutionSystemDoc* GetDocument();

	CHeadControl	*pHeadControl;
	CRobotBehavior	*pRobot;
	CMySocket		*pMySocket;

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
	//map
	IplImage *Map;
	CvPoint Robot1;
	CvPoint Robot2;
	CvPoint Robot_neck;
	CvPoint Target_Line;
	CvPoint Goal_Point;
	CvPoint Dribble_Point;
	CvPoint Robot_Location;

public:
	int ProcessTime;
	int Ctrl_Mode;
	int Kick_Mode;

	bool isHeadStart;
	bool isBodyStart;
	bool isManulControl;
	bool isCatchImg;
	bool isImgDone;
	bool isRobotFront;
	bool isGoalSide;
	bool isRobotKick;
	bool isReadyState;
	bool isNeckLock;
	bool isDeffence;

	int Compass_val;
	int CompassInit;
	int CompassNorth;
	int CompassWest;
	int CompassSouth;
	int CompassEast;
	int RobotDirection;
	int RG_theta;
	int TimeSystem;

	int Decision_rand;
	int Map_width;
	int Map_Height;
	

	bool isGameBegining;
	bool isCountOde;
	bool isCloseFinish;
	bool isKickFinish;
	bool isShiftMotion;

	int Real_posx;
	int Real_posy;
	int prex;
	int prey;

	int GameState;
	int RobotBehavior;
	
public:
	bool Flag_closeball;
	int Penalty;
	int Kickoff;
	int TimeWait;
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHuroEvolutionSystemView)
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
	void BigInitTurn();
	void AutonomousWalk();
	void InitSelfTurn();
	void GameBox();
	void DribbleRun();
	void DribblingDecision();
	void KickDecision();
	void MeasureDis();
	void RandomAns();
	void UpdateBavaior();
	void RobotDecision();
	void GameRound();
	void UpdateRobotPos();
	void UpdateMap();
	void CloseBall();
	void OnMovedOmni(int x, int y, int theta);

	void RobotState();
	void UpdateInfo();
	void HeadState();
	afx_msg LRESULT OnThread(WPARAM wParam, LPARAM lParam);
	static UINT DoThread(LPVOID pParam);
	virtual ~CHuroEvolutionSystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHuroEvolutionSystemView)
	afx_msg void OnButtonBodyStop();
	afx_msg void OnButtonBodyForward();
	afx_msg void OnButtonBodyBackward();
	afx_msg void OnButtonBodyShiftRight();
	afx_msg void OnButtonBodyShiftLeft();
	afx_msg void OnButtonBodyTurnLeft();
	afx_msg void OnButtonBodyTurnRight();
	afx_msg void OnButtonSaveImage();
	afx_msg void OnButtonEmergencyStop();
	afx_msg void OnButtonHeadStart();
	afx_msg void OnButtonBodyStart();
	afx_msg void OnButtonLoadParameter();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonManulControl();
	afx_msg void OnButtonViewleft();
	afx_msg void OnButtonViewright();
	afx_msg void OnButtonViewDown();
	afx_msg void OnButtonStepIt();
	afx_msg void OnKickleft();
	afx_msg void OnKickright();
	afx_msg void OnFallleft();
	afx_msg void OnFallright();
	afx_msg void OnKickleftStraight();
	afx_msg void OnKickrightStraight();
	afx_msg void OnButtonGAMEINIT();
	afx_msg void OnButtonGAMEREADY();
	afx_msg void OnButtonGAMESET();
	afx_msg void OnButtonGAMEPLAY();
	afx_msg void OnButtonCompassINIT();
	afx_msg void OnButtonCompassCalibrate();
	afx_msg void OnButtonCompassSetNorth();
	afx_msg void OnButtonCompassSetWest();
	afx_msg void OnButtonCompassSetSouth();
	afx_msg void OnButtonCompassSetEast();
	afx_msg void OnButtonBodySurrundRight();
	afx_msg void OnButtonBodySurrundLeft();
	afx_msg void OnButtonKickView();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in HuroEvolutionSystemView.cpp
inline CHuroEvolutionSystemDoc* CHuroEvolutionSystemView::GetDocument()
   { return (CHuroEvolutionSystemDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HUROEVOLUTIONSYSTEMVIEW_H__AAE275CF_E651_41C9_AC25_0F93F4A4DFB8__INCLUDED_)
