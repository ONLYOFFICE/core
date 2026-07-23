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
#include "../HtmlFile.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/graphics/BaseThread.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

class CCounter
{
private:
    NSCriticalSection::CRITICAL_SECTION m_oCS;
    int m_lCounter;

public:
    CCounter()
    {
        m_oCS.InitializeCriticalSection();
        m_lCounter = 0;
    }
    ~CCounter()
    {
        m_oCS.DeleteCriticalSection();
    }

public:
    void Increment(int nCount = 1)
    {
        CTemporaryCS oCS(&m_oCS);
        m_lCounter += nCount;
    }
    void Decrement()
    {
        CTemporaryCS oCS(&m_oCS);
        --m_lCounter;
    }
    int GetCounter()
    {
        CTemporaryCS oCS(&m_oCS);
        return m_lCounter;
    }
};

class CThreadConverter : public NSThreads::CBaseThread
{
public:
    CCounter* m_pCounter;
    int m_nDstId;

public:
    CThreadConverter() : NSThreads::CBaseThread()
    {
        m_pCounter = NULL;
        m_nDstId = 1;
    }
    void SetCounter(CCounter* pCounter)
    {
        m_pCounter = pCounter;
    }

protected:
    DWORD ThreadProc()
    {
        std::wstring sPath = NSFile::GetProcessDirectory() + L"/../../Internal/linux/Release/";
        std::wstring sSrc = L"/home/oleg/activex/37898EB";
        std::wstring sDstFolder = L"/home/oleg/activex/1/" + std::to_wstring(m_nDstId) + L"/";

        CHtmlFile oFile;
        std::wstring sMetaInfo;
        int nResult = oFile.ConvertEpub(sSrc, sMetaInfo, sDstFolder, sPath);
        nResult;

        m_pCounter->Decrement();
        return 0;
    }
};

int main(int argc, char *argv[])
{
#if 1
    CHtmlFile oFile;
    std::wstring sMetaInfo;
    //int nResult = oFile.ConvertEpub(L"D:/1", sMetaInfo, L"D:/test/Document");
    std::vector<std::wstring> arFiles;
    arFiles.push_back(L"Note.html");
    int nResult = oFile.Convert(arFiles, L"D:\\test\\Document");
    return 0;
#else

    int nCountTC = 1;
    CCounter oCounter;
    oCounter.Increment(nCountTC);

    for (int i = 0; i < nCountTC; i++)
    {
        CThreadConverter* pThread = new CThreadConverter();
        pThread->SetCounter(&oCounter);
        pThread->m_nDstId = i;
        pThread->Start(0);
    }

    while (0 != oCounter.GetCounter())
        NSThreads::Sleep(100);
#endif

    return 0;
}
