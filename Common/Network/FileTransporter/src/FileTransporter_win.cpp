/*
 * (c) Copyright Ascensio System SIA 2010-2021
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

#include "FileTransporter_private.h"

#include <wininet.h>
#pragma comment(lib, "Wininet")
#pragma comment(lib, "Ole32.lib")

#ifdef GetTempPath
#undef GetTempPath
#endif

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

namespace NSNetwork
{
    namespace NSFileTransport
    {
        class CFileTransporterBaseWin : public CFileTransporterBase
        {
        public :
            CFileTransporterBaseWin(const std::wstring &sDownloadFileUrl, bool bDelete = true) :
                CFileTransporterBase(sDownloadFileUrl, bDelete)
            {
            }

            CFileTransporterBaseWin(const std::wstring &sUploadUrl, const unsigned char* cData, const int nSize) :
                CFileTransporterBase(sUploadUrl, cData, nSize)
            {
            }

            CFileTransporterBaseWin(const std::wstring &sUploadUrl, const std::wstring &sUploadFilePath) :
                CFileTransporterBase(sUploadUrl, sUploadFilePath)
            {
            }

            virtual ~CFileTransporterBaseWin()
            {
                if ( m_pFile )
                {
                    ::fclose( m_pFile );
                    m_pFile = NULL;
                }
            }

            virtual int DownloadFile() override
            {
                CoInitialize ( NULL );
                if ( /*S_OK != _DownloadFile ( m_sFileUrl )*/TRUE )
                {
                    if (m_sDownloadFilePath.empty())
                    {
                        m_sDownloadFilePath = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSFile::CFileBinary::GetTempPath(), L"DWD");
                        if (NSFile::CFileBinary::Exists(m_sDownloadFilePath))
                            NSFile::CFileBinary::Remove(m_sDownloadFilePath);
                    }

                    HRESULT hrResultAll = DownloadFileAll(m_sDownloadFileUrl, m_sDownloadFilePath);

                    if(E_ABORT == hrResultAll /*&& m_bIsExit->load()*/)
                    {
                        //DeleteUrlCacheEntry(m_sDownloadFileUrl.c_str());
                        CoUninitialize ();
                        return hrResultAll;
                    }
                    if (S_OK != hrResultAll)
                    {
                        hrResultAll = (true == DownloadFilePS(m_sDownloadFileUrl, m_sDownloadFilePath)) ? S_OK : S_FALSE;
                        CoUninitialize ();
                        return hrResultAll;
                    }
                }

                CoUninitialize ();
                m_bComplete = true;
                return S_OK;
            }

            virtual int UploadData() override
            {
                //stub
                return S_OK;
            }

            virtual int UploadFile() override
            {
                //stub
                return S_OK;
            }

        protected:
            FILE * m_pFile = nullptr;           // Хэндл на временный файл
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

                m_sDownloadFilePath = std::wstring( sTempFile );

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
                    INT nStartIndex = (INT)strResult.find ( CONTENT_RANGE );
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

                DownloadProgress progress;
                progress.func_checkAborted = m_check_aborted;
                progress.func_onProgress = m_func_onProgress;
                // Скачиваем файл с возвратом процентов состояния
                return URLDownloadToFileW (NULL, sFileURL.c_str(), strFileOutput.c_str(), NULL, static_cast<IBindStatusCallback*>(&progress));
            }            

            class DownloadProgress : public IBindStatusCallback {
            public:
                HRESULT __stdcall QueryInterface(const IID &,void **) {
                    return E_NOINTERFACE;
                }
                ULONG STDMETHODCALLTYPE AddRef(void) {
                    return 1;
                }
                ULONG STDMETHODCALLTYPE Release(void) {
                    return 1;
                }
                HRESULT STDMETHODCALLTYPE OnStartBinding(DWORD dwReserved, IBinding *pib) {
                    return E_NOTIMPL;
                }
                virtual HRESULT STDMETHODCALLTYPE GetPriority(LONG *pnPriority) {
                    return E_NOTIMPL;
                }
                virtual HRESULT STDMETHODCALLTYPE OnLowResource(DWORD reserved) {
                    return S_OK;
                }
                virtual HRESULT STDMETHODCALLTYPE OnStopBinding(HRESULT hresult, LPCWSTR szError) {
                    return E_NOTIMPL;
                }
                virtual HRESULT STDMETHODCALLTYPE GetBindInfo(DWORD *grfBINDF, BINDINFO *pbindinfo) {
                    return E_NOTIMPL;
                }
                virtual HRESULT STDMETHODCALLTYPE OnDataAvailable(DWORD grfBSCF, DWORD dwSize, FORMATETC *pformatetc, STGMEDIUM *pstgmed) {
                    return E_NOTIMPL;
                }
                virtual HRESULT STDMETHODCALLTYPE OnObjectAvailable(REFIID riid, IUnknown *punk) {
                    return E_NOTIMPL;
                }

                virtual HRESULT __stdcall OnProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode, LPCWSTR szStatusText)
                {
                    if(func_checkAborted && func_checkAborted())
                    {
                        return E_ABORT;
                    }
                    if(ulProgressMax != 0)
                    {
                        int percent = static_cast<int>((100.0 * ulProgress) / ulProgressMax);
                        if(func_onProgress)
                            func_onProgress(percent);
                    }
                    return S_OK;
                }

                std::function<bool(void)> func_checkAborted = nullptr;
                std::function<void(int)> func_onProgress = nullptr;
            };


            bool DownloadFilePS(const std::wstring& sFileURL, const std::wstring& strFileOutput)
            {
                STARTUPINFO sturtupinfo;
                ZeroMemory(&sturtupinfo,sizeof(STARTUPINFO));
                sturtupinfo.cb = sizeof(STARTUPINFO);

                std::wstring sFileDst = strFileOutput;
                size_t posn = 0;
                while (std::wstring::npos != (posn = sFileDst.find('\\', posn)))
                {
                    sFileDst.replace(posn, 1, L"/");
                    posn += 1;
                }

                std::wstring sApp = L"powershell.exe –c \"(new-object System.Net.WebClient).DownloadFile('" + sFileURL + L"','" + sFileDst + L"')\"";
                wchar_t* pCommandLine = new wchar_t[sApp.length() + 1];
                memcpy(pCommandLine, sApp.c_str(), sApp.length() * sizeof(wchar_t));
                pCommandLine[sApp.length()] = (wchar_t)'\0';

                HANDLE ghJob = CreateJobObject(NULL, NULL);

                if (ghJob)
                {
                    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };

                    // Configure all child processes associated with the job to terminate when the
                    jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
                    if ( 0 == SetInformationJobObject( ghJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli)))
                    {
                        CloseHandle(ghJob);
                        ghJob = NULL;
                    }
                }

                PROCESS_INFORMATION processinfo;
                ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
                BOOL bResult = CreateProcessW(NULL, pCommandLine, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sturtupinfo, &processinfo);

                if (bResult && ghJob)
                {
                    AssignProcessToJobObject(ghJob, processinfo.hProcess);
                }

                ::WaitForSingleObject(processinfo.hProcess, INFINITE);

                RELEASEARRAYOBJECTS(pCommandLine);

                return NSFile::CFileBinary::Exists(sFileDst);
            }
        };

        CFileTransporter_private::CFileTransporter_private(const std::wstring &sDownloadFileUrl, bool bDelete)
            : m_pInternal(new CFileTransporterBaseWin(sDownloadFileUrl, bDelete))
        {
            m_pInternal->m_check_aborted = std::bind(&CBaseThread::isAborted, this);
        }

        CFileTransporter_private::CFileTransporter_private(const std::wstring &sUploadUrl, const unsigned char* cData, const int nSize)
            : m_pInternal(new CFileTransporterBaseWin(sUploadUrl, cData, nSize))
        {
            m_pInternal->m_check_aborted = std::bind(&CBaseThread::isAborted, this);
        }

        CFileTransporter_private::CFileTransporter_private(const std::wstring &sUploadUrl, const std::wstring &sUploadFilePath)
            : m_pInternal(new CFileTransporterBaseWin(sUploadUrl, sUploadFilePath))
        {
            m_pInternal->m_check_aborted = std::bind(&CBaseThread::isAborted, this);
        }
    }
}
