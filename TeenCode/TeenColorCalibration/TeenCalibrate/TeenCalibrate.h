// TeenCalibrate.h : main header file for the TeenCalibrate application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CTeenCalibrateApp:
// See TeenCalibrate.cpp for the implementation of this class
//

class CTeenCalibrateApp : public CWinApp
{
public:
	CTeenCalibrateApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CTeenCalibrateApp theApp;