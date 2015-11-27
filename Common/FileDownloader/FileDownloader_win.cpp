#include "FileDownloader.h"

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

class CFileDownloaderBaseWin : public CFileDownloaderBase
{
public :
    CFileDownloaderBaseWin(std::wstring sFileUrl, bool bDelete = true) :
        CFileDownloaderBase(sFileUrl, bDelete)
    {
    }
    virtual ~CFileDownloaderBaseWin()
    {
    }

    virtual int DownloadFile()
    {
        CoInitialize ( NULL );

        if ( S_OK != _DownloadFile ( m_sFileUrl ) )
        {
            HRESULT hrResultAll = DownloadFileAll(m_sFileUrl, m_sFilePath);

            if (S_OK != hrResultAll)
            {
                m_bRunThread = FALSE;
                CoUninitialize ();
                return S_FALSE;
            }
        }

        m_bRunThread = FALSE;
        CoUninitialize ();
        m_bComplete = true;
        return S_OK;
    }

protected:
    unsigned int _DownloadFile(std::wstring sFileUrl)
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
        HINTERNET hInternetOpenURL = InternetOpenUrl ( hInternetSession, sFileUrl.c_str(), sHTTPHdr, -1, INTERNET_FLAG_RESYNCHRONIZE, 0 );
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
    DWORD DownloadFilePath ( HINTERNET hInternet, LPBYTE pBuffer, LONGLONG nStartByte, LONGLONG nEndByte, std::wstring sFileURL )
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
        HINTERNET hInternetOpenURL = InternetOpenUrl ( hInternet, sFileURL.c_str(), sHTTPHdr, -1, INTERNET_FLAG_RESYNCHRONIZE, 0 );
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

    HRESULT DownloadFileAll(std::wstring sFileURL, std::wstring strFileOutput)
    {
        if ( m_pFile )
        {
            ::fclose( m_pFile );
            m_pFile = NULL;
        }
        // Скачиваем файл
        return URLDownloadToFile (NULL, sFileURL.c_str(), strFileOutput.c_str(), NULL, NULL);
    }
};

CFileDownloader::CFileDownloader(std::wstring sFileUrl, bool bDelete)
{
    m_pInternal = new CFileDownloaderBaseWin(sFileUrl, bDelete);
}
