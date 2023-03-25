// T-visionDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "Behavior.h"

// CTvisionDlg dialog
class CTvisionDlg : public CDialog
{
// Construction
public:
	CTvisionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TVISION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	Behavior behavior;

public:
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedExit();
	CSliderCtrl m_slider_Hmin;
	CSliderCtrl m_slider_Smin;
	CSliderCtrl m_slider_Vmin;
	CSliderCtrl m_slider_Hmax;
	CSliderCtrl m_slider_Smax;
	CSliderCtrl m_slider_Vmax;
	afx_msg void OnNMReleasedcaptureHmin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSmin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureVmin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureHmax(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSmax(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureVmax(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditHmin();
	afx_msg void OnEnChangeEditSmin();
	afx_msg void OnEnChangeEditVmin();
	afx_msg void OnEnChangeEditHmax();
	afx_msg void OnEnChangeEditSmax();
	afx_msg void OnEnChangeEditVmax();
	int m_edit_Hmin;
	int m_edit_Smin;
	int m_edit_Vmin;
	int m_edit_Hmax;
	int m_edit_Smax;
	int m_edit_Vmax;
	afx_msg void OnBnClickedOrange();
	afx_msg void OnBnClickedCyan();
	afx_msg void OnBnClickedMagenta();
	afx_msg void OnBnClickedYellow();
	CString m_sendData;
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedDisconnect();
	afx_msg void OnButtonForward();
	afx_msg void OnButtonShiftLeft();
	afx_msg void OnButtonShiftRight();
	afx_msg void OnButtonBackward();
	afx_msg void OnButtonFastWalk();
	afx_msg void OnButtonWalk();
	afx_msg void OnButtonStartStop();
	afx_msg void OnEnChangeEditRemoteComport();
	afx_msg void OnEnChangeEditComport();
	int m_edit_ComPort;
	int m_edit_RemoteComPort;
	afx_msg void OnButtonMoved();
	afx_msg void OnButtonUnmoved();
	afx_msg void OnButtonTurnOff();
};
