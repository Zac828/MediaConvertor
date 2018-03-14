#pragma once

enum MEDIA_CODEC
{
	MP4,
	MP3
};

class COperationParam
{
public:
	COperationParam(void);
	~COperationParam(void);
	
	CString GetOutputName(wchar_t* p_wcsFilePath);

	CString		GetFilePath()					{ return m_sFilePath; }
	CString		GetFileName()					{ return m_sFileName; }
	MEDIA_CODEC	GetOutputCodec()				{ return m_codec; }

	void SetFilePath(CString p_sFilePath)		{ m_sFilePath = p_sFilePath; }
	void SetFileName(CString p_sFileName)		{ m_sFileName = p_sFileName; }
	void SetOutputCodec(MEDIA_CODEC p_codec)	{ m_codec = p_codec; }

private:
	CString		m_sFilePath;
	CString		m_sFileName;
	MEDIA_CODEC m_codec;
};
