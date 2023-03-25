// TeenVision.h : main header file for the TEENVISION application
//

#if !defined(AFX_TEENVISION_H__EE4ED3FD_6F0A_4D2F_B9E0_D3CB6D78507F__INCLUDED_)
#define AFX_TEENVISION_H__EE4ED3FD_6F0A_4D2F_B9E0_D3CB6D78507F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTeenVisionApp:
// See TeenVision.cpp for the implementation of this class
//

class CTeenVisionApp : public CWinApp
{
public:
	CTeenVisionApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeenVisionApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTeenVisionApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEENVISION_H__EE4ED3FD_6F0A_4D2F_B9E0_D3CB6D78507F__INCLUDED_)
