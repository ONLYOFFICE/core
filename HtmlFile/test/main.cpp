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
