// HuroEvolutionSystemView.cpp : implementation of the CHuroEvolutionSystemView class
//

#include "stdafx.h"
#include "HuroEvolutionSystem.h"

#include "HuroEvolutionSystemDoc.h"
#include "HuroEvolutionSystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHuroEvolutionSystemView

IMPLEMENT_DYNCREATE(CHuroEvolutionSystemView, CFormView)

BEGIN_MESSAGE_MAP(CHuroEvolutionSystemView, CFormView)
	//{{AFX_MSG_MAP(CHuroEvolutionSystemView)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonBodyStop)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonBodyForward)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonBodyBackward)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonBodyShiftRight)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonBodyShiftLeft)
	ON_BN_CLICKED(IDC_BUTTON6, OnButtonBodyTurnLeft)
	ON_BN_CLICKED(IDC_BUTTON7, OnButtonBodyTurnRight)
	ON_BN_CLICKED(IDC_BUTTON8, OnButtonSaveImage)
	ON_BN_CLICKED(IDC_BUTTON11, OnButtonEmergencyStop)
	ON_BN_CLICKED(IDC_BUTTON9, OnButtonHeadStart)
	ON_BN_CLICKED(IDC_BUTTON10, OnButtonBodyStart)
	ON_BN_CLICKED(IDC_BUTTON12, OnButtonLoadParameter)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON13, OnButtonManulControl)
	ON_BN_CLICKED(IDC_BUTTON14, OnButtonViewleft)
	ON_BN_CLICKED(IDC_BUTTON15, OnButtonViewright)
	ON_BN_CLICKED(IDC_BUTTON16, OnButtonViewDown)
	ON_BN_CLICKED(IDC_BUTTON17, OnButtonStepIt)
	ON_BN_CLICKED(IDC_KICKLEFT, OnKickleft)
	ON_BN_CLICKED(IDC_KICKRIGHT, OnKickright)
	ON_BN_CLICKED(IDC_FALLLEFT, OnFallleft)
	ON_BN_CLICKED(IDC_FALLRIGHT, OnFallright)
	ON_BN_CLICKED(IDC_KICKLEFT2, OnKickleftStraight)
	ON_BN_CLICKED(IDC_KICKRIGHT2, OnKickrightStraight)
	ON_BN_CLICKED(IDC_BUTTON18, OnButtonGAMEINIT)
	ON_BN_CLICKED(IDC_BUTTON19, OnButtonGAMEREADY)
	ON_BN_CLICKED(IDC_BUTTON20, OnButtonGAMESET)
	ON_BN_CLICKED(IDC_BUTTON21, OnButtonGAMEPLAY)
	ON_BN_CLICKED(IDC_BUTTON26, OnButtonCompassINIT)
	ON_BN_CLICKED(IDC_BUTTON27, OnButtonCompassCalibrate)
	ON_BN_CLICKED(IDC_BUTTON22, OnButtonCompassSetNorth)
	ON_BN_CLICKED(IDC_BUTTON23, OnButtonCompassSetWest)
	ON_BN_CLICKED(IDC_BUTTON24, OnButtonCompassSetSouth)
	ON_BN_CLICKED(IDC_BUTTON25, OnButtonCompassSetEast)
	ON_BN_CLICKED(IDC_BUTTON28, OnButtonBodySurrundRight)
	ON_BN_CLICKED(IDC_BUTTON29, OnButtonBodySurrundLeft)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHuroEvolutionSystemView construction/destruction

CHuroEvolutionSystemView::CHuroEvolutionSystemView()
	: CFormView(CHuroEvolutionSystemView::IDD)
{
	//{{AFX_DATA_INIT(CHuroEvolutionSystemView)
	m_edit_Pix_ballx = 0.0;
	m_edit_Pix_bally = 0.0;
	m_edit_Real_ballx = 0.0;
	m_edit_Real_bally = 0.0;
	m_edit_Head_horpos = 0;
	m_edit_Head_verpos = 0;
	m_edit_processtime = 0;
	m_edit_isGetball = 0;
	m_edit_GrabTime = 0;
	m_edit_ballmode = 0;
	m_edit_odometery_y = 0;
	m_edit_odometery_x = 0;
	m_edit_compass = 0.0;
	m_edit_closeball = 0;
	m_edit_KickMode = 0;
	m_edit_GameState = _T("");
	m_edit_Behavior = _T("");
	m_edit_isGoalSide = 0;
	m_edit_CompassInit = 0;
	m_edit_CompassNorth = 0;
	m_edit_CompassWest = 0;
	m_edit_CompassSouth = 0;
	m_edit_CompassEast = 0;
	m_edit_Gamebegining = 0;
	m_edit_DecisionRand = 0;
	m_edit_odemtheta = 0;
	m_edit_robotface = 0;
	//}}AFX_DATA_INIT
	// TODO: add construction code here

	m_Thread_img.pImageProcess = new CImageProcess();
	pHeadControl = new CHeadControl(COM_PORT, BUNDRATE);
	pRobot = new CRobotBehavior(B_COM_PORT, B_BUNDRATE);
	pMySocket = new CMySocket();

	isHeadStart = 0;
	isBodyStart = 0;
	isCatchImg = 0;
	isImgDone = 0;
	isManulControl = 1;
	isRobotFront = 0;
	isRobotKick = 0;

	isGameBegining = 1;
	isGoalSide = 0;
	isCountOde = 0;

	RobotBehavior = 0;
	GameState = 0;
	Kick_Mode = 0;


	CompassInit = 0;
	CompassNorth = 0;
	CompassWest = 0;
	CompassSouth = 0;
	CompassEast = 0;
	RobotDirection = 0;
	isCloseFinish = 0;
	isKickFinish = 0;

	Flag_closeball = 0;
	Robot1 = cvPoint(510-80, 325);
	Robot2 = cvPoint(510-300, 325);

	Goal_Point = cvPoint(960, 325);
}

CHuroEvolutionSystemView::~CHuroEvolutionSystemView()
{
	m_Thread_img.isTerminate = 1;
	delete m_Thread_img.pImageProcess;
	delete pHeadControl;
	delete pRobot;
	delete pMySocket;

	OnButtonEmergencyStop();
}

void CHuroEvolutionSystemView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHuroEvolutionSystemView)
	DDX_Control(pDX, IDC_BUTTON10, m_button_bodystart);
	DDX_Control(pDX, IDC_BUTTON13, m_button_manulcontrol);
	DDX_Control(pDX, IDC_BUTTON12, m_button_LoadParameter);
	DDX_Text(pDX, IDC_EDIT1, m_edit_Pix_ballx);
	DDX_Text(pDX, IDC_EDIT2, m_edit_Pix_bally);
	DDX_Text(pDX, IDC_EDIT3, m_edit_Real_ballx);
	DDX_Text(pDX, IDC_EDIT4, m_edit_Real_bally);
	DDX_Text(pDX, IDC_EDIT5, m_edit_Head_horpos);
	DDX_Text(pDX, IDC_EDIT6, m_edit_Head_verpos);
	DDX_Text(pDX, IDC_EDIT7, m_edit_processtime);
	DDX_Text(pDX, IDC_EDIT8, m_edit_isGetball);
	DDX_Text(pDX, IDC_EDIT10, m_edit_GrabTime);
	DDX_Text(pDX, IDC_EDIT9, m_edit_ballmode);
	DDX_Text(pDX, IDC_EDIT12, m_edit_odometery_y);
	DDX_Text(pDX, IDC_EDIT11, m_edit_odometery_x);
	DDX_Text(pDX, IDC_EDIT13, m_edit_compass);
	DDX_Text(pDX, IDC_EDIT14, m_edit_closeball);
	DDX_Text(pDX, IDC_EDIT15, m_edit_KickMode);
	DDX_Text(pDX, IDC_EDIT16, m_edit_GameState);
	DDX_Text(pDX, IDC_EDIT21, m_edit_Behavior);
	DDX_Text(pDX, IDC_EDIT22, m_edit_isGoalSide);
	DDX_Text(pDX, IDC_EDIT23, m_edit_CompassInit);
	DDX_Text(pDX, IDC_EDIT17, m_edit_CompassNorth);
	DDX_Text(pDX, IDC_EDIT18, m_edit_CompassWest);
	DDX_Text(pDX, IDC_EDIT19, m_edit_CompassSouth);
	DDX_Text(pDX, IDC_EDIT20, m_edit_CompassEast);
	DDX_Text(pDX, IDC_EDIT24, m_edit_Gamebegining);
	DDX_Text(pDX, IDC_EDIT25, m_edit_DecisionRand);
	DDX_Text(pDX, IDC_EDIT26, m_edit_odemtheta);
	DDX_Text(pDX, IDC_EDIT27, m_edit_robotface);
	//}}AFX_DATA_MAP
}

BOOL CHuroEvolutionSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CHuroEvolutionSystemView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//Create thread
	m_Thread_img.isTerminate = false;
	m_Thread_img.wnd = this;
	m_Thread_img.hThread = CreateEvent( 0, 1, 0, "MyEvent");
	AfxBeginThread(DoThread,(LPVOID) this);

	cvNamedWindow("Map", CV_WINDOW_AUTOSIZE);

	pMySocket->Create(pMySocket->BroadcastPort, SOCK_DGRAM);
	pMySocket->Bind(pMySocket->LocalPort);

	SetTimer(1, 10, NULL);
	OnButtonLoadParameter();
}

/////////////////////////////////////////////////////////////////////////////
// CHuroEvolutionSystemView printing

BOOL CHuroEvolutionSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CHuroEvolutionSystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CHuroEvolutionSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CHuroEvolutionSystemView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CHuroEvolutionSystemView diagnostics

#ifdef _DEBUG
void CHuroEvolutionSystemView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHuroEvolutionSystemView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CHuroEvolutionSystemDoc* CHuroEvolutionSystemView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHuroEvolutionSystemDoc)));
	return (CHuroEvolutionSystemDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHuroEvolutionSystemView message handlers

void CHuroEvolutionSystemView::OnMovedOmni(int x, int y, int theta)
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = x + 128;
	array[4] = y + 128;
	array[5] = theta + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnButtonBodyStop() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 0;
	if(isManulControl)	Ctrl_Mode = Stop;
	RobotBehavior = Stop;
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
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnButtonBodyForward() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 1;
	if(isManulControl)	Ctrl_Mode = Forward;
	RobotBehavior = Forward;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 70 + 128;	//-5
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnButtonBodyBackward() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 1;
	if(isManulControl)	Ctrl_Mode = Backward;
	RobotBehavior = Backward;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = -50 + 128;
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnButtonBodyShiftRight() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 1;
	if(isManulControl)	Ctrl_Mode = ShiftRight;
	RobotBehavior = ShiftRight;
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
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnButtonBodyShiftLeft() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 1;
	if(isManulControl)	Ctrl_Mode = ShiftLeft;
	RobotBehavior = ShiftLeft;
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
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnButtonBodyTurnLeft() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 1;
	if(isManulControl)	Ctrl_Mode = TurnLeft;
	RobotBehavior = TurnLeft;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 0 + 128;
	array[5] = -50 + 128; 
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnButtonBodyTurnRight() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 1;
	if(isManulControl)	Ctrl_Mode = TurnRight;
	RobotBehavior = TurnRight;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 0 + 128;
	array[5] = 50 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnButtonSaveImage() 
{
	// TODO: Add your control notification handler code here
	m_Thread_img.pImageProcess->SaveImage();
}

void CHuroEvolutionSystemView::OnButtonEmergencyStop() 
{
	// TODO: Add your control notification handler code here
	m_Thread_img.isTerminate = 1;
	KillTimer(1);
	OnButtonBodyStop();
}

void CHuroEvolutionSystemView::OnButtonHeadStart() 
{
	// TODO: Add your control notification handler code here
	isHeadStart = 1;
}

void CHuroEvolutionSystemView::OnButtonBodyStart() 
{
	// TODO: Add your control notification handler code here
	isBodyStart = 1;
	isManulControl = 0;

	m_button_bodystart.EnableWindow(0);
	m_button_manulcontrol.EnableWindow(1);
}

void CHuroEvolutionSystemView::OnButtonLoadParameter() 
{
	// TODO: Add your control notification handler code here
	FilePointer = fopen("robocup_field.txt", "r");
	
	if(FilePointer)
	{
		int i;
		for(i = 0; i < 4; i++)
		{
			float temp1;
			if(fscanf(FilePointer,"%f\n", &temp1) != EOF)
			{
				m_Thread_img.pImageProcess->Curve_parameters_field[i] = double(temp1);
			}
			else
				break;
		}
		fclose(FilePointer);
		m_button_LoadParameter.EnableWindow(false);
	}
	else
	{
		AfxMessageBox("File Directory is wrong!!");
	}
}

UINT CHuroEvolutionSystemView::DoThread(LPVOID pParam)
{
	CHuroEvolutionSystemView *pthis = (CHuroEvolutionSystemView *)pParam;
	CWnd *wnd = pthis->m_Thread_img.wnd;	
	
	while(!pthis->m_Thread_img.isTerminate)
	{
		SYSTEMTIME T;
		GetLocalTime(&T);
		int t = T.wSecond*1000 + T.wMilliseconds;
		
		if(pthis->m_Thread_img.pImageProcess->isCameraOpen)
		{
			int hor = pthis->pHeadControl->Head_hor;
			int ver = pthis->pHeadControl->Head_ver;
			pthis->m_Thread_img.pImageProcess->Procedure(/*pthis->isCatchImg*/1, hor, ver, pthis->isRobotKick);
			pthis->m_Thread_img.pImageProcess->pKalman->UpdateState(pthis->ProcessTime/1000);
		}
		
		GetLocalTime(&T);
		int Now_Time = T.wSecond*1000 + T.wMilliseconds;
		pthis->ProcessTime = Now_Time - t;
		
		wnd->PostMessage(WM_THREAD, 1, 0);
		//Wait for function signal response
		WaitForSingleObject(pthis->m_Thread_img.hThread, 50);
	}
	return 0;
}

LRESULT CHuroEvolutionSystemView::OnThread(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CHuroEvolutionSystemView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	//pMySocket->Send();
/*	
	GameState = pMySocket->m_GameState.gamestate;
	if(GameState == INIT)
		OnButtonGAMEINIT();
	else if(GameState == READY)
		OnButtonGAMEREADY();
	else if(GameState == SET)
		OnButtonGAMESET();
	else if(GameState == PLAY)
		OnButtonGAMEPLAY();
*/
	RandomAns();

	HeadState();
	MeasureDis();

	if(isBodyStart && !isManulControl)
	{
		RobotState();
	}
	else if(!isBodyStart && isManulControl)
	{
		switch(Ctrl_Mode)
		{
			case Forward:
				OnButtonBodyForward();
				break;
			case ShiftLeft:
				OnButtonBodyShiftLeft();
				break;
			case ShiftRight:
				OnButtonBodyShiftRight();
				break;
			case Backward:
				OnButtonBodyBackward();
				break;
			case Stop:
				OnButtonBodyStop();
				break;
			case  TurnRight:
				OnButtonBodyTurnRight();
				break;
			case TurnLeft:
				OnButtonBodyTurnLeft();
				break;
			case KICKRIGHT45:
				OnKickright();
				break;	
			case KICKRIGHT:
				OnKickrightStraight();
				break;	
			case KICKLEFT45:
				OnKickleft();
				break;	
			case KICKLEFT:
				OnKickleftStraight();
				break;	
			case SurrandLeft:
				OnButtonBodySurrundLeft();
				break;	
			case SurrandRight:
				OnButtonBodySurrundRight();
				break;
		}
		isManulControl = 1;
		isBodyStart = 0;
	}


	pRobot->ReveiveData();
	UpdateInfo();
	UpdateBavaior();
	UpdateMap();
	CFormView::OnTimer(nIDEvent);
}

void CHuroEvolutionSystemView::HeadState()
{
	if(!isHeadStart)
	{
	}
	else
	{
		if(RobotBehavior != KICKLEFT && RobotBehavior != KICKLEFT45 && RobotBehavior != KICKRIGHT && RobotBehavior != KICKRIGHT45 && Flag_closeball != 1)
		{
			pHeadControl->V_Scanball(isImgDone);
		}
		else
		{
			pHeadControl->Head_state = 1;
			pHeadControl->V_ViewDown();
		}

	}
	m_Thread_img.pImageProcess->Head_hor = pHeadControl->Head_hor;
	m_Thread_img.pImageProcess->Head_ver = pHeadControl->Head_ver;
}

void CHuroEvolutionSystemView::UpdateInfo()
{
	pHeadControl->Getball = m_Thread_img.pImageProcess->isGetPT;
	pHeadControl->Pos_ball =m_Thread_img.pImageProcess->Predict_pt;
	pHeadControl->isBallGone = m_Thread_img.pImageProcess->isBallGone;
	pHeadControl->isZoomIn = m_Thread_img.pImageProcess->isZoomIn;
	isCatchImg = pHeadControl->Catch_start;
	
	isImgDone = m_Thread_img.pImageProcess->isImgDone;
	
	if(!pHeadControl->Catch_start)
	{
		m_Thread_img.pImageProcess->isImgDone = 0;
	}
	Compass_val = pRobot->m_Robot.high_byte_compass*256 + pRobot->m_Robot.low_byte_compass;
	//One_Step_Filter(Compass_val, 0.7, Compass_val);

	int dx = Goal_Point.x - Robot1.x;
	int dy = Robot1.y - Goal_Point.y;
	int ang = cvRound(Rad2Deg(atan2(dy, dx)));
	RG_theta = ang;
	RobotDirection = ang + Compass_val - CompassInit;


	m_edit_Pix_ballx = m_Thread_img.pImageProcess->Ball_pos.x;
	m_edit_Pix_bally = m_Thread_img.pImageProcess->Ball_pos.y;
	m_edit_Real_ballx = Real_posx;
	m_edit_Real_bally = Real_posy;
	m_edit_Head_horpos = pHeadControl->Head_hor;
	m_edit_Head_verpos = pHeadControl->Head_ver;
	m_edit_processtime = ProcessTime;
	m_edit_isGetball = m_Thread_img.pImageProcess->isGetPT;
	m_edit_GrabTime = m_Thread_img.pImageProcess->GrabTime;
	m_edit_ballmode = m_Thread_img.pImageProcess->ImageMode;
	m_edit_closeball = Flag_closeball;
	m_edit_odometery_x = pRobot->m_Robot.tx;
	m_edit_odometery_y = pRobot->m_Robot.ty;
	m_edit_odemtheta = pRobot->m_Robot.ttheta;
	m_edit_compass =  Compass_val - CompassInit;
	m_edit_KickMode = Kick_Mode;
	m_edit_isGoalSide = isGoalSide;
	m_edit_CompassInit = CompassInit;
	m_edit_CompassNorth = CompassNorth;
	m_edit_CompassWest = CompassWest;
	m_edit_CompassEast = CompassEast;
	m_edit_CompassSouth = CompassSouth;
	m_edit_Gamebegining = isGameBegining;
	m_edit_DecisionRand = Decision_rand;
	m_edit_robotface = RobotDirection;

	CString str;
	switch(GameState)
	{
		case INIT:
			str = "INIT";
			break;
		case READY:
			str = "READY";
			break;
		case SET:
			str = "SET";
			break;
		case PLAY:
			str = "PLAY";
			break;
		default:
			str = "WAIT";
			break;
	}
	m_edit_GameState = str;
	UpdateData(0);
}

void CHuroEvolutionSystemView::RobotState()
{
	//First kick
	if(isGameBegining)
	{
		//CloseBall();	
		DribblingDecision();
	}
	else
	{
		//GameRound();
		//RobotDecision();
	}
}

void CHuroEvolutionSystemView::OnButtonManulControl() 
{
	// TODO: Add your control notification handler code here

	m_button_bodystart.EnableWindow(1);
	m_button_manulcontrol.EnableWindow(0);
}

void CHuroEvolutionSystemView::OnButtonViewleft() 
{
	// TODO: Add your control notification handler code here
	pHeadControl->V_ViewLeft();
}

void CHuroEvolutionSystemView::OnButtonViewright() 
{
	// TODO: Add your control notification handler code here
	pHeadControl->V_ViewRight();
}

void CHuroEvolutionSystemView::OnButtonViewDown() 
{
	// TODO: Add your control notification handler code here
	pHeadControl->V_ViewDown();
}

void CHuroEvolutionSystemView::OnButtonStepIt() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 1;
	RobotBehavior = StepIt;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = -5 + 128;	//-5
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnKickleft() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 0;
	if(isManulControl)	Ctrl_Mode = KICKLEFT45;
	RobotBehavior = KICKLEFT45;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 50;
	array[2] = 45;
	array[3] = 2;
	array[4] = 128;
	array[5] = 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnKickright() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 0;
	if(isManulControl)	Ctrl_Mode = KICKRIGHT45;
	RobotBehavior = KICKRIGHT45;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 49;
	array[2] = 45;
	array[3] = 2;
	array[4] = 128;
	array[5] = 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnFallleft() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 0;
	RobotBehavior = FALLDOWNLEFT;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 51;
	array[2] = 0;
	array[3] = 128;
	array[4] = 128;
	array[5] = 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnFallright() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 0;
	RobotBehavior = FALLDOWNRUGHT;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 52;
	array[2] = 0;
	array[3] = 128;
	array[4] = 128;
	array[5] = 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnKickleftStraight() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 0;
	if(isManulControl)	Ctrl_Mode = KICKLEFT;
	RobotBehavior = KICKLEFT;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 50;
	array[2] = 0;
	array[3] = 15;
	array[4] = 128;
	array[5] = 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}	
}

void CHuroEvolutionSystemView::OnKickrightStraight() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 0;
	if(isManulControl)	Ctrl_Mode = KICKRIGHT;
	RobotBehavior = KICKRIGHT;
	isRobotKick = true;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 49;
	array[2] = 0;
	array[3] = 15;
	array[4] = 128;
	array[5] = 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}		
}

void CHuroEvolutionSystemView::CloseBall()
{
	#define CLOSE_LENGTH 40

	static bool compassinit = false;

	if(!compassinit)
	{
		if(pHeadControl->TimeDelay(2000))
		{
			OnButtonCompassINIT();
			compassinit = true;
			pHeadControl->Time_Init = true;
		}
	}
	

	//==========================================
	static bool shiftlock = false;
	static bool forwardlock = false;
	static bool condition_change = false;
	static bool shiftflag = false;
	static bool kickstandby = false;
	static bool kicklock = false;
	//===========================================

	int compass = Compass_val - CompassInit;
	//Flag_closeball = 0;
	if(m_Thread_img.pImageProcess->isGetPT)
	{
		//OnButtonBodySurrundRight();
		if(!isCloseFinish)
		{
			if(!condition_change)
			{
				if(!forwardlock)
				{
					if(!shiftlock)
					{
						if(abs(pRobot->m_Robot.ttheta) < 25)
						{
							if(Decision_rand == 1)
							{
								OnButtonBodySurrundLeft();
							}
							else
							{
								OnButtonBodySurrundRight();
							}
						}
						else
						{
							shiftlock = true;
						}
					}
					else
					{
						forwardlock = true;
					}
				}
				else
				{
					if(Real_posy > 40 && !Flag_closeball)
					{
						OnButtonBodyForward();
					}
					else
					{
						Flag_closeball = true;
						int Pix_bx = m_Thread_img.pImageProcess->Virtual_pt.x;
						int Pix_by = m_Thread_img.pImageProcess->Virtual_pt.y;
						CvPoint regionleft = m_Thread_img.pImageProcess->R_Kick_Region_topleft;
						CvPoint regionright = m_Thread_img.pImageProcess->R_Kick_Region_downright;
						
						if(!shiftflag)
						{
							if(Pix_bx < regionleft.x && !kickstandby)
								OnButtonBodyShiftLeft();
							else if(Pix_bx > regionright.x-5 && !kickstandby)
								OnButtonBodyShiftRight();
							else
							{
								kickstandby = true;
								if(Pix_by < regionleft.y - 25)
									OnButtonBodyForward();
								else
								{
									shiftflag = true;
								}
							}
						}
						else
						{
							OnButtonBodyStop();
							if(pHeadControl->TimeDelay(1000))
							{
								condition_change = true;
								pHeadControl->Time_Init = true;
							}
						}
					}
				}
			}
			else
			{
				OnButtonBodyStop();
				OnKickrightStraight();
				if(pHeadControl->TimeDelay(5000))
				{
					Flag_closeball = false;
					isCloseFinish = true;
					isRobotKick = false;
					pHeadControl->Time_Init = true;
				}
			}
		}
		else
		{
			OnButtonBodyStop();
			if(pHeadControl->TimeDelay(2000))
			{
				isRobotKick = true;
				isGameBegining = false;
				pHeadControl->Time_Init = true;
			}
		}
	}
}

void CHuroEvolutionSystemView::UpdateMap()
{
	Map = cvCreateImage(cvSize(1020, 650), 8, 3);

	int i, j;

	for(i = 0; i < Map->width; i++)
	{
		for(j = 0; j < Map->height; j++)
		{
			Map->imageData[j*Map->widthStep + i*Map->nChannels+2] = (unsigned char)0;
			Map->imageData[j*Map->widthStep + i*Map->nChannels+1] = (unsigned char)100;
			Map->imageData[j*Map->widthStep + i*Map->nChannels+0] = (unsigned char)0;
		}
	}
	CvScalar whitepen = cvScalar(255, 255, 255);
	CvScalar blackpen = cvScalar(0, 0, 0);

	cvRectangle(Map, cvPoint(60, 25), cvPoint(960, 625), whitepen, 2);

	CvPoint field_middle = cvPoint(510, 325);
	cvRectangle(Map, cvPoint(0, field_middle.y - 130), cvPoint(60, field_middle.y + 130), whitepen, 2);
	cvRectangle(Map, cvPoint(960, field_middle.y - 130), cvPoint(1020, field_middle.y + 130), whitepen, 2);
	for(i = 0; i < 60; i++)
	{
		for(j = field_middle.y - 130; j < field_middle.y + 130; j++)
		{
			Map->imageData[j*Map->widthStep + i*Map->nChannels+2] = (unsigned char)100;
			Map->imageData[j*Map->widthStep + i*Map->nChannels+1] = (unsigned char)0;
			Map->imageData[j*Map->widthStep + i*Map->nChannels+0] = (unsigned char)0;
		}
	}
	for(i = 960; i < 1020; i++)
	{
		for(j = field_middle.y - 130; j < field_middle.y + 130; j++)
		{
			Map->imageData[j*Map->widthStep + i*Map->nChannels+2] = (unsigned char)0;
			Map->imageData[j*Map->widthStep + i*Map->nChannels+1] = (unsigned char)0;
			Map->imageData[j*Map->widthStep + i*Map->nChannels+0] = (unsigned char)100;
		}
	}

	
	

	cvRectangle(Map, cvPoint(60, field_middle.y - 250), cvPoint(60+100, field_middle.y + 250), whitepen, 2);
	cvRectangle(Map, cvPoint(960, field_middle.y - 250), cvPoint(960-100, field_middle.y + 250), whitepen, 2);

	cvLine(Map, cvPoint(field_middle.x, 25), cvPoint(field_middle.x, 625), whitepen, 2);
	Target_Line = cvPoint(field_middle.x + 250, 0);
	cvLine(Map, cvPoint(Target_Line.x, 0), cvPoint(Target_Line.x, 650), blackpen, 2);
	
	
	int radius = 75;
	cvCircle(Map, field_middle, radius, whitepen, 2);

	cvLine(Map, cvPoint(field_middle.x+235, field_middle.y), cvPoint(field_middle.x+245, field_middle.y), whitepen, 2);
	cvLine(Map, cvPoint(field_middle.x+240, field_middle.y-5), cvPoint(field_middle.x+240, field_middle.y+5), whitepen, 2);
	cvLine(Map, cvPoint(field_middle.x-235, field_middle.y), cvPoint(field_middle.x-245, field_middle.y), whitepen, 2);
	cvLine(Map, cvPoint(field_middle.x-240, field_middle.y-5), cvPoint(field_middle.x-240, field_middle.y+5), whitepen, 2);

	
	cvCircle(Map, Goal_Point, 5, blackpen, -1);
	
	UpdateRobotPos();
	
	cvShowImage("Map", Map);
	cvReleaseImage(&Map);
}

void CHuroEvolutionSystemView::UpdateRobotPos()
{
	double ang_offset = 90.0;
	int compass = Compass_val - CompassInit;
	int theta = pRobot->m_Robot.ttheta - ang_offset;
	compass = compass - ang_offset;
	static int prex = 0;
	static int prey = 0;

	int unitx = pRobot->m_Robot.tx - prex;
	int unity = pRobot->m_Robot.ty - prey;

	Robot1.x = Robot1.x - (unity*cos(Deg2Rad(compass)) + unitx*sin(Deg2Rad(compass)));
	Robot1.y = Robot1.y + (-(unity*sin(Deg2Rad(compass))) + unitx*cos(Deg2Rad(compass)));

	//draw ball
	if(m_Thread_img.pImageProcess->isGetPT)
	{
		//if(pHeadControl->TimeDelay(5000))
		{
			cvCircle(Map, cvPoint((Robot1.x+Real_posy), (Robot1.y+Real_posx)), 15, cvScalar(0, 0, 255), -1);
		//	pHeadControl->Time_Init = true;
		}
	}
	
	int r = 20;
	Robot_neck.x = Robot1.x + cvRound(r * cos(Deg2Rad(compass+90)));
	Robot_neck.y = Robot1.y + cvRound(r * sin(Deg2Rad(compass+90)));



	CvScalar robotpen = cvScalar(100, 100, 100);
	cvCircle(Map, Robot1, 10, robotpen, -1);
	CvScalar neckpen = cvScalar(0, 0, 0);
	cvLine(Map, Robot1, Robot_neck, neckpen, 3);

	int dx = Goal_Point.x - Robot1.x;
	int dy = Robot1.y - Goal_Point.y;
	int dis = cvRound(Dist(dx, dy));
	int tx = Robot1.x + cvRound(dis*cos(Deg2Rad(RG_theta)));
	int ty = Robot1.y - cvRound(dis*sin(Deg2Rad(RG_theta)));
	cvLine(Map, Robot1, cvPoint(tx, ty), neckpen, 1);

	robotpen = cvScalar(100, 0, 100);
	cvCircle(Map, Robot2, 10, robotpen, -1);
	prex = pRobot->m_Robot.tx;
	prey = pRobot->m_Robot.ty;
}

void CHuroEvolutionSystemView::OnButtonGAMEINIT() 
{
	// TODO: Add your control notification handler code here
	GameState = INIT;
	isGameBegining = 1;
}

void CHuroEvolutionSystemView::OnButtonGAMEREADY() 
{
	// TODO: Add your control notification handler code here
	GameState = READY;
	
	OnButtonCompassINIT();
}

void CHuroEvolutionSystemView::OnButtonGAMESET() 
{
	// TODO: Add your control notification handler code here
	GameState = SET;
	OnButtonHeadStart();
}

void CHuroEvolutionSystemView::OnButtonGAMEPLAY() 
{
	// TODO: Add your control notification handler code here
	GameState = PLAY;
	OnButtonBodyStart();
}

void CHuroEvolutionSystemView::GameRound()
{
	//==========================================
	static bool shiftlock = false;
	static bool forwardlock = false;
	static bool condition_change = false;
	static bool shiftflag = false;
	static bool kick_shiftlock = false;
	static bool kicklock = false;
	static bool dribblelock = false;
	static bool kickstandby = false;
	//===========================================

	if(!isKickFinish)
	{
	if(m_Thread_img.pImageProcess->isGetPT)
	{
		if(!condition_change)
		{
			if(!forwardlock)
			{
				if(Real_posy > 40)
				{
					if(!shiftlock)
					{
						if(abs(Real_posx) > 5)
						{
							if(Real_posx > 0)
								OnButtonBodyShiftRight();
							else
								OnButtonBodyShiftLeft();
						}
						else
						{
							shiftlock = true;
						}
					}
					else
					{
						OnButtonBodyForward();
						if(abs(Real_posx) > 10)
							shiftlock = false;
					}
				}
				else
				{
					OnButtonBodyStop();
					forwardlock = true;
				}
			}
			else
			{
				if(!shiftflag)
				{
					if(pRobot->m_Robot.ttheta < 0)
					{
						OnButtonBodySurrundLeft();
					}
					else
					{
						shiftflag = true;
					}
				}
				else
				{
					if(!dribblelock)
					{
						if(abs(Real_posx) > 10)
						{
							if(Real_posx > 0)
								OnButtonBodyShiftRight();
							else
								OnButtonBodyShiftLeft();
						}
						else
						{
							dribblelock = true;
						}
					}
					else
					{
						OnButtonBodyForward();
						if(abs(Real_posx) > 15)
							dribblelock = false;
					}

					if(pHeadControl->TimeDelay(15000))
					{
						condition_change = true;
						Flag_closeball = true;
						pHeadControl->Time_Init = true;
					}
				}
			}
		}
		else
		{
			int Pix_bx = m_Thread_img.pImageProcess->Virtual_pt.x;
			int Pix_by = m_Thread_img.pImageProcess->Virtual_pt.y;
			CvPoint regionleft = m_Thread_img.pImageProcess->R_Kick_Region_topleft;
			CvPoint regionright = m_Thread_img.pImageProcess->R_Kick_Region_downright;

			if(!kicklock)
			{
				if(!kick_shiftlock)
				{
					if(Pix_bx < regionleft.x && !kickstandby)
						OnButtonBodyShiftLeft();
					else if(Pix_bx > regionright.x-5 && !kickstandby)
						OnButtonBodyShiftRight();
					else
					{
						kickstandby = true;
						if(Pix_by < regionleft.y - 25)
							OnButtonBodyForward();
						else
						{
							kick_shiftlock = true;
						}
					}
				}
				else
				{
					OnButtonBodyStop();
					if(pHeadControl->TimeDelay(2000))
					{
						kicklock = true;
						pHeadControl->Time_Init = true;
					}
				}
			}
			else
			{
				OnKickrightStraight();
				if(pHeadControl->TimeDelay(7000))
				{
					Flag_closeball = false;
					isKickFinish = true;
					pHeadControl->Time_Init = true;
				}
			}
			
		}
		}
	}
	else
	{
		OnButtonBodyStop();
	}
}

void CHuroEvolutionSystemView::RobotDecision()
{
	if(m_Thread_img.pImageProcess->isGetPT)
	{
		if(abs(RobotDirection) > 10)
		{
			if(RobotDirection > 0)
			{
				OnButtonBodyTurnLeft();
			}
			else if(RobotDirection < 0)
			{
				OnButtonBodyTurnRight();
			}
		}
		else
		{
			if(!isKickFinish)
				KickDecision();
			else
				OnButtonBodyStop();
		}
	}
}

void CHuroEvolutionSystemView::OnButtonCompassINIT() 
{
	// TODO: Add your control notification handler code here
	CompassInit = Compass_val;
}

void CHuroEvolutionSystemView::OnButtonCompassCalibrate() 
{
	// TODO: Add your control notification handler code here
	if(CompassNorth > 0 && CompassWest > 0 && CompassSouth > 0 && CompassEast > 0)
	{
		int NW_range = abs(CompassNorth - CompassWest);
		int WS_range = abs(CompassWest - CompassSouth);
		int SE_range = abs(CompassSouth - CompassEast);
		int EN_range = abs(CompassEast - CompassNorth);

// 		if(NW_range > 200)	NW_range = 360 - NW_range;
// 		if(WS_range > 200)	WS_range = 360 - WS_range;
// 		if(SE_range > 200)	SE_range = 360 - SE_range;
// 		if(EN_range > 200)	EN_range = 360 - EN_range;
// 
// 		if(Compass_val <= CompassNorth && Compass_val > CompassWest)
// 		{
// 			Compass_val = cvRound(Compass_val * 90 / (double)NW_range);
// 		}
// 		else if(Compass_val <= CompassWest && Compass_val > CompassSouth)
// 		{
// 			Compass_val = cvRound(Compass_val * 90 / (double)WS_range);
// 		}
// 		else if(Compass_val <= CompassSouth && Compass_val > CompassEast)
// 		{
// 			Compass_val = cvRound(Compass_val * 90 / (double)SE_range);
// 		}
// 		else
// 			Compass_val = cvRound(Compass_val * 90 / (double)EN_range);

	}
	else
	{

	}
}

void CHuroEvolutionSystemView::OnButtonCompassSetNorth() 
{
	// TODO: Add your control notification handler code here
	CompassNorth = Compass_val;
}

void CHuroEvolutionSystemView::OnButtonCompassSetWest() 
{
	// TODO: Add your control notification handler code here
	CompassWest = Compass_val;
}

void CHuroEvolutionSystemView::OnButtonCompassSetSouth() 
{
	// TODO: Add your control notification handler code here
	CompassSouth = Compass_val;
}

void CHuroEvolutionSystemView::OnButtonCompassSetEast() 
{
	// TODO: Add your control notification handler code here
	CompassEast = Compass_val;
}

void CHuroEvolutionSystemView::UpdateBavaior()
{
	CString str;
	switch(RobotBehavior)
	{
		case Stop:
			str = "STOP";
			break;
		case Forward:
			str = "FORWARD";
			break;
		case Backward:
			str = "BACKWARD";
			break;
		case ShiftLeft:
			str = "SHIFTLEFT";
			break;
		case ShiftRight:
			str = "SHIFTRIGHT";
			break;
		case TurnLeft:
			str = "TURNLEFT";
			break;
		case TurnRight:
			str = "TURNRIGHT";
			break;
		case KICKLEFT:
			str = "KICKLEFT";
			break;
		case KICKLEFT45:
			str = "KICKLEFT45";
			break;
		case KICKRIGHT:
			str = "KICKRIGHT";
			break;
		case KICKRIGHT45:
			str = "KICKRIGHT45";
			break;
		case StepIt:
			str = "STEPIT";
			break;
		case STANDUP:
			str = "STANDUP";
			break;
		case SurrandRight:
			str = "SurrandRight";
			break;
		case SurrandLeft:
			str = "SurrandLeft";
			break;
		default:
			str = "STAND";
			break;
	}
	m_edit_Behavior = str;
}

void CHuroEvolutionSystemView::OnButtonBodySurrundRight() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 1;
	if(isManulControl)	Ctrl_Mode = SurrandRight;
	RobotBehavior = SurrandRight;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 5 + 128;
	array[4] = 80 + 128;//40
	array[5] = -45 + 128;//-65
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::OnButtonBodySurrundLeft() 
{
	// TODO: Add your control notification handler code here
	isCountOde = 1;
	if(isManulControl)	Ctrl_Mode = SurrandLeft;
	RobotBehavior = SurrandLeft;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 5 + 128;
	array[4] = -80 + 128;//-40
	array[5] = 45 + 128;//50
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroEvolutionSystemView::RandomAns()
{
	static int cnt = 0;
	static int odd_cnt = 0;
	static int even_cnt = 0;
	if(cnt > 11)
		return;

	if(rand() % 2 == 0)
		even_cnt += 1;
	else
		odd_cnt += 1;

	if(cnt == 11)
	{
		if(even_cnt > odd_cnt)
			Decision_rand = 0;
		else
			Decision_rand = 1;
	}
	

	cnt = cnt + 1;
}

void CHuroEvolutionSystemView::MeasureDis()
{
	if(m_Thread_img.pImageProcess->isGetPT)
	{
		int width;
		int height;
		if(m_Thread_img.pImageProcess->isZoomIn)
		{
			width = CAM_ZOOMIN_WIDTH;
			height = CAM_ZOOMIN_HEIGHT;
		}
		else
		{
			width = CAM_ZOOMOUT_WIDTH;
			height = CAM_ZOOMOUT_HEIGHT;
		}
		
		
		int rx, ry;
		Estimate(m_Thread_img.pImageProcess->Ball_pos, double(pHeadControl->Head_hor), double(pHeadControl->Head_ver), &rx, &ry, width, height);
// 		One_Step_Filter(rx, 0.9, rx);
//  		One_Step_Filter(ry, 0.9, ry);
		
		Real_posx = rx;
		Real_posy = ry;
	}
}



void CHuroEvolutionSystemView::KickDecision()
{
	//===============================================
	static bool kick_shiftlock = false;
	static bool kicklock = false;
	static bool dribblelock = false;
	static bool kickstandby = false;
	//===============================================

	if(Robot1.x > Target_Line.x)
	{
		Flag_closeball = true;
		int Pix_bx = m_Thread_img.pImageProcess->Virtual_pt.x;
		int Pix_by = m_Thread_img.pImageProcess->Virtual_pt.y;
		CvPoint regionleft = m_Thread_img.pImageProcess->R_Kick_Region_topleft;
		CvPoint regionright = m_Thread_img.pImageProcess->R_Kick_Region_downright;

		if(!kicklock)
		{
			if(!kick_shiftlock)
			{
				if(Pix_bx < regionleft.x && !kickstandby)
					OnButtonBodyShiftLeft();
				else if(Pix_bx > regionright.x-5 && !kickstandby)
					OnButtonBodyShiftRight();
				else
				{
					kickstandby = true;
					if(Pix_by < regionleft.y - 25)
						OnButtonBodyForward();
					else
					{
						kick_shiftlock = true;
					}
				}
			}
			else
			{
				OnButtonBodyStop();
				if(pHeadControl->TimeDelay(2000))
				{
					kicklock = true;
					pHeadControl->Time_Init = true;
				}
			}
		}
		else
		{
			OnKickrightStraight();
			if(pHeadControl->TimeDelay(7000))
			{
				Flag_closeball = false;
				isKickFinish = true;
				pHeadControl->Time_Init = true;
			}
		}
	}
	else
	{
		DribblingDecision();
	}
}

void CHuroEvolutionSystemView::DribblingDecision()
{
	//==========================================
	static bool shiftlock = false;
	//===========================================
	if(!shiftlock)
	{
		if(abs(Real_posx) > 5)
		{
			if(Real_posx > 0)
				OnButtonBodyShiftRight();
			else
				OnButtonBodyShiftLeft();
		}
		else
		{
			shiftlock = true;
		}
	}
	else
	{
		OnButtonBodyForward();
		if(abs(Real_posx) > 10)
			shiftlock = false;
	}
}
