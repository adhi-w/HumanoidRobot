// FIRA_PK proc.h : main header file for the FIRA_PK PROC application
//

#if !defined(AFX_FIRA_PKPROC_H__CBB85603_FB23_4B02_89C8_9BE48DE790B5__INCLUDED_)
#define AFX_FIRA_PKPROC_H__CBB85603_FB23_4B02_89C8_9BE48DE790B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CFIRA_PKprocApp:
// See FIRA_PK proc.cpp for the implementation of this class
//

class CFIRA_PKprocApp : public CWinApp
{
public:
	CFIRA_PKprocApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFIRA_PKprocApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CFIRA_PKprocApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIRA_PKPROC_H__CBB85603_FB23_4B02_89C8_9BE48DE790B5__INCLUDED_)
