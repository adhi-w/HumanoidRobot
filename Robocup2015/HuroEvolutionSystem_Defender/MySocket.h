#if !defined(AFX_MYSOCKET_H__CDAB5659_A481_4FE4_85A1_13BAA242D77B__INCLUDED_)
#define AFX_MYSOCKET_H__CDAB5659_A481_4FE4_85A1_13BAA242D77B__INCLUDED_


#include "General.h"
#include <afxsock.h>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySocket.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CMySocket command target

class CMySocket : public CSocket
{
// Attributes
public:

	struct GameState
	{
		//gamebox : game state
		int gamestate;
		int behavior;
		int visualstate;
		//gamebox : rule state
		int Time;
		int Penalty;
		int Kickoff;
	};

// Operations
public:
	CMySocket();
	virtual ~CMySocket();
	char Buff[512];
	int BroadcastPort, LocalPort; 
	int Member, Team; 
	GameState m_GameState;

// Overrides
public:
	BOOL Send();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySocket)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CMySocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSOCKET_H__CDAB5659_A481_4FE4_85A1_13BAA242D77B__INCLUDED_)
