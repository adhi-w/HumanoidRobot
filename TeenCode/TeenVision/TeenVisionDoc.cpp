// TeenVisionDoc.cpp : implementation of the CTeenVisionDoc class
//

#include "stdafx.h"
#include "TeenVision.h"

#include "TeenVisionDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTeenVisionDoc

IMPLEMENT_DYNCREATE(CTeenVisionDoc, CDocument)

BEGIN_MESSAGE_MAP(CTeenVisionDoc, CDocument)
	//{{AFX_MSG_MAP(CTeenVisionDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeenVisionDoc construction/destruction

CTeenVisionDoc::CTeenVisionDoc()
{
	// TODO: add one-time construction code here

}

CTeenVisionDoc::~CTeenVisionDoc()
{
}

BOOL CTeenVisionDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTeenVisionDoc serialization

void CTeenVisionDoc::Serialize(CArchive& ar)
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
// CTeenVisionDoc diagnostics

#ifdef _DEBUG
void CTeenVisionDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTeenVisionDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTeenVisionDoc commands
