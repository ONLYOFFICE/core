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
#pragma once

#include "./FileTransporter_private.h"
#include "../include/FileTransporter.h"

namespace NSNetwork
{
    namespace NSFileTransport
    {
#ifdef _MAC
        bool m_bIsARCEnabled = false;

        void SetARCEnabled(const bool& enabled)
        {
            m_bIsARCEnabled = enabled;
        }
        bool GetARCEnabled()
        {
            return m_bIsARCEnabled;
        }
#endif
    }
}

namespace NSNetwork
{
    namespace NSFileTransport
    {
        // DOWNLOADER
        CFileDownloader::CFileDownloader(std::wstring sFileUrl, bool bDelete)
        {
            m_pInternal = new CFileTransporter_private(sFileUrl, bDelete);
        }
        CFileDownloader::~CFileDownloader()
        {
            Stop();
            delete m_pInternal;
        }

        void CFileDownloader::SetFilePath(const std::wstring &sFilePath)
        {
            m_pInternal->SetDownloadFilePath(sFilePath);
        }
        std::wstring CFileDownloader::GetFilePath()
        {
            return m_pInternal->GetDownloadFilePath();
        }

        bool CFileDownloader::IsFileDownloaded()
        {
            return m_pInternal->IsFileDownloaded();
        }

        void CFileDownloader::SetFileUrl(const std::wstring &sFileUrl, bool bDelete)
        {
            m_pInternal->SetDownloadFileUrl(sFileUrl, bDelete);
        }

        bool CFileDownloader::DownloadSync()
        {
            return m_pInternal->TransferSync();
        }
        void CFileDownloader::DownloadAsync()
        {
            m_pInternal->TransferAsync();
        }

        void CFileDownloader::Start(int lPriority) { m_pInternal->Start(lPriority); }
        int CFileDownloader::GetPriority() { return m_pInternal->GetPriority(); }
        void CFileDownloader::Suspend() { m_pInternal->Suspend(); }
        void CFileDownloader::Resume() { m_pInternal->Resume(); }
        void CFileDownloader::Stop() { m_pInternal->Stop(); }
        void CFileDownloader::StopNoJoin() { m_pInternal->StopNoJoin(); }
        void CFileDownloader::Cancel() { m_pInternal->Cancel(); }
        int CFileDownloader::IsRunned() { return m_pInternal->IsRunned(); }

        void CFileDownloader::SetEvent_OnProgress(std::function<void(int)> func)
        {
            m_pInternal->GetInternal()->m_func_onProgress = func;
        }
        void CFileDownloader::SetEvent_OnComplete(std::function<void(int)> func)
        {
            m_pInternal->GetInternal()->m_func_onComplete = func;
        }
    }
}

namespace NSNetwork
{
    namespace NSFileTransport
    {
        CFileUploader::CFileUploader(std::wstring sUrl, const unsigned char* cData, const int nSize)
        {
            m_pInternal = new CFileTransporter_private(sUrl, cData, nSize);
        }
        CFileUploader::CFileUploader(std::wstring sUrl, std::wstring sFilePath)
        {
            m_pInternal = new CFileTransporter_private(sUrl, sFilePath);
        }
        CFileUploader::~CFileUploader()
        {
            Stop();
            delete m_pInternal;
        }

        void CFileUploader::SetUrl(const std::wstring& sUrl)
        {
            m_pInternal->SetUploadUrl(sUrl);
        }
        void CFileUploader::SetBinaryData(const unsigned char* data, const int size)
        {
            m_pInternal->SetUploadBinaryDara(data, size);
        }
        void CFileUploader::SetFilePath(const std::wstring &sFilePath)
        {
            m_pInternal->SetUploadFilePath(sFilePath);
        }

        bool CFileUploader::UploadSync()
        {
            return m_pInternal->TransferSync();
        }
        void CFileUploader::UploadAsync()
        {
            m_pInternal->TransferAsync();
        }

        std::wstring CFileUploader::GetResponse()
        {
            return m_pInternal->GetResponse();
        }

        void CFileUploader::Start(int lPriority) { m_pInternal->Start(lPriority); }
        int CFileUploader::GetPriority() { return m_pInternal->GetPriority(); }
        void CFileUploader::Suspend() { m_pInternal->Suspend(); }
        void CFileUploader::Resume() { m_pInternal->Resume(); }
        void CFileUploader::Stop() { m_pInternal->Stop(); }
        void CFileUploader::StopNoJoin() { m_pInternal->StopNoJoin(); }
        void CFileUploader::Cancel() { m_pInternal->Cancel(); }
        int CFileUploader::IsRunned() { return m_pInternal->IsRunned(); }

        void CFileUploader::SetEvent_OnProgress(std::function<void(int)> func)
        {
            m_pInternal->GetInternal()->m_func_onProgress = func;
        }
        void CFileUploader::SetEvent_OnComplete(std::function<void(int)> func)
        {
            m_pInternal->GetInternal()->m_func_onComplete = func;
        }
    }
}
