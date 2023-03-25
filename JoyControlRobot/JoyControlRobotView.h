// JoyControlRobotView.h : interface of the CJoyControlRobotView class
//
/////////////////////////////////////////////////////////////////////////////
#include "Serial.h"
#include "JoyStick.h"
#include "math.h"

#if !defined(AFX_JOYCONTROLROBOTVIEW_H__489401A6_1F29_4A90_BC87_6D345DF212D5__INCLUDED_)
#define AFX_JOYCONTROLROBOTVIEW_H__489401A6_1F29_4A90_BC87_6D345DF212D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_THREAD1 (WM_USER + 10)

struct THREAD_INFO 
{
	CWnd *wnd; // Windows pointer
	CJoyStick *pJoyStick;
	int DX, DY, DZ;
	int button_status,previous_button_status;
	bool control_enable;
	double Radius, Ang;
	bool thread_enable;
	HANDLE hThread;
};


class CJoyControlRobotView : public CFormView
{
protected: // create from serialization only
	CJoyControlRobotView();
	DECLARE_DYNCREATE(CJoyControlRobotView)

public:
	//{{AFX_DATA(CJoyControlRobotView)
	enum { IDD = IDD_JOYCONTROLROBOT_FORM };
	int		m_port;
	//}}AFX_DATA

// Attributes
public:
	CJoyControlRobotDoc* GetDocument();

	CSerial m_serial;
	THREAD_INFO ThreadInfo;
	CWinThread *m_Thread;
	CString Msg,str;
	double Radius, Ang, Rotation;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJoyControlRobotView)
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
	void ControlPacket();
	static UINT DoThread1(LPVOID pParam);
	//Define a Function (OnThread) for handling the received Thread MESSAGE (WM_THREAD)
	afx_msg LRESULT OnThread1(WPARAM wParam, LPARAM lParam);
	virtual ~CJoyControlRobotView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CJoyControlRobotView)
	afx_msg void OnButtonConnect();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonStop();
	afx_msg void OnButtonRk();
	afx_msg void OnButtonLk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in JoyControlRobotView.cpp
inline CJoyControlRobotDoc* CJoyControlRobotView::GetDocument()
   { return (CJoyControlRobotDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOYCONTROLROBOTVIEW_H__489401A6_1F29_4A90_BC87_6D345DF212D5__INCLUDED_)
