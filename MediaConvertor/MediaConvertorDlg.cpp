// MediaConvertorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MediaConvertor.h"
#include "MediaConvertorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CVideoProcess	g_oVideoProcess;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMediaConvertorDlg dialog




CMediaConvertorDlg::CMediaConvertorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMediaConvertorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMediaConvertorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMediaConvertorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPEN, &CMediaConvertorDlg::OnBnClickedBtnOpen)
	ON_CBN_SELCHANGE(IDC_COMBO_FILE, &CMediaConvertorDlg::OnCbnSelchangeComboFile)
	ON_BN_CLICKED(IDC_BTN_CONVERT, &CMediaConvertorDlg::OnBnClickedBtnConvert)
	ON_BN_CLICKED(IDC_BTN_BROWSE, &CMediaConvertorDlg::OnBnClickedBtnBrowse)
	ON_CBN_SELCHANGE(IDC_COMBO_CODEC, &CMediaConvertorDlg::OnCbnSelchangeComboCodec)
END_MESSAGE_MAP()


// CMediaConvertorDlg message handlers

BOOL CMediaConvertorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Create codec combo box
	CComboBox* pCodecComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CODEC);
	pCodecComboBox->AddString(L"MP4");
	pCodecComboBox->AddString(L"MP3");
	pCodecComboBox->SetCurSel(0);

	// init
	m_nCurrentId = 0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMediaConvertorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMediaConvertorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMediaConvertorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMediaConvertorDlg::OnBnClickedBtnOpen()
{
	// Clear all
	m_vOP.clear();

	// Record current path
	wchar_t szCurrentPath[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szCurrentPath);

	// Open dialog for file selection
	wchar_t strFilter[] = { L"Video (*.mp4, *.wmv, *.avi)|*.mp4; *.wmv; *.avi|Audio (*.mp3)|*.mp3|All files (*.*)|*.*|" };
	CFileDialog FileDlg(TRUE, L"*", NULL, OFN_ALLOWMULTISELECT, strFilter);
	const DWORD numberOfFileNames = 100;
	const DWORD fileNameMaxLength = MAX_PATH + 1;
	const DWORD bufferSize = (numberOfFileNames * fileNameMaxLength) + 1;
    
	// Initialize beginning and end of buffer.
	TCHAR* filenamesBuffer = new TCHAR[bufferSize];
	filenamesBuffer[0] = NULL;
	filenamesBuffer[bufferSize-1] = NULL;

	// Attach buffer to OPENFILENAME member.
	FileDlg.m_ofn.lpstrFile = filenamesBuffer;
	FileDlg.m_ofn.nMaxFile = bufferSize;

	CComboBox* pFileComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FILE);
	int nCount = pFileComboBox->GetCount();				// Clear all old items
	for (int i = nCount - 1; i >= 0; i--)
		pFileComboBox->DeleteString(i);

	// Create array for file names.
	CString fileNameArray[numberOfFileNames];
	if( FileDlg.DoModal() == IDOK )
	{
	    // Retrieve file name(s).
		POSITION fileNamesPosition = FileDlg.GetStartPosition();
		int iCtr = 0;
		int iCountFilePPT = 0;
		while(fileNamesPosition != NULL)
		{
			fileNameArray[iCtr] = FileDlg.GetNextPathName(fileNamesPosition);
			pFileComboBox->AddString(fileNameArray[iCtr]);

			// Set initial parameters
			COperationParam OP;
			OP.SetFilePath(fileNameArray[iCtr]);
			OP.SetFileName(OP.GetOutputName(fileNameArray[iCtr].GetBuffer()));
			OP.SetOutputCodec(MP4);
			m_vOP.push_back(OP);

			// Next item in list
			iCtr++;
		}
	}
	delete[] filenamesBuffer;
	pFileComboBox->SetCurSel(0);

	// Set path back. If not set path back, the output file will be located on the same path of source file.
	SetCurrentDirectory(szCurrentPath);


}

void CMediaConvertorDlg::OnBnClickedBtnBrowse()
{
	CString strDefaultOutputPath = L".\\ConvertedVideo";
	if (ShellExecute(NULL, L"open", strDefaultOutputPath.GetBuffer(), NULL, NULL, SW_SHOWNORMAL) != 0)
	{
		CreateDirectory(strDefaultOutputPath.GetBuffer(), NULL);
		ShellExecute(NULL, L"open", strDefaultOutputPath.GetBuffer(), NULL, NULL, SW_SHOWNORMAL);
	}
}

void CMediaConvertorDlg::OnBnClickedBtnConvert()
{
	CComboBox* pFileComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_FILE);

	if (pFileComboBox->GetCount() > 0)
	{
		CString sFilePath;
		pFileComboBox->GetLBText(pFileComboBox->GetCurSel(), sFilePath);

		// Convert process
		g_oVideoProcess.Convert(m_vOP[m_nCurrentId]);
	}
}

void CMediaConvertorDlg::OnCbnSelchangeComboFile()
{
	m_nCurrentId = ((CComboBox*)GetDlgItem(IDC_COMBO_FILE))->GetCurSel();

	// Set opration
	ReadOptionSetting();

	// record codec
	OnCbnSelchangeComboCodec();
}

void CMediaConvertorDlg::OnCbnSelchangeComboCodec()
{
	if (m_vOP.size() > 0)
	{
		switch (((CComboBox *)GetDlgItem(IDC_COMBO_CODEC))->GetCurSel())
		{
		case 0:
			m_vOP[m_nCurrentId].SetOutputCodec(MP4);
			break;
		case 1:
			m_vOP[m_nCurrentId].SetOutputCodec(MP3);
			break;
		default:
			break;
		}
	}
}

HRESULT CMediaConvertorDlg::ReadOptionSetting()
{
	HRESULT hr = S_OK;

	if (m_vOP.size() > 0)
	{
		((CComboBox *)GetDlgItem(IDC_COMBO_CODEC))->SetCurSel((int)m_vOP[m_nCurrentId].GetOutputCodec());
	}

	return hr;
}