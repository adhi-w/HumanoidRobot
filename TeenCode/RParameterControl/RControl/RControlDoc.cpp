// RControlDoc.cpp : implementation of the CRControlDoc class
//

#include "stdafx.h"
#include "RControl.h"

#include "RControlDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRControlDoc

IMPLEMENT_DYNCREATE(CRControlDoc, CDocument)

BEGIN_MESSAGE_MAP(CRControlDoc, CDocument)
END_MESSAGE_MAP()


// CRControlDoc construction/destruction

CRControlDoc::CRControlDoc()
{
	// TODO: add one-time construction code here

}

CRControlDoc::~CRControlDoc()
{
}

BOOL CRControlDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CRControlDoc serialization

void CRControlDoc::Serialize(CArchive& ar)
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


// CRControlDoc diagnostics

#ifdef _DEBUG
void CRControlDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRControlDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRControlDoc commands
