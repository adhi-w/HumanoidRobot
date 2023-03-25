#if !defined(AFX_TEAMSOCKET_H__E88BCEFF_96D1_4B6F_B2C3_FADD420E9DCE__INCLUDED_)
#define AFX_TEAMSOCKET_H__E88BCEFF_96D1_4B6F_B2C3_FADD420E9DCE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeamSocket.h : header file
//


#include <afxsock.h>		// MFC socket extensions

/////////////////////////////////////////////////////////////////////////////
// CTeamSocket command target

class CTeamSocket : public CSocket
{
	
struct Robot
{
	bool Q_Ball;
	bool kicknow;
	int Ball_dist_x;
	int Ball_dist_y;
};
// Attributes
public:
	
// Operations
public:
	//CGameState *pGameState;
	//CLocalization TeamRobot[3];
	int TeamBuff[512];
	int BroadcastPort;
	
	Robot TeamRobot[3];
	BOOL bTeamPenalty;
	int Member;
	CTeamSocket();
	virtual ~CTeamSocket();

// Overrides
public:
	void Send();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeamSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CTeamSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEAMSOCKET_H__E88BCEFF_96D1_4B6F_B2C3_FADD420E9DCE__INCLUDED_)
