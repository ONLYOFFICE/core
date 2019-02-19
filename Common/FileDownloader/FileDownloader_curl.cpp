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

#include <iostream>
#include <unistd.h>
#include "../../DesktopEditor/common/Directory.h"

#ifndef USE_EXTERNAL_DOWNLOAD

#include <fcntl.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>

#else

#include <unistd.h>
#include <sys/wait.h>

int download_external(const std::wstring& sUrl, const std::wstring& sOutput)
{
    int nReturnCode = -1;

    std::string sUrlA = U_TO_UTF8(sUrl);
    //sUrlA =("\"" + sUrlA + "\"");
    std::string sOutputA = U_TO_UTF8(sOutput);
    //sOutputA =("\"" + sOutputA + "\"");

    if (0 != nReturnCode && NSFile::CFileBinary::Exists(L"/usr/bin/curl"))
    {
        pid_t pid = fork(); // create child process
        int status;

        switch (pid)
        {
        case -1: // error
            break;

        case 0: // child process
        {
            const char* nargs[6];
            nargs[0] = "/usr/bin/curl";
            nargs[1] = sUrlA.c_str();
            nargs[2] = "--output";
            nargs[3] = sOutputA.c_str();
            nargs[4] = "--silent";
            nargs[5] = NULL;

            const char* nenv[3];
            nenv[0] = "LD_PRELOAD=";
            nenv[1] = "LD_LIBRARY_PATH=";
            nenv[2] = NULL;

            execve("/usr/bin/curl", (char * const *)nargs, (char * const *)nenv);
            exit(EXIT_SUCCESS);
            break;
        }
        default: // parent process, pid now contains the child pid
            while (-1 == waitpid(pid, &status, 0)); // wait for child to complete
            if (WIFEXITED(status))
            {
                nReturnCode =  WEXITSTATUS(status);
            }
            break;
        }
    }

    if (0 != nReturnCode && NSFile::CFileBinary::Exists(L"/usr/bin/wget"))
    {
        pid_t pid = fork(); // create child process
        int status;

        switch (pid)
        {
        case -1: // error
            break;

        case 0: // child process
        {
            const char* nargs[6];
            nargs[0] = "/usr/bin/wget";
            nargs[1] = sUrlA.c_str();
            nargs[2] = "-O";
            nargs[3] = sOutputA.c_str();
            nargs[4] = "-q";
            nargs[5] = NULL;

            const char* nenv[2];
            nenv[0] = "LD_PRELOAD=";
            nenv[1] = NULL;

            execve("/usr/bin/wget", (char * const *)nargs, (char * const *)nenv);
            exit(EXIT_SUCCESS);
            break;
        }
        default: // parent process, pid now contains the child pid
            while (-1 == waitpid(pid, &status, 0)); // wait for child to complete
            if (WIFEXITED(status))
            {
                nReturnCode =  WEXITSTATUS(status);
            }
            break;
        }
    }

    if (0 == nReturnCode)
    {
        if (!NSFile::CFileBinary::Exists(sOutput))
            nReturnCode = -1;
    }

    return nReturnCode;
}

#endif

class CFileDownloaderBaseCURL : public CFileDownloaderBase
{
public :
    CFileDownloaderBaseCURL(std::wstring sFileUrl, bool bDelete = true)
        : CFileDownloaderBase(sFileUrl, bDelete)
    {
    }
    virtual ~CFileDownloaderBaseCURL()
    {
        if (m_bDelete && !m_sFilePath.empty())
        {
            std::string sFilePath = U_TO_UTF8(m_sFilePath);
            unlink(sFilePath.c_str());
        }
    }

#ifndef USE_EXTERNAL_DOWNLOAD
    static size_t write_data(void *ptr, size_t size, size_t nmemb, int fd) {
        size_t written = write(fd, ptr, size * nmemb);
        return written;
    }

    virtual int DownloadFile()
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
            if (m_sFilePath.empty())
                m_sFilePath = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sOut.c_str(), sOut.length());
            else
                NSFile::CFileBinary::Move(UTF8_TO_U(sOut), m_sFilePath);
        }
        //int nRes = execl("/usr/bin/wget", stringWstingToUtf8String (m_sFileUrl).c_str(), "-P", stringWstingToUtf8String (m_sFilePath).c_str(), (char *)NULL);
        //m_bComplete = nRes >= 0;

        return m_bComplete ? 0 : 1;
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
    virtual int DownloadFile()
    {
        if (m_sFilePath.empty())
        {
            m_sFilePath = NSFile::CFileBinary::CreateTempFileWithUniqueName(NSDirectory::GetTempPath(), L"DW");
            if (NSFile::CFileBinary::Exists(m_sFilePath))
                NSFile::CFileBinary::Remove(m_sFilePath);
        }
        return download_external(m_sFileUrl, m_sFilePath);
    }

#endif
};

CFileDownloader_private::CFileDownloader_private(std::wstring sFileUrl, bool bDelete)
{
    m_pInternal = new CFileDownloaderBaseCURL(sFileUrl, bDelete);
}
