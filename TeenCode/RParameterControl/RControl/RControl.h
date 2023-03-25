// RControl.h : main header file for the RControl application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRControlApp:
// See RControl.cpp for the implementation of this class
//

class CRControlApp : public CWinApp
{
public:
	CRControlApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRControlApp theApp;