// HuroEvolutionSystem.h : main header file for the HUROEVOLUTIONSYSTEM application
//

#if !defined(AFX_HUROEVOLUTIONSYSTEM_H__4611712C_FA6A_42DE_9BFF_E0F1381249AD__INCLUDED_)
#define AFX_HUROEVOLUTIONSYSTEM_H__4611712C_FA6A_42DE_9BFF_E0F1381249AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CHuroEvolutionSystemApp:
// See HuroEvolutionSystem.cpp for the implementation of this class
//

class CHuroEvolutionSystemApp : public CWinApp
{
public:
	CHuroEvolutionSystemApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHuroEvolutionSystemApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CHuroEvolutionSystemApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HUROEVOLUTIONSYSTEM_H__4611712C_FA6A_42DE_9BFF_E0F1381249AD__INCLUDED_)
