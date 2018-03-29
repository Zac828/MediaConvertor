#include "StdAfx.h"
#include "OperationParam.h"

COperationParam::COperationParam(void)
{
	m_bHorizontalMirror = FALSE;
	m_codec = MP4;
}

COperationParam::~COperationParam(void)
{
}

CString	COperationParam::GetOutputName(wchar_t* p_wcsFilePath)
{
	CString strOutputName = L"";
	CString	strTemp = L"";
	strTemp.Format(L"%s", p_wcsFilePath);
	strOutputName.Format(L"%s", p_wcsFilePath);

	// Get file name with format
	while (1)
	{
		int nPos = strTemp.Find(L"\\");
		if (nPos != -1)
		{
			strTemp = strTemp.Mid(nPos + 1);
		}
		else
		{
			strOutputName = strTemp;
			break;
		}
	}

	// Remove file format
	int nTemp = 0;
	while (1)
	{
		int nPos = strTemp.Find(L".");
		if (nPos != -1)
		{
			strTemp = strTemp.Mid(nPos + 1);
		}
		else
		{
			strOutputName = strOutputName.Left(nTemp - 1);
			break;
		}
		nTemp += (nPos + 1);
	}

	// Replace " " to "_"
	strOutputName.Replace(L" ", L"_");

	return strOutputName;
}