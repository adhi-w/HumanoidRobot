// TeenCalibrateDoc.h : interface of the CTeenCalibrateDoc class
//


#pragma once


class CTeenCalibrateDoc : public CDocument
{
protected: // create from serialization only
	CTeenCalibrateDoc();
	DECLARE_DYNCREATE(CTeenCalibrateDoc)

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
	virtual ~CTeenCalibrateDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


