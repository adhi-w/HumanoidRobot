// HuroEvolutionSystemDoc.cpp : implementation of the CHuroEvolutionSystemDoc class
//

#include "stdafx.h"
#include "HuroEvolutionSystem.h"

#include "HuroEvolutionSystemDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHuroEvolutionSystemDoc

IMPLEMENT_DYNCREATE(CHuroEvolutionSystemDoc, CDocument)

BEGIN_MESSAGE_MAP(CHuroEvolutionSystemDoc, CDocument)
	//{{AFX_MSG_MAP(CHuroEvolutionSystemDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHuroEvolutionSystemDoc construction/destruction

CHuroEvolutionSystemDoc::CHuroEvolutionSystemDoc()
{
	// TODO: add one-time construction code here

}

CHuroEvolutionSystemDoc::~CHuroEvolutionSystemDoc()
{
}

BOOL CHuroEvolutionSystemDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CHuroEvolutionSystemDoc serialization

void CHuroEvolutionSystemDoc::Serialize(CArchive& ar)
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
// CHuroEvolutionSystemDoc diagnostics

#ifdef _DEBUG
void CHuroEvolutionSystemDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHuroEvolutionSystemDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHuroEvolutionSystemDoc commands