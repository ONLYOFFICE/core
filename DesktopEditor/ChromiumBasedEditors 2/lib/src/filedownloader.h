#ifndef CEF_FILE_DOWNLOADER
#define CEF_FILE_DOWNLOADER

#include "../../../DesktopEditor/common/File.h"
#include "../../../DesktopEditor/common/Directory.h"
#include "../../../DesktopEditor/graphics/BaseThread.h"

#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)

#include <wininet.h>
#include <Urlmon.h>
//#pragma comment(lib, "Wininet")

//------------------------------------------------------------------------------------------------------

// Константа для максимального числа символов в строке
#define	MAX_SIZE						256
// Константа для максимального числа загружаемых байт
#define DOWNLOAD_FILE_SIZE                                      32768
#define MAX_SINGLE_DOWNLOAD_FILE_SIZE                           524288


// Константа для получения размера файла
#define CONTENT_RANGE                                           L"bytes 0-0/"
// Константа для колличества символов у CONTENT_RANGE
#define CONTENT_RANGE_SIZE                                      ( 11/*sizeof ( CONTENT_RANGE )*/ - 1 )


//------------------------------------------------------------------------------------------------------
// Класс для закачки файла из сети на локальный диск 
//------------------------------------------------------------------------------------------------------

class CFileDownloader : public NSThreads::CBaseThread
{
public :

    CFileDownloader(std::wstring sFileUrl, bool bDelete = true) : NSThreads::CBaseThread()
    {
        m_pFile     = NULL;
        m_sFilePath = L"";
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
        if ( m_sFilePath.length() > 0 && m_bDelete )
        {
            NSFile::CFileBinary::Remove(m_sFilePath);
            m_sFilePath = L"";
        }
    }

    void SetFilePath(const std::wstring& sPath)
    {
        m_sFilePath = sPath;
    }

    std::wstring GetFilePath()
    {
        return m_sFilePath;
    }
    bool IsFileDownloaded()
    {
        return m_bComplete;
    }
protected :

    unsigned int DownloadFile(std::wstring sFileUrl)
    {
        // Проверяем состояние соединения
        if ( FALSE == InternetGetConnectedState ( 0, 0 ) )
            return S_FALSE;

        if (m_sFilePath.empty())
            m_sFilePath = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"CSS");

        m_pFile = ::_wfopen( m_sFilePath.c_str(), L"wb" );
        if ( !m_pFile )
            return 1;

        // Открываем сессию
        HINTERNET hInternetSession = InternetOpen (L"Mozilla/4.0 (compatible; MSIE 5.0; Windows 98)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
        if ( NULL == hInternetSession )
            return S_FALSE;

        // Заголовок запроса ( пока содержит 0 байт ( необходимо для проверки ) )
        std::wstring sHTTPHdr = L"Range: bytes=0-0";
        // Открываем ссылку для проверки на ее существование, а также на возможность чтения частями
        HINTERNET hInternetOpenURL = InternetOpenUrl ( hInternetSession, sFileUrl.c_str(), sHTTPHdr.c_str(), -1, INTERNET_FLAG_RESYNCHRONIZE, 0 );
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
                    return 1;
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
                            return 0;
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
                            return 1;
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
                return 1;
            }
        }
        else
        {
            // Закрываем хендл соединения
            InternetCloseHandle ( hInternetSession );
            // Закрываем файл (сделается на DownloadAll)
            // Попробуем записать его целиком
            return 1;
        }

        // Закрываем хендл соединения
        InternetCloseHandle ( hInternetSession );

        return 0;
    }
    DWORD DownloadFilePath ( HINTERNET hInternet, LPBYTE pBuffer, LONGLONG nStartByte, LONGLONG nEndByte, std::wstring sFileURL )
    {
        // Неоткрытая сессия
        if ( NULL == hInternet )
            return -1;

        // Пришли непонятные параметры
        if ( nStartByte > nEndByte || !pBuffer )
            return -1;

        std::wstring sHTTPHdr = L"Range: bytes=" + std::to_wstring(nStartByte) + L"-" + std::to_wstring(nEndByte);

        // Открываем ссылку для закачки
        HINTERNET hInternetOpenURL = InternetOpenUrl ( hInternet, sFileURL.c_str(), sHTTPHdr.c_str(), -1, INTERNET_FLAG_RESYNCHRONIZE, 0 );
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

        if ( true/*0 != DownloadFile ( m_sFileUrl )*/ )
        {
            int hrResultAll = DownloadFileAll(m_sFileUrl);

            if (0 != hrResultAll)
            {
                m_bRunThread = FALSE;
                CoUninitialize ();
                return 0;
            }
        }

        CoUninitialize ();

        m_bComplete = true;
        m_bRunThread = FALSE;
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
        std::wstring strResult = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(arrResult, strlen(arrResult));

        // Содержит размер данных
        LONGLONG nFileSize = 0;

        try
        {
            // Ищем индекс размера данных в строке
            INT nStartIndex = strResult.find( CONTENT_RANGE );
            if ( std::wstring::npos == nStartIndex )
                return -1;

            // Оставляем в строке только размер данных
            strResult = strResult.substr( nStartIndex + CONTENT_RANGE_SIZE );
            // Теперь получим размер данных, переводя стринг в LONGLONG
            nFileSize = _wtoi64 ( strResult.c_str() );
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

    int DownloadFileAll(std::wstring sFileURL)
    {
        if (m_sFilePath.empty())
            m_sFilePath = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPathW(), L"CSS");

        if (m_pFile)
        {
            ::fclose( m_pFile );
            m_pFile = NULL;
        }
        // Скачиваем файл
        return (int)::URLDownloadToFileW (NULL, sFileURL.c_str(), m_sFilePath.c_str(), NULL, NULL);
    }

public:
    static bool IsNeedDownload(const std::wstring& FilePath)
    {
        int n1 = FilePath.find(L"www.");
        int n2 = FilePath.find(L"http://");
        int n3 = FilePath.find(L"ftp://");
        int n4 = FilePath.find(L"https://");

        if (n1 != std::wstring::npos && n1 < 10)
            return true;
        if (n2 != std::wstring::npos && n2 < 10)
            return true;
        if (n3 != std::wstring::npos && n3 < 10)
            return true;
        if (n4 != std::wstring::npos && n4 < 10)
            return true;

        return false;
    }

protected :

    FILE*           m_pFile;           // Хэндл на временный файл
    std::wstring    m_sFilePath;       // Путь к сохраненному файлу на диске
    std::wstring    m_sFileUrl;        // Ссылка на скачивание файла

    bool            m_bComplete;       // Закачался файл или нет
    bool            m_bDelete;         // Удалять ли файл в деструкторе

};
#else

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "../../../Common/3dParty/curl/include/curl/curl.h"
#include "../../../Common/3dParty/curl/include/curl/easy.h"

class CFileDownloader : public NSThreads::CBaseThread
{
public :

    CFileDownloader(std::wstring sFileUrl, bool bDelete = true) : NSThreads::CBaseThread()
    {        
        m_sFilePath = L"";
        m_sFileUrl  = sFileUrl;
        m_bComplete = false;
        m_bDelete   = bDelete;
    }
    ~CFileDownloader ()
    {
        if ( m_sFilePath.length() > 0 && m_bDelete )
        {
            NSFile::CFileBinary::Remove(m_sFilePath);
            m_sFilePath = L"";
        }
    }

    void SetFilePath(const std::wstring& sPath)
    {
        m_sFilePath = sPath;
    }

    std::wstring GetFilePath()
    {
        return m_sFilePath;
    }
    bool IsFileDownloaded()
    {
        return m_bComplete;
    }
protected :

    virtual      DWORD ThreadProc ()
    {
        m_bComplete = false;

        if ( true )
        {
            int hrResultAll = DownloadFileAll(m_sFileUrl);

            if (0 != hrResultAll)
            {
                m_bRunThread = FALSE;
                return 0;
            }
        }

        m_bComplete = true;
        m_bRunThread = FALSE;
        return 0;
    }

    int DownloadFileAll(std::wstring sFileURL)
    {
        CURL *curl;
        int fp;
        CURLcode res;
        std::string sUrl = U_TO_UTF8(m_sFileUrl);
        std::string sOut;
        const char *url = sUrl.c_str();
        curl = curl_easy_init();
        if (curl)
        {
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
        {
            if (m_sFilePath.empty())
                m_sFilePath = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sOut.c_str(), (LONG)sOut.length());
            else
            {
                std::wstring sSrc = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sOut.c_str(), (LONG)sOut.length());
                NSFile::CFileBinary::Move(sSrc, m_sFilePath);
            }
        }
        return 0;
    }

    int createUniqueTempFile(std::string &filename)
    {
        std::wstring sTempPathDir = NSFile::CFileBinary::GetTempPath();
        std::string sTempPath = U_TO_UTF8(sTempPathDir);
        sTempPath += "/fileXXXXXX";
        int fd = mkstemp(const_cast <char *> (sTempPath.c_str()));
        if (-1 != fd)
            filename = sTempPath;
        return fd;
    }
    static size_t write_data(void *ptr, size_t size, size_t nmemb, int fd)
    {
        size_t written = write(fd, ptr, size * nmemb);
        return written;
    }

public:
    static bool IsNeedDownload(const std::wstring& FilePath)
    {
        int n1 = FilePath.find(L"www.");
        int n2 = FilePath.find(L"http://");
        int n3 = FilePath.find(L"ftp://");
        int n4 = FilePath.find(L"https://");

        if (n1 != std::wstring::npos && n1 < 10)
            return true;
        if (n2 != std::wstring::npos && n2 < 10)
            return true;
        if (n3 != std::wstring::npos && n3 < 10)
            return true;
        if (n4 != std::wstring::npos && n4 < 10)
            return true;

        return false;
    }

protected :

    std::wstring    m_sFilePath;       // Путь к сохраненному файлу на диске
    std::wstring    m_sFileUrl;        // Ссылка на скачивание файла

    bool            m_bComplete;       // Закачался файл или нет
    bool            m_bDelete;         // Удалять ли файл в деструкторе

};


#endif

#endif // CEF_FILE_DOWNLOADER
