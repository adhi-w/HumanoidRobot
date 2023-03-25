// TeenVisionDoc.h : interface of the CTeenVisionDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEENVISIONDOC_H__B7BD0D5F_A950_4051_AA2E_96E5F0010604__INCLUDED_)
#define AFX_TEENVISIONDOC_H__B7BD0D5F_A950_4051_AA2E_96E5F0010604__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTeenVisionDoc : public CDocument
{
protected: // create from serialization only
	CTeenVisionDoc();
	DECLARE_DYNCREATE(CTeenVisionDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeenVisionDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTeenVisionDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTeenVisionDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEENVISIONDOC_H__B7BD0D5F_A950_4051_AA2E_96E5F0010604__INCLUDED_)
