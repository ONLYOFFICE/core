#pragma once

#ifdef _WIN32
#include "../../Common/BaseThread.h"

#include <wininet.h>
#pragma comment(lib, "Wininet")

//------------------------------------------------------------------------------------------------------

// Константа для максимального числа символов в строке
#define	MAX_SIZE						256
// Константа для максимального числа загружаемых байт
#define DOWNLOAD_FILE_SIZE				32768
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

    CFileDownloader (CString sFileUrl, bool bDelete = true) : CBaseThread(0)
	{
		m_pFile     = NULL;
		m_sFilePath = _T("");
		m_sFileUrl  = sFileUrl;
        m_bComplete = false;
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
    bool    IsFileDownloaded()
	{
		return m_bComplete;
	}
protected :

	unsigned int DownloadFile(CString sFileUrl)
	{
		// Проверяем состояние соединения
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

		// Открываем сессию
		HINTERNET hInternetSession = InternetOpen ( _T ("Mozilla/4.0 (compatible; MSIE 5.0; Windows 98)"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
		if ( NULL == hInternetSession )
			return S_FALSE;

		// Заголовок запроса ( пока содержит 0 байт ( необходимо для проверки ) )
		CString sHTTPHdr = _T ("Range: bytes=0-0");
		// Открываем ссылку для проверки на ее существование, а также на возможность чтения частями
		HINTERNET hInternetOpenURL = InternetOpenUrl ( hInternetSession, sFileUrl, sHTTPHdr, -1, INTERNET_FLAG_RESYNCHRONIZE, 0 );
		if ( NULL != hInternetOpenURL )
		{
			// Открытие произошло, проверяем ответ
			if ( TRUE == QueryStatusCode ( hInternetOpenURL, TRUE ) )
			{
				// Запрос прошел удачно, проверяем возможность чтения частями и получаем размер данных
				LONGLONG nFileSize = IsAccept_Ranges ( hInternetOpenURL );
				// Закрываем хендл
				InternetCloseHandle ( hInternetOpenURL );
				if ( -1 == nFileSize )
				{
					// Чтение частями недоступно
					// Закрываем хендл соединения
					InternetCloseHandle ( hInternetSession );
					// Закрываем файл (сделается на DownloadAll)
					// Попробуем записать его целиком
					return S_FALSE;
				}
				else
				{
					// Чтение частями доступно
					LONGLONG nStartByte = 0;
					while ( m_bRunThread )
					{
						// Если закачали весь файл - то выходим
						if ( nStartByte == nFileSize - 1 )
						{
							// Закрываем хендл соединения
							InternetCloseHandle ( hInternetSession );
							return S_OK;
						}
						LONGLONG nEndByte = nStartByte + DOWNLOAD_FILE_SIZE;
						// Если файл заканчивается, то загружаем меньшее колличество байт ( на 1 меньше, чем размер, т.к. начинается с 0 )
						if ( nEndByte >= nFileSize )
							nEndByte = nFileSize - 1;

						// Буффер для закачки
						BYTE arrBuffer [ DOWNLOAD_FILE_SIZE ] = { 0 };
						DWORD dwBytesDownload = DownloadFilePath ( hInternetSession, arrBuffer, nStartByte, nEndByte, sFileUrl );

						nStartByte = nEndByte;
						if ( -1 == dwBytesDownload )
						{
							// Ничего не прочиталось - это плохо!!!!
							// Закрываем хендл соединения
							InternetCloseHandle ( hInternetSession );
							// Закрываем файл (сделается на DownloadAll)
							// Попробуем записать его целиком
							return S_FALSE;
						}

						// Пишем в файл
						::fwrite( (BYTE*)arrBuffer, 1, dwBytesDownload, m_pFile );
						::fflush( m_pFile );
						// Проверка на приостановку
						CheckSuspend ();
					}
				}
			}
			else
			{
				// Закрываем хендл соединения
				InternetCloseHandle ( hInternetSession );
				// Закрываем файл (сделается на DownloadAll)
				// Попробуем записать его целиком
				return S_FALSE;
			}
		}
		else
		{
			// Закрываем хендл соединения
			InternetCloseHandle ( hInternetSession );
			// Закрываем файл (сделается на DownloadAll)
			// Попробуем записать его целиком
			return S_FALSE;
		}

		// Закрываем хендл соединения
		InternetCloseHandle ( hInternetSession );

		return S_OK;
	}
	DWORD DownloadFilePath ( HINTERNET hInternet, LPBYTE pBuffer, LONGLONG nStartByte, LONGLONG nEndByte, CString sFileURL )
	{
		// Неоткрытая сессия
		if ( NULL == hInternet )
			return -1;

		// Пришли непонятные параметры
		if ( nStartByte > nEndByte || !pBuffer )
			return -1;

		// Заголовок запроса ( содержит nEndByte - nStartByte байт )
		CString sHTTPHdr = _T (""); sHTTPHdr.Format ( _T ("Range: bytes=%lld-%lld"), nStartByte, nEndByte );
		// Открываем ссылку для закачки
		HINTERNET hInternetOpenURL = InternetOpenUrl ( hInternet, sFileURL, sHTTPHdr, -1, INTERNET_FLAG_RESYNCHRONIZE, 0 );
		if ( NULL == hInternetOpenURL )
			return -1;
		// Открытие произошло, проверяем ответ
		if ( FALSE == QueryStatusCode ( hInternetOpenURL, TRUE ) )
		{
			// Закрываем хендл соединения
			InternetCloseHandle ( hInternetOpenURL );
			return -1;
		}

		// Какое колличество байт прочитано
		DWORD dwBytesRead = 0;
		// Читаем файл
		if ( FALSE == InternetReadFile ( hInternetOpenURL, pBuffer, DOWNLOAD_FILE_SIZE, &dwBytesRead ) )
		{
			// Закрываем хендл соединения
			InternetCloseHandle ( hInternetOpenURL );
			return -1;
		}

		// Закрываем хендл соединения
		InternetCloseHandle ( hInternetOpenURL );

		return dwBytesRead;
	}
	virtual      DWORD ThreadProc ()
	{
        m_bComplete = false;

		CoInitialize ( NULL );

		if ( S_OK != DownloadFile ( m_sFileUrl ) )
		{
			HRESULT hrResultAll = DownloadFileAll(m_sFileUrl, m_sFilePath);

			if (S_OK != hrResultAll)
			{
				m_bRunThread = FALSE;
				CoUninitialize ();
				return 0;
			}
		}

		m_bRunThread = FALSE;
		CoUninitialize ();

        m_bComplete = true;
		return 0;
	}

	BOOL QueryStatusCode ( HINTERNET hInternet, BOOL bIsRanges )
	{
		// Зачем проверять у неоткрытой сессии что-то
		if ( NULL == hInternet )
			return FALSE;

		// Результат ответа
		INT nResult = 0;
		// Размер данных ответа ( должно быть = 4 )
		DWORD dwLengthDataSize = 4;

		// Делаем запрос, если не проходит - то возвращаем FALSE
		if ( FALSE == HttpQueryInfo ( hInternet, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &nResult, &dwLengthDataSize, NULL ) )
			return FALSE;

		// Запрос прошел, теперь проверяем код ответа
		if ( HTTP_STATUS_NOT_FOUND == nResult )
		{
			// Объект не найден, плохая ссылка или что-то еще
			return FALSE;
		}
		else if ( ( HTTP_STATUS_OK != nResult && FALSE == bIsRanges ) || ( HTTP_STATUS_PARTIAL_CONTENT != nResult && TRUE == bIsRanges ) )
		{
			// Запрос не прошел по какой-то причине
			return FALSE;
		}

		// Все отлично, запрос прошел
		return TRUE;
	}
	// Проверяет, доступно ли для ресурса чтение частями и возвращает -1 если не доступно и размер данных, если доступно
	LONGLONG IsAccept_Ranges ( HINTERNET hInternet )
	{
		// Зачем проверять у неоткрытой сессии что-то
		if ( NULL == hInternet )
			return -1;

		// Результат ответа
		char arrResult [ MAX_SIZE ] = { 0 };
		// Размер данных ответа
		DWORD dwLengthDataSize = sizeof ( arrResult );

		// Делаем запрос, если не проходит - то возвращаем FALSE
		if ( FALSE == HttpQueryInfoA ( hInternet, HTTP_QUERY_CONTENT_RANGE, &arrResult, &dwLengthDataSize, NULL ) )
		{
			// Получаем последнюю ошибку
			DWORD dwLastError = GetLastError ();
			if ( dwLastError == ERROR_HTTP_HEADER_NOT_FOUND )
			{
				// Не пришел заголовок, значит ресурс не поддерживает чтение частями
				return -1;
			}

			// Возникла какая-то другая ошибка - возвращаем FALSE
			return -1;
		}

		// Если размер 0, то заголовка нет
		if ( 0 >= dwLengthDataSize )
			return -1;

		// Приведем к CString
		CString strResult ( arrResult );

		// Содержит размер данных
		LONGLONG nFileSize = 0;

		try
		{
			// Ищем индекс размера данных в строке
			INT nStartIndex = strResult.Find ( CONTENT_RANGE );
			if ( -1 == nStartIndex )
				return -1;

			// Оставляем в строке только размер данных
			strResult = strResult.Mid ( nStartIndex + CONTENT_RANGE_SIZE );
			// Теперь получим размер данных, переводя стринг в LONGLONG
			nFileSize = _wtoi64 ( strResult.GetBuffer () );
			// Т.к. реально нумерация с 0 ( поэтому добавляем еще 1 байт )
			if ( 0 < nFileSize )
				nFileSize += 1;
		}
		catch ( ... )
		{
			// не нашли возвращаем ошибку
			return -1;
		}

		// Все отлично, ресурс поддерживает чтение частями, возвращаем размер
		return nFileSize;
	}

	HRESULT DownloadFileAll(CString sFileURL, CString strFileOutput)
	{
		if ( m_pFile )
		{
			::fclose( m_pFile );
			m_pFile = NULL;
		}
		// Скачиваем файл
		return URLDownloadToFile (NULL, sFileURL, strFileOutput, NULL, NULL);
	}

public:
	static bool IsNeedDownload(CString FilePath)
	{
		int n1 = FilePath.Find(_T("www."));
		int n2 = FilePath.Find(_T("http://"));
		int n3 = FilePath.Find(_T("ftp://"));
		int n4 = FilePath.Find(_T("https://"));

        //если nI сранивать не с 0, то будут проблемы
        //потому что в инсталяции мы кладем файлы в /var/www...
        if (0 == n1 || 0 == n2 || 0 == n3 || 0 == n4)
			return true;
		return false;
	}

protected :

	FILE    *m_pFile;           // Хэндл на временный файл
	CString  m_sFilePath;       // Путь к сохраненному файлу на диске
	CString  m_sFileUrl;        // Ссылка на скачивание файла

    bool     m_bComplete;       // Закачался файл или нет
    bool     m_bDelete;         // Удалять ли файл в деструкторе

};
#else

// not win32
//#include <boost/network/protocol/http/client.hpp>
#include <iostream>
#include <unistd.h>
#include "./DocxFormat/Source/Base/ASCString.h"
#include "./DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include<fcntl.h>
#include <string>

#include <stdio.h>

#if defined(__APPLE__)
#include "/usr/include/curl/curl.h"
#include "/usr/include/curl/easy.h"
#else
#include "3dParty/curl/include/curl/curl.h"
#include "3dParty/curl/include/curl/easy.h"
#endif

static void Sleep (unsigned int mSec)
{
    usleep(1000 * mSec);
}

class CFileDownloader
{
public :

    CFileDownloader (CString sFileUrl, bool bDelete = true)
    {

        char filename[L_tmpnam];
        std::tmpnam(filename);

        m_sFilePath;
        m_sFileUrl  = sFileUrl;
        m_bComplete = false;
        m_bDelete   = bDelete;
    }
    ~CFileDownloader ()
    {
        if (m_bDelete && !m_sFilePath.IsEmpty())
            unlink(stringWstingToUtf8String (m_sFilePath).c_str());
    }

    static size_t write_data(void *ptr, size_t size, size_t nmemb, int fd) {
        size_t written = write(fd, ptr, size * nmemb);
        return written;
    }


    void Start (int n)
    {
        CURL *curl;
           int fp;
           CURLcode res;
           std::string sUrl = stringWstingToUtf8String (m_sFileUrl);
           std::string sOut;
           const char *url = sUrl.c_str();
           curl = curl_easy_init();
           if (curl) {
               fp = createUniqueTempFile(sOut);
               curl_easy_setopt(curl, CURLOPT_URL, url);
               curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
               curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
#if defined(__linux__)
               //в linux нет встроенных в систему корневых сертификатов, поэтому отключаем проверку
               //http://curl.haxx.se/docs/sslcerts.html
               curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif
               /* some servers don't like requests that are made without a user-agent field, so we provide one */
               curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
               res = curl_easy_perform(curl);
               /* always cleanup */
               curl_easy_cleanup(curl);
               close(fp);
           }

           m_bComplete = (CURLE_OK == res);
           if (m_bComplete)
               m_sFilePath = sOut;
        //int nRes = execl("/usr/bin/wget", stringWstingToUtf8String (m_sFileUrl).c_str(), "-P", stringWstingToUtf8String (m_sFilePath).c_str(), (char *)NULL);
        //m_bComplete = nRes >= 0;
    }

    bool IsRunned()
    {
        return false;
    }

    CString GetFilePath()
    {
        return m_sFilePath;
    }
    bool    IsFileDownloaded()
    {
        return m_bComplete;
    }
protected :

    unsigned int DownloadFile(CString sFileUrl)
    {
        return S_OK;
    }


    int createUniqueTempFile (std::string &filename)
    {
        std::string sTempPath = stringWstingToUtf8String (FileSystem::Directory::GetTempPath());
        sTempPath += "/fileXXXXXX";
        int fd = mkstemp(const_cast <char *> (sTempPath.c_str()));
        if (-1 != fd)
            filename = sTempPath;
        return fd;
    }



public:
    static bool IsNeedDownload(CString FilePath)
    {
        int n1 = FilePath.Find(_T("www."));
        int n2 = FilePath.Find(_T("http://"));
        int n3 = FilePath.Find(_T("ftp://"));
        int n4 = FilePath.Find(_T("https://"));

        //если nI сранивать не с 0, то будут проблемы
        //потому что в инсталяции мы кладем файлы в /var/www...
        if (0 == n1 || 0 == n2 || 0 == n3 || 0 == n4)
            return true;
        return false;
    }
protected :

    CString  m_sFilePath;       // Путь к сохраненному файлу на диске
    CString  m_sFileUrl;        // Ссылка на скачивание файла

    bool     m_bComplete;       // Закачался файл или нет
    bool     m_bDelete;         // Удалять ли файл в деструкторе

};

#endif
