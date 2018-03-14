// MediaConvertorDlg.h : header file
//

#pragma once
#include "OperationParam.h"
#include "VideoProcess.h"
#include <vector>

using namespace std;

// CMediaConvertorDlg dialog
class CMediaConvertorDlg : public CDialog
{
// Construction
public:
	CMediaConvertorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MEDIACONVERTOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	HRESULT ReadOptionSetting();


// Implementation
protected:
	HICON m_hIcon;

	int	m_nCurrentId;
	vector<COperationParam> m_vOP;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnCbnSelchangeComboFile();
	afx_msg void OnBnClickedBtnConvert();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnCbnSelchangeComboCodec();
};
