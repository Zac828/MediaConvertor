#include "StdAfx.h"
#include "VideoProcess.h"

CVideoProcess::CVideoProcess(void)
{
}

CVideoProcess::~CVideoProcess(void)
{
}

HRESULT CVideoProcess::Convert(COperationParam p_OP)
{
	HRESULT hr = S_OK;

	CommandParam* pCP = new CommandParam;
	pCP->sFilePath = p_OP.GetFilePath();
	pCP->sFileName = p_OP.GetFileName();
	pCP->Codec = p_OP.GetOutputCodec();

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

	// Handle output format
	CString sFormat = L"mp4";
	if (pCP->Codec == MP3)
		sFormat = L"mp3";

	CString sCommand = L"";
	sCommand.Format(L"ffmpeg -i \"%s\" -pix_fmt yuv420p -c:v libx264 -c:a libmp3lame \".\\ConvertedVideo\\%s.%s\"", pCP->sFilePath, pCP->sFileName, sFormat);

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