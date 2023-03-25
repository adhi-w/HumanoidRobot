// MySocket.cpp : implementation file
//

#include "stdafx.h"
#include "HuroEvolutionSystem.h"
#include "MySocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySocket

CMySocket::CMySocket()
{
	m_GameState.gamestate = INIT;
	BroadcastPort = 3838;
	LocalPort = 3940;
	Member = 1;
	Team = 31;//TEAMNUMBER;
	m_GameState.Kickoff = true;
	m_GameState.Penalty = false;

	
	if (!AfxSocketInit())
	{
		AfxMessageBox("Socket Fail");
	}
}

CMySocket::~CMySocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CMySocket, CSocket)
	//{{AFX_MSG_MAP(CMySocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CMySocket member functions

void CMySocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnAccept(nErrorCode);
}

void CMySocket::OnReceive(int nErrorCode) 
{
	int byte = Receive(Buff, sizeof(Buff));
	if(byte == 116)
	{
		//Game State
		if(Buff[20] == Team)
		{
			m_GameState.gamestate = Buff[9];
			if(Buff[16] > 0) m_GameState.Time = Buff[16] + Buff[17] * 256;
			else m_GameState.Time = (256 + Buff[16]) + Buff[17] * 256;
			//Goal (BLUE : TRUE, Yellow : FALSE)
			//if(Buff[22])
			//{
			//	m_GameState.OwnGoal = TRUE;
			//}else
			//{
			//	m_GameState.OwnGoal = FALSE;
			//}
			switch(Member)
			{
				case 0:
					m_GameState.Penalty = (int)Buff[24];		
					break;
				case 1:
					m_GameState.Penalty = (int)Buff[28];
					break;
				case 2:
					m_GameState.Penalty = (int)Buff[32];
					break;
			}
			//Kickoff
			if(Buff[11])
			{
				m_GameState.Kickoff = 0;
			}else
			{
				m_GameState.Kickoff = 1;
			}	
		}else if(Buff[68] == Team)
		{
			if(Buff[16] > 0) m_GameState.Time = Buff[16] + Buff[17] * 256;
			else m_GameState.Time = (256 + Buff[16]) + Buff[17] * 256;
			m_GameState.gamestate = Buff[9];
			//Goal (BLUE : TRUE, Yellow : FALSE)
// 			if(Buff[70])
// 			{
// 				m_GameState.OwnGoal = TRUE;
// 			}else
// 			{
// 				m_GameState.OwnGoal = FALSE;
// 			}
			switch(Member)
			{
			case 0:
				m_GameState.Penalty = (int)Buff[72];
				break;
			case 1:
				m_GameState.Penalty = (int)Buff[76];
				break;
			case 2:
				m_GameState.Penalty = (int)Buff[80];
				break;
			}
			//Kickoff
			if(Buff[11])
			{
				m_GameState.Kickoff = 0;
			}else
			{
				m_GameState.Kickoff = 1;
			}	
		}
	}
	CSocket::OnReceive(nErrorCode);
}

BOOL CMySocket::Send()
{
	char buf[20];
	buf[0] = 82;
	buf[1] = 71;
	buf[2] = 114;
	buf[3] = 116;
	buf[4] = 1;
	buf[5] = 0;
	buf[6] = 0;
	buf[7] = 0;
	buf[8] = Team;
	buf[9] = 0;
	buf[10] = Member;
	buf[11] = 0;
	buf[12] = 0;
	buf[13] = 0;
	buf[14] = 0;
	buf[15] = 0;
	buf[16] = 0;
	buf[17] = 0;
	buf[18] = 0;
	buf[19] = 0;
	SendTo(buf, 20, BroadcastPort, "192.168.0.152");//192.168.110.100
	return TRUE;
}
