#pragma once

#include "BaseThread.h"
#include "Registration.h"

#include <wininet.h>
#pragma comment(lib, "Wininet")

//------------------------------------------------------------------------------------------------------

// Константа для максимального числа символов в строке
#define	MAX_SIZE			          256
// Константа для максимального числа загружаемых байт
#define MAX_PATH_DOWNLOAD_FILE_SIZE   32768
#define MAX_SINGLE_DOWNLOAD_FILE_SIZE 524288


// Константа для получения размера файла
#define CONTENT_RANGE		_T("bytes 0-0/")
// Константа для колличества символов у CONTENT_RANGE
#define CONTENT_RANGE_SIZE	( 11/*sizeof ( CONTENT_RANGE )*/ - 1 )


//------------------------------------------------------------------------------------------------------
// Класс для закачки файла из сети на локальный диск 
//------------------------------------------------------------------------------------------------------

class CFileDownloader : public CBaseThread
{
public :

	CFileDownloader (CString sFileUrl, BOOL bDelete = TRUE)
	{
		m_pFile     = NULL;
		m_sFilePath = _T("");
		m_sFileUrl  = sFileUrl;
		m_bComplete = FALSE;
		m_bDelete   = bDelete;
	}
	~CFileDownloader ()
	{
		if ( m_pFile )
		{
			::fclose( m_pFile );
			m_pFile = NULL;
		}
		if ( m_sFilePath.GetLength() > 0 && m_bDelete )
		{
			DeleteFileW( m_sFilePath.GetBuffer() );
			m_sFilePath = _T("");
		}

	}


	CString GetFilePath()
	{
		return m_sFilePath;
	}
	BOOL    IsFileDownloaded()
	{
		return m_bComplete;
	}
protected :

	unsigned int DownloadFile(CString sFileUrl)
	{
		if ( FALSE == InternetGetConnectedState ( 0, 0 ) )
			return S_FALSE;

		char sTempPath[MAX_PATH], sTempFile[MAX_PATH];
		if ( 0 == GetTempPathA( MAX_PATH, sTempPath ) )
			return S_FALSE;

		if ( 0 == GetTempFileNameA( sTempPath, "CSS", 0, sTempFile ) )
			return S_FALSE;

		m_pFile = ::fopen( sTempFile, "wb" );
		if ( !m_pFile )
			return S_FALSE;

		m_sFilePath = CString( sTempFile );

		HINTERNET hInternetSession = InternetOpen ( _T("Microsoft Internet Explorer"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
		if ( NULL == hInternetSession )
			return S_FALSE;

		CString sHTTPHeader = _T("Range: bytes=0-0");
		HINTERNET hInternetOpenURL = InternetOpenUrl ( hInternetSession, sFileUrl.GetBuffer(), sHTTPHeader, -1, INTERNET_FLAG_RESYNCHRONIZE, 0 );
		if ( NULL != hInternetOpenURL )
		{
			if ( TRUE == QueryStatusCode ( hInternetOpenURL, TRUE ) )
			{
				LONGLONG nFileSize = GetFileLength( hInternetOpenURL, TRUE );
				InternetCloseHandle ( hInternetOpenURL );
				if ( -1 != nFileSize )
				{
					LONGLONG nStartByte = 0;
					while ( m_bRunThread )
					{
						if ( nStartByte == nFileSize - 1 )
						{
							InternetCloseHandle ( hInternetSession );
							return S_OK;
						}

						LONGLONG nEndByte = nStartByte + MAX_PATH_DOWNLOAD_FILE_SIZE;

						if ( nEndByte >= nFileSize )
							nEndByte = nFileSize - 1;

						BYTE arrBuffer[ MAX_PATH_DOWNLOAD_FILE_SIZE ] = { 0 };
						DWORD dwBytesDownload = DownloadFilePath ( hInternetSession, arrBuffer, nStartByte, nEndByte, sFileUrl, TRUE, MAX_PATH_DOWNLOAD_FILE_SIZE );

						nStartByte = nEndByte;
						if ( -1 == dwBytesDownload )
						{
							InternetCloseHandle ( hInternetSession );
							return S_FALSE;
						}

						::fwrite( (BYTE*)arrBuffer, 1, dwBytesDownload, m_pFile );
						::fflush( m_pFile );
						CheckSuspend ();
					}
				}
			}
			else if ( TRUE == QueryStatusCode( hInternetOpenURL, FALSE ) )
			{
				LONGLONG nFileSize = GetFileLength( hInternetOpenURL, FALSE );
				InternetCloseHandle ( hInternetOpenURL );
				if ( -1 != nFileSize && nFileSize < MAX_SINGLE_DOWNLOAD_FILE_SIZE )
				{
					while ( m_bRunThread )
					{
						BYTE arrBuffer[ MAX_SINGLE_DOWNLOAD_FILE_SIZE ] = { 0 };
						DWORD dwBytesDownload = DownloadFilePath ( hInternetSession, arrBuffer, 0, nFileSize, sFileUrl, FALSE, MAX_SINGLE_DOWNLOAD_FILE_SIZE );
						if ( -1 == dwBytesDownload )
						{
							InternetCloseHandle ( hInternetSession );
							return S_FALSE;
						}
						::fwrite( (BYTE*)arrBuffer, 1, dwBytesDownload, m_pFile );
						::fflush( m_pFile );
						InternetCloseHandle( hInternetSession );
						return S_OK;
					}
				}

			}
		}

		DWORD dwError = GetLastError();
		InternetCloseHandle ( hInternetSession );

		return S_OK;
	}
	DWORD        DownloadFilePath(HINTERNET hInternet, LPBYTE pBuffer, LONGLONG nStartByte, LONGLONG nEndByte, CString sFileURL, BOOL bIsRanges, DWORD dwReadBytes )
	{
		if ( NULL == hInternet )
			return -1;

		if ( nStartByte > nEndByte || !pBuffer )
			return -1;

		CString sHTTPHeader = _T(""); 
		sHTTPHeader.Format( _T("Range: bytes=%I64d-%I64d"), nStartByte, nEndByte );

		HINTERNET hInternetOpenURL = InternetOpenUrl ( hInternet, sFileURL, sHTTPHeader, -1, INTERNET_FLAG_RESYNCHRONIZE, 0 );
		if ( NULL == hInternetOpenURL )
			return -1;

		if ( FALSE == QueryStatusCode ( hInternetOpenURL, bIsRanges ) )
		{
			InternetCloseHandle ( hInternetOpenURL );
			return -1;
		}

		DWORD dwBytesRead = 0;
		if ( FALSE == InternetReadFile ( hInternetOpenURL, pBuffer, dwReadBytes, &dwBytesRead ) )
		{
			InternetCloseHandle ( hInternetOpenURL );
			return -1;
		}

		InternetCloseHandle ( hInternetOpenURL );
		return dwBytesRead;
	}
	virtual      DWORD ThreadProc ()
	{
		m_bComplete = FALSE;

		CoInitialize ( NULL );

		if ( !CRegistratorClient::IsRegistered () )
		{
			m_bRunThread = FALSE;
			CoUninitialize ();
			return 0;
		}

		if ( S_OK != DownloadFile ( m_sFileUrl ) )
		{
			m_bRunThread = FALSE;
			CoUninitialize ();
			return 0;
		}

		m_bRunThread = FALSE;
		CoUninitialize ();

		m_bComplete = TRUE;
		return 0;
	}

	BOOL         QueryStatusCode (HINTERNET hInternet, BOOL bIsRanges)
	{
		if ( NULL == hInternet )
			return FALSE;

		int   nResult = 0;
		DWORD dwLengthDataSize = 4;

		if ( FALSE == HttpQueryInfo ( hInternet, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &nResult, &dwLengthDataSize, NULL ) )
			return FALSE;

		if ( HTTP_STATUS_NOT_FOUND == nResult )
		{
			return FALSE;
		}
		else if ( ( HTTP_STATUS_OK != nResult && FALSE == bIsRanges ) || ( HTTP_STATUS_PARTIAL_CONTENT != nResult && TRUE == bIsRanges ) )
		{
			return FALSE;
		}

		return TRUE;
	}
	LONGLONG     GetFileLength   (HINTERNET hInternet, BOOL bIsRanges )
	{
		if ( NULL == hInternet )
			return -1;

		if ( bIsRanges )
		{
			char arrResult[ MAX_SIZE ] = { 0 };
			DWORD dwLengthDataSize = sizeof ( arrResult );

			if ( FALSE == HttpQueryInfoA( hInternet, HTTP_QUERY_CONTENT_RANGE, &arrResult, &dwLengthDataSize, NULL ) )
			{
				return -1;
			}

			if ( 0 >= dwLengthDataSize )
				return -1;

			CString sResult( arrResult );

			int nStartIndex = sResult.Find ( CONTENT_RANGE );
			if ( -1 == nStartIndex )
				return -1;

			sResult = sResult.Mid( nStartIndex + CONTENT_RANGE_SIZE  );

			LONGLONG nFileSize = _wtoi64 ( sResult.GetBuffer() );
			if ( 0 < nFileSize )
				nFileSize += 1;


			return nFileSize;
		}
		else
		{
			char arrResult[ MAX_SIZE ] = { 0 };
			DWORD dwLengthDataSize = sizeof ( arrResult );

			if ( FALSE == HttpQueryInfoA( hInternet, HTTP_QUERY_CONTENT_LENGTH, &arrResult, &dwLengthDataSize, NULL ) )
			{
				return -1;
			}

			if ( 0 >= dwLengthDataSize )
				return -1;

			CString sResult( arrResult );
			LONGLONG nFileSize = 0;
			nFileSize = _wtoi64 ( sResult.GetBuffer () );

			return nFileSize;
		}
	}
protected :

	FILE    *m_pFile;           // Хэндл на временный файл
	CString  m_sFilePath;       // Путь к сохраненному файлу на диске
	CString  m_sFileUrl;        // Ссылка на скачивание файла

	BOOL     m_bComplete;       // Закачался файл или нет
	BOOL     m_bDelete;         // Удалять ли файл в деструкторе

};
