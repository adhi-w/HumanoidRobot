// T-visionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T-vision.h"
#include "T-visionDlg.h"
#include <highgui.h> 
#include <cxcore.h> 
#include <cv.h> 
#include <iostream> 
#include <math.h> 
#include <stdlib.h>
#using <System.dll>
#include "Tracking.h"
#include "Cam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTvisionDlg dialog

CTvisionDlg::CTvisionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTvisionDlg::IDD, pParent)
	, m_edit_Hmin(0)
	, m_edit_Smin(0)
	, m_edit_Vmin(0)
	, m_edit_Hmax(0)
	, m_edit_Smax(0)
	, m_edit_Vmax(0)
	, m_sendData(_T(""))
	, m_edit_ComPort(4)
	, m_edit_RemoteComPort(7)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTvisionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Hmin, m_slider_Hmin);
	DDX_Control(pDX, IDC_Smin, m_slider_Smin);
	DDX_Control(pDX, IDC_Vmin, m_slider_Vmin);
	DDX_Control(pDX, IDC_Hmax, m_slider_Hmax);
	DDX_Control(pDX, IDC_Smax, m_slider_Smax);
	DDX_Control(pDX, IDC_Vmax, m_slider_Vmax);
	DDX_Text(pDX, IDC_EDIT_Hmin, m_edit_Hmin);
	DDX_Text(pDX, IDC_EDIT_Smin, m_edit_Smin);
	DDX_Text(pDX, IDC_EDIT_Vmin, m_edit_Vmin);
	DDX_Text(pDX, IDC_EDIT_Hmax, m_edit_Hmax);
	DDX_Text(pDX, IDC_EDIT_Smax, m_edit_Smax);
	DDX_Text(pDX, IDC_EDIT_Vmax, m_edit_Vmax);
	DDX_Text(pDX, IDC_SendData, m_sendData);
	DDX_Text(pDX, IDC_EDIT_ComPort, m_edit_ComPort);
	DDX_Text(pDX, IDC_EDIT_RemoteComPort, m_edit_RemoteComPort);
}

BEGIN_MESSAGE_MAP(CTvisionDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDSTART, &CTvisionDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDEXIT, &CTvisionDlg::OnBnClickedExit)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_Hmin, &CTvisionDlg::OnNMReleasedcaptureHmin)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_Smin, &CTvisionDlg::OnNMReleasedcaptureSmin)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_Vmin, &CTvisionDlg::OnNMReleasedcaptureVmin)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_Hmax, &CTvisionDlg::OnNMReleasedcaptureHmax)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_Smax, &CTvisionDlg::OnNMReleasedcaptureSmax)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_Vmax, &CTvisionDlg::OnNMReleasedcaptureVmax)
	ON_EN_CHANGE(IDC_EDIT_Hmin, &CTvisionDlg::OnEnChangeEditHmin)
	ON_EN_CHANGE(IDC_EDIT_Smin, &CTvisionDlg::OnEnChangeEditSmin)
	ON_EN_CHANGE(IDC_EDIT_Vmin, &CTvisionDlg::OnEnChangeEditVmin)
	ON_EN_CHANGE(IDC_EDIT_Hmax, &CTvisionDlg::OnEnChangeEditHmax)
	ON_EN_CHANGE(IDC_EDIT_Smax, &CTvisionDlg::OnEnChangeEditSmax)
	ON_EN_CHANGE(IDC_EDIT_Vmax, &CTvisionDlg::OnEnChangeEditVmax)
	ON_BN_CLICKED(IDC_Orange, &CTvisionDlg::OnBnClickedOrange)
	ON_BN_CLICKED(IDC_Cyan, &CTvisionDlg::OnBnClickedCyan)
	ON_BN_CLICKED(IDC_Magenta, &CTvisionDlg::OnBnClickedMagenta)
	ON_BN_CLICKED(IDC_Yellow, &CTvisionDlg::OnBnClickedYellow)
	ON_BN_CLICKED(IDC_Connect, &CTvisionDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_Disconnect, &CTvisionDlg::OnBnClickedDisconnect)
	ON_BN_CLICKED(IDC_BUTTON1, &CTvisionDlg::OnButtonForward)
	ON_BN_CLICKED(IDC_BUTTON3, &CTvisionDlg::OnButtonShiftLeft)
	ON_BN_CLICKED(IDC_BUTTON4, &CTvisionDlg::OnButtonShiftRight)
	ON_BN_CLICKED(IDC_BUTTON2, &CTvisionDlg::OnButtonBackward)
	ON_BN_CLICKED(IDC_BUTTON5, &CTvisionDlg::OnButtonFastWalk)
	ON_BN_CLICKED(IDC_BUTTON6, &CTvisionDlg::OnButtonWalk)
	ON_BN_CLICKED(IDC_BUTTON7, &CTvisionDlg::OnButtonStartStop)
	ON_EN_CHANGE(IDC_EDIT_RemoteComPort, &CTvisionDlg::OnEnChangeEditRemoteComport)
	ON_EN_CHANGE(IDC_EDIT_ComPort, &CTvisionDlg::OnEnChangeEditComport)
	ON_BN_CLICKED(IDSTART2, &CTvisionDlg::OnButtonMoved)
	ON_BN_CLICKED(IDSTART3, &CTvisionDlg::OnButtonUnmoved)
	ON_BN_CLICKED(IDC_BUTTON8, &CTvisionDlg::OnButtonTurnOff)
END_MESSAGE_MAP()


// CTvisionDlg message handlers

BOOL CTvisionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	UpdateData(1);
	m_slider_Hmin.SetRange(0,255);
	m_slider_Smin.SetRange(0,255);
	m_slider_Vmin.SetRange(0,255);
	m_slider_Hmax.SetRange(0,255);
	m_slider_Smax.SetRange(0,255);
	m_slider_Vmax.SetRange(0,255);
	UpdateData(0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTvisionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTvisionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTvisionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*
    Orange  0-22
    Yellow 22- 38
    Green 38-75
    Blue 75-130
    Violet 130-160
    Red 160-179
*/

//-------------White Detect------------
//int Hmin_o=0, Smin_o=98, Vmin_o=0;		//HSL
//int Hmax_o=97, Smax_o=255, Vmax_o=59;	

//int Hmin_o=0, Smin_o=147, Vmin_o=166;		//HSV
//int Hmax_o=22, Smax_o=255, Vmax_o=255;	

//int Hmin_o=0, Smin_o=0, Vmin_o=0;		 //RGB
//int Hmax_o=191, Smax_o=187, Vmax_o=160;
//-------------------------------------

//int Hmin_o=72, Smin_o=134, Vmin_o=0;		
//int Hmax_o=104, Smax_o=255, Vmax_o=59;

int Hmin_o=0, Smin_o=129, Vmin_o=111;		//Orange
int Hmax_o=22, Smax_o=255, Vmax_o=255;	

//int Hmin_o=49, Smin_o=38, Vmin_o=76;	//Green	
//int Hmax_o=114, Smax_o=255, Vmax_o=255;

int Hmin_y=25, Smin_y=129, Vmin_y=88;
int Hmax_y=63, Smax_y=255, Vmax_y=255;

int Hmin_m=121, Smin_m=87, Vmin_m=92;		//127, 75, 93
int Hmax_m=181, Smax_m=255, Vmax_m=255;		//205

int Hmin_c=45, Smin_c=121, Vmin_c=125;
int Hmax_c=137, Smax_c=255, Vmax_c=255;

int h_o=Hmin_o,  s_o=Smin_o, v_o=Vmin_o;
int hm_o=Hmax_o, sm_o=Smax_o, vm_o=Vmax_o;

int h_y=Hmin_y , s_y=Smin_y, v_y=Vmin_y;
int hm_y=Hmax_y, sm_y=Smax_y, vm_y=Vmax_y;

int h_m=Hmin_m , s_m=Smin_m, v_m=Vmin_m;
int hm_m=Hmax_m, sm_m=Smax_m, vm_m=Vmax_m;

int h_c=Hmin_c , s_c=Smin_c, v_c=Vmin_c;
int hm_c=Hmax_c, sm_c=Smax_c, vm_c=Vmax_c;


CvCapture *capture;
bool tutup, koneksi, moved;
int flag=0, tanda=0;

void CTvisionDlg::OnBnClickedStart()
{	
	// TODO: Add your control notification handler code here

	cvNamedWindow( "camera");	
	cvNamedWindow( "thresh");

	HWND hWnd1 = (HWND) cvGetWindowHandle("camera");
	HWND hParent1 = ::GetParent(hWnd1);
	::SetParent(hWnd1, GetDlgItem(IDC_pic_capture)->m_hWnd);
	::ShowWindow(hParent1, SW_HIDE);

	HWND hWnd2 = (HWND) cvGetWindowHandle("thresh");
	HWND hParent2 = ::GetParent(hWnd2);
	::SetParent(hWnd2, GetDlgItem(IDC_pic_thresh)->m_hWnd);
	::ShowWindow(hParent2, SW_HIDE);

	capture=cvCaptureFromCAM(0);
	cvResizeWindow("camera",480,320);
	
	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,480);//160);320
	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,320);//120);240

	while(!tutup){
	//if( !cvGrabFrame( capture )) break;
	IplImage* streaming =  cvRetrieveFrame(capture);	//cvLoadImage("Picture 197.jpg", CV_LOAD_IMAGE_COLOR);	//
	cvFlip(streaming,streaming, 1);
	streaming->origin=0;

	IplImage *imgThresh_o;
	IplImage *imgThresh_y;
	IplImage *imgThresh_m;
	IplImage *imgThresh_c;

	switch(flag)
	{
	case 1:		h_o=GetDlgItemInt(IDC_EDIT_Hmin);	hm_o=GetDlgItemInt(IDC_EDIT_Hmax);
				s_o=GetDlgItemInt(IDC_EDIT_Smin);	sm_o=GetDlgItemInt(IDC_EDIT_Smax);
				v_o=GetDlgItemInt(IDC_EDIT_Vmin);	vm_o=GetDlgItemInt(IDC_EDIT_Vmax);	break;
	case 2:		h_y=GetDlgItemInt(IDC_EDIT_Hmin);	hm_y=GetDlgItemInt(IDC_EDIT_Hmax);
				s_y=GetDlgItemInt(IDC_EDIT_Smin);	sm_y=GetDlgItemInt(IDC_EDIT_Smax);
				v_y=GetDlgItemInt(IDC_EDIT_Vmin);	vm_y=GetDlgItemInt(IDC_EDIT_Vmax);	break;
	case 3:		h_m=GetDlgItemInt(IDC_EDIT_Hmin);	hm_m=GetDlgItemInt(IDC_EDIT_Hmax);
				s_m=GetDlgItemInt(IDC_EDIT_Smin);	sm_m=GetDlgItemInt(IDC_EDIT_Smax);
				v_m=GetDlgItemInt(IDC_EDIT_Vmin);	vm_m=GetDlgItemInt(IDC_EDIT_Vmax);	break;
	case 4:		h_c=GetDlgItemInt(IDC_EDIT_Hmin);	hm_c=GetDlgItemInt(IDC_EDIT_Hmax);
				s_c=GetDlgItemInt(IDC_EDIT_Smin);	sm_c=GetDlgItemInt(IDC_EDIT_Smax);
				v_c=GetDlgItemInt(IDC_EDIT_Vmin);	vm_c=GetDlgItemInt(IDC_EDIT_Vmax);	break;
	
	}
	
	//imgThresh_o=detect(streaming);
	imgThresh_o = ThreshImage( streaming,hm_o,sm_o,vm_o, h_o,s_o,v_o );
	tracking(imgThresh_o, streaming,'O');
	//circle(imgThresh_o, streaming);
	int kx= x_o;	int ky=y_o;
	if(kx!=0 || ky!=0)findBall=true;
	else findBall=false;
	/*imgThresh_y = ThreshImage( streaming,hm_y,sm_y,vm_y, h_y,s_y,v_y );
	tracking(imgThresh_y, streaming,'Y');
	
	imgThresh_m = ThreshImage( streaming,hm_m,sm_m,vm_m, h_m,s_m,v_m );
	tracking(imgThresh_m, streaming,'M');
	
	imgThresh_c = ThreshImage( streaming,hm_c,sm_c,vm_c, h_c,s_c,v_c );
	tracking(imgThresh_c, streaming,'C'); */

	if(!koneksi)
		{	UpdateData(1);
			m_sendData.Format(L"Koneksi Terputus....Silahkan Tekan Tombol Connect");
			//add('o'); add(13);
			UpdateData(0);		
		}
	else 
		{	UpdateData(1);
			m_sendData.Format(L"O = %d %d,  %d %d, %d %d   %d, %d",kx,ky, hor, ver, dhor, dver,PresentHorPos,PresentVerPos);  //kirim('O',kx,ky); add(13);
			UpdateData(0);
			cam_pos(kx,ky,480,320);//320,240
			
			if(moved)
			{		
				behavior.TrackBall(PresentHorPos,PresentVerPos,512,350);
			}			
		}
			
	cvShowImage( "camera", streaming);

	switch(tanda)
	{
		case 1: cvShowImage( "thresh", imgThresh_o );  break; // Hasil streaming setelah diFilter	
		case 2: cvShowImage( "thresh", imgThresh_y );  break;
		case 3:	cvShowImage( "thresh", imgThresh_m );  break;	
		case 4: cvShowImage( "thresh", imgThresh_c );  break;	
	}

	cvWaitKey(5);
	cvReleaseImage( &imgThresh_o );
	/*cvReleaseImage( &imgThresh_y );
	cvReleaseImage( &imgThresh_m );
	cvReleaseImage( &imgThresh_c );*/
	}
	cvReleaseCapture( &capture );
}

void CTvisionDlg::OnBnClickedExit()
{
	// TODO: Add your control notification handler code here
	tutup=true;
	cvReleaseCapture(&capture);
	behavior.Set_Robot();	//set standby
	serial.Close();
	behavior.ClosePort();
	OnCancel();
}

void CTvisionDlg::OnBnClickedConnect()
{
	koneksi=true;
	m_edit_ComPort=GetDlgItemInt(IDC_EDIT_ComPort); 
	m_edit_RemoteComPort=GetDlgItemInt(IDC_EDIT_RemoteComPort);
	serial.Open(m_edit_ComPort,1000000);
	behavior.OpenPort(m_edit_RemoteComPort,115200);
	//CamSet(true);

	UpdateData(0);
}

void CTvisionDlg::OnBnClickedDisconnect()
{	behavior.Set_Robot();	//set standby
	serial.Close();
	behavior.ClosePort();
	koneksi=false;
}

void CTvisionDlg::OnButtonMoved()
{
	moved=true;
}

void CTvisionDlg::OnButtonUnmoved()
{
	moved=false;
}
void CTvisionDlg::OnNMReleasedcaptureHmin(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	m_edit_Hmin=m_slider_Hmin.GetPos();
	UpdateData(0);
	*pResult = 0;
}

void CTvisionDlg::OnNMReleasedcaptureSmin(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	m_edit_Smin=m_slider_Smin.GetPos();
	UpdateData(0);
	*pResult = 0;
}

void CTvisionDlg::OnNMReleasedcaptureVmin(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	m_edit_Vmin=m_slider_Vmin.GetPos();
	UpdateData(0);
	*pResult = 0;
}

void CTvisionDlg::OnNMReleasedcaptureHmax(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	m_edit_Hmax=m_slider_Hmax.GetPos();
	UpdateData(0);
	*pResult = 0;
}

void CTvisionDlg::OnNMReleasedcaptureSmax(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	m_edit_Smax=m_slider_Smax.GetPos();
	UpdateData(0);
	*pResult = 0;
}

void CTvisionDlg::OnNMReleasedcaptureVmax(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	UpdateData(1);
	m_edit_Vmax=m_slider_Vmax.GetPos();
	UpdateData(0);
	*pResult = 0;
}

void CTvisionDlg::OnEnChangeEditHmin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	int k=GetDlgItemInt(IDC_EDIT_Hmin);
	UpdateData(1);
	m_slider_Hmin.SetPos(k);
	UpdateData(0);
}

void CTvisionDlg::OnEnChangeEditSmin()
{
	int k=GetDlgItemInt(IDC_EDIT_Smin);
	UpdateData(1);
	m_slider_Smin.SetPos(k);
	UpdateData(0);
}

void CTvisionDlg::OnEnChangeEditVmin()
{
	int k=GetDlgItemInt(IDC_EDIT_Vmin);
	UpdateData(1);
	m_slider_Vmin.SetPos(k);
	UpdateData(0);
}

void CTvisionDlg::OnEnChangeEditHmax()
{
	int k=GetDlgItemInt(IDC_EDIT_Hmax);
	UpdateData(1);
	m_slider_Hmax.SetPos(k);
	UpdateData(0);
}

void CTvisionDlg::OnEnChangeEditSmax()
{
	int k=GetDlgItemInt(IDC_EDIT_Smax);
	UpdateData(1);
	m_slider_Smax.SetPos(k);
	UpdateData(0);
}

void CTvisionDlg::OnEnChangeEditVmax()
{
	int k=GetDlgItemInt(IDC_EDIT_Vmax);
	UpdateData(1);
	m_slider_Vmax.SetPos(k);
	UpdateData(0);
}

void CTvisionDlg::OnBnClickedOrange()
{
	// TODO: Add your control notification handler code here
	flag=1; tanda=1;
	UpdateData(1);	
	m_slider_Hmin.SetPos(h_o); m_edit_Hmin=h_o;
	m_slider_Smin.SetPos(s_o); m_edit_Smin=s_o;
	m_slider_Vmin.SetPos(v_o); m_edit_Vmin=v_o;
	m_slider_Hmax.SetPos(hm_o); m_edit_Hmax=hm_o;
	m_slider_Smax.SetPos(sm_o); m_edit_Smax=sm_o;
	m_slider_Vmax.SetPos(vm_o); m_edit_Vmax=vm_o;
	UpdateData(0);
}

void CTvisionDlg::OnBnClickedYellow()
{
	// TODO: Add your control notification handler code here
	flag=2; tanda=2; 
	UpdateData(1);	
	m_slider_Hmin.SetPos(h_y); m_edit_Hmin=h_y;
	m_slider_Smin.SetPos(s_y); m_edit_Smin=s_y;
	m_slider_Vmin.SetPos(v_y); m_edit_Vmin=v_y;
	m_slider_Hmax.SetPos(hm_y); m_edit_Hmax=hm_y;
	m_slider_Smax.SetPos(sm_y); m_edit_Smax=sm_y;
	m_slider_Vmax.SetPos(vm_y); m_edit_Vmax=vm_y;
	UpdateData(0);
}


void CTvisionDlg::OnBnClickedMagenta()
{
	// TODO: Add your control notification handler code here
	flag=3; tanda=3; 
	UpdateData(1);	
	m_slider_Hmin.SetPos(h_m); m_edit_Hmin=h_m;
	m_slider_Smin.SetPos(s_m); m_edit_Smin=s_m;
	m_slider_Vmin.SetPos(v_m); m_edit_Vmin=v_m;
	m_slider_Hmax.SetPos(hm_m); m_edit_Hmax=hm_m;
	m_slider_Smax.SetPos(sm_m); m_edit_Smax=sm_m;
	m_slider_Vmax.SetPos(vm_m); m_edit_Vmax=vm_m;
	UpdateData(0);
}

void CTvisionDlg::OnBnClickedCyan()
{
	// TODO: Add your control notification handler code here
	flag=4; tanda=4; UpdateData(1);	
	m_slider_Hmin.SetPos(h_c); m_edit_Hmin=h_c;
	m_slider_Smin.SetPos(s_c); m_edit_Smin=s_c;
	m_slider_Vmin.SetPos(v_c); m_edit_Vmin=v_c;
	m_slider_Hmax.SetPos(hm_c); m_edit_Hmax=hm_c;
	m_slider_Smax.SetPos(sm_c); m_edit_Smax=sm_c;
	m_slider_Vmax.SetPos(vm_c); m_edit_Vmax=vm_c;
	UpdateData(0);
}

void CTvisionDlg::OnButtonForward()
{	behavior.Forward();	
}

void CTvisionDlg::OnButtonBackward()
{	behavior.Backward();	
}

void CTvisionDlg::OnButtonShiftLeft()
{	behavior.Shift_Left();
	behavior.Shift_Left();
	behavior.Shift_Left();
}

void CTvisionDlg::OnButtonShiftRight()
{	behavior.Shift_Right();	
	behavior.Shift_Right();	
	behavior.Shift_Right();	
}

void CTvisionDlg::OnButtonFastWalk()
{	behavior.Fast_Walk();	
}

void CTvisionDlg::OnButtonWalk()
{	behavior.Walk();	
}

void CTvisionDlg::OnButtonStartStop()
{	behavior.Set_Robot();	
}

void CTvisionDlg::OnButtonTurnOff()
{	behavior.Turn_Off();	
}
void CTvisionDlg::OnEnChangeEditComport()
{
	m_edit_ComPort=GetDlgItemInt(IDC_EDIT_ComPort);
	UpdateData(0);
}

void CTvisionDlg::OnEnChangeEditRemoteComport()
{
	m_edit_RemoteComPort=GetDlgItemInt(IDC_EDIT_RemoteComPort);
	UpdateData(0);
}


