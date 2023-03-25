// JoyControlRobotDoc.h : interface of the CJoyControlRobotDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_JOYCONTROLROBOTDOC_H__7601ACE1_5D39_4703_9610_80413F5FFE19__INCLUDED_)
#define AFX_JOYCONTROLROBOTDOC_H__7601ACE1_5D39_4703_9610_80413F5FFE19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CJoyControlRobotDoc : public CDocument
{
protected: // create from serialization only
	CJoyControlRobotDoc();
	DECLARE_DYNCREATE(CJoyControlRobotDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJoyControlRobotDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CJoyControlRobotDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CJoyControlRobotDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JOYCONTROLROBOTDOC_H__7601ACE1_5D39_4703_9610_80413F5FFE19__INCLUDED_)
