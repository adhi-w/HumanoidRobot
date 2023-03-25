// OdemeterTestView.h : interface of the COdemeterTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODEMETERTESTVIEW_H__B4DC5F47_E05F_4638_979B_85CEE4989866__INCLUDED_)
#define AFX_ODEMETERTESTVIEW_H__B4DC5F47_E05F_4638_979B_85CEE4989866__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cv.h>
#include <highgui.h>

#include "stdafx.h"
#include "math.h"

#include "BodyControl.h"
#include "HeadControl.h"
#include "General.h"
#include "Serial.h"

class COdemeterTestView : public CFormView
{
protected: // create from serialization only
	COdemeterTestView();
	DECLARE_DYNCREATE(COdemeterTestView)

public:
	//{{AFX_DATA(COdemeterTestView)
	enum { IDD = IDD_ODEMETERTEST_FORM };
	double	m_edit_odemter_x;
	double	m_edit_odemter_y;
	double	m_edit_odemeter_x_unit;
	double	m_edit_odemeter_y_unit;
	double	m_edit_compass;
	//}}AFX_DATA

// Attributes
public:
	COdemeterTestDoc* GetDocument();

	CBodyControl *pBodyControl;
	CHeadControl *pHeadControl;

	int Motion_Type;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COdemeterTestView)
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
	virtual ~COdemeterTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COdemeterTestView)
	afx_msg void OnBtn_Forward();
	afx_msg void OnBtn_Backward();
	afx_msg void OnBtn_LeftShift();
	afx_msg void OnBtn_RightShift();
	afx_msg void OnBtn_Stop();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtn_Step();
	afx_msg void OnBtn_Init();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in OdemeterTestView.cpp
inline COdemeterTestDoc* COdemeterTestView::GetDocument()
   { return (COdemeterTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODEMETERTESTVIEW_H__B4DC5F47_E05F_4638_979B_85CEE4989866__INCLUDED_)
