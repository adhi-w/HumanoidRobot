// OdemeterTestDoc.cpp : implementation of the COdemeterTestDoc class
//

#include "stdafx.h"
#include "OdemeterTest.h"

#include "OdemeterTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COdemeterTestDoc

IMPLEMENT_DYNCREATE(COdemeterTestDoc, CDocument)

BEGIN_MESSAGE_MAP(COdemeterTestDoc, CDocument)
	//{{AFX_MSG_MAP(COdemeterTestDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COdemeterTestDoc construction/destruction

COdemeterTestDoc::COdemeterTestDoc()
{
	// TODO: add one-time construction code here

}

COdemeterTestDoc::~COdemeterTestDoc()
{
}

BOOL COdemeterTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// COdemeterTestDoc serialization

void COdemeterTestDoc::Serialize(CArchive& ar)
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
// COdemeterTestDoc diagnostics

#ifdef _DEBUG
void COdemeterTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COdemeterTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COdemeterTestDoc commands
