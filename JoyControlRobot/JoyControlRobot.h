// JoyControlRobot.h : main header file for the JOYCONTROLROBOT application
//

#if !defined(AFX_JOYCONTROLROBOT_H__9AEF4EAE_4861_4DC7_BBE6_57312C2AAD11__INCLUDED_)
#define AFX_JOYCONTROLROBOT_H__9AEF4EAE_4861_4DC7_BBE6_57312C2AAD11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CJoyControlRobotApp:
// See JoyControlRobot.cpp for the implementation of this class
//

class CJoyControlRobotApp : public CWinApp
{
public:
	CJoyControlRobotApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJoyControlRobotApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CJoyControlRobotApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOYCONTROLROBOT_H__9AEF4EAE_4861_4DC7_BBE6_57312C2AAD11__INCLUDED_)
