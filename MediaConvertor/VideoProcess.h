#pragma once
#include "OperationParam.h"

struct CommandParam
{
	CString			sFilePath;
	CString			sFileName;
	MEDIA_CODEC		Codec;
	CommandParam() 
	{
		sFilePath = L"";
		sFileName = L"";
		Codec = MP4;
	}
};

class CVideoProcess
{
public:
	CVideoProcess(void);
	~CVideoProcess(void);

	HRESULT Convert(COperationParam p_OP);

	static void	ThreadConvert(LPVOID lpParam);

private:
	CString GetOutputName(wchar_t* p_wcsFilePath);
};
