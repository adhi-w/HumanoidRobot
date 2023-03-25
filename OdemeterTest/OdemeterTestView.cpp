// OdemeterTestView.cpp : implementation of the COdemeterTestView class
//

#include "stdafx.h"
#include "OdemeterTest.h"

#include "OdemeterTestDoc.h"
#include "OdemeterTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COdemeterTestView

IMPLEMENT_DYNCREATE(COdemeterTestView, CFormView)

BEGIN_MESSAGE_MAP(COdemeterTestView, CFormView)
	//{{AFX_MSG_MAP(COdemeterTestView)
	ON_BN_CLICKED(IDC_BUTTON1, OnBtn_Forward)
	ON_BN_CLICKED(IDC_BUTTON4, OnBtn_Backward)
	ON_BN_CLICKED(IDC_BUTTON2, OnBtn_LeftShift)
	ON_BN_CLICKED(IDC_BUTTON3, OnBtn_RightShift)
	ON_BN_CLICKED(IDC_BUTTON5, OnBtn_Stop)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON7, OnBtn_Step)
	ON_BN_CLICKED(IDC_BUTTON6, OnBtn_Init)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdemeterTestView construction/destruction

COdemeterTestView::COdemeterTestView()
	: CFormView(COdemeterTestView::IDD)
{
	//{{AFX_DATA_INIT(COdemeterTestView)
	m_edit_odemter_x = 0.0;
	m_edit_odemter_y = 0.0;
	m_edit_odemeter_x_unit = 0.0;
	m_edit_odemeter_y_unit = 0.0;
	m_edit_compass = 0.0;
	//}}AFX_DATA_INIT
	// TODO: add construction code here

	pBodyControl = new CBodyControl();
	pHeadControl = new CHeadControl(COM_PORT, BUNDRATE);

}

COdemeterTestView::~COdemeterTestView()
{
	delete pBodyControl;
	delete pHeadControl;
}

void COdemeterTestView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COdemeterTestView)
	DDX_Text(pDX, IDC_EDIT1, m_edit_odemter_x);
	DDX_Text(pDX, IDC_EDIT2, m_edit_odemter_y);
	DDX_Text(pDX, IDC_EDIT3, m_edit_odemeter_x_unit);
	DDX_Text(pDX, IDC_EDIT4, m_edit_odemeter_y_unit);
	DDX_Text(pDX, IDC_EDIT5, m_edit_compass);
	//}}AFX_DATA_MAP
}

BOOL COdemeterTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void COdemeterTestView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	
	Motion_Type = 0;
	SetTimer(1, 10, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// COdemeterTestView printing

BOOL COdemeterTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COdemeterTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COdemeterTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void COdemeterTestView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// COdemeterTestView diagnostics

#ifdef _DEBUG
void COdemeterTestView::AssertValid() const
{
	CFormView::AssertValid();
}

void COdemeterTestView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

COdemeterTestDoc* COdemeterTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COdemeterTestDoc)));
	return (COdemeterTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COdemeterTestView message handlers

void COdemeterTestView::OnBtn_Forward() 
{
	// TODO: Add your control notification handler code here
	Motion_Type = 1;
}

void COdemeterTestView::OnBtn_Backward() 
{
	// TODO: Add your control notification handler code here
	Motion_Type = 2;
}

void COdemeterTestView::OnBtn_LeftShift() 
{
	// TODO: Add your control notification handler code here
	Motion_Type = 3;
}

void COdemeterTestView::OnBtn_RightShift() 
{
	// TODO: Add your control notification handler code here
	Motion_Type = 4;
}

void COdemeterTestView::OnBtn_Stop() 
{
	// TODO: Add your control notification handler code here
	Motion_Type = 0;
}

void COdemeterTestView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 1)
	{
		if(Motion_Type == 0)
		{
			pBodyControl->Stop();
		}
		else if(Motion_Type == 1)
		{
			pBodyControl->Forward(70);
		}
		else if(Motion_Type == 2)
		{
			pBodyControl->Backward(50);
		}
		else if(Motion_Type == 3)
		{
			pBodyControl->LeftShift(50);
		}
		else if(Motion_Type == 4)
		{
			pBodyControl->RightShift(50);
		}
		else if(Motion_Type == 5)
		{
			pBodyControl->MultMove(0,0,0);
		}
		pBodyControl->odemeter_measure();

		m_edit_odemeter_x_unit = pBodyControl->odemter_x_unit;
		m_edit_odemeter_y_unit = pBodyControl->odemter_y_unit;

		m_edit_odemter_x = pBodyControl->odemter_x;
		m_edit_odemter_y = pBodyControl->odemter_y;

		m_edit_compass = pBodyControl->compass_show;
	}

	
	UpdateData(FALSE);

	CFormView::OnTimer(nIDEvent);
}

void COdemeterTestView::OnBtn_Step() 
{
	// TODO: Add your control notification handler code here
	Motion_Type = 5;	
}

void COdemeterTestView::OnBtn_Init() 
{
	// TODO: Add your control notification handler code here
	pBodyControl->odemter_x = 0.0;	
	pBodyControl->odemter_y = 0.0;	
}
