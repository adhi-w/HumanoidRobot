// JoyControlRobotDoc.cpp : implementation of the CJoyControlRobotDoc class
//

#include "stdafx.h"
#include "JoyControlRobot.h"

#include "JoyControlRobotDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJoyControlRobotDoc

IMPLEMENT_DYNCREATE(CJoyControlRobotDoc, CDocument)

BEGIN_MESSAGE_MAP(CJoyControlRobotDoc, CDocument)
	//{{AFX_MSG_MAP(CJoyControlRobotDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJoyControlRobotDoc construction/destruction

CJoyControlRobotDoc::CJoyControlRobotDoc()
{
	// TODO: add one-time construction code here

}

CJoyControlRobotDoc::~CJoyControlRobotDoc()
{
}

BOOL CJoyControlRobotDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CJoyControlRobotDoc serialization

void CJoyControlRobotDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CJoyControlRobotDoc diagnostics

#ifdef _DEBUG
void CJoyControlRobotDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CJoyControlRobotDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CJoyControlRobotDoc commands
