// OdemeterTestDoc.h : interface of the COdemeterTestDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ODEMETERTESTDOC_H__111FA187_6CF4_4472_85AB_AECE4D84D39A__INCLUDED_)
#define AFX_ODEMETERTESTDOC_H__111FA187_6CF4_4472_85AB_AECE4D84D39A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COdemeterTestDoc : public CDocument
{
protected: // create from serialization only
	COdemeterTestDoc();
	DECLARE_DYNCREATE(COdemeterTestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COdemeterTestDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COdemeterTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COdemeterTestDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ODEMETERTESTDOC_H__111FA187_6CF4_4472_85AB_AECE4D84D39A__INCLUDED_)
