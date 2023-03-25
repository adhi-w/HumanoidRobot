// TeenVisionView.h : interface of the CTeenVisionView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEENVISIONVIEW_H__AE09D28B_C99A_48F8_BBF2_6DEE6C24F3FC__INCLUDED_)
#define AFX_TEENVISIONVIEW_H__AE09D28B_C99A_48F8_BBF2_6DEE6C24F3FC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ImageProcessing.h"
#include "Behavior.h"
#include "Head.h"
#include "General.h"

#define WM_THREAD (WM_USER + 20)

class CTeenVisionView : public CFormView
{
protected: // create from serialization only
	CTeenVisionView();
	DECLARE_DYNCREATE(CTeenVisionView)

public:
	//{{AFX_DATA(CTeenVisionView)
	enum { IDD = IDD_TEENVISION_FORM };
	int		m_edit_ComPort;
	int		m_edit_RemoteComPort;
	CString	m_message;
	int		m_edit_ProcessingTime;
	int		m_edit_distX;
	int		m_edit_distY;
	CString	m_edit_Status;
	CString	m_edit_Decision;
	//}}AFX_DATA

// Attributes
public:
	CTeenVisionDoc* GetDocument();

// Operations
// public:
	struct ImThread
	{
		CWnd *wnd;
		bool isTerminate;
		HANDLE hThread;
		ImageProcessing *imagePro;
	};

public:

	ImThread m_ImThread;
	int ProcessTime;

	//ImageProcessing *imagePro;
	Behavior behavior;
	Head *head;

	bool moved;

	int rx,ry;
	double dist;
		
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeenVisionView)
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
	void ShowPicture(IplImage *img, UINT ID);
	void LoadParam();
	void Message();

	afx_msg LRESULT OnThread(WPARAM wParam, LPARAM lParam);
	static UINT DoThread(LPVOID pParam);

	virtual ~CTeenVisionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTeenVisionView)
	afx_msg void OnButtonStart();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonDisconnect();
	afx_msg void OnButtonMoved();
	afx_msg void OnButtonForward();
	afx_msg void OnButtonShiftLeft();
	afx_msg void OnButtonShiftRight();
	afx_msg void OnButtonBackward();
	afx_msg void OnButtonFastWalk();
	afx_msg void OnButtonWalk();
	afx_msg void OnButtonStartStop();
	afx_msg void OnButtonTurnOff();
	afx_msg void OnChangeEditComPort();
	afx_msg void OnChangeEditRemoteComPort();
	afx_msg void OnButtonExit();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButton_FallDownLeft();
	afx_msg void OnButton_FallDownRight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TeenVisionView.cpp
inline CTeenVisionDoc* CTeenVisionView::GetDocument()
   { return (CTeenVisionDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEENVISIONVIEW_H__AE09D28B_C99A_48F8_BBF2_6DEE6C24F3FC__INCLUDED_)
