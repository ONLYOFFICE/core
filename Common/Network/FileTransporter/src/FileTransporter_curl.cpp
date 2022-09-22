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

#include <iostream>
#include <unistd.h>
#include "../../DesktopEditor/common/Directory.h"

#ifndef USE_EXTERNAL_TRANSPORT

#include <fcntl.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>

#else

#include "transport_external.h"

#endif

namespace NSNetwork
{
    namespace NSFileTransport
    {
        class CFileTransporterBaseCURL : public CFileTransporterBase
        {
        public :
            CFileTransporterBaseCURL(const std::wstring &sDownloadFileUrl, bool bDelete = true)
                : CFileTransporterBase(sDownloadFileUrl, bDelete)
            {
            }
            CFileTransporterBaseCURL(const std::wstring &sUploadUrl, const unsigned char* cData, const int nSize)
                : CFileTransporterBase(sUploadUrl, cData, nSize)
            {
            }
            CFileTransporterBaseCURL(const std::wstring &sUploadUrl, const std::wstring &sUploadFilePath)
                : CFileTransporterBase(sUploadUrl, sUploadFilePath)
            {

            }
            virtual ~CFileTransporterBaseCURL()
            {
                if (m_bDelete && !m_sDownloadFilePath.empty())
                {
                    std::string sFilePath = U_TO_UTF8(m_sDownloadFilePath);
                    unlink(sFilePath.c_str());
                }
            }

        #ifndef USE_EXTERNAL_TRANSPORT
            static size_t write_data(void *ptr, size_t size, size_t nmemb, int fd) {
                size_t written = write(fd, ptr, size * nmemb);
                return written;
            }

            /*int progress_func(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
            {
                // It's here you will write the code for the progress message or bar
                int percent = static_cast<int>((100.0 * NowDownloaded) / TotalToDownload);

                if(CFileTransporterBase::m_func_onProgress)
                    CFileTransporterBase::m_func_onProgress(percent);
                return 0;
            }
            */


            static size_t write_data_to_string(char *contents, size_t size, size_t nmemb, void *userp)
            {
                ((std::string*)userp)->append((char*)contents, size * nmemb);
                return size * nmemb;
            }

            virtual int DownloadFile() override
            {
                CURL *curl;
                int fp;
                CURLcode res;
                std::string sUrl = U_TO_UTF8(m_sDownloadFileUrl);
                std::string sOut;
                const char *url = sUrl.c_str();
                curl = curl_easy_init();
                if (curl)
                {
                   fp = createUniqueTempFile(sOut);
                   curl_easy_setopt(curl, CURLOPT_URL, url);
                   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                   curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
                   //curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
                   // Install the callback function
                   //curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progress_func);
        #if defined(__linux__)
                   //в linux нет встроенных в систему корневых сертификатов, поэтому отключаем проверку
                   //http://curl.haxx.se/docs/sslcerts.html
                   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        #endif
                   /* tell libcurl to follow redirection(default false) */
                   curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                   /* some servers don't like requests that are made without a user-agent field, so we provide one */
                   curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:38.0) Gecko/20100101 Firefox/38.0");
                   res = curl_easy_perform(curl);
                   /* always cleanup */
                   curl_easy_cleanup(curl);
                   close(fp);
                }

                m_bComplete = (CURLE_OK == res);
                if (m_bComplete)
                {
                    if (m_sDownloadFilePath.empty())
                        m_sDownloadFilePath = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sOut.c_str(), sOut.length());
                    else
                        NSFile::CFileBinary::Move(UTF8_TO_U(sOut), m_sDownloadFilePath);
                }
                //int nRes = execl("/usr/bin/wget", stringWstingToUtf8String (m_sFileUrl).c_str(), "-P", stringWstingToUtf8String (m_sDownloadFilePath).c_str(), (char *)NULL);
                //m_bComplete = nRes >= 0;

                return m_bComplete ? 0 : 1;
            }

            virtual int UploadData() override
            {
                CURL *curl;
                CURLcode res;
                std::string sUrl = U_TO_UTF8(m_sUploadUrl);
                const char *url = sUrl.c_str();
                struct curl_slist *headerlist = NULL;
                std::string readBuffer;

                /* get a curl handle */
                curl = curl_easy_init();
                if(curl) {

                    headerlist = curl_slist_append(headerlist, "Content-Type: application/octet-stream");

                    curl_easy_setopt(curl, CURLOPT_POST, true);
                    curl_easy_setopt(curl, CURLOPT_HEADER, true);
                    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
                    /* First set the URL that is about to receive our POST. This URL can
                       just as well be a https:// URL if that is what should receive the
                       data. */
                    curl_easy_setopt(curl, CURLOPT_URL, url);
                    /* Now specify the POST data */
                    /* size of the POST data */
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, m_nSize);
                    /* binary data */
                    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_cData);

                    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data_to_string);
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        #if defined(__linux__)
                    //в linux нет встроенных в систему корневых сертификатов, поэтому отключаем проверку
                   //http://curl.haxx.se/docs/sslcerts.html
                   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        #endif

                    /* Perform the request, res will get the return code */
                    res = curl_easy_perform(curl);

                    long http_code = 0;
                    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

                    if (res == CURLE_OK)
                    {
                        if (http_code == 200 || http_code == 1223)
                        {
                            size_t startLenghtPos = readBuffer.find("Content-Length:") + sizeof("Content-Length:");
                            size_t endLenghtPos = readBuffer.substr(startLenghtPos, readBuffer.length()).find("\r");
                            std::string dataSize = readBuffer.substr(startLenghtPos, endLenghtPos);
                            readBuffer = readBuffer.substr(readBuffer.length() - std::stoi(dataSize));
                            NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)readBuffer.c_str(), (LONG)readBuffer.length(), m_sResponse);
                        }
                        else
                        {
                            res = CURLE_HTTP_RETURNED_ERROR;
                        }
                    }

                    /* always cleanup */
                    curl_easy_cleanup(curl);
                }
                m_bComplete = (CURLE_OK == res);

                return m_bComplete ? 0 : 1;
            }

            virtual int UploadFile() override
            {
                //stub
                return -1;
            }

        protected:
            int createUniqueTempFile (std::string &filename)
            {
                std::string sTempPath = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(NSDirectory::GetTempPath());
                sTempPath += "/fileXXXXXX";
                int fd = mkstemp(const_cast <char *> (sTempPath.c_str()));
                if (-1 != fd)
                    filename = sTempPath;
                return fd;
            }
        #else
            virtual int DownloadFile() override
            {
                if (m_sDownloadFilePath.empty())
                {
                    m_sDownloadFilePath = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSDirectory::GetTempPath(), L"DW");
                    if (NSFile::CFileBinary::Exists(m_sDownloadFilePath))
                        NSFile::CFileBinary::Remove(m_sDownloadFilePath);
                }
                return download_external(m_sDownloadFileUrl, m_sDownloadFilePath, m_func_onProgress, m_check_aborted);
            }
            virtual int UploadData() override
            {
                if (!m_sUploadUrl.empty() && m_cData != NULL && m_nSize != 0)
                {
                     return uploaddata_external(m_sUploadUrl, m_cData, m_nSize);
                }
                return -1;
            }
            virtual int UploadFile() override
            {
                if (!m_sUploadUrl.empty() && !m_sUploadFilePath.empty())
                {
                     return uploadfile_external(m_sUploadUrl, m_sUploadFilePath);
                }
                return -1;
            }

        #endif
        };

        CFileTransporter_private::CFileTransporter_private(const std::wstring &sDownloadFileUrl, bool bDelete)
            : m_pInternal(new CFileTransporterBaseCURL(sDownloadFileUrl, bDelete))

        {
            m_pInternal->m_check_aborted = std::bind(&CBaseThread::isAborted, this);
        }

        CFileTransporter_private::CFileTransporter_private(const std::wstring &sUploadUrl, const unsigned char* cData, const int nSize)
            : m_pInternal(new CFileTransporterBaseCURL(sUploadUrl, cData, nSize))
        {
            m_pInternal->m_check_aborted = std::bind(&CBaseThread::isAborted, this);
        }

        CFileTransporter_private::CFileTransporter_private(const std::wstring &sUploadUrl, const std::wstring &sUploadFilePath)
            : m_pInternal(new CFileTransporterBaseCURL(sUploadUrl, sUploadFilePath))
        {
            m_pInternal->m_check_aborted = std::bind(&CBaseThread::isAborted, this);
        }
    }
}
