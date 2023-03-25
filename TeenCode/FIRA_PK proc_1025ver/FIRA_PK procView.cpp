// FIRA_PK procView.cpp : implementation of the CFIRA_PKprocView class
//

#include "stdafx.h"
#include "FIRA_PK proc.h"

#include "FIRA_PK procDoc.h"
#include "FIRA_PK procView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFIRA_PKprocView

IMPLEMENT_DYNCREATE(CFIRA_PKprocView, CFormView)

BEGIN_MESSAGE_MAP(CFIRA_PKprocView, CFormView)
	//{{AFX_MSG_MAP(CFIRA_PKprocView)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonForward)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonShiftleft)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonShiftright)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonBackward)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonRightkick)
	ON_BN_CLICKED(IDC_BUTTON6, OnButtonLeftkick)
	ON_BN_CLICKED(IDC_BUTTON7, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON8, OnButtonSelfturnright)
	ON_BN_CLICKED(IDC_BUTTON9, OnButtonSelfturnleft)
	ON_BN_CLICKED(IDC_BUTTON10, OnButtonLoadparameters)
	ON_BN_CLICKED(IDC_BUTTON11, OnButtonPathplaning)
	ON_BN_CLICKED(IDC_BUTTON12, OnButtonHeadtrajectory)
	ON_BN_CLICKED(IDC_BUTTON13, OnButtonSimulate)
	ON_BN_CLICKED(IDC_BUTTON14, OnButtonOmniLeft)
	ON_BN_CLICKED(IDC_BUTTON15, OnButtonOmniRight)
	ON_BN_CLICKED(IDC_BUTTON16, OnModeTest1)
	ON_BN_CLICKED(IDC_BUTTON17, OnButtonModeTest2)
	ON_BN_CLICKED(IDC_BUTTON18, OnButtonCalibrateView)
	ON_BN_CLICKED(IDC_BUTTON19, OnButtonGoalleft)
	ON_BN_CLICKED(IDC_BUTTON20, OnButtonGoalright)
	ON_BN_CLICKED(IDC_BUTTON21, OnButtonWrongside)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_MESSAGE(WM_THREAD, OnThread)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFIRA_PKprocView construction/destruction

CFIRA_PKprocView::CFIRA_PKprocView()
	: CFormView(CFIRA_PKprocView::IDD)
{
	//{{AFX_DATA_INIT(CFIRA_PKprocView)
	m_edit_headstate = 0;
	m_edit_gamestate = 0;
	m_edit_posx = 0;
	m_edit_posy = 0;
	m_edit_horpos = 0;
	m_edit_verpos = 0;
	m_edit_odometerx = 0;
	m_edit_odometery = 0;
	m_edit_odometertheta = 0;
	m_edit_distance = 0;
	m_edit_targetmode = 0;
	m_edit_obstaclecondition = 0;
	m_edit_qobstacle = 0;
	m_edit_robstaclex = 0;
	m_edit_robstacley = 0;
	m_edit_lobstaclex = 0;
	m_edit_lobstacley = 0;
	//}}AFX_DATA_INIT
	// TODO: add construction code here

	m_Imgthread.pImageProcess = new CImageProcess();
	pHeadcontrol = new CHeadControl(COM_PORT, BUNDRATE);
	pDecision = new CDecision();

	isPlaningpath = false;
	isFirstGet = true;
	isGetPath = false;
	isRobotInit = false;
	isGoalkeeper = false;

	Img_catchstart = false;
	Img_processfinish = false;
	
	Gamestate = 1;

	Robot_offset = CPoint(0, 0);
	Robot_error = CPoint(0, 0);
	//==================Ball region====================//
// 	TopLeft   = cvPoint(195, 175);
// 	DownRight = cvPoint(215, 195);

	TopLeft   = cvPoint(115, 175);
	DownRight = cvPoint(135, 195);

	//==================Head flag======================//
	HeadtrajStart = false;
	isObstacle[0] = isObstacle[1] = isObstacle[2] = false;

	//=================================================//
	m_Odemeter_info.x = 0;
	m_Odemeter_info.y = 0;
	m_Odemeter_info.theta = 0;
	m_Odemeter_info.tx = 0;
	m_Odemeter_info.ty = 0;
	m_Odemeter_info.ttheta = 0;
	m_Odemeter_info.flag_stop = false;
	m_Odemeter_info.flag_clear = false;
	//===================flags=========================//
	flag_obstacle = false;
	flag_ball = false;
	flag_localize = false;
	flag_endcalibrate = false;
	flag_start = false;
	//===================Simulate======================//
	isSimulate = false;
	isRotate = false;
	Simulate_mode = 0;
	Test_mode = 1;
	Target_mode = 2;
	Wrongside = 0;
}

CFIRA_PKprocView::~CFIRA_PKprocView()
{
	OnButtonStop();
	m_Imgthread.isTerminate = true;
	delete m_Imgthread.pImageProcess;
	delete pDecision;
	delete pHeadcontrol;
}

void CFIRA_PKprocView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFIRA_PKprocView)
	DDX_Control(pDX, IDC_BUTTON11, m_button_pathplaning);
	DDX_Control(pDX, IDC_BUTTON10, m_button_loadparameters);
	DDX_Control(pDX, IDC_BUTTON9, m_button_selfturnleft);
	DDX_Control(pDX, IDC_BUTTON8, m_button_selfturnright);
	DDX_Control(pDX, IDC_BUTTON7, m_button_stop);
	DDX_Control(pDX, IDC_BUTTON6, m_button_leftkick);
	DDX_Control(pDX, IDC_BUTTON5, m_button_rightkick);
	DDX_Control(pDX, IDC_BUTTON4, m_button_backward);
	DDX_Control(pDX, IDC_BUTTON3, m_button_shiftright);
	DDX_Control(pDX, IDC_BUTTON2, m_button_shiftleft);
	DDX_Control(pDX, IDC_BUTTON1, m_button_forward);
	DDX_Text(pDX, IDC_EDIT1, m_edit_headstate);
	DDX_Text(pDX, IDC_EDIT2, m_edit_gamestate);
	DDX_Text(pDX, IDC_EDIT3, m_edit_posx);
	DDX_Text(pDX, IDC_EDIT4, m_edit_posy);
	DDX_Text(pDX, IDC_EDIT5, m_edit_horpos);
	DDX_Text(pDX, IDC_EDIT6, m_edit_verpos);
	DDX_Text(pDX, IDC_EDIT7, m_edit_odometerx);
	DDX_Text(pDX, IDC_EDIT8, m_edit_odometery);
	DDX_Text(pDX, IDC_EDIT9, m_edit_odometertheta);
	DDX_Text(pDX, IDC_EDIT10, m_edit_distance);
	DDX_Text(pDX, IDC_EDIT11, m_edit_targetmode);
	DDX_Text(pDX, IDC_EDIT12, m_edit_obstaclecondition);
	DDX_Text(pDX, IDC_EDIT13, m_edit_qobstacle);
	DDX_Text(pDX, IDC_EDIT14, m_edit_robstaclex);
	DDX_Text(pDX, IDC_EDIT15, m_edit_robstacley);
	DDX_Text(pDX, IDC_EDIT16, m_edit_lobstaclex);
	DDX_Text(pDX, IDC_EDIT17, m_edit_lobstacley);
	//}}AFX_DATA_MAP
}

BOOL CFIRA_PKprocView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CFIRA_PKprocView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

//	OnButtonLoadparameters();

	m_Imgthread.isTerminate = false;
	m_Imgthread.wnd = this;
	m_Imgthread.hThread = CreateEvent( 0, 1, 0, "MyEvent");
 	AfxBeginThread(DoThread,(LPVOID) this);

	if(	m_Serial.Open(B_COM_PORT, B_BUNDRATE) )
	{
		//AfxMessageBox("Connect!");
	}

//	m_Imgthread.pImageProcess->ProcessProcedure(Gamestate, 1, 1);

	SetTimer(1, 50, NULL);
	SetTimer(2, 10, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CFIRA_PKprocView printing

BOOL CFIRA_PKprocView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFIRA_PKprocView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CFIRA_PKprocView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CFIRA_PKprocView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CFIRA_PKprocView diagnostics

#ifdef _DEBUG
void CFIRA_PKprocView::AssertValid() const
{
	CFormView::AssertValid();
}

void CFIRA_PKprocView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CFIRA_PKprocDoc* CFIRA_PKprocView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFIRA_PKprocDoc)));
	return (CFIRA_PKprocDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////

UINT CFIRA_PKprocView::DoThread(LPVOID pParam)
{
	CFIRA_PKprocView *pthis = (CFIRA_PKprocView *)pParam;
	CWnd *wnd = pthis->m_Imgthread.wnd;	
	
	while(!pthis->m_Imgthread.isTerminate)
	{
		
		pthis->m_Imgthread.pImageProcess->ProcessProcedure(pthis->Gamestate, pthis->Head_state, pthis->Img_catchstart, pthis->TopLeft, pthis->DownRight);
		
		wnd->PostMessage(WM_THREAD, 1, 0);
		//Wait for function signal response
		WaitForSingleObject(pthis->m_Imgthread.hThread, 30);
	}

	return 0;
}

LRESULT CFIRA_PKprocView::OnThread(WPARAM wParam, LPARAM lParam)
{
	Img_catchstart = pHeadcontrol->Catch_start;
	Img_processfinish = m_Imgthread.pImageProcess->ImgCatchFinish;
	Head_state = pHeadcontrol->Head_state;

	pHeadcontrol->Getball = m_Imgthread.pImageProcess->Q_ball;
	pHeadcontrol->Pos_ball = m_Imgthread.pImageProcess->Pos_ball;

	//find ball position
	if(pHeadcontrol->Getball)
	{
		flag_ball = true;
	}
	//Game state 1 is finished
	if(pHeadcontrol->Scan_Finish)
	{
		flag_obstacle = true;
	}

	//reset image switch value
	if(pHeadcontrol->Head_Init)
	{
		m_Imgthread.pImageProcess->ImgCatchFinish = false;
	}

	m_edit_headstate = Head_state;

	m_edit_gamestate = Gamestate;
	
	m_edit_horpos = pHeadcontrol->Head_hor;
	m_edit_verpos = pHeadcontrol->Head_ver;
	
	m_edit_odometerx = m_Odemeter_info.tx;
	m_edit_odometery = m_Odemeter_info.ty;
	m_edit_odometertheta = m_Odemeter_info.ttheta;
	m_edit_targetmode = Target_mode;

	UpdateData(false);
//	AfxMessageBox("GO");
	return 0;
}

// CFIRA_PKprocView message handlers

void CFIRA_PKprocView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(flag_start)
	{
		OnButtonHeadtrajectory();
		flag_start = false;
	}

	if(!isSimulate)
	{
		if(nIDEvent == 1)
		{
			if(Gamestate < 4)
				pHeadcontrol->HeadState(HeadtrajStart, Img_processfinish);
		}
		if(nIDEvent == 2)
		{
			GameState();
		}
		if(nIDEvent == 3)
		{
			if(!flag_endcalibrate)
				m_Imgthread.pImageProcess->ViewCalibrate();
			else
				KillTimer(3);
		}
	}
	else
	{
//		pHeadcontrol->HeadState(1, 1);
 		if(nIDEvent == 10)
		{

  /*			Gamestate = 1;
 			pHeadcontrol->Head_state = 5;
 			pHeadcontrol->Catch_start = true;
 			pHeadcontrol->Scan_Finish = true;
 			pHeadcontrol->V_Scan(1, 0);*/
			//pHeadcontrol->V_Lock();
			//ObstacleAvoidance();

			rDribbling();
 			switch(Simulate_mode)
			{
				case Forward:
					OnButtonForward();
					break;
				case ShiftLeft:
					OnButtonShiftleft();
					//m_Odemeter_info.ttheta = 0;
					break;
				case ShiftRight:
					OnButtonShiftright();
					//m_Odemeter_info.ttheta = 0;
					break;
				case Backward:
					OnButtonBackward();
					break;
				case SurroundBallLeft:
					OnButtonSelfturnleft();
					break;
				case SurroundBallRight:
					OnButtonSelfturnright();
					break;
				case STAND:
					OnButtonStop();
					break;
				case  TurnRight:
					OnModeTest1();
					break;
				case TurnLeft:
					OnButtonModeTest2();
					break;


			}
			//PassThrough();
		}
	}
	
	ReceiveData();
	Invalidate(false);
	CFormView::OnTimer(nIDEvent);
}

void CFIRA_PKprocView::GameState()
{
	//1.search and localize obstacles
	//2.search and localize ball
	//3.dribbling ball(first kick ball) 
	//4.search ball and localize ball position
	//5.pass through obstacle

	if(Gamestate == 1)
	{
		OnButtonStop();

		if(flag_obstacle && pHeadcontrol->Scan_Finish)
		{
			Gamestate = 2;
		}
	}
	else if(Gamestate == 2)
	{
		if(flag_ball)
		{
			int rx, ry;
			Estimate(m_Imgthread.pImageProcess->Pos_ball, double(pHeadcontrol->Head_hor), double(pHeadcontrol->Head_ver), &rx, &ry);
			
			double dis = Dist(rx, ry);
			Robot_offset = CPoint(rx, ry);	
			
			if(pHeadcontrol->TimeDelay(1500))
			{
				OnButtonPathplaning();
				Gamestate = 3;
				isRobotInit = true;
				isFirstGet = false;
				pHeadcontrol->Head_Init = true;
			}
		}
	}
	else if(Gamestate == 3)
	{
		if(Target_mode == 1)
		{
			if(lDribbling())
			{
				Gamestate = 4;
			}
		}
		else
		{
			if(rDribbling())
			{
				Gamestate = 4;
			}
		}
	}
	else if(Gamestate == 4)
	{
		if(Target_mode == 1)
		{
			if(lCheckBallPos())
			{
				Gamestate = 5;
			}
		}
		else
		{
			if(rCheckBallPos())
			{
				Gamestate = 5;
			}
		}
	}
	else if(Gamestate == 5)
	{
		pHeadcontrol->V_Lock();
		
		if(Target_mode == 1)
		{
			if(lObstacleAvoidance())
			{
				Gamestate = 6;
			}
		}
		else
		{
			if(rObstacleAvoidance())
			{
				Gamestate = 6;
			}
		}

	}
	else if(Gamestate == 6)
	{
		if(Target_mode == 1)
		{
			lCloseBall();
		}
		else
		{
			rCloseBall();
		}
	}

}

void CFIRA_PKprocView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CBitmap bmp;
	CDC mendc; 
	pDC = GetDC();

	int offset = 10;
	
	mendc.CreateCompatibleDC(pDC); 
	bmp.CreateCompatibleBitmap(pDC, 350, 250);
	mendc.SelectObject(&bmp);
	mendc.FillSolidRect(0, 0, 350, 250 + offset, RGB(0, 255, 0));

//=================kick region===========================

	CBrush yellowbrush;
	yellowbrush.CreateSolidBrush(RGB(255, 255, 0));
	mendc.SelectObject(yellowbrush);

	CPoint topLeft     = CPoint(25, 0 + offset);
	CPoint bottomRight = CPoint(325, 120 + offset);
	CRect rect = CRect(topLeft, bottomRight);
	mendc.Rectangle(rect);

//=======================================================
//================goalkeeper=============================
	
	mendc.SelectObject(yellowbrush);
	
	if(Gamestate == 1)
	{
		/*
		if(m_Imgthread.pImageProcess->Q_GaolKeeper)
		{
			int view_center = int(CAM_WIDTH / 2.0);
			if(m_Imgthread.pImageProcess->GoalKeeper_center.x > view_center + 15)
			{
				topLeft     = CPoint(50, 0 + offset);
				bottomRight = CPoint(175, 30 + offset);
				TopLeft   = cvPoint(195, 175);
				DownRight = cvPoint(215, 195);
				//Target_mode = 1;//left
			}
			else if(m_Imgthread.pImageProcess->GoalKeeper_center.x < view_center - 15)
			{
				topLeft     = CPoint(175, 0 + offset);
				bottomRight = CPoint(300, 30 + offset);
				
				TopLeft   = cvPoint(115, 175);
				DownRight = cvPoint(135, 195);
				Target_mode = 2;//right
			}
			else
			{
				TopLeft   = cvPoint(195, 175);
				DownRight = cvPoint(215, 195);
				//Target_mode = 1;
			}
			rect = CRect(topLeft, bottomRight);
			mendc.Rectangle(rect);
			//isGoalkeeper = true;
			
		}
		*/
	}
	
	yellowbrush.DeleteObject();
//=======================================================

//=================forbiden region=======================

	CBrush redbrush;
	redbrush.CreateSolidBrush(RGB(255, 0, 0));
	mendc.SelectObject(redbrush);
	
	topLeft     = CPoint(50, 0 + offset);
	bottomRight = CPoint(300, 30 + offset);
	rect = CRect(topLeft, bottomRight);
	mendc.Rectangle(rect);
	
	//redbrush.DeleteObject();
//=======================================================

//================obstacles region=======================

 	CPen bluepen;
	CPen *oldpen;
 	bluepen.CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
	mendc.SelectObject(bluepen);
	oldpen = pDC->SelectObject(&bluepen);

	if(m_Imgthread.pImageProcess->buttom_pts[2].GetSize() > 0)
	{
		isObstacle[2] = true;
	}
	if(m_Imgthread.pImageProcess->buttom_pts[1].GetSize() > 0)
	{
		isObstacle[1] = true;
	}
	if(m_Imgthread.pImageProcess->buttom_pts[0].GetSize() > 0)
	{
		isObstacle[0] = true;
	}

	if(isObstacle[2])
	{
		mendc.MoveTo(55, 160 + offset);
		mendc.LineTo(75, 140 + offset);
		mendc.MoveTo(55, 160 + offset);
		mendc.LineTo(35, 140 + offset);
	}
	if(isObstacle[1])
	{
		mendc.MoveTo(175, 160 + offset);
		mendc.LineTo(155, 140 + offset);
		mendc.MoveTo(175, 160 + offset);
		mendc.LineTo(195, 140 + offset);
	}
	if(isObstacle[0])
	{
		mendc.MoveTo(295, 160 + offset);
		mendc.LineTo(275, 140 + offset);
		mendc.MoveTo(295, 160 + offset);
		mendc.LineTo(315, 140 + offset);
	}
	bluepen.DeleteObject();
//==========================================================

//=======================ball===============================
	int dx = 0, dy = 0, dtheta = 0;
	dx     = cvRound(1 * m_Odemeter_info.tx);
	dy     = cvRound(1 * m_Odemeter_info.ty);
	dtheta = cvRound(1 * m_Odemeter_info.ttheta);

	CPen redpen;
	redpen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	mendc.SelectObject(redpen);

	mendc.SelectObject(redbrush);

	CPoint ballpos;
	if(!isSimulate)
	{
		if(isFirstGet)
		{
			ballpos = CPoint(175, 200 + offset);
			
			topLeft     = CPoint(ballpos.x - 5, ballpos.y - 5);
			bottomRight = CPoint(ballpos.x + 5, ballpos.y + 5);
			rect = CRect(topLeft, bottomRight);
		}
		else
		{
			int ddx = cvRound((Robot_offset.x * cos(Deg2Rad(dtheta))) + (Robot_offset.y * sin(Deg2Rad(dtheta))));
			int ddy = cvRound(-(Robot_offset.x * sin(Deg2Rad(dtheta))) + (Robot_offset.y * cos(Deg2Rad(dtheta))));
// 			topLeft     = CPoint(Robot_pos.x + Robot_offset.x - 5, Robot_pos.y - Robot_offset.y - 5);
// 			bottomRight = CPoint(Robot_pos.x + Robot_offset.x + 5, Robot_pos.y - Robot_offset.y + 5);
			topLeft     = CPoint(Robot_pos.x + ddx - 5, Robot_pos.y - ddy - 5);
			bottomRight = CPoint(Robot_pos.x + ddx + 5, Robot_pos.y - ddy + 5);

			rect = CRect(topLeft, bottomRight);
		}
	}
// 	else
// 	{
// 		if(isFirstGet)
// 		{
// 			ballpos = CPoint(175, 200 + offset);
// 			
// 			topLeft     = CPoint(ballpos.x - 5, ballpos.y - 5);
// 			bottomRight = CPoint(ballpos.x + 5, ballpos.y + 5);
// 			rect = CRect(topLeft, bottomRight);
// 		}
// 		else
// 		{
// 			topLeft     = CPoint(Robot_pos.x + Robot_offset.x - 5, Robot_pos.y - Robot_offset.y - 5);
// 			bottomRight = CPoint(Robot_pos.x + Robot_offset.x + 5, Robot_pos.y - Robot_offset.y + 5);
// 			rect = CRect(topLeft, bottomRight);
// 		}
// 	}

	Ball_pos = CPoint(topLeft.x + 5, topLeft.y + 5);
	mendc.Ellipse(rect);

	redbrush.DeleteObject();
//==========================================================

//=======================robot==============================

	mendc.SelectObject(oldpen);
	CBrush blackbrush;
	blackbrush.CreateSolidBrush(RGB(0, 0, 0));
	mendc.SelectObject(blackbrush);

	int r = 20;

	if(!isSimulate)
	{
		if(isFirstGet)
		{
			Robot_pos	= CPoint(175 - Robot_offset.x, 200 + Robot_offset.y + offset);
			Origin_pos	= Robot_pos;
			Robot_error = CPoint(Robot_pos.x, Robot_pos.y - r);
			
			topLeft     = CPoint(Robot_pos.x - 5, Robot_pos.y - 5);
			bottomRight = CPoint(Robot_pos.x + 5, Robot_pos.y + 5);
			rect		= CRect(topLeft, bottomRight);
		}
		else
		{
			topLeft     = CPoint(Robot_pos.x - 5, Robot_pos.y - 5);
			bottomRight = CPoint(Robot_pos.x + 5, Robot_pos.y + 5);
			rect        = CRect(topLeft, bottomRight);
			
			if(m_Odemeter_info.x == 0 && m_Odemeter_info.y == 0 && m_Odemeter_info.theta == 0)
			{
// 				Origin_pos = Robot_pos;
// 				m_Odemeter_info.tx = 0;
// 				m_Odemeter_info.ty = 0;
// 				m_Odemeter_info.ttheta = 0;
				m_Odemeter_info.flag_clear = false;
			}

			double rx = (dy * cos(Deg2Rad(dtheta))) + (dx * sin(Deg2Rad(dtheta)));
			double ry = -(dy * sin(Deg2Rad(dtheta))) + (dx * cos(Deg2Rad(dtheta)));

			Robot_pos.x = cvRound(Origin_pos.x + rx);
			Robot_pos.y = cvRound(Origin_pos.y - ry);

			Robot_error.x = Robot_pos.x - cvRound(r * cos(Deg2Rad(dtheta+90)));
			Robot_error.y = Robot_pos.y - cvRound(r * sin(Deg2Rad(dtheta+90)));
		}
	}
	else
	{
		if(isFirstGet)
		{
			Robot_pos = CPoint(175 - Robot_offset.x, 215 + Robot_offset.y + offset);
			Origin_pos = Robot_pos;
			Robot_error = CPoint(Robot_pos.x, Robot_pos.y - r);
			
			topLeft     = CPoint(Robot_pos.x - 5, Robot_pos.y - 5);
			bottomRight = CPoint(Robot_pos.x + 5, Robot_pos.y + 5);
			rect = CRect(topLeft, bottomRight);
		}
		else
		{
			topLeft     = CPoint(Robot_pos.x - 5, Robot_pos.y - 5);
			bottomRight = CPoint(Robot_pos.x + 5, Robot_pos.y + 5);
			rect = CRect(topLeft, bottomRight);
			
			if(m_Odemeter_info.flag_clear)
			{
				if(m_Odemeter_info.x == 0 && m_Odemeter_info.y == 0 && m_Odemeter_info.theta == 0)
				{
// 					Origin_pos = Robot_pos;
// 					m_Odemeter_info.tx = 0;
// 					m_Odemeter_info.ty = 0;
// 					m_Odemeter_info.ttheta = 0;
					m_Odemeter_info.flag_clear = false;
				}
			}

			double rx =  (dy * cos(Deg2Rad(dtheta))) + (dx * sin(Deg2Rad(dtheta)));
			double ry = -(dy * sin(Deg2Rad(dtheta))) + (dx * cos(Deg2Rad(dtheta)));

			Robot_pos.x = cvRound(Origin_pos.x + rx);
			Robot_pos.y = cvRound(Origin_pos.y - ry);
			
			Robot_error.x = Robot_pos.x - cvRound(r * cos(Deg2Rad(dtheta+90)));
			Robot_error.y = Robot_pos.y - cvRound(r * sin(Deg2Rad(dtheta+90)));
		}
	}

	mendc.Ellipse(rect);
	mendc.MoveTo(Robot_pos);
	mendc.LineTo(Robot_error);

	CString str;
	str.Format("[%d, %d]", Origin_pos.x, Origin_pos.y);
	mendc.TextOut(Origin_pos.x, Origin_pos.y, str);
	str.Format("[%d, %d]", Robot_pos.x, Robot_pos.y);
	mendc.TextOut(Robot_pos.x, Robot_pos.y, str);

//==========================================================
//=======================target=============================
	mendc.SelectObject(blackbrush);

	if(Target_mode == 1)
	{
		topLeft     = CPoint(95, 115 + offset);
		bottomRight = CPoint(105, 125 + offset);
	}
	else if(Target_mode == 2)
	{
		topLeft     = CPoint(275, 115 + offset);
		bottomRight = CPoint(285, 125 + offset);
	}
	
	rect = CRect(topLeft, bottomRight);
	mendc.Ellipse(rect);
	
	blackbrush.DeleteObject();

//==========================================================

//=======================sensor=============================
	CPen purpalpen;
	purpalpen.CreatePen(PS_SOLID, 2, RGB(255, 0, 255));
	mendc.SelectObject(purpalpen);
	
	CPoint senser_line1, senser_line2;
	senser_line1.x = (Robot_error.x) + (-25)*cos(Deg2Rad(m_Odemeter_info.ttheta));
	senser_line1.y = Robot_error.y - 25*sin(Deg2Rad(m_Odemeter_info.ttheta));
	senser_line2.x = (Robot_error.x) + (25)*cos(Deg2Rad(m_Odemeter_info.ttheta));
	senser_line2.y = Robot_error.y + 25*sin(Deg2Rad(m_Odemeter_info.ttheta));

	Sensorline[0] = senser_line1;
	Sensorline[1] = senser_line2;

	mendc.MoveTo(senser_line1.x, senser_line1.y);
	mendc.LineTo(senser_line2.x, senser_line2.y);

	purpalpen.DeleteObject();

//==================path planing============================
	mendc.SelectObject(oldpen);
	mendc.SelectObject(blackbrush);
	int i;
	if(isPlaningpath && !isFirstGet && !isGetPath)
	{
		if(Target_mode == 1)
			Target_pos = CPoint(100, 120 + offset);
		else
			Target_pos = CPoint(280, 120 + offset);
		CPoint lobst[2], mlobst[2], mrobst[2], robst[2];
		lobst[0] = CPoint(55, 160 + offset);
		lobst[1] = CPoint(75, 140 + offset);
		L_obst_pos[0] = lobst[0];
		L_obst_pos[1] = lobst[1];
		
		mlobst[0] = CPoint(175, 160 + offset);
		mlobst[1] = CPoint(155, 140 + offset);
		ML_obst_pos[0] = mlobst[0];
		ML_obst_pos[1] = mlobst[1];
		
		mrobst[0] = CPoint(175, 160 + offset);
		mrobst[1] = CPoint(195, 140 + offset);
		MR_obst_pos[0] = mrobst[0];
		MR_obst_pos[1] = mrobst[1];
		
		robst[0] = CPoint(295, 160 + offset);
		robst[1] = CPoint(275, 140 + offset);
		R_obst_pos[0] = robst[0];
		R_obst_pos[1] = robst[1];
		
//		PathPlanning(Origin_pos, Target_pos, L_obst_pos, ML_obst_pos, MR_obst_pos, R_obst_pos);
		isGetPath = true;
	}
	if(isGetPath)
	{
		if(Path_pos[0].GetSize() > 0)
		{
			for(i = 1; i < Path_pos[0].GetSize(); i++)
			{
				mendc.MoveTo(Path_pos[0][i-1]);
				mendc.LineTo(Path_pos[0][i]);
			}
		}
	}
	else
		Path_pos[0].RemoveAll();

	pDC->BitBlt(0, offset, 350, 250 + offset, &mendc, 0, 0, SRCCOPY);
	bmp.DeleteObject();
	ReleaseDC(&mendc);
	ReleaseDC(pDC);
}

void CFIRA_PKprocView::PathPlanning(CPoint robot, CPoint target, CPoint l_obst[], CPoint ml_obst[], CPoint mr_obst[], CPoint r_obst[])
{
	int i;

	//robot to obstacle button
	int dx = robot.x - int((l_obst[0].x + ml_obst[0].x)/2);
	int dy = robot.y - l_obst[0].y;
	
	double dis = sqrt(dx*dx + dy*dy);
	double ang = atan2(dy, dx);
	
	int    line_part = int(dis);
	double unit = double(dis / (double)line_part);

	int next_x, next_y;
	for(i = 0; i < line_part; i++)
	{
		next_x = robot.x - int(i*unit*cos(ang));
		next_y = robot.y - int(i*unit*sin(ang));
		Path_pos[0].Add(CPoint(next_x, next_y));
	}

	int startposx = Path_pos[0][Path_pos[0].GetSize()-1].x;
	int startposy = Path_pos[0][Path_pos[0].GetSize()-1].y;
	//obstacle button to obstacle top
	dx = 0;
	dy = l_obst[1].y - l_obst[0].y;

	dis = sqrt(dx*dx + dy*dy);

	line_part = int(dis);
	unit = double(dis / (double)line_part);

	for(i = 0; i < line_part; i++)
	{
		next_x = startposx;
		next_y = startposy - i;
		Path_pos[0].Add(CPoint(next_x, next_y));
	}

	//obstacle top to target

	startposx = Path_pos[0][Path_pos[0].GetSize()-1].x;
	startposy = Path_pos[0][Path_pos[0].GetSize()-1].y;

	dx = Path_pos[0][Path_pos[0].GetSize()-1].x - target.x;
	dy = Path_pos[0][Path_pos[0].GetSize()-1].y - target.y;
	
	dis = sqrt(dx*dx + dy*dy);
	ang = atan2(dy, dx);
	
	line_part = int(dis);
	unit = double(dis / (double)line_part);
	
	for(i = 0; i < line_part; i++)
	{
		next_x = startposx - int(i*unit*cos(ang));
		next_y = startposy - int(i*unit*sin(ang));
		Path_pos[0].Add(CPoint(next_x, next_y));
	}
}

void CFIRA_PKprocView::OnButtonForward() 
{
	// TODO: Add your control notification handler code here
	if(isSimulate)	Simulate_mode = Forward;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 40 + 128;
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

void CFIRA_PKprocView::OnButtonShiftleft() 
{
	// TODO: Add your control notification handler code here
	if(isSimulate)	Simulate_mode = ShiftLeft;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = -50 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

void CFIRA_PKprocView::OnButtonShiftright() 
{
	// TODO: Add your control notification handler code here
	if(isSimulate)	Simulate_mode = ShiftRight;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 50 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

void CFIRA_PKprocView::OnButtonBackward() 
{
	// TODO: Add your control notification handler code here
	if(isSimulate)	Simulate_mode = Backward;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = -30 + 128;
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

void CFIRA_PKprocView::OnButtonRightkick() 
{
	// TODO: Add your control notification handler code here
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 51;
	array[2] = 0;
	array[3] = 0 + 128;
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

void CFIRA_PKprocView::OnButtonLeftkick() 
{
	// TODO: Add your control notification handler code here
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 52;
	array[2] = 0;
	array[3] = 0 + 128;
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

void CFIRA_PKprocView::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	if(isSimulate)	Simulate_mode = STAND;
	isRobotInit = true;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 48;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}	

}

void CFIRA_PKprocView::OnButtonSelfturnright() 
{
	// TODO: Add your control notification handler code here
	if(isSimulate)	Simulate_mode = SurroundBallRight;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 30 + 128;//40
	array[5] = -65 + 128;//-65
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

void CFIRA_PKprocView::OnButtonSelfturnleft() 
{
	// TODO: Add your control notification handler code here
	if(isSimulate)	Simulate_mode = SurroundBallLeft;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = -30 + 128;//-40
	array[5] = 65 + 128;//50
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}

}

void CFIRA_PKprocView::OnButtonLoadparameters() 
{
	// TODO: Add your control notification handler code here
	Fp1 = fopen("C:\\pk_ball.txt", "r");
	Fp2 = fopen("C:\\pk_obstacle.txt", "r");
	Fp3 = fopen("C:\\pk_field.txt", "r");
	
	
	int i;
	for(i = 0; i < 4; i++)
	{
		float temp1, temp2, temp3;
		if(fscanf(Fp1,"%f\n", &temp1) != EOF)
		{
			m_Imgthread.pImageProcess->Curve_Parameters_ball[i] = double(temp1);
		}
		if(fscanf(Fp2,"%f\n", &temp2) != EOF)
		{
			m_Imgthread.pImageProcess->Curve_Parameters_obstacle[i] = double(temp2);
		}
		if(fscanf(Fp3,"%f\n", &temp3) != EOF)
		{
			m_Imgthread.pImageProcess->Curve_Parameters_field[i] = double(temp3);
		}
		else
			break;
	}
	
	fclose(Fp1);
	fclose(Fp2);
	fclose(Fp3);

	m_button_loadparameters.EnableWindow(false);
}

void CFIRA_PKprocView::OnButtonPathplaning() 
{
	// TODO: Add your control notification handler code here

	isPlaningpath = true;
	m_button_pathplaning.EnableWindow(false);
}



void CFIRA_PKprocView::OnButtonHeadtrajectory() 
{
	// TODO: Add your control notification handler code here
	HeadtrajStart = true;
}

bool CFIRA_PKprocView::DribbleAndPassby()
{
	int rx, ry;
	Estimate(m_Imgthread.pImageProcess->Pos_ball, double(pHeadcontrol->Head_hor), double(pHeadcontrol->Head_ver), &rx, &ry);
	
	double dis = Dist(rx, ry);
	Robot_offset = CPoint(rx, ry);
		
	//==========dribbling=======================
	if(m_Imgthread.pImageProcess->Q_ball)
	{
		if(!isRotate)
		{
			if(m_Odemeter_info.ttheta > -60.0)
			{
				if(dis > 35)
				{
					OnButtonForward();
				}
				else if(dis > 20)
				{
					OnButtonSelfturnright();
				}
				else
				{
					OnButtonBackward();
				}
			}
			else
				isRotate = true;	
		}
		else
		{
			if(Robot_pos.y > L_obst_pos[0].y + 10)
			{
				if(fabs(rx) > 5)
				{
					if(rx > 0)
						OnButtonShiftright();
					else
						OnButtonShiftleft();
				}
				else
				{
					OnButtonForward();
				}
			}
			else
			{
				if(Robot_pos.y > Target_pos.y + 10)
				{
					if(abs(m_Odemeter_info.ttheta) > 5)
					{
						OnButtonSelfturnleft();
					}
					else
					{
						OnButtonForward();
					}
				}
				else
				{
					OnButtonStop();
				}
			}
		}
	}
	else
	{
		OnButtonStop();
	}



// 	if(!isFirstGet)
// 	{
// 		PassThrough();
// 	}
// 	

	m_edit_posx = rx;
	m_edit_posy = ry;
	UpdateData(false);
	return false;
}

void CFIRA_PKprocView::ReceiveData()
{
	BYTE buffer[1024] = {0};
	int msg = m_Serial.ReadDataWaiting();

	if(msg >= 6)
	{
		if(msg > 1024)	
			msg = 1024;

		m_Serial.ReadData(buffer, msg);

		for(int i = (msg-1); i >= 0; i--)
		{
			if(buffer[i - 10] == 254 && buffer[i] == 255)
			{
				if(buffer[i - 2] == 1)
				{
					flag_start = true;
				}
				m_Odemeter_info.x     = (int)buffer[i - 9] - 128;
				m_Odemeter_info.y     = (int)buffer[i - 8] - 128;
				m_Odemeter_info.theta = (int)buffer[i - 7] - 128;

				if(m_Odemeter_info.x == 0 && m_Odemeter_info.theta == 0 && m_Odemeter_info.y != 0)
				{
					m_Odemeter_info.tx     += cvRound(0.91*m_Odemeter_info.x);
					m_Odemeter_info.ty     += cvRound(0.83*m_Odemeter_info.y);	
				}
				else
				{
					m_Odemeter_info.tx         += cvRound(0.954459*0.91*m_Odemeter_info.x);
					//odemetry y-axis 
					if(m_Odemeter_info.y > 0)
						m_Odemeter_info.ty     += cvRound(0.83*m_Odemeter_info.y);
					else
						m_Odemeter_info.ty     += cvRound(0.83*m_Odemeter_info.y);
					//odemetry rotate
					if(m_Odemeter_info.theta >= 0)
						m_Odemeter_info.ttheta += cvRound(0.841121*0.405405*m_Odemeter_info.theta);//0.590617
					else
						m_Odemeter_info.ttheta += cvRound(0.915789*0.74359*0.31104*m_Odemeter_info.theta);//0.25702
					
				}
				

				if(buffer[i - 6] != 100)
					m_Odemeter_info.flag_stop = true;
				else
					m_Odemeter_info.flag_stop = false;
			}
		}
	}
}

void CFIRA_PKprocView::OnButtonSimulate() 
{
	// TODO: Add your control notification handler code here
	isSimulate = true;
	OnButtonStop();
	SetTimer(10, 50, NULL);
}

void CFIRA_PKprocView::PassThrough()
{
	if(isSimulate)
		isObstacle[0] = isObstacle[1] = isObstacle[2] = true;

	if(isRobotInit)
	{
		if(isPlaningpath && isGetPath)
		{
			for(int i = 0; i < Path_pos[0].GetSize(); i++)
			{
				if(Robot_pos.y > Target_pos.y+20)
				{
					if(Robot_pos.y == Path_pos[0][i].y)
					{
						if(abs(Robot_pos.x - Path_pos[0][i].x) > 6)
						{
							if(Robot_pos.x < Path_pos[0][i].x)
							{
								OnButtonShiftright();
								break;
							}
							else
							{
								OnButtonShiftleft();
								break;
							}
							
						}//end if(abs(Robot_pos.x - Path_pos[0][i].x) > 6)
						else if(abs(Robot_pos.x - Path_pos[0][i].x) > 2)
						{
							if(Robot_pos.x < Path_pos[0][i].x)
							{
								OnButtonOmniRight();
								break;
							}
							else
							{
								OnButtonOmniLeft();
								break;
							}
						}//end else if(abs(Robot_pos.x - Path_pos[0][i].x) > 2)
						else
						{
							OnButtonForward();
							break;
						}
						
					}//end if(Robot_pos.y == Path_pos[0][i].y)

				}//end if(Robot_pos.y != Path_pos[0][Path_pos[0].GetSize()-1].y)
				else
				{
					OnButtonStop();
					m_Odemeter_info.flag_clear = true;
					break;
				}
			}//end for(int i = 0; i < Path_pos[0].GetSize(); i++)

		}//end if(isPlaningpath && isGetPath)

	}//end if(isRobotInit)
	
}

void CFIRA_PKprocView::OnButtonOmniLeft() 
{
	// TODO: Add your control notification handler code here
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 10 + 128;
	array[4] = -40 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

void CFIRA_PKprocView::OnButtonOmniRight() 
{
	// TODO: Add your control notification handler code here
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 10 + 128;
	array[4] = 40 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

void CFIRA_PKprocView::OnModeTest1() 
{
	// TODO: Add your control notification handler code here
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 0 + 128;
	array[5] = 60 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

void CFIRA_PKprocView::OnButtonModeTest2() 
{
	// TODO: Add your control notification handler code here
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 0 + 128;
	array[5] = -60 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

bool CFIRA_PKprocView::lDribbling()
{
	if(isSimulate)
	{
		OnButtonPathplaning();
		isObstacle[0] = isObstacle[1] = isObstacle[2] = true;
		Gamestate = 2;
		pHeadcontrol->Scan_Finish = true;
		pHeadcontrol->Catch_start = true;
	}
	
	int bx = m_Imgthread.pImageProcess->Pos_ball_center.x;
	int by = m_Imgthread.pImageProcess->Pos_ball_center.y;

	int rx, ry;
	Estimate(m_Imgthread.pImageProcess->Pos_ball, double(pHeadcontrol->Head_hor), double(pHeadcontrol->Head_ver), &rx, &ry);
	
	double dis = Dist(rx, ry);
	Robot_offset = CPoint(rx, ry);

	static bool forwardlock = false;
	static bool backlock = false;
	static bool shiftlock = false;
	static bool latchflag = false;
	//================test dribbling================
	if(Test_mode == 1)
	{
		if(m_Imgthread.pImageProcess->Q_ball)
		{
			if(Robot_pos.y > Target_pos.y+10)
			{
				if(abs(m_Odemeter_info.ttheta) < 40)
				{
					OnButtonSelfturnright();
				}
				else
				{
					if(!forwardlock)
					{
						if(!backlock)
						{
							if(dis < 55)
							{
								OnButtonBackward();
							}
							else
								backlock  = true;
						}
						else
						{
							if(!shiftlock)
							{
								if(bx < TopLeft.x)
								{
									OnButtonShiftleft();
								}
								else if(bx > DownRight.x)
								{
									OnButtonShiftright();
								}
								else
								{
									shiftlock = true;
								}
							}
							else
							{
								OnDribblingBall();
								forwardlock = true;
							}
							
						}
					}
					else
					{
						static bool dribblelock = false;
						
						if(Robot_pos.x < ML_obst_pos[1].x)
						//if(fabs(dis) < 30)
							dribblelock = true;
						if(!dribblelock && !latchflag)
							OnDribblingBall();
						else
						{
							latchflag = true;
							if(pHeadcontrol->TimeDelay(2000))
							{
								Test_mode = 2;
								pHeadcontrol->Head_Init = true;
 							}	
						}
					}
				}
			}
			else
			{
				OnButtonStop();
			}
			
		}
		else
		{
			OnButtonStop();
		}
	}
	else if(Test_mode == 2)
	{
		if(m_Odemeter_info.ttheta < -5)
		{
			OnButtonSelfturnleft();
		}
		else
		{
			OnButtonStop();
			return true;
		}
			
	}
	m_edit_posx = rx;
	m_edit_posy = ry;
	m_edit_distance = cvRound(dis);
	UpdateData(false);
	return false;
}

void CFIRA_PKprocView::OnDribblingBall()
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 62 + 128;
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

bool CFIRA_PKprocView::lObstacleAvoidance()
{
	static short obstacle_cond = 1;
	//if robot not arrive target point
	if(Robot_pos.y > Target_pos.y-5)
	{
		//if ball is not behind the leftest obstacle
		if(m_Imgthread.pImageProcess->Q_Obstacle)
		{
			int view_center = int(CAM_WIDTH / 2.0);
			int yd = CAM_HEIGHT;
			static bool lcnt_check = false;
			static bool rcnt_check = false;
			
			if((m_Imgthread.pImageProcess->Obstacle_center[0].x > view_center + 30) && (m_Imgthread.pImageProcess->Obstacle_center[0].x < view_center + 70))
			{	
				if(m_Imgthread.pImageProcess->Obstacle_center[0].y > yd - 90)
				{
					OnButtonShiftleft();
					obstacle_cond = 10;
				}
				else
				{
					OnButtonOmniLeft();
					obstacle_cond = 20;
				}
				lcnt_check = true;
				
				//	start_time = PTIME;

			}
// 				else
// 				{
// 					//	end_time = PTIME;
// 					// if(end_time - start_time > 2000)
// 					//	lcnt_check = falsae;
// 					lcnt_check = false;
// 					
// 				}
			if((m_Imgthread.pImageProcess->Obstacle_center[1].x < view_center - 30) && (m_Imgthread.pImageProcess->Obstacle_center[1].x > view_center - 70))
			{
				if(m_Imgthread.pImageProcess->Obstacle_center[1].y > yd - 90)
				{
					OnButtonShiftright();
					obstacle_cond = 30;
				}
				else
				{
					OnButtonOmniRight();
					obstacle_cond = 40;
				}
				rcnt_check = true;
			}
// 				else
// 					rcnt_check = false;

			if(!lcnt_check && !rcnt_check)
			{
				OnButtonForward();
				obstacle_cond = 50;
			}
			if(pHeadcontrol->TimeDelay(1000))
			{
				lcnt_check = false;
				rcnt_check = false;
				pHeadcontrol->Head_Init = true;
			}
		}
		else
		{
			OnButtonForward();
			obstacle_cond = 50;
		}
	}
	//if robot arrive target point
	else
	{
		OnButtonStop();
		return true;
	}
	m_edit_obstaclecondition = obstacle_cond;
	m_edit_qobstacle = m_Imgthread.pImageProcess->Q_Obstacle;
	m_edit_robstaclex = m_Imgthread.pImageProcess->Obstacle_center[0].x;
	m_edit_robstacley = m_Imgthread.pImageProcess->Obstacle_center[0].y;
	m_edit_lobstaclex = m_Imgthread.pImageProcess->Obstacle_center[1].x;
	m_edit_lobstacley = m_Imgthread.pImageProcess->Obstacle_center[1].y;
	UpdateData(false);
	return false;
}

bool CFIRA_PKprocView::lCloseBall()
{
	pHeadcontrol->Ver_state = 2;
	pHeadcontrol->V_Scanball();
	if(m_Imgthread.pImageProcess->Q_ball)
	{
		int rx, ry;
		Estimate(m_Imgthread.pImageProcess->Pos_ball, double(pHeadcontrol->Head_hor), double(pHeadcontrol->Head_ver), &rx, &ry);
		
		double dis = Dist(rx, ry);
		Robot_offset = CPoint(rx, ry);

		//==========================================
		static bool shiftlock = false;
		static bool forwardlock = false;
		static bool backwardlock = false;
		static bool speciallock = false;
		static bool condtion_jump = false;
		//===========================================

		if(dis > 60)
		{
			OnButtonForward();
		}
		else if(dis > 0)
		{
			if(abs(Ball_pos.y - ML_obst_pos[1].y) < 5 && abs(Ball_pos.x - ML_obst_pos[1].x) < 5)
			{
				speciallock = true;
			}

			if(speciallock)
			{
				if(abs(Ball_pos.y - ML_obst_pos[1].y) < 15)
					OnButtonOmniRight();
				else
				{
					if(abs(rx) > 5 && !condtion_jump)
					{
						if(rx > 0)
						{
							OnButtonShiftright();
						}
						else
						{
							OnButtonShiftleft();
						}
					}
					else
					{
						OnDribblingBall();
						condtion_jump = true;
						if(abs(rx) > 20)
						{
							condtion_jump = false;
						}
					}
				}
			}
			else
			{
				int lcenter = int((L_obst_pos[0].x + ML_obst_pos[0].x) / 2.0);
				
				if(abs(rx) > 5 && !condtion_jump)
				{
					if(rx > 0)
					{
						OnButtonShiftright();
					}
					else
					{
						OnButtonShiftleft();
					}
				}
				else
				{
					if(Ball_pos.x < lcenter)
					//if(pHeadcontrol->Head_hor > 2048)
					{
						if(!shiftlock)
						{
							if(rx < 0)
							{
								OnButtonShiftleft();
							}
							else
							{
								shiftlock = true;
							}
						}
						else
						{
							if(m_Odemeter_info.ttheta < 30)
								OnButtonSelfturnleft();
							else
								OnDribblingBall();
						}
					}
					else
					{
						OnDribblingBall();
					}
					condtion_jump = true;
					if(abs(rx) > 20)
						condtion_jump = false;
				}
			}
	
		}
		else
		{
			OnButtonBackward();
		}

		m_edit_posx = rx;
		m_edit_posy = ry;
		m_edit_distance = cvRound(dis);
		UpdateData(false);
	}
	else
	{
		OnButtonBackward();
	}
	return false;
}

void CFIRA_PKprocView::OnStandWalk()
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 20 + 128;
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

bool CFIRA_PKprocView::rDribbling()
{
	if(isSimulate)
	{
		OnButtonPathplaning();
		isObstacle[0] = isObstacle[1] = isObstacle[2] = true;
		Gamestate = 2;
		pHeadcontrol->Scan_Finish = true;
		pHeadcontrol->Catch_start = true;
	}
	
	int bx = m_Imgthread.pImageProcess->Pos_ball_center.x;
	int by = m_Imgthread.pImageProcess->Pos_ball_center.y;

	int rx, ry;
	Estimate(m_Imgthread.pImageProcess->Pos_ball, double(pHeadcontrol->Head_hor), double(pHeadcontrol->Head_ver), &rx, &ry);
	
	double dis = Dist(rx, ry);
	Robot_offset = CPoint(rx, ry);

	static bool forwardlock = false;
	static bool backlock = false;
	static bool shiftlock = false;
	static bool latchflag = false;
	//================test dribbling================
	if(Test_mode == 1)
	{
		if(m_Imgthread.pImageProcess->Q_ball)
		{
			if(Robot_pos.y > Target_pos.y+10)
			{
				if(abs(m_Odemeter_info.ttheta) < 45)
				{
					OnSelfturnleft();
				}
				else
				{
					if(!forwardlock)
					{
						if(!backlock)
						{
							if(dis < 55)
							{
								OnButtonBackward();
							}
							else
								backlock  = true;
						}
						else
						{
							if(!shiftlock)
							{
								if(bx < TopLeft.x)
								{
									OnButtonShiftleft();
								}
								else if(bx > DownRight.x)
								{
									OnButtonShiftright();
								}
								else
								{
									shiftlock = true;
								}
							}
							else
							{
								OnDribblingBall();
								forwardlock = true;
							}
							
						}
					}
					else
					{
						static bool dribblelock = false;
						
						if(Robot_pos.x > MR_obst_pos[1].x-10)
							dribblelock = true;
						if(!dribblelock && !latchflag)
							OnDribblingBall();
						else
						{
							latchflag = true;
							if(pHeadcontrol->TimeDelay(2000))
							{
								Test_mode = 2;
								pHeadcontrol->Head_Init = true;
 							}	
						}
					}
				}
			}
			else
			{
				OnButtonStop();
			}
			
		}
		else
		{
			OnButtonStop();
		}
	}
	else if(Test_mode == 2)
	{
		if(m_Odemeter_info.ttheta > 17)
		{
			OnButtonSelfturnright();
		}
		else
		{
			OnButtonStop();
			return true;
		}
			
	}
	m_edit_posx = rx;
	m_edit_posy = ry;
	m_edit_distance = cvRound(dis);
	UpdateData(false);
	return false;
}

bool CFIRA_PKprocView::lCheckBallPos()
{
	pHeadcontrol->Ver_state = 1;
	pHeadcontrol->V_Scanball();
	
	static int rx, ry;
	static double dis;
	static bool delaylock = false;
	
	if(m_Imgthread.pImageProcess->Q_ball)
	{
		Estimate(m_Imgthread.pImageProcess->Pos_ball, double(pHeadcontrol->Head_hor), double(pHeadcontrol->Head_ver), &rx, &ry);
		
		dis = Dist(rx, ry);
		Robot_offset = CPoint(rx, ry);
		
		if(!delaylock)
		{
			OnButtonStop();
			if(pHeadcontrol->TimeDelay(3000))
			{
				pHeadcontrol->Head_Init = true;
				delaylock = true;
			}
		}
		else
		{
			//===============================
			static int shiftlock = false;
			static int backwardlock = false;
			
			if(!backwardlock)
			{
				if((Ball_pos.y > Robot_pos.y) || abs(Robot_pos.y - Ball_pos.y) < 20)
					OnButtonBackward();
				else
					backwardlock = true;
			}
			else
			{
				if(!shiftlock)
				{
					int obs_center = ML_obst_pos[1].y;
					if(Ball_pos.y > obs_center)
					{
						if(abs(rx) > 10)
						{
							if(rx > 0)
								OnButtonShiftright();
							else 
								OnButtonShiftleft();
						}
						else 
							shiftlock = true;
					}
					else
						shiftlock = true;
					
				}
			}
			
			if(backwardlock && shiftlock)
			{
				return true;
			}
			
			m_edit_posx = rx;
			m_edit_posy = ry;
			m_edit_distance = cvRound(dis);
			UpdateData(false);
		}
	}	
	return false;
}

bool CFIRA_PKprocView::rCheckBallPos()
{	
	pHeadcontrol->Ver_state = 1;
	pHeadcontrol->V_Scanball();
	
	static int rx, ry;
	static double dis;
	static bool delaylock = false;
	
	if(m_Imgthread.pImageProcess->Q_ball)
	{
		Estimate(m_Imgthread.pImageProcess->Pos_ball, double(pHeadcontrol->Head_hor), double(pHeadcontrol->Head_ver), &rx, &ry);
// 		rx = m_edit_posx;
// 		ry = m_edit_posy;
		dis = Dist(rx, ry);
		Robot_offset = CPoint(rx, ry);
		
		if(!delaylock)
		{
			OnButtonStop();
			if(pHeadcontrol->TimeDelay(3000))
			{
				pHeadcontrol->Head_Init = true;
				delaylock = true;
			}
		}
		else
		{
			//===============================
			static bool shiftlock = false;
			static bool backwardlock = false;
			static bool lockcheck = false;
			
			if(!backwardlock)
			{
				//if((Ball_pos.y > Robot_pos.y) || abs(Robot_pos.y - Ball_pos.y) < 20)
				if(ry < 20)
				{
					OnButtonBackward();
					lockcheck = true;
				}
				else
				{
					if(ry > 30)
						backwardlock = true;
					else
						OnButtonBackward();
				}
			}
			else
			{
				if(!shiftlock)
				{
					int obs_center = MR_obst_pos[1].y;
					if(lockcheck)
					//if(Ball_pos.y > obs_center)
					{
						if(abs(rx) > 10)
						{
							if(rx > 0)
								OnButtonShiftright();
							else 
								OnButtonShiftleft();
						}
						else 
							shiftlock = true;
					}
					else
						shiftlock = true;
					
				}
			}
			
			if(backwardlock && shiftlock)
			{
				return true;
			}
			
			m_edit_posx = rx;
			m_edit_posy = ry;
			m_edit_distance = cvRound(dis);
			UpdateData(false);
		}
	}
	else
	{
		OnButtonBackward();
	}
	return false;
}

bool CFIRA_PKprocView::rObstacleAvoidance()
{
	static short obstacle_cond = 1;
	static bool forword_flag = false;
	//if robot not arrive target point
	//if(Robot_pos.y > Target_pos.y + 5)
	{
		//if ball is not behind the leftest obstacle
		if(m_Imgthread.pImageProcess->Q_Obstacle)
		{
			int view_center = int(CAM_WIDTH / 2.0);
			int yd = CAM_HEIGHT;
			static bool lcnt_check = false;
			static bool rcnt_check = false;
			
			if((m_Imgthread.pImageProcess->Obstacle_center[0].x > view_center + 30) && (m_Imgthread.pImageProcess->Obstacle_center[0].x < view_center + 70))
			{	
				if(m_Imgthread.pImageProcess->Obstacle_center[0].y > yd - 90)
				{
					OnButtonShiftleft();
					obstacle_cond = 10;
				}
				else
				{
					OnButtonOmniLeft();
					obstacle_cond = 20;
				}
				lcnt_check = true;
				
				//	start_time = PTIME;
				
			}
// 			else
// 			{
// 				//	end_time = PTIME;
// 				// if(end_time - start_time > 2000)
// 				//	lcnt_check = falsae;
// 				lcnt_check = false;
// 				
// 			}
			if((m_Imgthread.pImageProcess->Obstacle_center[1].x < view_center - 30) && (m_Imgthread.pImageProcess->Obstacle_center[1].x > view_center - 70))
			{
				if(m_Imgthread.pImageProcess->Obstacle_center[1].y > yd - 90)
				{
					OnButtonShiftright();
					obstacle_cond = 30;
				}
				else
				{
					OnButtonOmniRight();
					obstacle_cond = 40;
				}
				rcnt_check = true;
			}
// 			else
// 				rcnt_check = false;
			
			if(!lcnt_check && !rcnt_check)
			{
				OnButtonForward();
				obstacle_cond = 50;
			}
		 	if(pHeadcontrol->TimeDelay(1000))
		 	{
			 	lcnt_check = false;
			 	rcnt_check = false;
			 	pHeadcontrol->Head_Init = true;
		 	}
		}
		else
		{	
			if(!forword_flag)
			{
				OnButtonForward();
				obstacle_cond = 50;
			}
			else
			{
				OnButtonStop();
				return true;
			}
			if(pHeadcontrol->TimeDelay(1000))
			{
				forword_flag = true;
				pHeadcontrol->Head_Init = true;
			}

		}
	}
	//if robot arrive target point
// 	else
// 	{
// 		OnButtonStop();
// 		return true;
// 	}
	m_edit_obstaclecondition = obstacle_cond;
	m_edit_qobstacle = m_Imgthread.pImageProcess->Q_Obstacle;
	m_edit_robstaclex = m_Imgthread.pImageProcess->Obstacle_center[0].x;
	m_edit_robstacley = m_Imgthread.pImageProcess->Obstacle_center[0].y;
	m_edit_lobstaclex = m_Imgthread.pImageProcess->Obstacle_center[1].x;
	m_edit_lobstacley = m_Imgthread.pImageProcess->Obstacle_center[1].y;
	UpdateData(false);
	return false;
}

void CFIRA_PKprocView::rCloseBall()
{
	static bool flag_save = false;
	static int head_holdpos = 0;

	pHeadcontrol->Ver_state = 2;
	pHeadcontrol->V_Scanball();
	if(m_Imgthread.pImageProcess->Q_ball)
	{
		int rx, ry;
		Estimate(m_Imgthread.pImageProcess->Pos_ball, double(pHeadcontrol->Head_hor), double(pHeadcontrol->Head_ver), &rx, &ry);
		
		if(!flag_save)
		{
			if(pHeadcontrol->TimeDelay(1000))
			{
				head_holdpos = pHeadcontrol->Head_hor;
				pHeadcontrol->Head_Init = true;
				flag_save = true;
			}
		}
		
		double dis = Dist(rx, ry);
		Robot_offset = CPoint(rx, ry);

		//==========================================
		static bool shiftlock = false;
		static bool forwardlock = false;
		static bool backwardlock = false;
		static bool speciallock = false;
		static bool condition_jump = false;
		//===========================================

		if(fabs(ry) > 55)
		{
			OnButtonForward();
		}
		else if(ry > 15)
		{
			/*if(abs(Ball_pos.y - R_obst_pos[1].y) < 5 && abs(Ball_pos.x - R_obst_pos[1].x) < 5)
			{
				speciallock = true;
			}

			if(speciallock)
			{
				if(abs(Ball_pos.y - R_obst_pos[1].y) < 15)
					OnButtonOmniRight();
				else
				{
					if(abs(rx) > 5 && !condition_jump)
					{
						if(rx > 0)
						{
							OnButtonShiftright();
						}
						else
						{
							OnButtonShiftleft();
						}
					}
					else
					{
						OnDribblingBall();
						condition_jump = true;
						if(abs(rx) > 20)
						{
							condition_jump = false;
						}
					}
				}
			}
			else*/
			{
				int rcenter = int((R_obst_pos[1].x + MR_obst_pos[1].x) / 2.0);
				//if(head_holdpos < 2048)
				//if(Ball_pos.x > rcenter)
				if(Wrongside == 1)
				{
					if(!shiftlock)
					{
						if(rx > 0)
						{
							OnButtonShiftright();
						}
						else
						{
							shiftlock = true;
						}
					}
					else
					{
						if(m_Odemeter_info.ttheta > -30)
							OnButtonSelfturnright();
						else
							OnKickingBall();
					}
					
				}
				else
				{
					if(abs(rx) > 5 && !condition_jump)
					{
						if(rx > 0)
						{
							OnButtonShiftright();
						}
						else
						{
							OnButtonShiftleft();
						}
					}
					else
					{
						OnKickingBall();
						condition_jump = true;
						if(abs(rx) > 20)
						{
							condition_jump = false;
						}
					}
				}	
			}
	
		}
		else
		{
			OnButtonBackward();
		}

		m_edit_posx = rx;
		m_edit_posy = ry;
		m_edit_distance = cvRound(dis);
		UpdateData(false);
	}
	else
	{
		OnButtonBackward();
	}
}

void CFIRA_PKprocView::OnButtonCalibrateView() 
{
	// TODO: Add your control notification handler code here
	static int cnt = 0;
	pHeadcontrol->V_Lock();

	cnt ++;
	if(cnt == 1)
	{
		SetTimer(3, 50, NULL);
	}
	else
	{
		flag_endcalibrate = true;
	}
}

void CFIRA_PKprocView::OnSelfturnleft()
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = -40 + 128;//-40
	array[5] = 80 + 128;//50
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

void CFIRA_PKprocView::OnSelfturnrihgt()
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 30 + 128;//-40
	array[5] = -75 + 128;//50
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}

void CFIRA_PKprocView::OnButtonGoalleft() 
{
	// TODO: Add your control notification handler code here
	TopLeft   = cvPoint(195, 175);
	DownRight = cvPoint(215, 195);
	Target_mode = 1;
}

void CFIRA_PKprocView::OnButtonGoalright() 
{
	// TODO: Add your control notification handler code here
	TopLeft   = cvPoint(115, 175);
	DownRight = cvPoint(135, 195);
	Target_mode = 2;
}

void CFIRA_PKprocView::OnButtonWrongside() 
{
	// TODO: Add your control notification handler code here
	Wrongside = 1;
}

void CFIRA_PKprocView::OnKickingBall()
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 80 + 128;
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		m_Serial.WriteCommByte(array[i]);
	}
}
