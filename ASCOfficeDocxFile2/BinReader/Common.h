#pragma once
#ifndef BINREADER_COMMON
#define BINREADER_COMMON 

#include "FileDownloader.h"
#include "../Foreign/StringWriter.h"

bool IsUnicodeSymbol( WCHAR symbol )
{
	bool result = false;

	if ( ( 0x0009 == symbol ) || ( 0x000A == symbol ) || ( 0x000D == symbol ) ||
		( ( 0x0020 <= symbol ) && ( 0xD7FF >= symbol ) ) || ( ( 0xE000 <= symbol ) && ( symbol <= 0xFFFD ) ) ||
		( ( 0x10000 <= symbol ) && symbol ) )
	{
		result = true;
	}

	return result;		  
}
void CorrectString(CString& strValue)
{
	for (unsigned int i = 0, length = strValue.GetLength(); i < length; ++i )
	{
		if ( false == IsUnicodeSymbol( strValue.GetAt(i) ) )
		{
			strValue.SetAt(i, ' ');
		}
	}
	strValue.Replace(_T("&"),	_T("&amp;"));			
	strValue.Replace(_T("'"),	_T("&apos;"));
	strValue.Replace(_T("<"),	_T("&lt;"));
	strValue.Replace(_T(">"),	_T("&gt;"));
	strValue.Replace(_T("\""),	_T("&quot;"));
}
long Round(double val)
{
	return (long)(val+ 0.5);
}
CString DownloadImage(const CString& strFile)
{
	CFileDownloader oDownloader(strFile, FALSE);
	oDownloader.Start( 1 );
	while ( oDownloader.IsRunned() )
	{
		::Sleep( 10 );
	}
	CString strFileName;
	if ( oDownloader.IsFileDownloaded() )
	{
		strFileName = oDownloader.GetFilePath();
	}
	return strFileName;
}
VOID convertBase64ToImage (CString sImage, CString &pBase64)
{
	HANDLE hFileWriteHandle;
	// Открываем на диске файл и получаем на него указатель
	hFileWriteHandle = ::CreateFile (sImage, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE != hFileWriteHandle)
	{
		INT nUTF8Len = WideCharToMultiByte (CP_UTF8, 0, pBase64, pBase64.GetLength (), NULL, NULL, NULL, NULL);
		CHAR *pUTF8String = new CHAR [nUTF8Len + 1];
		::ZeroMemory (pUTF8String, sizeof (CHAR) * (nUTF8Len + 1));

		WideCharToMultiByte (CP_UTF8, 0, pBase64, -1, pUTF8String, nUTF8Len, NULL, NULL);
		CStringA sUnicode; sUnicode = pUTF8String;
		delete[] pUTF8String;

		//Убираем "data:image/jpg;base64,"
		int nShift = 0;
		int nIndex = sUnicode.Find("base64,");
		if(-1 != nIndex)
		{
			nShift = nIndex + 7;
		}
		// Получаем размер файла
		LONG lFileSize = sUnicode.GetLength () - nShift;
		INT nDstLength = lFileSize;
		BYTE *pBuffer = new BYTE [lFileSize];
		::ZeroMemory (pBuffer, lFileSize);
		Base64::Base64Decode ((LPCSTR)sUnicode.GetBuffer () + nShift, lFileSize, pBuffer, &nDstLength);

		// Пишем в файл
		DWORD dwBytesWrite = 0;
		::WriteFile (hFileWriteHandle, pBuffer, nDstLength, &dwBytesWrite, 0);

		RELEASEARRAYOBJECTS (pBuffer);

		CloseHandle (hFileWriteHandle);
	}
}
#endif