// RControlDoc.h : interface of the CRControlDoc class
//


#pragma once


class CRControlDoc : public CDocument
{
protected: // create from serialization only
	CRControlDoc();
	DECLARE_DYNCREATE(CRControlDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CRControlDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


