// FIRA_PK procDoc.h : interface of the CFIRA_PKprocDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIRA_PKPROCDOC_H__19FFC7E6_E463_47E9_BACC_D14305A2A0F4__INCLUDED_)
#define AFX_FIRA_PKPROCDOC_H__19FFC7E6_E463_47E9_BACC_D14305A2A0F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFIRA_PKprocDoc : public CDocument
{
protected: // create from serialization only
	CFIRA_PKprocDoc();
	DECLARE_DYNCREATE(CFIRA_PKprocDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFIRA_PKprocDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFIRA_PKprocDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFIRA_PKprocDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIRA_PKPROCDOC_H__19FFC7E6_E463_47E9_BACC_D14305A2A0F4__INCLUDED_)
