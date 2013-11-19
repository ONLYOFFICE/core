#pragma once

#include <stdafx.h>
#include <MemoryUtils.h>
#include <atlfile.h>

class CTempFile
{
private:

	CString m_sFilePath;

public:
	CTempFile()
	{
		LPSTR sTempFile = new char[MAX_PATH+1];
		LPSTR sTempPath = new char[MAX_PATH+1];

		GetTempPath( MAX_PATH, sTempPath);
//		LPSTR sTempPath = "E:\\OutPut";
		GetTempFileNameA( sTempPath, "~PS", 0, sTempFile);

		CString sResult( sTempFile);
		m_sFilePath = sResult;

		delete sTempFile;
		delete sTempPath;
	};
	~CTempFile()
	{
	};

	BOOL FillFromStdIn(void)
	{
		const int nBufLen = 1048576;

		HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

		CAtlFile File;

		if (S_OK != File.Create(m_sFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS))
			return FALSE; // Не могу создать файл
		
		BYTE *pBuffer = new BYTE[nBufLen];
		ZeroMemory(pBuffer, nBufLen);

		DWORD nBytesRead = 0;
		DWORD nBytesWrite = 0;
		BOOL bEOF = FALSE;

		while ( 1 )
		{
			ZeroMemory(pBuffer, nBufLen);
			if ( ReadFile(hStdIn, pBuffer, nBufLen, &nBytesRead, 0) )
				File.Write(pBuffer, nBytesRead, &nBytesWrite );
			else 
				break;
		}

		File.Close();

		delete pBuffer;

		return TRUE;
	};

	CString GetFile(void)
	{
		return m_sFilePath;
	};

	void SetFile(CString sPath)
	{
		m_sFilePath = sPath;
	};

	BOOL Delete(void)
	{
		BOOL bResult = DeleteFile((LPCSTR)m_sFilePath.GetBuffer());
		if ( bResult )
			m_sFilePath = "";
		return bResult;
	};

	void CreateNew(void)
	{
		LPSTR sTempFile = new char[MAX_PATH];
		LPSTR sTempPath = new char[MAX_PATH];

		GetTempPath( MAX_PATH, sTempPath);
		GetTempFileNameA( sTempPath, "~PS", 0, sTempFile);

		CString sResult( sTempFile);
		m_sFilePath = sResult;

		delete sTempFile;
		delete sTempPath;
	};

};