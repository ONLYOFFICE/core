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

#include "./FileDownloader_private.h"
#include "./FileDownloader.h"

CFileDownloader::CFileDownloader(std::wstring sFileUrl, bool bDelete)
{
    m_pInternal = new CFileDownloader_private(sFileUrl, bDelete);
}
CFileDownloader::~CFileDownloader()
{
    Stop();
    if (NULL != m_pInternal)
        delete m_pInternal;
}

void CFileDownloader::SetFilePath(const std::wstring& sPath)
{
    return m_pInternal->SetFilePath(sPath);
}
std::wstring CFileDownloader::GetFilePath()
{
    return m_pInternal->GetFilePath();
}
bool CFileDownloader::IsFileDownloaded()
{
    return m_pInternal->IsFileDownloaded();
}

bool CFileDownloader::DownloadSync()
{
    return m_pInternal->DownloadSync();
}

void CFileDownloader::Start(int lPriority)
{
    return m_pInternal->Start(lPriority);
}
void CFileDownloader::Suspend()
{
    return m_pInternal->Suspend();
}
void CFileDownloader::Resume()
{
    return m_pInternal->Resume();
}
void CFileDownloader::Stop()
{
    return m_pInternal->Stop();
}

int CFileDownloader::IsSuspended()
{
    return m_pInternal->IsSuspended();
}
int CFileDownloader::IsRunned()
{
    return m_pInternal->IsRunned();
}
int CFileDownloader::GetError()
{
    return m_pInternal->GetError();
}

int CFileDownloader::GetPriority()
{
    return m_pInternal->GetPriority();
}

void CFileDownloader::CheckSuspend()
{
    return m_pInternal->CheckSuspend();
}

void CFileDownloader::SetEvent_OnProgress(CFileDownloader_OnProgress func)
{
    m_pInternal->GetInternal()->m_func_onProgress = func;
}
void CFileDownloader::SetEvent_OnComplete(CFileDownloader_OnComplete func)
{
    m_pInternal->GetInternal()->m_func_onComplete = func;
}

#ifdef _MAC
bool CFileDownloader::m_bIsARCEnabled = false;

void CFileDownloader::SetARCEnabled(const bool& enabled)
{
    m_bIsARCEnabled = enabled;
}
bool CFileDownloader::GetARCEnabled()
{
    return m_bIsARCEnabled;
}
#endif
