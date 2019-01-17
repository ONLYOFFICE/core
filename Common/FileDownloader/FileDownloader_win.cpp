/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "FileDownloader_private.h"


#include <wininet.h>
#pragma comment(lib, "Wininet")
#pragma comment(lib, "Ole32.lib")

//------------------------------------------------------------------------------------------------------

// Константа для максимального числа символов в строке
#define	MAX_SIZE						256
// Константа для максимального числа загружаемых байт
#define DOWNLOAD_FILE_SIZE				32768
#define MAX_SINGLE_DOWNLOAD_FILE_SIZE 524288


// Константа для получения размера файла
#define CONTENT_RANGE		L"bytes 0-0/"
// Константа для колличества символов у CONTENT_RANGE
#define CONTENT_RANGE_SIZE	( 11/*sizeof ( CONTENT_RANGE )*/ - 1 )

class CFileDownloaderBaseWin : public CFileDownloaderBase
{
public :
    CFileDownloaderBaseWin(std::wstring sFileUrl, bool bDelete = true) :
        CFileDownloaderBase(sFileUrl, bDelete)
    {
        m_pFile     = NULL;
    }
    virtual ~CFileDownloaderBaseWin()
    {
        if ( m_pFile )
        {
            ::fclose( m_pFile );
            m_pFile = NULL;
        }
    }

    virtual int DownloadFile()
    {
        CoInitialize ( NULL );
        if ( /*S_OK != _DownloadFile ( m_sFileUrl )*/TRUE )
        {
            HRESULT hrResultAll = DownloadFileAll(m_sFileUrl, m_sFilePath);

            if (S_OK != hrResultAll)
            {
                CoUninitialize ();
                return S_FALSE;
            }
        }

        CoUninitialize ();
        m_bComplete = true;
        return S_OK;
    }

protected:
    FILE			*m_pFile;           // Хэндл на временный файл
    unsigned int _DownloadFile(std::wstring sFileUrl)
    {
        // Проверяем состояние соединения
        if ( FALSE == InternetGetConnectedState ( 0, 0 ) )
            return S_FALSE;

        wchar_t sTempPath[MAX_PATH], sTempFile[MAX_PATH];
        if ( 0 == GetTempPathW( MAX_PATH, sTempPath ) )
            return S_FALSE;

        if ( 0 == GetTempFileNameW( sTempPath, L"CSS", 0, sTempFile ) )
            return S_FALSE;

        m_pFile = ::_wfopen( sTempFile, L"wb" );
        if ( !m_pFile )
            return S_FALSE;

        m_sFilePath = std::wstring( sTempFile );

        // Открываем сессию
        HINTERNET hInternetSession = InternetOpenW ( L"Mozilla/4.0 (compatible; MSIE 5.0; Windows 98)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
        if ( NULL == hInternetSession )
            return S_FALSE;

        // Заголовок запроса ( пока содержит 0 байт ( необходимо для проверки ) )
        std::wstring sHTTPHdr = L"Range: bytes=0-0";
        // Открываем ссылку для проверки на ее существование, а также на возможность чтения частями
        HINTERNET hInternetOpenURL = InternetOpenUrlW ( hInternetSession, sFileUrl.c_str(), sHTTPHdr.c_str(), -1, INTERNET_FLAG_RESYNCHRONIZE, 0 );
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
                    while ( true )
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

                        NSThreads::Sleep(10);
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
        std::wstring sHTTPHdr = L"Range: bytes=" + std::to_wstring(nStartByte) + L"-" + std::to_wstring(nEndByte);
        // Открываем ссылку для закачки
        HINTERNET hInternetOpenURL = InternetOpenUrlW ( hInternet, sFileURL.c_str(), sHTTPHdr.c_str(), -1, INTERNET_FLAG_RESYNCHRONIZE, 0 );
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
        wchar_t arrResult [ MAX_SIZE ] = { 0 };
        // Размер данных ответа
        DWORD dwLengthDataSize = sizeof ( arrResult );

        // Делаем запрос, если не проходит - то возвращаем FALSE
        if ( FALSE == HttpQueryInfoW ( hInternet, HTTP_QUERY_CONTENT_RANGE, &arrResult, &dwLengthDataSize, NULL ) )
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

        // Приведем к std::wstring
        std::wstring strResult ( arrResult );

        // Содержит размер данных
        LONGLONG nFileSize = 0;

        try
        {
            // Ищем индекс размера данных в строке
            INT nStartIndex = strResult.find ( CONTENT_RANGE );
            if ( -1 == nStartIndex )
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

    HRESULT DownloadFileAll(std::wstring sFileURL, std::wstring strFileOutput)
    {
        if ( m_pFile )
        {
            ::fclose( m_pFile );
            m_pFile = NULL;
        }
        // Скачиваем файл
        return URLDownloadToFileW (NULL, sFileURL.c_str(), strFileOutput.c_str(), NULL, NULL);
    }
};

CFileDownloader_private::CFileDownloader_private(std::wstring sFileUrl, bool bDelete)
{
    m_pInternal = new CFileDownloaderBaseWin(sFileUrl, bDelete);
}
