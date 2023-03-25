// RControlView.h : interface of the CRControlView class
//


#pragma once
#include "afxwin.h"
#include "Serial.h"


class CRControlView : public CFormView
{
protected: // create from serialization only
	CRControlView();
	DECLARE_DYNCREATE(CRControlView)

public:
	enum{ IDD = IDD_RCONTROL_FORM };

// Attributes
public:
	CRControlDoc* GetDocument() const;

// Operations
public:
	Serial m_serial;
	int port;
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CRControlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonLeftHipMin();
	afx_msg void OnButtonLeftHipPlus();
	float m_edit_LeftHip;
	float m_edit_LeftThigh;
	float m_edit_LeftKnee;
	float m_edit_LeftAnkleRoll;
	float m_edit_LeftAnklePitch;
	float m_edit_RightHip;
	float m_edit_RightThigh;
	float m_edit_RightKnee;
	float m_edit_RightAnkleRoll;
	float m_edit_RightAnklePitch;
	afx_msg void OnButtonLeftThighMin();
	afx_msg void OnButtonLeftThighPlus();
	afx_msg void OnButtonLeftKneeMin();
	afx_msg void OnButtonLeftKneePlus();
	afx_msg void OnButtonLeftAnkleRollMin();
	afx_msg void OnButtonLeftAnkleRollPlus();
	afx_msg void OnButtonLeftAnklePitchMin();
	afx_msg void OnButtonLeftAnklePitchPlus();
	afx_msg void OnButtonRightHipMin();
	afx_msg void OnButtonRightHipPlus();
	afx_msg void OnButtonRightThighMin();
	afx_msg void OnButtonRightThighPlus();
	afx_msg void OnButtonRightKneeMin();
	afx_msg void OnButtonRightKneePlus();
	afx_msg void OnButtonRightAnkleRollMin();
	afx_msg void OnButtonRightAnkleRollPlus();
	afx_msg void OnButtonRightAnklePitchMin();
	afx_msg void OnButtonRightAnklePitchPlus();
	float m_edit_LIPM;
	afx_msg void OnButtonLIPMMin();
	afx_msg void OnButtonLIPMPlus();
	afx_msg void OnButtonForward();
	afx_msg void OnButtonBackward();
	afx_msg void OnButtonRightShift();
	afx_msg void OnButtonLeftShift();
	afx_msg void OnButtonFastWalk();
	afx_msg void OnButtonWalk();
	afx_msg void OnButtonStartStop();
	int m_edit_ComPort;
	afx_msg void OnEnChangeEditComport();
//	afx_msg void OnButtonConnect();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonDisconnect();
	afx_msg void OnButtonSaveEEPROM();
	afx_msg void OnButonTurnOff();
};

#ifndef _DEBUG  // debug version in RControlView.cpp
inline CRControlDoc* CRControlView::GetDocument() const
   { return reinterpret_cast<CRControlDoc*>(m_pDocument); }
#endif

