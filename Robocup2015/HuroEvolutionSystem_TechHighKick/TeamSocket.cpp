// TeamSocket.cpp : implementation file
//

#include "stdafx.h"
#include "TeamSocket.h"
#include "HuroEvolutionSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTeamSocket

CTeamSocket::CTeamSocket()
{
	Member = 0;
	BroadcastPort = 5757;

	if (!AfxSocketInit())
	{
		AfxMessageBox("Socket Fail");
//		return FALSE;
	}
}

CTeamSocket::~CTeamSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CTeamSocket, CSocket)
	//{{AFX_MSG_MAP(CTeamSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CTeamSocket member functions

void CTeamSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	int byte = Receive(TeamBuff, sizeof(TeamBuff));
	
	if(TeamBuff[0] == 254 && TeamBuff[3] == 255 && TeamBuff[1] != 1)
	{
		TeamRobot[0].Q_Ball = (bool)TeamBuff[2];
	}
	CSocket::OnReceive(nErrorCode);
}

void CTeamSocket::Send()
{
	int buf[10];

	SendTo(buf, sizeof(buf), BroadcastPort, "192.168.1.255");
}
