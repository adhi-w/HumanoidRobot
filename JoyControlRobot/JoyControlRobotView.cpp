// JoyControlRobotView.cpp : implementation of the CJoyControlRobotView class
//

#include "stdafx.h"
#include "JoyControlRobot.h"

#include "JoyControlRobotDoc.h"
#include "JoyControlRobotView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJoyControlRobotView

IMPLEMENT_DYNCREATE(CJoyControlRobotView, CFormView)

BEGIN_MESSAGE_MAP(CJoyControlRobotView, CFormView)
	//{{AFX_MSG_MAP(CJoyControlRobotView)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_RK, OnButtonRk)
	ON_BN_CLICKED(IDC_BUTTON_LK, OnButtonLk)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_MESSAGE(WM_THREAD1, OnThread1)
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJoyControlRobotView construction/destruction

CJoyControlRobotView::CJoyControlRobotView()
	: CFormView(CJoyControlRobotView::IDD)
{
	//{{AFX_DATA_INIT(CJoyControlRobotView)
	m_port = 0;
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CJoyControlRobotView::~CJoyControlRobotView()
{
	ThreadInfo.thread_enable = 0;
	SetEvent(ThreadInfo.hThread);
}

void CJoyControlRobotView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJoyControlRobotView)
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	//}}AFX_DATA_MAP
}

BOOL CJoyControlRobotView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CJoyControlRobotView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
	ThreadInfo.wnd = this;
	ThreadInfo.pJoyStick = new CJoyStick();
	ThreadInfo.Radius = 0.0; 
	ThreadInfo.Ang = 0.0;
	ThreadInfo.DX = 0; 
	ThreadInfo.DY = 0;
	ThreadInfo.DZ = 0;
	ThreadInfo.button_status = 0;
	ThreadInfo.previous_button_status = 0;
	ThreadInfo.control_enable = 0;
	ThreadInfo.thread_enable = 1;
	ThreadInfo.hThread = CreateEvent( 0, 1, 0, "MyEvent");
	m_Thread = AfxBeginThread(DoThread1, (LPVOID) &ThreadInfo);  

	SetTimer(1,100,NULL);
}

/////////////////////////////////////////////////////////////////////////////
// CJoyControlRobotView printing

BOOL CJoyControlRobotView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CJoyControlRobotView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CJoyControlRobotView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CJoyControlRobotView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here

}

/////////////////////////////////////////////////////////////////////////////
// CJoyControlRobotView diagnostics

#ifdef _DEBUG
void CJoyControlRobotView::AssertValid() const
{
	CFormView::AssertValid();
}

void CJoyControlRobotView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CJoyControlRobotDoc* CJoyControlRobotView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CJoyControlRobotDoc)));
	return (CJoyControlRobotDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CJoyControlRobotView message handlers

void CJoyControlRobotView::OnButtonConnect() 
{
	// TODO: Add your control notification handler code here
	int temp_x;

	UpdateData(1);
	temp_x=m_serial.Open(m_port,115200);
	if(temp_x==0)
		AfxMessageBox("Connect Error!");
	else
		AfxMessageBox("Connect Success");

}

void CJoyControlRobotView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static int couter=0;
	couter++;
	//Msg.Format("計時:%d",couter);

	//Invalidate(0);

	CFormView::OnTimer(nIDEvent);
}

void CJoyControlRobotView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString Str_temp;
	COLORREF color; 
	color=(COLORREF)GetSysColor(COLOR_BTNFACE);

	CBitmap bmp;
	CDC mendc;									//宣告DC型態			
	mendc.CreateCompatibleDC(pDC);				//宣告貼圖區的記憶體(在記憶體中建立"mendc"的裝置環境代碼(Device Context))
	bmp.CreateCompatibleBitmap(pDC, 2500, 1500);//宣告彩色位元圖記憶體
	mendc.SelectObject(&bmp);					//使DC和Bitmap建立關聯，Bitmap的圖像才會在DC裡出現了
	mendc.FillSolidRect(0, 0, 2500, 1500, color);	//畫底色


	mendc.SetTextColor(RGB(0, 0, 0));
//	mendc.TextOut(300,100,Msg);	
	mendc.DrawText(Msg,CRect(300,100,450,300),DT_LEFT|DT_WORDBREAK|DT_EDITCONTROL);

	ThreadInfo.pJoyStick->DrawJoyStick(mendc, 100, 100, Radius, Ang);

	Str_temp.Format("length= %d,   shift= %d,   theta= %d,      button_status= %d",ThreadInfo.DY,ThreadInfo.DX,ThreadInfo.DZ,ThreadInfo.button_status);
	mendc.DrawText(Str_temp,CRect(20,180,190,400),DT_LEFT|DT_WORDBREAK|DT_EDITCONTROL);


	pDC->BitBlt(0, 0, 2500, 1500, &mendc, 0, 0, SRCCOPY);	//將DC的圖像轉移(參數:目的地四端點,來源,來源起始點,貼圖方法)
	ReleaseDC(pDC);
	mendc.DeleteDC();
	bmp.DeleteObject();
}

UINT CJoyControlRobotView::DoThread1(LPVOID pParam)
{
	int cnt = 0; // For Local variable in the Thread
	CWnd *wnd = ((THREAD_INFO *)pParam)->wnd;
	while(  ((THREAD_INFO *)pParam)->thread_enable )//!((THREAD_INFO*)pParam)->b_stop) 
	{
		((THREAD_INFO*)pParam)->pJoyStick->GetPos();
		int dx = ((THREAD_INFO*)pParam)->pJoyStick->DX - 512;
		int dy = ((THREAD_INFO*)pParam)->pJoyStick->DY - 512;
		int dz = ((THREAD_INFO*)pParam)->pJoyStick->DZ - 128;
		int button = ((THREAD_INFO*)pParam)->pJoyStick->button_status;;
		dy=dy*(-1);
		
		if( abs(dx)<10 )
			dx=0;
		if( abs(dy)<10 )
			dy=0;
		if( abs(dz)<3  )
			dz=0;
		
		((THREAD_INFO*)pParam)->DX = int( (float(dx)/512.0)*100 +0.5);
		((THREAD_INFO*)pParam)->DY = int( (float(dy)/512.0)*100 +0.5);
		((THREAD_INFO*)pParam)->DZ = int( (float(dz)/128.0)*100 +0.5);
		((THREAD_INFO*)pParam)->previous_button_status = ((THREAD_INFO*)pParam)->button_status;
		((THREAD_INFO*)pParam)->button_status = button;
		((THREAD_INFO*)pParam)->Radius = sqrt(dx * dx + dy * dy) * 0.1; 
		((THREAD_INFO*)pParam)->Ang = atan2((double)-dy, (double)dx); 

		wnd->PostMessage(WM_THREAD1, 0, 0);
		WaitForSingleObject(((THREAD_INFO*)pParam)->hThread, 200);
	}
	return 0;
}

LRESULT CJoyControlRobotView::OnThread1(WPARAM wParam, LPARAM lParam)
{
	Radius = ThreadInfo.Radius;
	Ang = ThreadInfo.Ang;
	Rotation = ThreadInfo.DZ;


	if(ThreadInfo.previous_button_status == 2 && ThreadInfo.button_status == 0)
		ThreadInfo.control_enable = !ThreadInfo.control_enable;

	if(ThreadInfo.previous_button_status == 1 && ThreadInfo.button_status == 0)
	{
		OnButtonStop();
		ThreadInfo.control_enable =0;
		Msg.Format("In OnButtonStop");
	}
	else if(ThreadInfo.control_enable ==1)
		ControlPacket();
	else if(ThreadInfo.control_enable ==0 && ThreadInfo.previous_button_status == 4 && ThreadInfo.button_status == 0)
	{
		OnButtonLk();
		Msg.Format("Left kick");
	}
	else if(ThreadInfo.control_enable ==0 && ThreadInfo.previous_button_status == 8 && ThreadInfo.button_status == 0)
	{
		OnButtonRk();
		Msg.Format("Right kick");
	}
	else
	{
		Msg.Format("No Packet out");
	}
		

	Invalidate(0);
	return 0;
}

void CJoyControlRobotView::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	unsigned char array[7];
	int i;

	array[0]=254;
	array[1]=48;
	array[2]=128;
	array[3]=1;	//不重要
	array[4]=2;	//不重要
	array[5]=3;	//不重要
	array[6]=255;

	for(i=0;i<7;i++)
	{
		m_serial.WriteCommByte(array[i]);
	}
	
}


void CJoyControlRobotView::ControlPacket()
{
	unsigned char array[7];
	int i;

	array[0]=254;
	array[1]=0;
	array[2]=128;
	array[3]=128+ThreadInfo.DY;	
	array[4]=128+ThreadInfo.DX;	
	array[5]=128+ThreadInfo.DZ;	
	array[6]=255;

	for(i=0;i<7;i++)
	{
		m_serial.WriteCommByte(array[i]);
	}
	Msg.Format("In ControlPacket, length=%d,shift=%d,   theta=%d",array[3],array[4],array[5]);
}


void CJoyControlRobotView::OnButtonRk() 
{
	// TODO: Add your control notification handler code here
	unsigned char array[7];
	int i;

	array[0]=254;
	array[1]=51;
	array[2]=128;
	array[3]=1;	//不重要
	array[4]=2;	//不重要
	array[5]=3;	//不重要
	array[6]=255;

	for(i=0;i<7;i++)
	{
		m_serial.WriteCommByte(array[i]);
	}
		
}

void CJoyControlRobotView::OnButtonLk() 
{
	// TODO: Add your control notification handler code here
	unsigned char array[7];
	int i;

	array[0]=254;
	array[1]=52;
	array[2]=128;
	array[3]=1;	//不重要
	array[4]=2;	//不重要
	array[5]=3;	//不重要
	array[6]=255;

	for(i=0;i<7;i++)
	{
		m_serial.WriteCommByte(array[i]);
	}
			
}
