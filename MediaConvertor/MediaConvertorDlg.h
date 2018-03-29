// MediaConvertorDlg.h : header file
//

#pragma once
#include "OperationParam.h"
#include "VideoProcess.h"
#include <vector>

using namespace std;

#define LENGTH_FOR_STRING_TABLE 160

enum LanguageType
{
	DT_LANG_EN,
	DT_LANG_SCH,
	DT_LANG_TCH,
	DT_LANG_JP,
	DT_LANG_GE,
	DT_LANG_FR,
	DT_LANG_ES,
	DT_LANG_KO
};

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

	// for multi-language
	int			FindSystemLang();
	void		LocalizeDlg();
	wchar_t*	GetStringFromTable(int nId);


// Implementation
protected:
	HICON m_hIcon;

	int	m_nCurrentId;
	vector<COperationParam> m_vOP;

	LanguageType m_LangType;

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
	afx_msg void OnCbnSelchangeComboLang();
	afx_msg void OnBnClickedCheckFilp();
};
