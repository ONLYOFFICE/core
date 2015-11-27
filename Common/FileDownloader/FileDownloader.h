#pragma once

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/graphics/BaseThread.h"

class CFileDownloaderBase
{
public :
    CFileDownloaderBase(std::wstring sFileUrl, bool bDelete = true)
    {
        m_sFilePath = L"";
        m_sFileUrl  = sFileUrl;
        m_bComplete = false;
        m_bDelete   = bDelete;
    }
    virtual ~CFileDownloaderBase ()
    {
        if ( m_sFilePath.length() > 0 && m_bDelete )
        {
            NSFile::CFileBinary::Remove(m_sFilePath);
            m_sFilePath = L"";
        }
    }

    virtual int DownloadFile() = 0;

public:
    std::wstring    m_sFilePath;       // Путь к сохраненному файлу на диске
    std::wstring    m_sFileUrl;        // Ссылка на скачивание файла

    bool            m_bComplete;       // Закачался файл или нет
    bool            m_bDelete;         // Удалять ли файл в деструкторе

};

class CFileDownloader : public NSThreads::CBaseThread
{
protected:
    // создаем в зависимости от платформы
    CFileDownloaderBase* m_pInternal;

public:
    CFileDownloader(std::wstring sFileUrl, bool bDelete = true);
    virtual ~CFileDownloader()
    {
        Stop();
        if (NULL != m_pInternal)
            delete m_pInternal;
    }

    void SetFilePath(const std::wstring& sPath)
    {
        m_pInternal->m_sFilePath = sPath;
    }
    std::wstring GetFilePath()
    {
        return m_pInternal->m_sFilePath;
    }
    bool IsFileDownloaded()
    {
        return m_pInternal->m_bComplete;
    }

    bool DownloadSync()
    {
        this->Start( 1 );
        while ( this->IsRunned() )
        {
            NSThreads::Sleep( 10 );
        }
        return IsFileDownloaded();
    }

protected :

    virtual DWORD ThreadProc ()
    {
        m_pInternal->m_bComplete = false;

        if ( true )
        {
            int hrResultAll = m_pInternal->DownloadFile();

            if (0 != hrResultAll)
            {
                m_bRunThread = FALSE;
                return 0;
            }
        }

        m_pInternal->m_bComplete = true;
        m_bRunThread = FALSE;
        return 0;
    }
};
