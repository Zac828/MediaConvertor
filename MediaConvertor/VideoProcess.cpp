#include "StdAfx.h"
#include "VideoProcess.h"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

CVideoProcess::CVideoProcess(void)
{
}

CVideoProcess::~CVideoProcess(void)
{
}


// Get ffmpeg start-up message
CString	CVideoProcess::ExecCommandWithReturnStr(const wchar_t* wcsCmd)
{
	WCHAR buffer[128];
    CString sResult = L"";
    FILE* pipe = _wpopen(wcsCmd, L"r");
    if (!pipe) 
		return L"";

    try {
        while (!feof(pipe)) {
            if (fgetws(buffer, 128, pipe) != NULL)
                sResult += buffer;
        }
    } catch (...) {
        _pclose(pipe);
        throw;
    }
    _pclose(pipe);
    return sResult;
}

// filter the message that we want
int CVideoProcess::GetMediaDurationInSec(wchar_t* p_wcsFilePath)
{
	CString sCommand = L"";
	sCommand.Format(L"ffmpeg -i \"%s\" 2>&1 | findstr Duration", p_wcsFilePath);

	CString sReturnStr = ExecCommandWithReturnStr(sCommand.GetBuffer());

	// Parse duration string to Second integer
	int nTotalSec = 0;
	if (sReturnStr.Compare(L"") != 0)
	{
		CString sDuration = sReturnStr.Mid(sReturnStr.Find(L"Duration:") + 10, 8);
		int nHour = _wtoi(sDuration.Left(2));
		int nMin  = _wtoi(sDuration.Mid(3, 2));
		int nSec  = _wtoi(sDuration.Right(2));

		nTotalSec = nHour * 3600 + nMin * 60 + nSec;
	}

	return nTotalSec;
}

HRESULT CVideoProcess::Convert(COperationParam p_OP)
{
	HRESULT hr = S_OK;

	CommandParam* pCP = new CommandParam;
	pCP->sFilePath = p_OP.GetFilePath();
	pCP->sFileName = p_OP.GetFileName();
	pCP->Codec = p_OP.GetOutputCodec();
	pCP->bHorizontalMirror = p_OP.GetHorizontalMirror();

	// Create thread
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadConvert, pCP, 0, NULL);

	return hr;
}

////////////////////////////////////////////////////////////////
//					Thread for ffmpeg convert				  //
////////////////////////////////////////////////////////////////

void CVideoProcess::ThreadConvert(LPVOID lpParam)
{
	CommandParam* pCP = (CommandParam*)lpParam;

	// Handle horizontal mirror
	CString sHorizontalMirror = L"";
	if (pCP->bHorizontalMirror)
		sHorizontalMirror = L"-vf hflip"; 

	// Handle output format
	CString sFormat = L"mp4";
	if (pCP->Codec == MP3)
		sFormat = L"mp3";

	CString sCommand = L"";
	sCommand.Format(L"ffmpeg -i \"%s\" -pix_fmt yuv420p %s -c:v libx264 -c:a libmp3lame \".\\ConvertedVideo\\%s.%s\"", pCP->sFilePath, sHorizontalMirror, pCP->sFileName, sFormat);

	// Save command
	WCHAR wcslog[1024] = {0};
	wcscpy(wcslog, sCommand.GetBuffer());

	// Output command log with newline character
	try
	{
		FILE *pFile = _wfopen(L"MediaConvertor.log", L"ab+");
		if (pFile)
		{
			wcscat(wcslog, L"\n");
			fwrite(wcslog, wcslen(wcslog)*2, 1, pFile);
			fclose(pFile);
		}
	}
	catch (...)
	{
	}
	// Create output folder
	_wsystem(L"cmd.exe /c mkdir ConvertedVideo");
	// Run command
	int ret = _wsystem(sCommand);
}