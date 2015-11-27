#include "FileDownloader.h"

#include <iostream>
#include <unistd.h>
#include "./../DocxFormat/Source/Base/ASCString.h"
#include "./../DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include<fcntl.h>
#include <string>

#include <stdio.h>

#include "../3dParty/curl/include/curl/curl.h"
#include "../3dParty/curl/include/curl/easy.h"


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
            m_sFilePath = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8((BYTE*)sOut.c_str(), sOut.length());
        //int nRes = execl("/usr/bin/wget", stringWstingToUtf8String (m_sFileUrl).c_str(), "-P", stringWstingToUtf8String (m_sFilePath).c_str(), (char *)NULL);
        //m_bComplete = nRes >= 0;

        return m_bComplete ? 0 : 1;
    }

protected:
    int createUniqueTempFile (std::string &filename)
    {
        std::string sTempPath = stringWstingToUtf8String (FileSystem::Directory::GetTempPath());
        sTempPath += "/fileXXXXXX";
        int fd = mkstemp(const_cast <char *> (sTempPath.c_str()));
        if (-1 != fd)
            filename = sTempPath;
        return fd;
    }
};

CFileDownloader::CFileDownloader(std::wstring sFileUrl, bool bDelete)
{
    m_pInternal = new CFileDownloaderBaseCURL(sFileUrl, bDelete);
}
