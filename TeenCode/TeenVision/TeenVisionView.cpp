// TeenVisionView.cpp : implementation of the CTeenVisionView class
//

#include "stdafx.h"
#include "TeenVision.h"

#include "TeenVisionDoc.h"
#include "TeenVisionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTeenVisionView

IMPLEMENT_DYNCREATE(CTeenVisionView, CFormView)

BEGIN_MESSAGE_MAP(CTeenVisionView, CFormView)
	//{{AFX_MSG_MAP(CTeenVisionView)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonMoved)
	ON_BN_CLICKED(IDC_BUTTON6, OnButtonForward)
	ON_BN_CLICKED(IDC_BUTTON7, OnButtonShiftLeft)
	ON_BN_CLICKED(IDC_BUTTON8, OnButtonShiftRight)
	ON_BN_CLICKED(IDC_BUTTON9, OnButtonBackward)
	ON_BN_CLICKED(IDC_BUTTON10, OnButtonFastWalk)
	ON_BN_CLICKED(IDC_BUTTON11, OnButtonWalk)
	ON_BN_CLICKED(IDC_BUTTON12, OnButtonStartStop)
	ON_BN_CLICKED(IDC_BUTTON13, OnButtonTurnOff)
	ON_EN_CHANGE(IDC_EDIT_ComPort, OnChangeEditComPort)
	ON_EN_CHANGE(IDC_EDIT_RemoteComPort, OnChangeEditRemoteComPort)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonExit)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON14, OnButton_FallDownLeft)
	ON_BN_CLICKED(IDC_BUTTON15, OnButton_FallDownRight)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_MESSAGE(WM_THREAD, OnThread)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeenVisionView construction/destruction

CTeenVisionView::CTeenVisionView()
	: CFormView(CTeenVisionView::IDD)
{
	//{{AFX_DATA_INIT(CTeenVisionView)
	m_edit_ComPort = H_comPort;
	m_edit_RemoteComPort = B_comPort;
	m_message = _T("");
	m_edit_ProcessingTime = 0;
	m_edit_distX = 0;
	m_edit_distY = 0;
	m_edit_Status = _T("");
	m_edit_Decision = _T("");
	//}}AFX_DATA_INIT

	//---------------------////-----------------------
	m_ImThread.imagePro = new ImageProcessing();
	head = new Head();
	

}

CTeenVisionView::~CTeenVisionView()
{
	delete	m_ImThread.imagePro;
 	
	delete head;
	
	//behavior.Set_Robot();	//set standby
	//behavior.ClosePort(); 		
}

void CTeenVisionView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTeenVisionView)
	DDX_Text(pDX, IDC_EDIT_ComPort, m_edit_ComPort);
	DDX_Text(pDX, IDC_EDIT_RemoteComPort, m_edit_RemoteComPort);
	DDX_Text(pDX, IDC_Message, m_message);
	DDX_Text(pDX, IDC_ProTime, m_edit_ProcessingTime);
	DDX_Text(pDX, IDC_X_dist, m_edit_distX);
	DDX_Text(pDX, IDC_Y_dist, m_edit_distY);
	DDX_Text(pDX, IDC_Status, m_edit_Status);
	DDX_Text(pDX, IDC_Decision, m_edit_Decision);
	//}}AFX_DATA_MAP
}

BOOL CTeenVisionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CTeenVisionView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	LoadParam();
	
	m_ImThread.isTerminate = false;
	m_ImThread.wnd = this;
	m_ImThread.hThread = CreateEvent(0, 1, 0, "MyEvent");
	AfxBeginThread(DoThread, (LPVOID) this);

	SetTimer(1, 10, NULL);
 //	SetTimer(2, 10, NULL);
// 	
}

/////////////////////////////////////////////////////////////////////////////
// CTeenVisionView printing

BOOL CTeenVisionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTeenVisionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTeenVisionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTeenVisionView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CTeenVisionView diagnostics

#ifdef _DEBUG
void CTeenVisionView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTeenVisionView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTeenVisionDoc* CTeenVisionView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTeenVisionDoc)));
	return (CTeenVisionDoc*)m_pDocument;
}
#endif //_DEBUG

///////////////-----------------------------//////////////////

UINT CTeenVisionView::DoThread(LPVOID pParam)
{
	CTeenVisionView *pthis = (CTeenVisionView *)pParam;
	CWnd *wnd = pthis->m_ImThread.wnd;

	while(!pthis->m_ImThread.isTerminate)
	{
		SYSTEMTIME T;
		GetLocalTime(&T);
		int t=T.wSecond*1000 + T.wMilliseconds;

		pthis->m_ImThread.imagePro->ImageProcedure(1);

		GetLocalTime(&T);
		int Now_Time=T.wSecond*1000 + T.wMilliseconds;
		pthis->ProcessTime = Now_Time-t;

		wnd->PostMessage(WM_THREAD, 1, 0);

		WaitForSingleObject(pthis->m_ImThread.hThread, 30);
	}
 	return 0;
}

LRESULT CTeenVisionView::OnThread(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// CTeenVisionView message handlers

void CTeenVisionView::OnTimer(UINT nIDEvent) 
{	
	m_edit_ProcessingTime = ProcessTime;
	UpdateData(false);

 	head->CamPos(m_ImThread.imagePro->findBall,m_ImThread.imagePro->Pos_ball, cvPoint(camWidth, camHeight));

		Estimate(m_ImThread.imagePro->Pos_ball, double(head->dhor), double(head->dver), &rx, &ry);
		//dist = Dist(rx,ry);

 	behavior.TrackBall(m_ImThread.imagePro->findBall, moved,cvPoint(rx, ry));

 	Message();

	//ShowPicture(imagePro->img,IDC_PicCapture);
	//ShowPicture(imagePro->streaming,IDC_PicCapture);
	//imagePro->ImageReleasing(imagePro->streaming);

	
	CFormView::OnTimer(nIDEvent);
}

void CTeenVisionView::Message()
{	
	m_message.Format("Data = %d, head = %d", m_ImThread.imagePro->isGetPT, head->Head_state);
	
	m_edit_distX = rx;		m_edit_distY = ry;
	m_edit_Status = behavior.status;
	m_edit_Decision = behavior.decision;

	UpdateData(0);
}

void CTeenVisionView::LoadParam()
{
	FILE *fp1, *fp2;

	fp1 = fopen("D:\\Lab Project\\TeenVision\\Ball.txt", "r");
	//fp1 = fopen("C:\\pk_ball.txt", "r");
	fp2 = fopen("D:\\Lab Project\\TeenVision\\Parameters.txt", "r");

	int a;
	for(a = 0; a < 4; a++)
	{
		float temp1,temp2;
		if(fscanf(fp1,"%f\n", &temp1) != EOF)
		{
			m_ImThread.imagePro->Ball_Parameter[a] = double(temp1);
		}
		if(fscanf(fp2,"%f\n", &temp2) != EOF)
		{
			m_ImThread.imagePro->Field_Parameter[a] = double(temp2);
		}
		else
			break;
	}
	fclose(fp1);
	fclose(fp2);
}

void CTeenVisionView::ShowPicture(IplImage *img, UINT ID)
{	
	
	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	SetRect(rect, rect.left, rect.top, rect.right, rect.bottom);
	CvvImage cimg;
	cimg.CopyOf(img);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);	
}

void CTeenVisionView::OnButtonStart() 
{		
	head->headtrack=true;
}

void CTeenVisionView::OnButtonExit() 
{
	head->headtrack= false;
}

void CTeenVisionView::OnButtonConnect() 
{	
	m_edit_ComPort=GetDlgItemInt(IDC_EDIT_ComPort); 
	m_edit_RemoteComPort=GetDlgItemInt(IDC_EDIT_RemoteComPort);
	
	behavior.OpenPort(m_edit_RemoteComPort,B_Baudrate);
	
	head->serial.Open(m_edit_ComPort,H_Baudrate);
			
	UpdateData(0);
}

void CTeenVisionView::OnButtonDisconnect() 
{	
	head->serial.Close();
	moved = false;
	behavior.Set_Robot();
	behavior.ClosePort();	

}

void CTeenVisionView::OnButtonMoved() 
{
	moved =true;	
}

void CTeenVisionView::OnButtonForward() 
{
	behavior.Forward();	
	
}

void CTeenVisionView::OnButtonBackward() 
{
	behavior.Backward();
	
}

void CTeenVisionView::OnButtonShiftLeft() 
{
	behavior.Shift_Left();
	behavior.Shift_Left();
	behavior.Shift_Left();
}

void CTeenVisionView::OnButtonShiftRight() 
{
	behavior.Shift_Right();	
	behavior.Shift_Right();	
	behavior.Shift_Right();	
}

void CTeenVisionView::OnButton_FallDownLeft() 
{
	behavior.FallDown_Left();
}

void CTeenVisionView::OnButton_FallDownRight() 
{
	behavior.FallDown_Right();
}

void CTeenVisionView::OnButtonFastWalk() 
{
	behavior.Fast_Walk();
}

void CTeenVisionView::OnButtonWalk() 
{
	behavior.Walk();
}

void CTeenVisionView::OnButtonStartStop() 
{
	behavior.Set_Robot();	
}

void CTeenVisionView::OnButtonTurnOff() 
{	moved=false;
	behavior.Turn_Off();
}

void CTeenVisionView::OnChangeEditComPort() 
{	
	m_edit_ComPort=GetDlgItemInt(IDC_EDIT_ComPort);
	UpdateData(0);
}

void CTeenVisionView::OnChangeEditRemoteComPort() 
{
	m_edit_RemoteComPort=GetDlgItemInt(IDC_EDIT_RemoteComPort);
	UpdateData(0);
}


