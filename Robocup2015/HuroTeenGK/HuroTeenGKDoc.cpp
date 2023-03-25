// HuroTeenGKDoc.cpp : implementation of the CHuroTeenGKDoc class
//

#include "stdafx.h"
#include "HuroTeenGK.h"

#include "HuroTeenGKDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHuroTeenGKDoc

IMPLEMENT_DYNCREATE(CHuroTeenGKDoc, CDocument)

BEGIN_MESSAGE_MAP(CHuroTeenGKDoc, CDocument)
	//{{AFX_MSG_MAP(CHuroTeenGKDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHuroTeenGKDoc construction/destruction

CHuroTeenGKDoc::CHuroTeenGKDoc()
{
	// TODO: add one-time construction code here

}

CHuroTeenGKDoc::~CHuroTeenGKDoc()
{
}

BOOL CHuroTeenGKDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CHuroTeenGKDoc serialization

void CHuroTeenGKDoc::Serialize(CArchive& ar)
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
// CHuroTeenGKDoc diagnostics

#ifdef _DEBUG
void CHuroTeenGKDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHuroTeenGKDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHuroTeenGKDoc commands
