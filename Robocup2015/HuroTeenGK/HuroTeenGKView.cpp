// HuroTeenGKView.cpp : implementation of the CHuroTeenGKView class
//

#include "stdafx.h"
#include "HuroTeenGK.h"

#include "HuroTeenGKDoc.h"
#include "HuroTeenGKView.h"

#include "RobotBehavior.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHuroTeenGKView

IMPLEMENT_DYNCREATE(CHuroTeenGKView, CFormView)

BEGIN_MESSAGE_MAP(CHuroTeenGKView, CFormView)
	//{{AFX_MSG_MAP(CHuroTeenGKView)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonHeadStart)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonBodyStart)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonSaveImage)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonEmergencyStop)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonManualControl)
	ON_BN_CLICKED(IDC_BUTTON6, OnButtonBodyStop)
	ON_BN_CLICKED(IDC_BUTTON7, OnButtonBodyForward)
	ON_BN_CLICKED(IDC_BUTTON8, OnButtonBodyBackward)
	ON_BN_CLICKED(IDC_BUTTON9, OnButtonBodyShiftRight)
	ON_BN_CLICKED(IDC_BUTTON10, OnButtonBodyShiftLeft)
	ON_BN_CLICKED(IDC_BUTTON11, OnButtonBodyStepIt)
	ON_BN_CLICKED(IDC_BUTTON12, OnButtonBodyTurnRight)
	ON_BN_CLICKED(IDC_BUTTON13, OnButtonBodyTurnLeft)
	ON_BN_CLICKED(IDC_BUTTON14, OnButtonViewLeft)
	ON_BN_CLICKED(IDC_BUTTON15, OnButtonViewRight)
	ON_BN_CLICKED(IDC_BUTTON16, OnButtonViewDown)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON17, OnButtonBodyLeftKick)
	ON_BN_CLICKED(IDC_BUTTON18, OnButtonBodyRightKick)
	ON_BN_CLICKED(IDC_BUTTON19, OnButtonBodyFallLeft)
	ON_BN_CLICKED(IDC_BUTTON20, OnButtonBodyFallRight)
	ON_EN_CHANGE(IDC_EDIT13, OnChangeEdit13)
	ON_EN_CHANGE(IDC_EDIT12, OnChangeEdit12)
	ON_BN_CLICKED(IDC_BUTTON21, OnButton21)
	ON_BN_CLICKED(IDC_BUTTON22, OnButton22)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHuroTeenGKView construction/destruction

CHuroTeenGKView::CHuroTeenGKView()
	: CFormView(CHuroTeenGKView::IDD)
{
	//{{AFX_DATA_INIT(CHuroTeenGKView)
	m_edit_processtime = 0;
	m_edit_Pix_ballx = 0.0;
	m_edit_Pix_bally = 0.0;
	m_edit_Real_bally = 0.0;
	m_edit_Real_ballx = 0.0;
	m_edit_Head_horpos = 0;
	m_edit_Head_verpos = 0;
	m_edit_isGetball = 0;
	m_edit_Ballgone = 0;
	m_edit_vball = 0.0f;
	m_edit_odometeryX = 0.0f;
	m_edit_decision = _T("");
	m_falldown_show = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here

	m_Thread_img.pImageProcess = new CImageProcess();
	pHeadControl = new CHeadControl(COM_PORT, BUNDRATE);
	pRobot = new CRobotBehavior(B_COM_PORT, B_BUNDRATE);
	
	isHeadStart = 0;
	isBodyStart = 0;
	isCatchImg = 0;
	isImgDone = 0;
	isManulControl = 1;
	vData = fopen("Velocity.txt", "w");
}

CHuroTeenGKView::~CHuroTeenGKView()
{
	m_Thread_img.isTerminate = 1;
	delete m_Thread_img.pImageProcess;
	delete pHeadControl;
	delete pRobot;
	fclose(vData);
}

void CHuroTeenGKView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHuroTeenGKView)
	DDX_Control(pDX, IDC_BUTTON2, m_button_bodystart);
	DDX_Control(pDX, IDC_BUTTON5, m_button_manualcontrol);
	DDX_Text(pDX, IDC_EDIT1, m_edit_processtime);
	DDX_Text(pDX, IDC_EDIT2, m_edit_Pix_ballx);
	DDX_Text(pDX, IDC_EDIT3, m_edit_Pix_bally);
	DDX_Text(pDX, IDC_EDIT5, m_edit_Real_bally);
	DDX_Text(pDX, IDC_EDIT4, m_edit_Real_ballx);
	DDX_Text(pDX, IDC_EDIT6, m_edit_Head_horpos);
	DDX_Text(pDX, IDC_EDIT7, m_edit_Head_verpos);
	DDX_Text(pDX, IDC_EDIT8, m_edit_isGetball);
	DDX_Text(pDX, IDC_EDIT9, m_edit_Ballgone);
	DDX_Text(pDX, IDC_EDIT10, m_edit_vball);
	DDX_Text(pDX, IDC_EDIT11, m_edit_odometeryX);
	DDX_Text(pDX, IDC_EDIT12, m_edit_decision);
	DDX_Text(pDX, IDC_EDIT13, m_falldown_show);
	//}}AFX_DATA_MAP
}

BOOL CHuroTeenGKView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CHuroTeenGKView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	//Create thread
	m_Thread_img.isTerminate = false;
	m_Thread_img.wnd = this;
	m_Thread_img.hThread = CreateEvent( 0, 1, 0, "MyEvent");
	AfxBeginThread(DoThread,(LPVOID) this);
	
	SetTimer(1, 100, NULL);
	LoadParameter();

}

/////////////////////////////////////////////////////////////////////////////
// CHuroTeenGKView printing

BOOL CHuroTeenGKView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CHuroTeenGKView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CHuroTeenGKView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CHuroTeenGKView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CHuroTeenGKView diagnostics

#ifdef _DEBUG
void CHuroTeenGKView::AssertValid() const
{
	CFormView::AssertValid();
}

void CHuroTeenGKView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CHuroTeenGKDoc* CHuroTeenGKView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHuroTeenGKDoc)));
	return (CHuroTeenGKDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHuroTeenGKView message handlers
UINT CHuroTeenGKView::DoThread(LPVOID pParam)
{
	CHuroTeenGKView *pthis = (CHuroTeenGKView *)pParam;
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
			pthis->m_Thread_img.pImageProcess->Procedure(pthis->isCatchImg, hor, ver);
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

LRESULT CHuroTeenGKView::OnThread(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void CHuroTeenGKView::OnTimer(UINT nIDEvent) 
{
	HeadState();
	
	if(isBodyStart && !isManulControl)
		RobotState();
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
		}
		isManulControl = 1;
		isBodyStart = 0;
	}
	
	pRobot->ReveiveData();
	UpdateInfo();
	
	CFormView::OnTimer(nIDEvent);
}

void CHuroTeenGKView::LoadParameter() 
{
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
		
	}
	else
	{
		AfxMessageBox("File Directory is wrong!!");
	}
}

void CHuroTeenGKView::HeadState()
{
	if(!isHeadStart)
	{
	}
	else
	{
		pHeadControl->V_Scanball(isImgDone);
	}
	m_Thread_img.pImageProcess->Head_hor = pHeadControl->Head_hor;
	m_Thread_img.pImageProcess->Head_ver = pHeadControl->Head_ver;
}

void CHuroTeenGKView::UpdateInfo()
{
	pHeadControl->Getball = m_Thread_img.pImageProcess->isGetPT;
	pHeadControl->Pos_ball =m_Thread_img.pImageProcess->Predict_pt;
	pHeadControl->isBallGone = m_Thread_img.pImageProcess->isBallGone;
	isCatchImg = pHeadControl->Catch_start;
	
	isImgDone = m_Thread_img.pImageProcess->isImgDone;
	
	if(!pHeadControl->Catch_start)
	{
		m_Thread_img.pImageProcess->isImgDone = 0;
	}
	
	
	m_edit_Pix_ballx = m_Thread_img.pImageProcess->topleft.x;
	m_edit_Pix_bally = m_Thread_img.pImageProcess->topleft.y;
	m_edit_Real_ballx = Real_posx;
	m_edit_Real_bally = Real_posy;
	m_edit_Head_horpos = pHeadControl->Head_hor;
	m_edit_Head_verpos = pHeadControl->Head_ver;
	m_edit_processtime = ProcessTime;
	m_edit_isGetball = m_Thread_img.pImageProcess->isGetPT;
	m_edit_Ballgone = m_Thread_img.pImageProcess->isBallGone;

	static int xx=0, xt=0 ,x1=0, x2=0, x3=0;
	static int  yy=0,yt=0 ,y1=0, y2=0, y3=0;
	
	x3 = x2;	y3 = y2;
	x2 = x1;	y2 = y1;
	x1 = xx;	y1 = yy;

	xt = (x1+x2+x3)/3;
	yt = (y1+y2+y3)/3;

	int  last_x = xt, last_y = yt;
	
	xx = m_Thread_img.pImageProcess->Predict_pt.x;
	yy = m_Thread_img.pImageProcess->Predict_pt.y;	

	velocity = Q_Dist(cvPoint(last_x, last_y), cvPoint(xx, yy)) / ProcessTime;
	m_edit_vball = cvRound(velocity * 100);
	
	if(pHeadControl->Getball)
		fprintf(vData,"%f\n", m_edit_vball);

	
	m_edit_decision = decision;

	UpdateData(0);
}

void CHuroTeenGKView::RobotState()
{	
	
	if(m_Thread_img.pImageProcess->isGetPT)
	{	
		Estimate(m_Thread_img.pImageProcess->Ball_pos, double(pHeadControl->Head_hor), double(pHeadControl->Head_ver), &Real_posx, &Real_posy);
		int rx = Real_posx;
		int ry = Real_posy;
		//==========================================
		static bool shiftlock = false;
		static bool forwardlock = false;
		static bool backwardlock = false;
		static bool speciallock = false;
		static bool condition_jump = false;
		//===========================================
		float odo_x =0;
		int distance_far = 400;
		int distance_middle = 210;
		int falldown_distance_x = 100;
		int falldown_distance_y = 150;
		int falldown_velocity = 150;
		int robot_location_x = 0;
		int robot_last_x = 0;
		int reverse_falldown_right = 0;
		int reverse_falldown_left = 0;
		//===========================================

		if(fabs(ry) > distance_far) // far
		{
			OnButtonBodyStop();	decision = "Stop";
		}

		else if(fabs(ry) > distance_middle && fabs(ry) < distance_far) //middle
		{
			OnButtonBodyStepIt();	decision = "Stepping";
		}

		else if(ry > 5 && ry <= distance_middle) //near 
		{
			if(abs(rx) > 5 && !condition_jump)
			{	
				if(reverse_falldown_left == 1 && reverse_falldown_right == 1)
				{	
					if(robot_location_x < 0)
					{
						OnButtonBodyFallRight();	m_falldown_show = "Down_Right";
					}
					else
					{
						OnButtonBodyFallLeft();     m_falldown_show = "Down_Left";
					}
					reverse_falldown_right = 0 ;	reverse_falldown_left = 0;
				}

				if(rx > 0)
				{	
					reverse_falldown_right = 1;
					robot_last_x = pRobot->m_Robot.ty;
					OnButtonBodyShiftRight();	decision = "Falling Right";

					robot_location_x += pRobot->m_Robot.ty - robot_last_x;

					if( ry <falldown_distance_y && rx >falldown_distance_x && velocity >falldown_velocity)
					{	
						robot_last_x = pRobot->m_Robot.ty;
						//fall_down_right
						OnButtonBodyFallRight();  m_falldown_show = "Down_Right";

						robot_location_x += pRobot->m_Robot.ty - robot_last_x;
						robot_last_x = pRobot->m_Robot.ty;

						for (;abs(robot_location_x) > 5;)
						{
							OnButtonBodyStepIt();	decision = "Stepping";
							if(robot_location_x > 0)
							{
								OnButtonBodyShiftLeft();	decision = "Falling Left";
								robot_location_x += pRobot->m_Robot.ty - robot_last_x;
								robot_last_x = pRobot->m_Robot.ty;
							}
							else
							{
								OnButtonBodyShiftRight();	decision = "Falling Right";
								robot_location_x += pRobot->m_Robot.ty - robot_last_x;
								robot_last_x = pRobot->m_Robot.ty;
							}

						}
					}
				}
				else
				{	
					reverse_falldown_left = 1;
					robot_last_x = pRobot->m_Robot.ty;
					OnButtonBodyShiftLeft();	decision = "Falling Left";
					robot_location_x += pRobot->m_Robot.ty - robot_last_x;
					
					if( ry <falldown_distance_y && rx <falldown_distance_x && velocity >falldown_velocity)
					{	
						robot_last_x = pRobot->m_Robot.ty;
						//fall_down_left
						OnButtonBodyFallLeft();   m_falldown_show = "Down_Left";

						robot_location_x += pRobot->m_Robot.ty - robot_last_x;
						robot_last_x = pRobot->m_Robot.ty;

						for (; abs(robot_location_x) < 5 ;)
						{
							OnButtonBodyStepIt();	decision = "Stepping";
							if(robot_location_x > 0)
							{
								OnButtonBodyShiftLeft();	decision = "Falling Left";
								robot_location_x += pRobot->m_Robot.ty - robot_last_x;
								robot_last_x = pRobot->m_Robot.ty;
							}
							else
							{
								OnButtonBodyShiftRight();	decision = "Falling Right";
								robot_location_x += pRobot->m_Robot.ty - robot_last_x;
								robot_last_x = pRobot->m_Robot.ty;
							}
							
						}
					}
				}
				
			}
			else
			{
				if(pHeadControl->TimeDelay(1500))
				{		
					OnButtonBodyStepIt();	decision = "Stepping";

//  					if(velocity <=150 && !forwardlock)
//  					{
//  						OnButtonBodyForward(); 
// 						odo_x += pRobot->m_Robot.tx;
// 						
// 						if(ry<=10)
// 						{
// 							OnButtonBodyStop();
// 							OnButtonBodyRightKick();
// 							forwardlock = true;
// 						}
// 					}
// 
// 					else if( forwardlock)
// 					{
// 						if(pHeadControl->TimeDelay(1500))
// 						{
// 							OnButtonBodyBackward();
// 							forwardlock =false;
// 						}
// 					}

					condition_jump = true;

					if(abs(rx) > 8)
					{
						condition_jump = false;
					}
					pHeadControl->Time_Init = true;
				}
			}
		}
		else
		{
			OnButtonBodyBackward();	decision = "Backward";
		}

		m_edit_odometeryX = odo_x;
		UpdateData(0);
	}
	else
	{
		OnButtonBodyStepIt();
	}

	
}

void CHuroTeenGKView::OnButtonBodyStart() 
{
	isBodyStart = 1;
	isManulControl = 0;
	
	m_button_bodystart.EnableWindow(0);
	m_button_manualcontrol.EnableWindow(1);
}

void CHuroTeenGKView::OnButtonHeadStart() 
{
	isHeadStart = 1;
}

void CHuroTeenGKView::OnButtonManualControl() 
{
	m_button_bodystart.EnableWindow(1);
	m_button_manualcontrol.EnableWindow(0);
	
}

void CHuroTeenGKView::OnButtonEmergencyStop() 
{
	m_Thread_img.isTerminate = 1;
	KillTimer(1);
	OnButtonBodyStop();
}

void CHuroTeenGKView::OnButtonBodyStop() 
{
	if(isManulControl)	Ctrl_Mode = Stop;
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

void CHuroTeenGKView::OnButtonSaveImage() 
{
	m_Thread_img.pImageProcess->SaveImage();
}

void CHuroTeenGKView::OnButtonViewLeft() 
{
	pHeadControl->V_ViewLeft();
}

void CHuroTeenGKView::OnButtonViewRight() 
{
	pHeadControl->V_ViewRight();
}

void CHuroTeenGKView::OnButtonViewDown() 
{
	pHeadControl->V_ViewDown();
}

void CHuroTeenGKView::OnMovedOmni(int x, int y, int theta) 
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

void CHuroTeenGKView::OnButtonBodyForward() 
{
	if(isManulControl)	Ctrl_Mode = Forward;
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

void CHuroTeenGKView::OnButtonBodyBackward() 
{
	if(isManulControl)	Ctrl_Mode = Backward;
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
		pRobot->SendData(array, i);
	}
}

void CHuroTeenGKView::OnButtonBodyShiftRight() 
{
	if(isManulControl)	Ctrl_Mode = ShiftRight;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 5 + 128;
	array[4] = 80 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroTeenGKView::OnButtonBodyShiftLeft() 
{
	if(isManulControl)	Ctrl_Mode = ShiftLeft;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 5 + 128;
	array[4] = -80 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroTeenGKView::OnButtonBodyStepIt() 
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;	//-5
	array[4] = 0 + 128;
	array[5] = 0 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroTeenGKView::OnButtonBodyTurnRight() 
{
	if(isManulControl)	Ctrl_Mode = TurnRight;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 0 + 128;
	array[5] = 90 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroTeenGKView::OnButtonBodyTurnLeft() 
{
	if(isManulControl)	Ctrl_Mode = TurnLeft;
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 0;
	array[2] = 128;
	array[3] = 0 + 128;
	array[4] = 0 + 128;
	array[5] = -90 + 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}


void CHuroTeenGKView::OnButtonBodyLeftKick() 
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 50;
	array[2] = 0;
	array[3] = 2;
	array[4] = 128;
	array[5] = 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroTeenGKView::OnButtonBodyRightKick() 
{
	unsigned char array[7];
	array[0] = 0xFE;
	array[1] = 49;
	array[2] = 0;
	array[3] = 2;
	array[4] = 128;
	array[5] = 128;
	array[6] = 0xFF;			
	for(int i = 0; i < 7; i++)
	{
		pRobot->SendData(array, i);
	}
}

void CHuroTeenGKView::OnButtonBodyFallLeft() 
{
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

void CHuroTeenGKView::OnButtonBodyFallRight() 
{
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

void CHuroTeenGKView::OnChangeEdit13() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CHuroTeenGKView::OnChangeEdit12() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void CHuroTeenGKView::OnButton21() 
{
	// TODO: Add your control notification handler code here
	OnButtonBodyFallRight();  m_falldown_show = "Down_Right";
}

void CHuroTeenGKView::OnButton22() 
{
	// TODO: Add your control notification handler code here
	OnButtonBodyFallLeft();  m_falldown_show = "Down_Left";
}
