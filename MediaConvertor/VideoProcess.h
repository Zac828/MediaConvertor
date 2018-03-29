#pragma once
#include "OperationParam.h"

struct CommandParam
{
	CString			sFilePath;
	CString			sFileName;
	MEDIA_CODEC		Codec;
	BOOL			bHorizontalMirror;
	CommandParam() 
	{
		sFilePath = L"";
		sFileName = L"";
		Codec = MP4;
		bHorizontalMirror = FALSE;
	}
};

class CVideoProcess
{
public:
	CVideoProcess(void);
	~CVideoProcess(void);

	HRESULT Convert(COperationParam p_OP);

	static void	ThreadConvert(LPVOID lpParam);
	int			GetMediaDurationInSec(wchar_t* p_wcsFilePath);

private:
	CString		ExecCommandWithReturnStr(const wchar_t* wcsCmd);
};
