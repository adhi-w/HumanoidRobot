// HuroEvolutionSystemDoc.h : interface of the CHuroEvolutionSystemDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUROEVOLUTIONSYSTEMDOC_H__13BCAA31_AD01_4DE3_84DC_A9FF163529D7__INCLUDED_)
#define AFX_HUROEVOLUTIONSYSTEMDOC_H__13BCAA31_AD01_4DE3_84DC_A9FF163529D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CHuroEvolutionSystemDoc : public CDocument
{
protected: // create from serialization only
	CHuroEvolutionSystemDoc();
	DECLARE_DYNCREATE(CHuroEvolutionSystemDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHuroEvolutionSystemDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHuroEvolutionSystemDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHuroEvolutionSystemDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HUROEVOLUTIONSYSTEMDOC_H__13BCAA31_AD01_4DE3_84DC_A9FF163529D7__INCLUDED_)
