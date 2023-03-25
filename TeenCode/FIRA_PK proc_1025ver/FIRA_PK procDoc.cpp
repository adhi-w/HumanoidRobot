// FIRA_PK procDoc.cpp : implementation of the CFIRA_PKprocDoc class
//

#include "stdafx.h"
#include "FIRA_PK proc.h"

#include "FIRA_PK procDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFIRA_PKprocDoc

IMPLEMENT_DYNCREATE(CFIRA_PKprocDoc, CDocument)

BEGIN_MESSAGE_MAP(CFIRA_PKprocDoc, CDocument)
	//{{AFX_MSG_MAP(CFIRA_PKprocDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFIRA_PKprocDoc construction/destruction

CFIRA_PKprocDoc::CFIRA_PKprocDoc()
{
	// TODO: add one-time construction code here

}

CFIRA_PKprocDoc::~CFIRA_PKprocDoc()
{
}

BOOL CFIRA_PKprocDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFIRA_PKprocDoc serialization

void CFIRA_PKprocDoc::Serialize(CArchive& ar)
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
// CFIRA_PKprocDoc diagnostics

#ifdef _DEBUG
void CFIRA_PKprocDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFIRA_PKprocDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFIRA_PKprocDoc commands
