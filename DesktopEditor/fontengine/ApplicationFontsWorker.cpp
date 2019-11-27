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
#include "ApplicationFontsWorker.h"
#include "application_generate_fonts.h"

#define ONLYOFFICE_FONTS_VERSION_ 3

CApplicationFontsWorker::CApplicationFontsWorker()
{
    m_bIsUseSystemFonts = true;
    m_bIsNeedThumbnails = true;
    m_bIsUseOpenType    = true;
}
CApplicationFontsWorker::~CApplicationFontsWorker()
{
    
}

NSFonts::IApplicationFonts* CApplicationFontsWorker::Check()
{
    if (m_sDirectory.empty())
        return NULL;

    std::wstring strAllFontsJSPath      = m_sDirectory + L"/AllFonts.js";
    std::wstring strFontsSelectionBin   = m_sDirectory + L"/font_selection.bin";

    std::vector<std::string> strFonts;
    std::wstring strFontsCheckPath = m_sDirectory + L"/fonts.log";

    if (true)
    {
        NSFile::CFileBinary oFile;
        if (oFile.OpenFile(strFontsCheckPath))
        {
            int nSize = oFile.GetFileSize();
            char* pBuffer = new char[nSize];
            DWORD dwReaden = 0;
            oFile.ReadFile((BYTE*)pBuffer, nSize, dwReaden);
            oFile.CloseFile();

            int nStart = 0;
            int nCur = nStart;
            for (; nCur < nSize; ++nCur)
            {
                if (pBuffer[nCur] == '\n')
                {
                    int nEnd = nCur - 1;
                    if (nEnd > nStart)
                    {
                        std::string s(pBuffer + nStart, nEnd - nStart + 1);
                        strFonts.push_back(s);
                    }
                    nStart = nCur + 1;
                }
            }

            delete[] pBuffer;
        }

#ifdef ONLYOFFICE_FONTS_VERSION_
        if (0 != strFonts.size())
        {
            // check version!!!
            std::string sOO_Version = strFonts[0];
            if (0 != sOO_Version.find("ONLYOFFICE_FONTS_VERSION_"))
            {
                strFonts.clear();
            }
            else
            {
                std::string sVersion = sOO_Version.substr(25);
                int nVersion = std::stoi(sVersion);
                if (nVersion != ONLYOFFICE_FONTS_VERSION_)
                    strFonts.clear();
                else
                    strFonts.erase(strFonts.begin());
            }
        }
#endif
    }

    NSFonts::IApplicationFonts* pApplicationF = NSFonts::NSApplication::Create();
    std::vector<std::wstring> strFontsW_Cur;

    if (m_bIsUseSystemFonts)
        strFontsW_Cur = pApplicationF->GetSetupFontFiles();

    for (std::vector<std::wstring>::iterator i = m_arAdditionalFolders.begin(); i != m_arAdditionalFolders.end(); i++)
    {
        NSDirectory::GetFiles2(*i, strFontsW_Cur, true);
    }

    bool bIsEqual = true;
    if (strFonts.size() != strFontsW_Cur.size())
        bIsEqual = false;

    if (bIsEqual)
    {
        int nCount = (int)strFonts.size();
        for (int i = 0; i < nCount; ++i)
        {
            if (strFonts[i] != NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(strFontsW_Cur[i].c_str(), strFontsW_Cur[i].length()))
            {
                bIsEqual = false;
                break;
            }
        }
    }

    if (bIsEqual)
    {
        if (!NSFile::CFileBinary::Exists(strFontsSelectionBin))
            bIsEqual = false;
    }

    if (!bIsEqual)
    {
        if (NSFile::CFileBinary::Exists(strFontsCheckPath))
            NSFile::CFileBinary::Remove(strFontsCheckPath);
        if (NSFile::CFileBinary::Exists(strAllFontsJSPath))
            NSFile::CFileBinary::Remove(strAllFontsJSPath);
        if (NSFile::CFileBinary::Exists(strFontsSelectionBin))
            NSFile::CFileBinary::Remove(strFontsSelectionBin);
        if (NSFile::CFileBinary::Exists(m_sDirectory + L"/fonts_thumbnail.png"))
            NSFile::CFileBinary::Remove(m_sDirectory + L"/fonts_thumbnail.png");
        if (NSFile::CFileBinary::Exists(m_sDirectory + L"/fonts_thumbnail@2x.png"))
            NSFile::CFileBinary::Remove(m_sDirectory + L"/fonts_thumbnail@2x.png");

        int nFlag = 3;
        if (!m_bIsUseOpenType)
            nFlag = 2;

        pApplicationF->InitializeFromArrayFiles(strFontsW_Cur, nFlag);

        NSCommon::SaveAllFontsJS(pApplicationF, strAllFontsJSPath, m_bIsNeedThumbnails ? m_sDirectory : L"", strFontsSelectionBin);
    }

    NSFile::CFileBinary oFile;
    oFile.CreateFileW(strFontsCheckPath);
#ifdef ONLYOFFICE_FONTS_VERSION_
    oFile.WriteStringUTF8(L"ONLYOFFICE_FONTS_VERSION_");
    oFile.WriteStringUTF8(std::to_wstring(ONLYOFFICE_FONTS_VERSION_));
    oFile.WriteFile((BYTE*)"\n", 1);
#endif
    int nCount = (int)strFontsW_Cur.size();
    for (int i = 0; i < nCount; ++i)
    {
        oFile.WriteStringUTF8(strFontsW_Cur[i]);
        oFile.WriteFile((BYTE*)"\n", 1);
    }
    oFile.CloseFile();

    pApplicationF->Release();
    pApplicationF = NSFonts::NSApplication::Create();
    pApplicationF->InitializeFromFolder(m_sDirectory);

    return pApplicationF;
}

std::string CApplicationFontsWorker::GetAllFonts()
{
    std::string sAllFonts = "";
    NSFile::CFileBinary::ReadAllTextUtf8A(m_sDirectory + L"/AllFonts.js", sAllFonts);
    return sAllFonts;
}

std::vector<std::wstring> CApplicationFontsWorker::GetFontNames(NSFonts::IApplicationFonts* pFonts)
{
    std::vector<std::wstring> arNames;
    if (!pFonts || !pFonts->GetList())
        return arNames;
    std::vector<NSFonts::CFontInfo*>* arInfos = pFonts->GetList()->GetFonts();

    std::map<std::wstring, bool> map;

    for (std::vector<NSFonts::CFontInfo*>::iterator iter = arInfos->begin(); iter != arInfos->end(); iter++)
    {
        if (map.find((*iter)->m_wsFontName) == map.end())
            arNames.push_back((*iter)->m_wsFontName);
    }

    std::sort(arNames.begin(), arNames.end());
    return arNames;
}
