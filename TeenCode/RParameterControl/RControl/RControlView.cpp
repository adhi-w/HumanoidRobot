// RControlView.cpp : implementation of the CRControlView class
//

#include "stdafx.h"
#include "math.h"
#include "RControl.h"

#include "RControlDoc.h"
#include "RControlView.h"
#include "Serial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRControlView

IMPLEMENT_DYNCREATE(CRControlView, CFormView)

BEGIN_MESSAGE_MAP(CRControlView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CRControlView::OnButtonLeftHipMin)
	ON_BN_CLICKED(IDC_BUTTON2, &CRControlView::OnButtonLeftHipPlus)
	ON_BN_CLICKED(IDC_BUTTON3, &CRControlView::OnButtonLeftThighMin)
	ON_BN_CLICKED(IDC_BUTTON4, &CRControlView::OnButtonLeftThighPlus)
	ON_BN_CLICKED(IDC_BUTTON5, &CRControlView::OnButtonLeftKneeMin)
	ON_BN_CLICKED(IDC_BUTTON6, &CRControlView::OnButtonLeftKneePlus)
	ON_BN_CLICKED(IDC_BUTTON7, &CRControlView::OnButtonLeftAnkleRollMin)
	ON_BN_CLICKED(IDC_BUTTON8, &CRControlView::OnButtonLeftAnkleRollPlus)
	ON_BN_CLICKED(IDC_BUTTON9, &CRControlView::OnButtonLeftAnklePitchMin)
	ON_BN_CLICKED(IDC_BUTTON10, &CRControlView::OnButtonLeftAnklePitchPlus)
	ON_BN_CLICKED(IDC_BUTTON11, &CRControlView::OnButtonRightHipMin)
	ON_BN_CLICKED(IDC_BUTTON12, &CRControlView::OnButtonRightHipPlus)
	ON_BN_CLICKED(IDC_BUTTON13, &CRControlView::OnButtonRightThighMin)
	ON_BN_CLICKED(IDC_BUTTON14, &CRControlView::OnButtonRightThighPlus)
	ON_BN_CLICKED(IDC_BUTTON15, &CRControlView::OnButtonRightKneeMin)
	ON_BN_CLICKED(IDC_BUTTON16, &CRControlView::OnButtonRightKneePlus)
	ON_BN_CLICKED(IDC_BUTTON17, &CRControlView::OnButtonRightAnkleRollMin)
	ON_BN_CLICKED(IDC_BUTTON18, &CRControlView::OnButtonRightAnkleRollPlus)
	ON_BN_CLICKED(IDC_BUTTON19, &CRControlView::OnButtonRightAnklePitchMin)
	ON_BN_CLICKED(IDC_BUTTON20, &CRControlView::OnButtonRightAnklePitchPlus)
	ON_BN_CLICKED(IDC_BUTTON28, &CRControlView::OnButtonLIPMMin)
	ON_BN_CLICKED(IDC_BUTTON29, &CRControlView::OnButtonLIPMPlus)
	ON_BN_CLICKED(IDC_BUTTON22, &CRControlView::OnButtonForward)
	ON_BN_CLICKED(IDC_BUTTON23, &CRControlView::OnButtonBackward)
	ON_BN_CLICKED(IDC_BUTTON24, &CRControlView::OnButtonRightShift)
	ON_BN_CLICKED(IDC_BUTTON25, &CRControlView::OnButtonLeftShift)
	ON_BN_CLICKED(IDC_BUTTON26, &CRControlView::OnButtonFastWalk)
	ON_BN_CLICKED(IDC_BUTTON27, &CRControlView::OnButtonWalk)
	ON_BN_CLICKED(IDC_BUTTON21, &CRControlView::OnButtonStartStop)
	ON_EN_CHANGE(IDC_EDIT_ComPort, &CRControlView::OnEnChangeEditComport)
//	ON_BN_CLICKED(IDC_BUTTON30, &CRControlView::OnButtonConnect)
ON_BN_CLICKED(IDC_BUTTON30, &CRControlView::OnButtonConnect)
ON_BN_CLICKED(IDC_BUTTON31, &CRControlView::OnButtonDisconnect)
ON_BN_CLICKED(IDC_BUTTON32, &CRControlView::OnButtonSaveEEPROM)
ON_BN_CLICKED(IDC_BUTTON33, &CRControlView::OnButonTurnOff)
END_MESSAGE_MAP()

// CRControlView construction/destruction

CRControlView::CRControlView()
	: CFormView(CRControlView::IDD)
	, m_edit_LeftHip(4)
	, m_edit_LeftThigh(-4)
	, m_edit_LeftKnee(3.5)
	, m_edit_LeftAnkleRoll(4.5)
	, m_edit_LeftAnklePitch(0)
	, m_edit_RightHip(-13)
	, m_edit_RightThigh(0)
	, m_edit_RightKnee(0)
	, m_edit_RightAnkleRoll(13.5)
	, m_edit_RightAnklePitch(0)
	, m_edit_LIPM(13)
	, m_edit_ComPort(7)
{
	// TODO: add construction code here
	
}

CRControlView::~CRControlView()
{	

}

void CRControlView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_edit_LeftHip);
	DDX_Text(pDX, IDC_EDIT2, m_edit_LeftThigh);
	DDX_Text(pDX, IDC_EDIT3, m_edit_LeftKnee);
	DDX_Text(pDX, IDC_EDIT4, m_edit_LeftAnkleRoll);
	DDX_Text(pDX, IDC_EDIT5, m_edit_LeftAnklePitch);
	DDX_Text(pDX, IDC_EDIT6, m_edit_RightHip);
	DDX_Text(pDX, IDC_EDIT7, m_edit_RightThigh);
	DDX_Text(pDX, IDC_EDIT8, m_edit_RightKnee);
	DDX_Text(pDX, IDC_EDIT9, m_edit_RightAnkleRoll);
	DDX_Text(pDX, IDC_EDIT10, m_edit_RightAnklePitch);
	DDX_Text(pDX, IDC_EDIT11, m_edit_LIPM);
	DDX_Text(pDX, IDC_EDIT_ComPort, m_edit_ComPort);
}

BOOL CRControlView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CRControlView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
}


// CRControlView diagnostics

#ifdef _DEBUG
void CRControlView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRControlView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRControlDoc* CRControlView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRControlDoc)));
	return (CRControlDoc*)m_pDocument;
}
#endif //_DEBUG


// CRControlView message handlers

//------------------LEFT FOOT----------------------------////
void CRControlView::OnButtonLeftHipMin()
{	m_serial.WriteCommByte('I');
	m_edit_LeftHip=m_edit_LeftHip-0.5; UpdateData(0);
}

void CRControlView::OnButtonLeftHipPlus()
{	m_serial.WriteCommByte('Y');
	m_edit_LeftHip=m_edit_LeftHip+0.5; UpdateData(0);
}

void CRControlView::OnButtonLeftThighMin()
{	m_serial.WriteCommByte('J');
	m_edit_LeftThigh=m_edit_LeftThigh-0.5; UpdateData(0);
}

void CRControlView::OnButtonLeftThighPlus()
{	m_serial.WriteCommByte('U');
	m_edit_LeftThigh=m_edit_LeftThigh+0.5; UpdateData(0);
}

void CRControlView::OnButtonLeftKneeMin()
{	m_serial.WriteCommByte(';');
	m_edit_LeftKnee=m_edit_LeftKnee-0.5; UpdateData(0);
}

void CRControlView::OnButtonLeftKneePlus()
{	m_serial.WriteCommByte('L');
	m_edit_LeftKnee=m_edit_LeftKnee+0.5; UpdateData(0);
}

void CRControlView::OnButtonLeftAnkleRollMin()
{	m_serial.WriteCommByte('K');
	m_edit_LeftAnkleRoll=m_edit_LeftAnkleRoll-0.5; UpdateData(0);
}

void CRControlView::OnButtonLeftAnkleRollPlus()
{	m_serial.WriteCommByte('H');
	m_edit_LeftAnkleRoll=m_edit_LeftAnkleRoll+0.5; UpdateData(0);
}

void CRControlView::OnButtonLeftAnklePitchMin()
{	m_serial.WriteCommByte(',');
	m_edit_LeftAnklePitch=m_edit_LeftAnklePitch-0.5; UpdateData(0);
}

void CRControlView::OnButtonLeftAnklePitchPlus()
{	m_serial.WriteCommByte('N');
	m_edit_LeftAnklePitch=m_edit_LeftAnklePitch+0.5; UpdateData(0);
}


///---------------------RIGHT FOOT-----------------------------///

void CRControlView::OnButtonRightHipMin()
{	m_serial.WriteCommByte('E');
	m_edit_RightHip=m_edit_RightHip-0.5; UpdateData(0);
}

void CRControlView::OnButtonRightHipPlus()
{	m_serial.WriteCommByte('Q');
	m_edit_RightHip=m_edit_RightHip+0.5; UpdateData(0);
}

void CRControlView::OnButtonRightThighMin()
{	m_serial.WriteCommByte('S');
	m_edit_RightThigh=m_edit_RightThigh-0.5; UpdateData(0);
}

void CRControlView::OnButtonRightThighPlus()
{	m_serial.WriteCommByte('W');
	m_edit_RightThigh=m_edit_RightThigh+0.5; UpdateData(0);
}

void CRControlView::OnButtonRightKneeMin()
{	m_serial.WriteCommByte('G');
	m_edit_RightKnee=m_edit_RightKnee-0.5; UpdateData(0);
}

void CRControlView::OnButtonRightKneePlus()
{	m_serial.WriteCommByte('F');
	m_edit_RightKnee=m_edit_RightKnee+0.5; UpdateData(0);
}

void CRControlView::OnButtonRightAnkleRollMin()
{	m_serial.WriteCommByte('D');
	m_edit_RightAnkleRoll=m_edit_RightAnkleRoll-0.5; UpdateData(0);
}

void CRControlView::OnButtonRightAnkleRollPlus()
{	m_serial.WriteCommByte('A');
	m_edit_RightAnkleRoll=m_edit_RightAnkleRoll+0.5; UpdateData(0);
}

void CRControlView::OnButtonRightAnklePitchMin()
{	m_serial.WriteCommByte('C');
	m_edit_RightAnklePitch=m_edit_RightAnklePitch-0.5; UpdateData(0);
}

void CRControlView::OnButtonRightAnklePitchPlus()
{	m_serial.WriteCommByte('Z');
	m_edit_RightAnklePitch=m_edit_RightAnklePitch+0.5; UpdateData(0);
}

//-----------------------------LIPM------------------------------//
void CRControlView::OnButtonLIPMMin()
{	m_serial.WriteCommByte('B');
	m_edit_LIPM=m_edit_LIPM-0.25;	UpdateData(0);
}

void CRControlView::OnButtonLIPMPlus()
{	m_serial.WriteCommByte('V');
	m_edit_LIPM=m_edit_LIPM+0.25;	UpdateData(0);
}


//---------------------REMOTE CONTROL-------------------//

void CRControlView::OnButtonForward()
{
	m_serial.WriteCommByte('w');
}

void CRControlView::OnButtonBackward()
{
	m_serial.WriteCommByte('s');
}

void CRControlView::OnButtonRightShift()
{
	m_serial.WriteCommByte('d');
}

void CRControlView::OnButtonLeftShift()
{
	m_serial.WriteCommByte('a');
}

void CRControlView::OnButtonFastWalk()
{
	m_serial.WriteCommByte('p');
}

void CRControlView::OnButtonWalk()
{
	m_serial.WriteCommByte('o');
}

void CRControlView::OnButtonSaveEEPROM()
{
	m_serial.WriteCommByte('/');
}

void CRControlView::OnButonTurnOff()
{
	m_serial.WriteCommByte('P');
}

void CRControlView::OnButtonStartStop()
{	UpdateData(1);
	m_serial.WriteCommByte('0');	UpdateData(0);
}

void CRControlView::OnEnChangeEditComport()
{	
	m_edit_ComPort=GetDlgItemInt(IDC_EDIT_ComPort);
	UpdateData(0);
}

void CRControlView::OnButtonConnect()
{	
	m_edit_ComPort=GetDlgItemInt(IDC_EDIT_ComPort); UpdateData(0);
	m_serial.Open(m_edit_ComPort,115200);
}

void CRControlView::OnButtonDisconnect()
{	
	m_serial.WriteCommByte('0');
	m_serial.Close();
}
