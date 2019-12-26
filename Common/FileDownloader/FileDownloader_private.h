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
#pragma once

#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/graphics/BaseThread.h"
#include "./FileDownloader.h"

class CFileDownloaderBase
{
public :
    CFileDownloaderBase(std::wstring sFileUrl, bool bDelete = true)
    {
        m_sFilePath = L"";
        m_sFileUrl  = sFileUrl;
        m_bComplete = false;
        m_bDelete   = bDelete;

        m_func_onComplete = NULL;
        m_func_onProgress = NULL;
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

    CFileDownloader_OnComplete m_func_onComplete;
    CFileDownloader_OnProgress m_func_onProgress;
};

class CFileDownloader_private : public NSThreads::CBaseThread
{
protected:
    // создаем в зависимости от платформы
    CFileDownloaderBase* m_pInternal;

public:
    CFileDownloaderBase* GetInternal()
    {
        return m_pInternal;
    }

public:
    CFileDownloader_private(std::wstring sFileUrl, bool bDelete = true);
    virtual ~CFileDownloader_private()
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

        int hrResultAll = m_pInternal->DownloadFile();
        if (0 == hrResultAll)
            m_pInternal->m_bComplete = true;

        if (m_pInternal->m_func_onComplete)
            m_pInternal->m_func_onComplete(hrResultAll);

        m_bRunThread = FALSE;
        return 0;
    }
};
