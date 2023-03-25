// TeenCalibrateDoc.cpp : implementation of the CTeenCalibrateDoc class
//

#include "stdafx.h"
#include "TeenCalibrate.h"

#include "TeenCalibrateDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTeenCalibrateDoc

IMPLEMENT_DYNCREATE(CTeenCalibrateDoc, CDocument)

BEGIN_MESSAGE_MAP(CTeenCalibrateDoc, CDocument)
END_MESSAGE_MAP()


// CTeenCalibrateDoc construction/destruction

CTeenCalibrateDoc::CTeenCalibrateDoc()
{
	// TODO: add one-time construction code here

}

CTeenCalibrateDoc::~CTeenCalibrateDoc()
{
}

BOOL CTeenCalibrateDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CTeenCalibrateDoc serialization

void CTeenCalibrateDoc::Serialize(CArchive& ar)
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


// CTeenCalibrateDoc diagnostics

#ifdef _DEBUG
void CTeenCalibrateDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTeenCalibrateDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CTeenCalibrateDoc commands
