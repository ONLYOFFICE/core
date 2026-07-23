/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "FileTransporter_private.h"
#include "../../../../DesktopEditor/common/StringExt.h"

#include <wininet.h>
#include <objbase.h>
#include <urlmon.h>
#pragma comment(lib, "Wininet")
#pragma comment(lib, "Ole32.lib")

#ifdef GetTempPath
#undef GetTempPath
#endif

//------------------------------------------------------------------------------------------------------

// Constant for maximum number of characters in a string
#define	MAX_SIZE						256
// Constant for maximum number of downloadable bytes
#define DOWNLOAD_FILE_SIZE				32768
#define MAX_SINGLE_DOWNLOAD_FILE_SIZE 524288


// Constant for getting file size
#define CONTENT_RANGE		L"bytes 0-0/"
// Constant for CONTENT_RANGE character count
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
			FILE * m_pFile = nullptr;           // Handle to temporary file
			unsigned int _DownloadFile(std::wstring sFileUrl)
			{
				// Check connection state
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

				// Open session
				HINTERNET hInternetSession = InternetOpenW ( L"Mozilla/4.0 (compatible; MSIE 5.0; Windows 98)", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
				if ( NULL == hInternetSession )
					return S_FALSE;

				// Request header (currently contains 0 bytes, needed for verification)
				std::wstring sHTTPHdr = L"Range: bytes=0-0";
				// Open URL to check its existence and ability to read in chunks
				HINTERNET hInternetOpenURL = InternetOpenUrlW ( hInternetSession, sFileUrl.c_str(), sHTTPHdr.c_str(), -1, INTERNET_FLAG_RESYNCHRONIZE, 0 );
				if ( NULL != hInternetOpenURL )
				{
					// Opened successfully, check response
					if ( TRUE == QueryStatusCode ( hInternetOpenURL, TRUE ) )
					{
						// Request succeeded, check ability to read in chunks and get data size
						LONGLONG nFileSize = IsAccept_Ranges ( hInternetOpenURL );
						// Close handle
						InternetCloseHandle ( hInternetOpenURL );
						if ( -1 == nFileSize )
						{
							// Reading in chunks is not available
							// Close connection handle
							InternetCloseHandle ( hInternetSession );
							// Close file (will be done in DownloadAll)
							// Try to write it entirely
							return S_FALSE;
						}
						else
						{
							// Reading in chunks is available
							LONGLONG nStartByte = 0;
							while ( true )
							{
								// If entire file is downloaded - exit
								if ( nStartByte == nFileSize - 1 )
								{
									// Close connection handle
									InternetCloseHandle ( hInternetSession );
									return S_OK;
								}
								LONGLONG nEndByte = nStartByte + DOWNLOAD_FILE_SIZE;
								// If file is ending, download fewer bytes (1 less than size, since it starts from 0)
								if ( nEndByte >= nFileSize )
									nEndByte = nFileSize - 1;

								// Download buffer
								BYTE arrBuffer [ DOWNLOAD_FILE_SIZE ] = { 0 };
								DWORD dwBytesDownload = DownloadFilePath ( hInternetSession, arrBuffer, nStartByte, nEndByte, sFileUrl );

								nStartByte = nEndByte;
								if ( -1 == dwBytesDownload )
								{
									// Nothing was read - this is bad!!!!
									// Close connection handle
									InternetCloseHandle ( hInternetSession );
									// Close file (will be done in DownloadAll)
									// Try to write it entirely
									return S_FALSE;
								}

								// Write to file
								::fwrite( (BYTE*)arrBuffer, 1, dwBytesDownload, m_pFile );
								::fflush( m_pFile );

								NSThreads::Sleep(10);
							}
						}
					}
					else
					{
						// Close connection handle
						InternetCloseHandle ( hInternetSession );
						// Close file (will be done in DownloadAll)
						// Try to write it entirely
						return S_FALSE;
					}
				}
				else
				{
					// Close connection handle
					InternetCloseHandle ( hInternetSession );
					// Close file (will be done in DownloadAll)
					// Try to write it entirely
					return S_FALSE;
				}

				// Close connection handle
				InternetCloseHandle ( hInternetSession );

				return S_OK;
			}
			DWORD DownloadFilePath ( HINTERNET hInternet, LPBYTE pBuffer, LONGLONG nStartByte, LONGLONG nEndByte, std::wstring sFileURL )
			{
				// Session not opened
				if ( NULL == hInternet )
					return -1;

				// Invalid parameters received
				if ( nStartByte > nEndByte || !pBuffer )
					return -1;

				// Request header (contains nEndByte - nStartByte bytes)
				std::wstring sHTTPHdr = L"Range: bytes=" + std::to_wstring(nStartByte) + L"-" + std::to_wstring(nEndByte);
				// Open URL for download
				HINTERNET hInternetOpenURL = InternetOpenUrlW ( hInternet, sFileURL.c_str(), sHTTPHdr.c_str(), -1, INTERNET_FLAG_RESYNCHRONIZE, 0 );
				if ( NULL == hInternetOpenURL )
					return -1;
				// Opened successfully, check response
				if ( FALSE == QueryStatusCode ( hInternetOpenURL, TRUE ) )
				{
					// Close connection handle
					InternetCloseHandle ( hInternetOpenURL );
					return -1;
				}

				// Number of bytes read
				DWORD dwBytesRead = 0;
				// Read file
				if ( FALSE == InternetReadFile ( hInternetOpenURL, pBuffer, DOWNLOAD_FILE_SIZE, &dwBytesRead ) )
				{
					// Close connection handle
					InternetCloseHandle ( hInternetOpenURL );
					return -1;
				}

				// Close connection handle
				InternetCloseHandle ( hInternetOpenURL );

				return dwBytesRead;
			}

			BOOL QueryStatusCode ( HINTERNET hInternet, BOOL bIsRanges )
			{
				// Why check anything for an unopened session
				if ( NULL == hInternet )
					return FALSE;

				// Response result
				INT nResult = 0;
				// Response data size (should be = 4)
				DWORD dwLengthDataSize = 4;

				// Make request, if it fails - return FALSE
				if ( FALSE == HttpQueryInfo ( hInternet, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &nResult, &dwLengthDataSize, NULL ) )
					return FALSE;

				// Request succeeded, now check response code
				if ( HTTP_STATUS_NOT_FOUND == nResult )
				{
					// Object not found, bad link or something else
					return FALSE;
				}
				else if ( ( HTTP_STATUS_OK != nResult && FALSE == bIsRanges ) || ( HTTP_STATUS_PARTIAL_CONTENT != nResult && TRUE == bIsRanges ) )
				{
					// Request failed for some reason
					return FALSE;
				}

				// Everything is fine, request succeeded
				return TRUE;
			}
			// Checks if resource supports reading in chunks and returns -1 if not available, or data size if available
			LONGLONG IsAccept_Ranges ( HINTERNET hInternet )
			{
				// Why check anything for an unopened session
				if ( NULL == hInternet )
					return -1;

				// Response result
				wchar_t arrResult [ MAX_SIZE ] = { 0 };
				// Response data size
				DWORD dwLengthDataSize = sizeof ( arrResult );

				// Make request, if it fails - return FALSE
				if ( FALSE == HttpQueryInfoW ( hInternet, HTTP_QUERY_CONTENT_RANGE, &arrResult, &dwLengthDataSize, NULL ) )
				{
					// Get last error
					DWORD dwLastError = GetLastError ();
					if ( dwLastError == ERROR_HTTP_HEADER_NOT_FOUND )
					{
						// Header not received, resource doesn't support reading in chunks
						return -1;
					}

					// Some other error occurred - return FALSE
					return -1;
				}

				// If size is 0, there is no header
				if ( 0 >= dwLengthDataSize )
					return -1;

				// Convert to std::wstring
				std::wstring strResult ( arrResult );

				// Contains data size
				LONGLONG nFileSize = 0;

				try
				{
					// Find data size index in string
					INT nStartIndex = (INT)strResult.find ( CONTENT_RANGE );
					if ( -1 == nStartIndex )
						return -1;

					// Leave only data size in string
					strResult = strResult.substr( nStartIndex + CONTENT_RANGE_SIZE );
					// Now get the data size by converting string to LONGLONG
					nFileSize = _wtoi64 ( strResult.c_str() );
					// Since numbering starts from 0 (so we add 1 more byte)
					if ( 0 < nFileSize )
						nFileSize += 1;
				}
				catch ( ... )
				{
					// not found, return error
					return -1;
				}

				// All good, resource supports partial reading, return size
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
				// Download file with progress percentage callback
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

			void EscapeQuotesPS(std::wstring& command, bool isPath)
			{
				/*
				var symbols = [0x22, 0x27, 0x2018, 0x2019, 0x201a, 0x201b, 0x201c, 0x201d, 0x201e, 0x201f];
				var output = "";
				for (let i = 0; i < symbols.length; i++) output += (" " + encodeURI(String.fromCharCode(symbols[i])));
				console.log(output);

				result:
				"%22 %27 %E2%80%98 %E2%80%99 %E2%80%9A %E2%80%9B %E2%80%9C %E2%80%9D %E2%80%9E %E2%80%9F"
				*/

				std::wstring sTmp = L" ";

				if (isPath)
				{
					sTmp[0] = (wchar_t)'\\';   NSStringExt::Replace(command, sTmp, L"/");
				}

				sTmp[0] = (wchar_t)0x22;   NSStringExt::Replace(command, sTmp, L"%22");
				sTmp[0] = (wchar_t)0x27;   NSStringExt::Replace(command, sTmp, L"%27");
				sTmp[0] = (wchar_t)0x2018; NSStringExt::Replace(command, sTmp, L"%E2%80%98");
				sTmp[0] = (wchar_t)0x2019; NSStringExt::Replace(command, sTmp, L"%E2%80%99");
				sTmp[0] = (wchar_t)0x201a; NSStringExt::Replace(command, sTmp, L"%E2%80%9A");
				sTmp[0] = (wchar_t)0x201b; NSStringExt::Replace(command, sTmp, L"%E2%80%9B");
				sTmp[0] = (wchar_t)0x201c; NSStringExt::Replace(command, sTmp, L"%E2%80%9C");
				sTmp[0] = (wchar_t)0x201d; NSStringExt::Replace(command, sTmp, L"%E2%80%9D");
				sTmp[0] = (wchar_t)0x201e; NSStringExt::Replace(command, sTmp, L"%E2%80%9E");
				sTmp[0] = (wchar_t)0x201f; NSStringExt::Replace(command, sTmp, L"%E2%80%9F");
			}

			bool DownloadFilePS(const std::wstring& sFileURLOriginal, const std::wstring& strFileOutput)
			{
				STARTUPINFO sturtupinfo;
				ZeroMemory(&sturtupinfo,sizeof(STARTUPINFO));
				sturtupinfo.cb = sizeof(STARTUPINFO);

				std::wstring sFileDst = strFileOutput;
				std::wstring sFileURL = sFileURLOriginal;

				EscapeQuotesPS(sFileDst, true);
				EscapeQuotesPS(sFileURL, false);

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
