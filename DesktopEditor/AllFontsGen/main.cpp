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
#include <iostream>
#include <set>

#include "../graphics/pro/Graphics.h"
#include "../common/File.h"
#include "../common/Array.h"
#include "../common/Directory.h"
#include "../graphics/Timer.h"
#include "../common/StringBuilder.h"

//#define _GENERATE_FONT_MAP_

#ifdef _GENERATE_FONT_MAP_
#include "../freetype_names/FontMaps/FontDictionary.h"
#endif

//#define RANGES_LOG

using namespace std;

namespace NSCommon
{
    class CFontInfoJS
    {
    public:
        std::wstring m_sName;

        int	m_lIndexR;
        int	m_lFaceIndexR;
        int	m_lIndexI;
        int	m_lFaceIndexI;
        int	m_lIndexB;
        int	m_lFaceIndexB;
        int	m_lIndexBI;
        int	m_lFaceIndexBI;

        CFontInfoJS()
        {
            m_sName			= L"";

            m_lIndexR		= -1;
            m_lFaceIndexR	= -1;
            m_lIndexI		= -1;
            m_lFaceIndexI	= -1;
            m_lIndexB		= -1;
            m_lFaceIndexB	= -1;
            m_lIndexBI		= -1;
            m_lFaceIndexBI	= -1;
        }

        CFontInfoJS(const CFontInfoJS& oSrc)
        {
            *this = oSrc;
        }

        CFontInfoJS& operator=(const CFontInfoJS& oSrc)
        {
            m_sName			= oSrc.m_sName;

            m_lIndexR	= oSrc.m_lIndexR;
            m_lIndexI	= oSrc.m_lIndexI;
            m_lIndexB	= oSrc.m_lIndexB;
            m_lIndexBI	= oSrc.m_lIndexBI;

            m_lFaceIndexR	= oSrc.m_lFaceIndexR;
            m_lFaceIndexI	= oSrc.m_lFaceIndexI;
            m_lFaceIndexB	= oSrc.m_lFaceIndexB;
            m_lFaceIndexBI	= oSrc.m_lFaceIndexBI;

            return *this;
        }
    };

    class CSymbolSimpleChecker : public NSFonts::CApplicationFontsSymbolsChecker
    {
    public:
        int m_nMaxSymbols;
        int* m_pSymbols;
        int m_nPriority;

    public:
        CSymbolSimpleChecker()
        {
        }

        virtual void Check(const int& nCode, const unsigned int& nIndex)
        {
            if (nCode > m_nMaxSymbols)
                return;

            if (m_pSymbols[nCode] == 0)
                m_pSymbols[nCode] = m_nPriority;
            else if (m_pSymbols[nCode] > m_nPriority)
                m_pSymbols[nCode] = m_nPriority;
        }

        ~CSymbolSimpleChecker()
        {

        }
    };

    class CSymbolSimpleChecker2 : public NSFonts::CApplicationFontsSymbolsChecker
    {
    public:
        int m_nMaxSymbols;
        int* m_pSymbols;
        int m_nPriority;

        int m_nStyle;
        BYTE* m_pTmpSymbols;
        int m_nMin;
        int m_nMax;

    public:
        CSymbolSimpleChecker2(int* pSymbols, int nMax)
        {
            m_pSymbols = pSymbols;
            m_nMaxSymbols = nMax;

            m_pTmpSymbols = new BYTE[m_nMaxSymbols + 1];
            memset(m_pTmpSymbols, 0, (m_nMaxSymbols + 1) * sizeof(BYTE));

            m_nMin = m_nMaxSymbols + 1;
            m_nMax = -1;

            m_nStyle = 0;
        }

        void Apply1(int nMask)
        {
            BYTE* tmp = m_pTmpSymbols + m_nMin;
            for (int i = m_nMin; i <= m_nMax; ++i, ++tmp)
            {
                if (nMask == *tmp)
                {
                    if (m_pSymbols[i] == 0)
                        m_pSymbols[i] = m_nPriority;
                    else if (m_pSymbols[i] > m_nPriority)
                        m_pSymbols[i] = m_nPriority;
                }
            }

            if (m_nMax >= m_nMin)
                memset(m_pTmpSymbols, 0, (m_nMax - m_nMin) * sizeof(BYTE));

            m_nMin = m_nMaxSymbols + 1;
            m_nMax = -1;
        }

        void Apply2(int nMask, int nSumPriority)
        {
            int nSmallRangeLen = 10;
            int nSmallRangeLenCJK = 30;

            BYTE* tmp = m_pTmpSymbols + m_nMin;
            BYTE* tmpLast = m_pTmpSymbols + m_nMax + 1;
            BYTE* tmpFirst = NULL;
            int* pSymbols = NULL;
            int* pSymbolsLast = NULL;
            int nPriority = 0;
            int nFirstOffset = 0;

            while (tmp < tmpLast)
            {
                if (nMask != *tmp)
                {
                    ++tmp;
                    continue;
                }

                tmpFirst = tmp;
                int nCount = 1;
                ++tmp;

                while (nMask == *tmp && tmp < tmpLast)
                {
                    ++tmp;
                    nCount++;
                }

                nFirstOffset = (int)(tmpFirst - m_pTmpSymbols);

                pSymbols = m_pSymbols + nFirstOffset;
                pSymbolsLast = pSymbols + nCount;

                if (nFirstOffset > 0x4DFF && nFirstOffset < 0x9FFF)
                    nPriority = (nCount > nSmallRangeLenCJK) ? m_nPriority : (m_nPriority + nSumPriority);
                else
                    nPriority = (nCount > nSmallRangeLen) ? m_nPriority : (m_nPriority + nSumPriority);

                while (pSymbols < pSymbolsLast)
                {
                    if (*pSymbols == 0)
                        *pSymbols = nPriority;
                    else if (*pSymbols > nPriority)
                        *pSymbols = nPriority;

                    ++pSymbols;
                }
            }

            if (m_nMax >= m_nMin)
                memset(m_pTmpSymbols, 0, (m_nMax - m_nMin) * sizeof(BYTE));

            m_nMin = m_nMaxSymbols + 1;
            m_nMax = -1;
        }

        virtual void Check(const int& nCode, const unsigned int& nIndex)
        {
            if (nCode > m_nMax)
                m_nMax = nCode;
            if (nCode < m_nMin)
                m_nMin = nCode;
            m_pTmpSymbols[nCode] |= m_nStyle;
        }

        ~CSymbolSimpleChecker2()
        {
            RELEASEARRAYOBJECTS(m_pTmpSymbols);
        }
    };

    class CFontPriority
    {
    public:
        std::wstring name;
        int priority;

        CFontPriority()
        {
            priority = 0;
        }

        static bool Compare(const CFontPriority& p1, const CFontPriority& p2)
        {
            if (p1.priority != p2.priority)
                return (p1.priority < p2.priority) ? true : false;

            return (p1.name < p2.name) ? true : false;
        }
    };

    void SaveAllFontsJS(NSFonts::IApplicationFonts* applicationFonts, std::wstring strFile, std::wstring strFile2, std::wstring strFolderThumbnails, std::wstring strFontSelectionBin, std::wstring strOutputDir)
    {
        std::vector<NSFonts::CFontInfo*>* pList = applicationFonts->GetList()->GetFonts();
        int nCount = pList->size();

        // сначала строим массив всех файлов шрифтов
        std::map<std::wstring, LONG> mapFontFiles;
        std::vector<std::wstring> mapFontFiles2;
        LONG lFontFiles = 0;
        for (std::vector<NSFonts::CFontInfo*>::iterator i = pList->begin(); i != pList->end(); i++)
        {
            NSFonts::CFontInfo* pInfo = *i;

            if (mapFontFiles.find(pInfo->m_wsFontPath) == mapFontFiles.end())
            {
                mapFontFiles.insert(std::pair<std::wstring, LONG>(pInfo->m_wsFontPath, lFontFiles));
                mapFontFiles2.push_back(pInfo->m_wsFontPath);
                ++lFontFiles;
            }
        }
        // -----------------------------------------

        // теперь строим массив всех шрифтов по имени
        std::map<std::wstring, CFontInfoJS> mapFonts;
        CArray<std::wstring> arrFonts;

        for (std::vector<NSFonts::CFontInfo*>::iterator i = pList->begin(); i != pList->end(); i++)
        {
            NSFonts::CFontInfo* pInfo = *i;
            std::wstring strPath = pInfo->m_wsFontPath;
            std::wstring strName = pInfo->m_wsFontName;

            int lFontIndex = 0;
            int lFaceIndex = 0;

            std::map<std::wstring, LONG>::iterator it = mapFontFiles.find(strPath);
            lFontIndex = it->second;

            if (pInfo->m_lIndex >= 0)
                lFaceIndex = (int)pInfo->m_lIndex;

            std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(pInfo->m_wsFontName);
            if (mapFonts.end() != pPair)
            {
                pPair->second.m_sName = pInfo->m_wsFontName;

                if (pInfo->m_bBold && pInfo->m_bItalic)
                {
                    pPair->second.m_lIndexBI = lFontIndex;
                    pPair->second.m_lFaceIndexBI = lFaceIndex;
                }
                else if (pInfo->m_bBold)
                {
                    pPair->second.m_lIndexB = lFontIndex;
                    pPair->second.m_lFaceIndexB = lFaceIndex;
                }
                else if (pInfo->m_bItalic)
                {
                    pPair->second.m_lIndexI = lFontIndex;
                    pPair->second.m_lFaceIndexI = lFaceIndex;
                }
                else
                {
                    pPair->second.m_lIndexR = lFontIndex;
                    pPair->second.m_lFaceIndexR = lFaceIndex;
                }
            }
            else
            {
                CFontInfoJS fontInfo;

                fontInfo.m_sName = pInfo->m_wsFontName;

                if (pInfo->m_bBold && pInfo->m_bItalic)
                {
                    fontInfo.m_lIndexBI = lFontIndex;
                    fontInfo.m_lFaceIndexBI = lFaceIndex;
                }
                else if (pInfo->m_bBold)
                {
                    fontInfo.m_lIndexB = lFontIndex;
                    fontInfo.m_lFaceIndexB = lFaceIndex;
                }
                else if (pInfo->m_bItalic)
                {
                    fontInfo.m_lIndexI = lFontIndex;
                    fontInfo.m_lFaceIndexI = lFaceIndex;
                }
                else
                {
                    fontInfo.m_lIndexR = lFontIndex;
                    fontInfo.m_lFaceIndexR = lFaceIndex;
                }

                mapFonts.insert(std::pair<std::wstring, CFontInfoJS>(fontInfo.m_sName, fontInfo));
                arrFonts.Add(fontInfo.m_sName);
            }
        }
        // -------------------------------------------

        // теперь сортируем шрифты по имени ----------
        int nCountFonts = arrFonts.GetCount();
        for (int i = 0; i < nCountFonts; ++i)
        {
            for (int j = i + 1; j < nCountFonts; ++j)
            {
                if (arrFonts[i] > arrFonts[j])
                {
                    std::wstring temp = arrFonts[i];
                    arrFonts[i] = arrFonts[j];
                    arrFonts[j] = temp;
                }
            }
        }
        // -------------------------------------------
        if (L"" != strFolderThumbnails)
        {
            NSFonts::IFontManager* pManager = applicationFonts->GenerateFontManager();
            NSFonts::IFontsCache* pCache = NSFonts::NSFontCache::Create();
            pCache->SetStreams(applicationFonts->GetStreams());
            pManager->SetOwnerCache(pCache);

            for (int iX = 1; iX <= 2; ++iX)
            {
                // создаем картинку для табнейлов
                double dDpi = 96 * iX;
                double dW_mm = 80;
                LONG lH1_px = LONG(7 * dDpi / 25.4);
                LONG lWidthPix = (LONG)(dW_mm * dDpi / 25.4);
                LONG lHeightPix = (LONG)(nCountFonts * lH1_px);

                LONG lCountPixels = 4 * lWidthPix * lHeightPix;
                BYTE* pImageData = new BYTE[lCountPixels];
                memset(pImageData, 0xFF, lCountPixels);

                CBgraFrame oFrame;
                oFrame.put_Data(pImageData);
                oFrame.put_Width((int)lWidthPix);
                oFrame.put_Height((int)lHeightPix);
                oFrame.put_Stride(4 * (int)lWidthPix);

                for (LONG i = 3; i < lWidthPix * lHeightPix * 4; i += 4)
                {
                    pImageData[i] = 0;
                }

                NSGraphics::IGraphicsRenderer* pRenderer = NSGraphics::Create();
                pRenderer->CreateFromBgraFrame(&oFrame);

                pRenderer->SetFontManager(pManager);

                pRenderer->put_Width(lWidthPix * 25.4 / dDpi);
                pRenderer->put_Height(lHeightPix * 25.4 / dDpi);

                for (int index = 0; index < nCountFonts; ++index)
                {
                    std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(arrFonts[index]);

                    // thumbnail
                    LONG lFontIndex = 0;
                    LONG lFaceIndex = 0;
                    if (pPair->second.m_lIndexR != -1)
                    {
                        lFontIndex = pPair->second.m_lIndexR;
                        lFaceIndex = pPair->second.m_lFaceIndexR;
                    }
                    else if (pPair->second.m_lIndexI != -1)
                    {
                        lFontIndex = pPair->second.m_lIndexI;
                        lFaceIndex = pPair->second.m_lFaceIndexI;
                    }
                    else if (pPair->second.m_lIndexB != -1)
                    {
                        lFontIndex = pPair->second.m_lIndexB;
                        lFaceIndex = pPair->second.m_lFaceIndexB;
                    }
                    else if (pPair->second.m_lIndexBI != -1)
                    {
                        lFontIndex = pPair->second.m_lIndexBI;
                        lFaceIndex = pPair->second.m_lFaceIndexBI;
                    }

                    std::wstring strFontPath = mapFontFiles2[lFontIndex];

                    pRenderer->put_FontPath(strFontPath);
                    pManager->LoadFontFromFile(strFontPath, lFaceIndex, 14, dDpi, dDpi);

                    bool bIsSymbol = false;
                    NSFonts::IFontFile* pFile = pManager->GetFile();

                    if (pFile)
                        bIsSymbol = pFile->IsSymbolic(false);

                    std::wstring sFontName = pPair->second.m_sName;

                    if (bIsSymbol)
                    {
                        NSFonts::CFontSelectFormat oSelectFormat;
                        oSelectFormat.wsName = new std::wstring(L"Courier New");
                        NSFonts::CFontInfo* pInfoCur = pManager->GetFontInfoByParams(oSelectFormat);

                        if (NULL != pInfoCur)
                        {
                            pManager->LoadFontFromFile(pInfoCur->m_wsFontPath, 0, 14, dDpi, dDpi);
                        }
                        pRenderer->put_FontPath(pInfoCur->m_wsFontPath);
                    }
                    else if (pFile)
                    {
                        int nFontNameLen = (int)sFontName.length();
                        bool bIsPresentAll = true;

                        for (int nC = 0; nC < nFontNameLen; nC++)
                        {
                            int nCMapIndex = 0;
                            if (0 >= pFile->SetCMapForCharCode(sFontName.at(nC), &nCMapIndex))
                            {
                                bIsPresentAll = false;
                                break;
                            }
                        }

                        if (!bIsPresentAll)
                        {
                            NSFonts::CFontSelectFormat oSelectFormat;
                            oSelectFormat.wsName = new std::wstring(L"Arial");
                            NSFonts::CFontInfo* pInfoCur = pManager->GetFontInfoByParams(oSelectFormat);

                            if (NULL != pInfoCur)
                            {
                                pManager->LoadFontFromFile(pInfoCur->m_wsFontPath, 0, 14, dDpi, dDpi);
                            }
                            pRenderer->put_FontPath(pInfoCur->m_wsFontPath);
                        }
                    }

                    pRenderer->put_FontStringGID(FALSE);
                    pRenderer->put_FontCharSpace(0);
                    pRenderer->put_FontSize(14);

                    pRenderer->CommandDrawText(pPair->second.m_sName, 5, 25.4 * (index * lH1_px + lH1_px) / dDpi - 2, 0, 0);
                }

                std::wstring strThumbnailPath = strFolderThumbnails + L"/fonts_thumbnail";
                if (iX == 1)
                    strThumbnailPath += L".png";
                else
                    strThumbnailPath += L"@2x.png";

                oFrame.SaveFile(strThumbnailPath, 4);

                RELEASEOBJECT(pRenderer);
            }

            RELEASEOBJECT(pManager);

        }

        // и самое главное. Здесь должен скидываться скрипт для работы со всеми шрифтами.
        // все объекты, которые позволят не знать о существующих фонтах
        if (0 != strFile.length())
        {
            bool bIsDumpAllFontWeb = (strFile2.empty() || strOutputDir.empty()) ? false : true;

            BYTE correct16[16] = {0xA0, 0x66, 0xD6, 0x20, 0x14, 0x96, 0x47, 0xfa, 0x95, 0x69, 0xB8, 0x50, 0xB0, 0x41, 0x49, 0x48};
            int nCountIdSymbols = (nCountFonts >= 1000) ? 4 : 3;
            BYTE encode[32];

            NSStringUtils::CStringBuilder oWriterJS;
            NSStringUtils::CStringBuilder oWriterJS2;

            // сначала все файлы
            size_t nCountFiles = mapFontFiles.size();
            if (nCountFiles == 0)
            {
                oWriterJS += (L"window[\"__fonts_files\"] = []; \n\n");
                oWriterJS2 += (L"window[\"__fonts_files\"] = []; \n\n");
            }
            else
            {
                std::wstring* pMassFiles = new std::wstring[nCountFiles];
                std::wstring* pMassFiles2 = new std::wstring[nCountFiles];

                int nCurrentId = 0;
                for ( std::map<std::wstring, LONG>::iterator pos = mapFontFiles.begin(); pos != mapFontFiles.end(); ++pos)
                {
                    std::wstring strFontId = pos->first;

                    NSStringUtils::string_replace(strFontId, L"\\\\", L"\\");
                    NSStringUtils::string_replace(strFontId, L"\\", L"/");

                    pMassFiles[pos->second] = strFontId;

                    if (bIsDumpAllFontWeb)
                    {
                        std::wstring sId = std::to_wstring(nCurrentId++);
                        int nLenId = (int)sId.length();
                        while (nLenId < nCountIdSymbols)
                        {
                            sId = L"0" + sId;
                            ++nLenId;
                        }

                        pMassFiles2[pos->second] = sId;
                        NSFile::CFileBinary::Copy(strFontId, strOutputDir + L"/" + sId);

                        NSFile::CFileBinary oFileDst;
                        if (oFileDst.OpenFile(strOutputDir + L"/" + sId, true))
                        {
                            DWORD dwRead = (DWORD)(oFileDst.GetFileSize());
                            if (dwRead > 32)
                                dwRead = 32;

                            DWORD dwWorked = 0;
                            oFileDst.SeekFile(0);
                            oFileDst.ReadFile(encode, dwRead, dwWorked);

                            for (DWORD k = 0; k < dwRead; ++k)
                                encode[k] ^= correct16[k & 0x0F];

                            oFileDst.SeekFile(0);
                            oFileDst.WriteFile(encode, dwRead);
                            oFileDst.CloseFile();
                        }
                    }
                }

                oWriterJS += (L"window[\"__fonts_files\"] = [\n");
                oWriterJS2 += (L"window[\"__fonts_files\"] = [\n");
                for (size_t nIndex = 0; nIndex < nCountFiles; ++nIndex)
                {
                    oWriterJS += (L"\"");
                    oWriterJS2 += (L"\"");

                    oWriterJS += (pMassFiles[nIndex]);
                    oWriterJS2 += (pMassFiles2[nIndex]);

                    if (nIndex != (nCountFiles - 1))
                    {
                        oWriterJS += (L"\",\n");
                        oWriterJS2 += (L"\",\n");
                    }
                    else
                    {
                        oWriterJS += (L"\"");
                        oWriterJS2 += (L"\"");
                    }
                }
                oWriterJS += (L"\n];\n\n");
                oWriterJS2 += (L"\n];\n\n");

                delete [] pMassFiles;
                delete [] pMassFiles2;
            }

            size_t nPosForWriter2 = oWriterJS.GetCurSize();

            oWriterJS += L"window[\"__fonts_infos\"] = [\n";

            std::map<std::wstring, int> mapFontIndexes;
            for (int index = 0; index < nCountFonts; ++index)
            {
                std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(arrFonts[index]);
                mapFontIndexes.insert(std::pair<std::wstring, int>(arrFonts[index], index));

                oWriterJS += L"[\"";
                oWriterJS += pPair->second.m_sName;

                oWriterJS.AddSize(120);
                oWriterJS.AddCharNoCheck('\"');
                oWriterJS.AddCharNoCheck(',');
                oWriterJS.AddIntNoCheck(pPair->second.m_lIndexR);
                oWriterJS.AddCharNoCheck(',');
                oWriterJS.AddIntNoCheck(pPair->second.m_lFaceIndexR);
                oWriterJS.AddCharNoCheck(',');
                oWriterJS.AddIntNoCheck(pPair->second.m_lIndexI);
                oWriterJS.AddCharNoCheck(',');
                oWriterJS.AddIntNoCheck(pPair->second.m_lFaceIndexI);
                oWriterJS.AddCharNoCheck(',');
                oWriterJS.AddIntNoCheck(pPair->second.m_lIndexB);
                oWriterJS.AddCharNoCheck(',');
                oWriterJS.AddIntNoCheck(pPair->second.m_lFaceIndexB);
                oWriterJS.AddCharNoCheck(',');
                oWriterJS.AddIntNoCheck(pPair->second.m_lIndexBI);
                oWriterJS.AddCharNoCheck(',');
                oWriterJS.AddIntNoCheck(pPair->second.m_lFaceIndexBI);

                if (index != (nCountFonts - 1))
                    oWriterJS += (L"],\n");
                else
                    oWriterJS += (L"]\n");
            }
            oWriterJS += (L"];\n\n");

            if (true)
            {
                //DWORD dwTime = NSTimers::GetTickCount();

                int nMaxSymbol = 0x10FFFF;
                int* arSymbolsAll = new int[nMaxSymbol + 1];
                memset(arSymbolsAll, 0, (nMaxSymbol + 1) * sizeof(int));

                CSymbolSimpleChecker2 oAllChecker(arSymbolsAll, nMaxSymbol);

                std::map<std::wstring, int> mapFontsPriorityStandard;
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Arial", 1));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Times New Roman", 2));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Tahoma", 3));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Cambria", 4));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Calibri", 5));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Verdana", 6));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Georgia", 7));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Open Sans", 8));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Liberation Sans", 9));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Helvetica", 10));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Nimbus Sans L", 11));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"DejaVu Sans", 12));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Liberation Serif", 13));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Trebuchet MS", 14));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Courier New", 15));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Carlito", 16));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Segoe UI", 17));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"SimSun", 18));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"MS Gothic", 19));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Nirmala UI", 20));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Batang", 21));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"MS Mincho", 22));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Wingdings", 23));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Microsoft JhengHei", 24));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Microsoft JhengHei UI", 25));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Microsoft YaHei", 26));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"PMingLiU", 27));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"MingLiU", 28));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"DFKai-SB", 29));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"FangSong", 30));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"KaiTi", 31));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"SimKai", 32));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"SimHei", 33));
                mapFontsPriorityStandard.insert(std::pair<std::wstring, int>(L"Meiryo", 34));



                NSFonts::CApplicationFontsSymbols oApplicationChecker;

                std::vector<CFontPriority> arrFontsPriority;
                std::map<std::wstring, int> mapFontsPriority;
                for (int index = 0; index < nCountFonts; ++index)
                {
                    std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(arrFonts[index]);
                    CFontInfoJS& info = pPair->second;

                    std::map<std::wstring, int>::iterator find = mapFontsPriorityStandard.find(arrFonts[index]);

                    if (find != mapFontsPriorityStandard.end())
                    {
                        CFontPriority f;
                        f.name = arrFonts[index];
                        f.priority = find->second;
                        arrFontsPriority.push_back(f);
                        continue;
                    }

                    int nSize = 0;
                    if (-1 != info.m_lIndexR)
                    {
                        NSFile::CFileBinary oFile;
                        if (oFile.OpenFile(mapFontFiles2[info.m_lIndexR]))
                        {
                            int nTmp = oFile.GetFileSize();
                            if (nTmp > nSize)
                                nSize = nTmp;
                        }
                    }
                    if (-1 != info.m_lIndexB)
                    {
                        NSFile::CFileBinary oFile;
                        if (oFile.OpenFile(mapFontFiles2[info.m_lIndexB]))
                        {
                            int nTmp = oFile.GetFileSize();
                            if (nTmp > nSize)
                                nSize = nTmp;
                        }
                    }
                    if (-1 != info.m_lIndexI)
                    {
                        NSFile::CFileBinary oFile;
                        if (oFile.OpenFile(mapFontFiles2[info.m_lIndexI]))
                        {
                            int nTmp = oFile.GetFileSize();
                            if (nTmp > nSize)
                                nSize = nTmp;
                        }
                    }
                    if (-1 != info.m_lIndexBI)
                    {
                        NSFile::CFileBinary oFile;
                        if (oFile.OpenFile(mapFontFiles2[info.m_lIndexBI]))
                        {
                            int nTmp = oFile.GetFileSize();
                            if (nTmp > nSize)
                                nSize = nTmp;
                        }
                    }

                    CFontPriority f;
                    f.name = arrFonts[index];
                    f.priority = nSize;
                    arrFontsPriority.push_back(f);
                }

                std::sort(arrFontsPriority.begin(), arrFontsPriority.end(), CFontPriority::Compare);

                int nIndexPriority = 1;
                for (std::vector<CFontPriority>::iterator i = arrFontsPriority.begin(); i != arrFontsPriority.end(); i++)
                {
                    CFontPriority& o = *i;
                    mapFontsPriority.insert(std::pair<std::wstring, int>(o.name, nIndexPriority));
                    nIndexPriority++;
                }

                int nSumPriority = (int)(arrFontsPriority.size() + 1);

                bool bIsSmallRangesDetect = true;
                for (int index = 0; index < nCountFonts; ++index)
                {
                    std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(arrFonts[index]);
                    CFontInfoJS& info = pPair->second;

                    int nPriority = mapFontsPriority.find(arrFonts[index])->second;
                    oAllChecker.m_nPriority = nPriority;

                    int nCounterFonts = 0;
                    if (-1 != info.m_lIndexR)
                        nCounterFonts++;
                    if (-1 != info.m_lIndexB)
                        nCounterFonts++;
                    if (-1 != info.m_lIndexI)
                        nCounterFonts++;
                    if (-1 != info.m_lIndexBI)
                        nCounterFonts++;

                    if (1 == nCounterFonts && !bIsSmallRangesDetect)
                    {
                        std::wstring sPathC = L"";
                        int nFaceIndexC = 0;
                        if (-1 != info.m_lIndexR)
                        {
                            sPathC = mapFontFiles2[info.m_lIndexR];
                            nFaceIndexC = info.m_lFaceIndexR;
                        }
                        else if (-1 != info.m_lIndexB)
                        {
                            sPathC = mapFontFiles2[info.m_lIndexB];
                            nFaceIndexC = info.m_lFaceIndexB;
                        }
                        else if (-1 != info.m_lIndexI)
                        {
                            sPathC = mapFontFiles2[info.m_lIndexI];
                            nFaceIndexC = info.m_lFaceIndexI;
                        }
                        else if (-1 != info.m_lIndexBI)
                        {
                            sPathC = mapFontFiles2[info.m_lIndexBI];
                            nFaceIndexC = info.m_lFaceIndexBI;
                        }

                        CSymbolSimpleChecker checker;
                        checker.m_nMaxSymbols = nMaxSymbol;
                        checker.m_pSymbols = arSymbolsAll;
                        checker.m_nPriority = nPriority;
                        oApplicationChecker.CheckSymbols(sPathC, nFaceIndexC, &checker);
                    }
                    else
                    {
                        int nMask = 0;
                        if (-1 != info.m_lIndexR)
                        {
                            oAllChecker.m_nStyle = 1;
                            nMask |= 1;
                            oApplicationChecker.CheckSymbols(mapFontFiles2[info.m_lIndexR], info.m_lFaceIndexR, &oAllChecker);
                        }
                        if (-1 != info.m_lIndexB)
                        {
                            oAllChecker.m_nStyle = 2;
                            nMask |= 2;
                            oApplicationChecker.CheckSymbols(mapFontFiles2[info.m_lIndexB], info.m_lFaceIndexB, &oAllChecker);
                        }
                        if (-1 != info.m_lIndexI)
                        {
                            oAllChecker.m_nStyle = 4;
                            nMask |= 4;
                            oApplicationChecker.CheckSymbols(mapFontFiles2[info.m_lIndexI], info.m_lFaceIndexI, &oAllChecker);
                        }
                        if (-1 != info.m_lIndexBI)
                        {
                            oAllChecker.m_nStyle = 8;
                            nMask |= 8;
                            oApplicationChecker.CheckSymbols(mapFontFiles2[info.m_lIndexBI], info.m_lFaceIndexBI, &oAllChecker);
                        }

                        if (bIsSmallRangesDetect)
                            oAllChecker.Apply2(nMask, nSumPriority);
                        else
                            oAllChecker.Apply1(nMask);
                    }
                }

                //dwTime = NSTimers::GetTickCount() - dwTime;

                oWriterJS += L"window[\"__fonts_ranges\"] = [\n";

                int nFontPriority = 0;
                int nFontPriorityStart = 0;
                int nTopBorder = nMaxSymbol + 1;
                for (int i = 0; i < nTopBorder; ++i)
                {
                    int nFontPriorityTestSym = arSymbolsAll[i];
                    int nFontPriorityTest = (nFontPriorityTestSym > nSumPriority) ? (nFontPriorityTestSym - nSumPriority) : nFontPriorityTestSym;

                    if (nFontPriority == nFontPriorityTest)
                        continue;

                    if (nFontPriority != 0)
                    {
                        oWriterJS.AddInt(nFontPriorityStart);
                        oWriterJS.AddCharSafe(',');
                        oWriterJS.AddInt(i - 1);
                        oWriterJS.AddCharSafe(',');
                        oWriterJS.AddInt(mapFontIndexes.find(arrFontsPriority[nFontPriority - 1].name)->second);
                        oWriterJS.AddCharSafe(',');

#ifdef RANGES_LOG
                        FILE* f = fopen("D:\\fonts.log", "a+");
                        fprintf(f, "[%d - %d]: ", nFontPriorityStart, i - 1);
                        std::string sTmp = U_TO_UTF8(arrFontsPriority[nFontPriority - 1].name);
                        fprintf(f, sTmp.c_str());
                        fprintf(f, "\n");
                        fclose(f);
#endif
                    }
                    nFontPriority = nFontPriorityTest;
                    nFontPriorityStart = i;
                }

                if (nFontPriority != 0)
                {
                    oWriterJS.AddInt(nFontPriorityStart);
                    oWriterJS.AddCharSafe(',');
                    oWriterJS.AddInt(nMaxSymbol - 1);
                    oWriterJS.AddCharSafe(',');
                    oWriterJS.AddInt(mapFontIndexes.find(arrFontsPriority[nFontPriority - 1].name)->second);
                    oWriterJS.AddCharSafe(',');

#ifdef RANGES_LOG
                    FILE* f = fopen("D:\\fonts.log", "a+");
                    fprintf(f, "[%d - %d]: ", nFontPriorityStart, nMaxSymbol - 1);
                    std::string sTmp = U_TO_UTF8(arrFontsPriority[nFontPriority - 1].name);
                    fprintf(f, sTmp.c_str());
                    fprintf(f, "\n");
                    fclose(f);
#endif
                }

                oWriterJS.SetCurSize(oWriterJS.GetCurSize() - 1);

                oWriterJS += (L"];\n\n");

                delete[] arSymbolsAll;
            }

            if (true)
            {
                BYTE* pData = NULL;
                LONG lLen = 0;
                applicationFonts->GetList()->ToBuffer(&pData, &lLen, L"", true);

                char* cData64 = NULL;
                int nData64Dst = 0;
                NSFile::CBase64Converter::Encode(pData, (int)lLen, cData64, nData64Dst, NSBase64::B64_BASE64_FLAG_NOCRLF);

                std::wstring sData64 = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(cData64, (LONG)nData64Dst, FALSE);

                oWriterJS += (L"window[\"g_fonts_selection_bin\"] = \"");
                oWriterJS += sData64;
                oWriterJS += L"\";\n";

                RELEASEARRAYOBJECTS(pData);
                RELEASEARRAYOBJECTS(cData64);
            }

            NSFile::CFileBinary oFile;
            oFile.CreateFileW(strFile);
            oFile.WriteStringUTF8(oWriterJS.GetData(), true);
            oFile.CloseFile();

            if (bIsDumpAllFontWeb)
            {
                oWriterJS2.Write(oWriterJS, nPosForWriter2);

                NSFile::CFileBinary oFile2;
                oFile2.CreateFileW(strFile2);
                oFile2.WriteStringUTF8(oWriterJS2.GetData(), true);
                oFile2.CloseFile();
            }
        }

        if (0 != strFontSelectionBin.length())
        {
            BYTE* pData = NULL;
            LONG lLen = 0;
            applicationFonts->GetList()->ToBuffer(&pData, &lLen, L"", false);

            NSFile::CFileBinary oFile;
            oFile.CreateFileW(strFontSelectionBin);
            oFile.WriteFile(pData, (DWORD)lLen);
            oFile.CloseFile();

            RELEASEARRAYOBJECTS(pData);
        }
    }

#ifdef _GENERATE_FONT_MAP_

    void WriteMemoryInt4(const int& value, BYTE*& pData)
    {
        *((FT_Int32*)(pData)) = value;
        pData += 4;
    }
    void WriteMemoryUInt4(const unsigned int& value, BYTE*& pData)
    {
        *((FT_UInt32*)(pData)) = value;
        pData += 4;
    }
    void WriteMemoryInt1(const char& value, BYTE*& pData)
    {
        *((BYTE*)(pData)) = (BYTE)value;
        pData += 1;
    }
    void WriteMemoryUInt1(const BYTE& value, BYTE*& pData)
    {
        *((BYTE*)(pData)) = value;
        pData += 1;
    }
    void WriteMemoryInt2(const short& value, BYTE*& pData)
    {
        *((short*)(pData)) = value;
        pData += 2;
    }
    void WriteMemoryUInt2(const unsigned short& value, BYTE*& pData)
    {
        *((unsigned short*)(pData)) = value;
        pData += 2;
    }
    void WriteMemoryStringA(const std::string& value, BYTE*& pData)
    {
        size_t len = value.length();
        WriteMemoryInt4((int)len, pData);
        if (0 != len)
        {
            memcpy(pData, value.c_str(), len);
            pData += len;
        }
    }

    void DumpFontsDictionary(const std::wstring& strDumpFile)
    {
        int nFileDictionaryBinaryLen = 0;

        // FD_Ascii_Names ---------------------------------------
        nFileDictionaryBinaryLen += 4;
        for (int i = 0; i < FONTS_DICT_ASCII_NAMES_COUNT; ++i)
        {
            const FD_FontMapRec _rec = FD_Ascii_Names[i];

            std::string sName(_rec.m_name);
            nFileDictionaryBinaryLen += 4;
            nFileDictionaryBinaryLen += (int)sName.length();
            nFileDictionaryBinaryLen += 4 * 4;
        }

        // FD_Ascii_Names_Offsets
        nFileDictionaryBinaryLen += 4 * 256;
        // ------------------------------------------------------

        // FD_Unicode_Names -------------------------------------
        nFileDictionaryBinaryLen += 4;
        for (int i = 0; i < FONTS_DICT_UNICODE_NAMES_COUNT; ++i)
        {
            const FD_FontMapRecW _rec = FD_Unicode_Names[i];

            std::wstring sName(_rec.m_name);
            std::string sNameA = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.c_str(), (LONG)sName.length());
            nFileDictionaryBinaryLen += 4;
            nFileDictionaryBinaryLen += (int)sNameA.length();
            nFileDictionaryBinaryLen += 4 * 4;
        }
        // ------------------------------------------------------

        // FD_Ascii_Files ---------------------------------------
        nFileDictionaryBinaryLen += 4;
        for (int i = 0; i < FONTS_DICT_ASCII_FONTS_COUNT; ++i)
        {
            const FD_Font _rec = FD_Ascii_Files[i];

            std::string sName(_rec.m_name);
            nFileDictionaryBinaryLen += 4;
            nFileDictionaryBinaryLen += (int)sName.length();

            nFileDictionaryBinaryLen += 4;
            nFileDictionaryBinaryLen += 3 * 4;
            nFileDictionaryBinaryLen += 10;
            nFileDictionaryBinaryLen += 4 * 6;
            nFileDictionaryBinaryLen += 2 * 3;
            nFileDictionaryBinaryLen += 2;
            nFileDictionaryBinaryLen += 2 * 6;
        }
        // ------------------------------------------------------

        BYTE* pBinaryData = new BYTE[nFileDictionaryBinaryLen];
        BYTE* pBinaryDataMem = pBinaryData;

        // FD_Ascii_Names ---------------------------------------
        WriteMemoryInt4((int)FONTS_DICT_ASCII_NAMES_COUNT, pBinaryDataMem);
        for (int i = 0; i < FONTS_DICT_ASCII_NAMES_COUNT; ++i)
        {
            const FD_FontMapRec _rec = FD_Ascii_Names[i];

            std::string sName(_rec.m_name);
            WriteMemoryStringA(sName, pBinaryDataMem);

            WriteMemoryInt4(_rec.m_index_r, pBinaryDataMem);
            WriteMemoryInt4(_rec.m_index_i, pBinaryDataMem);
            WriteMemoryInt4(_rec.m_index_b, pBinaryDataMem);
            WriteMemoryInt4(_rec.m_index_bi, pBinaryDataMem);
        }

        // FD_Ascii_Names_Offsets
        memcpy(pBinaryDataMem, FD_Ascii_Names_Offsets, 256 * 4);
        pBinaryDataMem += 4 * 256;
        // ------------------------------------------------------

        // FD_Unicode_Names -------------------------------------
        WriteMemoryInt4((int)FONTS_DICT_UNICODE_NAMES_COUNT, pBinaryDataMem);
        for (int i = 0; i < FONTS_DICT_UNICODE_NAMES_COUNT; ++i)
        {
            const FD_FontMapRecW _rec = FD_Unicode_Names[i];

            std::wstring sName(_rec.m_name);
            std::string sNameA = NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(sName.c_str(), (LONG)sName.length());

            WriteMemoryStringA(sNameA, pBinaryDataMem);

            WriteMemoryInt4(_rec.m_index_r, pBinaryDataMem);
            WriteMemoryInt4(_rec.m_index_i, pBinaryDataMem);
            WriteMemoryInt4(_rec.m_index_b, pBinaryDataMem);
            WriteMemoryInt4(_rec.m_index_bi, pBinaryDataMem);
        }
        // ------------------------------------------------------

        // FD_Ascii_Files ---------------------------------------
        WriteMemoryInt4((int)FONTS_DICT_ASCII_FONTS_COUNT, pBinaryDataMem);
        for (int i = 0; i < FONTS_DICT_ASCII_FONTS_COUNT; ++i)
        {
            const FD_Font _rec = FD_Ascii_Files[i];

            std::string sName(_rec.m_name);
            WriteMemoryStringA(sName, pBinaryDataMem);

            WriteMemoryInt4((int)_rec.m_lIndex, pBinaryDataMem);

            WriteMemoryInt4((FT_Int32)_rec.m_bBold, pBinaryDataMem);
            WriteMemoryInt4((FT_Int32)_rec.m_bItalic, pBinaryDataMem);
            WriteMemoryInt4((FT_Int32)_rec.m_bIsFixed, pBinaryDataMem);

            memcpy(pBinaryDataMem, _rec.m_aPanose, 10);
            pBinaryDataMem += 10;

            WriteMemoryUInt4(_rec.m_ulUnicodeRange1, pBinaryDataMem);
            WriteMemoryUInt4(_rec.m_ulUnicodeRange2, pBinaryDataMem);
            WriteMemoryUInt4(_rec.m_ulUnicodeRange3, pBinaryDataMem);
            WriteMemoryUInt4(_rec.m_ulUnicodeRange4, pBinaryDataMem);

            WriteMemoryUInt4(_rec.m_ulCodePageRange1, pBinaryDataMem);
            WriteMemoryUInt4(_rec.m_ulCodePageRange2, pBinaryDataMem);

            WriteMemoryUInt2(_rec.m_usWeigth, pBinaryDataMem);
            WriteMemoryUInt2(_rec.m_usWidth, pBinaryDataMem);

            WriteMemoryInt2(_rec.m_sFamilyClass, pBinaryDataMem);

            WriteMemoryInt2((short)_rec.m_eFontFormat, pBinaryDataMem);

            WriteMemoryInt2(_rec.m_shAvgCharWidth, pBinaryDataMem);
            WriteMemoryInt2(_rec.m_shAscent, pBinaryDataMem);
            WriteMemoryInt2(_rec.m_shDescent, pBinaryDataMem);
            WriteMemoryInt2(_rec.m_shLineGap, pBinaryDataMem);
            WriteMemoryInt2(_rec.m_shXHeight, pBinaryDataMem);
            WriteMemoryInt2(_rec.m_shCapHeight, pBinaryDataMem);
        }
        // ------------------------------------------------------

        char* cData64 = NULL;
        int nData64Dst = 0;
        NSFile::CBase64Converter::Encode(pBinaryData, (int)nFileDictionaryBinaryLen, cData64, nData64Dst, NSBase64::B64_BASE64_FLAG_NOCRLF);

        RELEASEARRAYOBJECTS(pBinaryData);

        NSFile::CFileBinary oFile;
        oFile.CreateFileW(strDumpFile);
        oFile.WriteFile((BYTE*)cData64, (DWORD)nData64Dst);
        oFile.CloseFile();

        RELEASEARRAYOBJECTS(cData64);
    }

#endif
}

std::wstring CorrectDir(const std::wstring& sDir)
{
    if (sDir.empty())
        return L"";

    const wchar_t* data = sDir.c_str();

    std::wstring::size_type pos1 = (data[0] == '\"') ? 1 : 0;
    std::wstring::size_type pos2 = sDir.length();

    if (data[pos2 - 1] == '\"')
        --pos2;

    if (pos2 > 0 && ((data[pos2 - 1] == '\\') || (data[pos2 - 1] == '/')))
        --pos2;

    return sDir.substr(pos1, pos2 - pos1);
}

std::wstring CorrectValue(const std::wstring& value)
{
    if (value.empty())
        return L"";

    const wchar_t* data = value.c_str();

    std::wstring::size_type pos1 = (data[0] == '\"') ? 1 : 0;
    std::wstring::size_type pos2 = value.length();

    if (data[pos2 - 1] == '\"')
        --pos2;

    return value.substr(pos1, pos2 - pos1);
}

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
    std::vector<std::wstring> arFontsDirs;
    bool bIsUseSystemFonts = false;
    std::wstring strAllFontsWebPath = L"";
    std::wstring strAllFontsPath = L"";
    std::wstring strThumbnailsFolder = L"";
    std::wstring strFontsSelectionBin = L"";
    std::wstring strOutputDir = L"";
    int nFontFlag = 3;

    for (int i = 0; i < argc; ++i)
    {
#ifdef WIN32
        std::wstring sParam(argv[i]);
#else
        std::string sParamA(argv[i]);
        std::wstring sParam = UTF8_TO_U(sParamA);
#endif

        if (sParam.find(L"--") == 0)
        {
            std::wstring sKey = L"";
            std::wstring sValue = L"";

            std::wstring::size_type _pos = sParam.find('=');
            if (std::wstring::npos == _pos)
            {
                sKey = sParam;
            }
            else
            {
                sKey = sParam.substr(0, _pos);
                sValue = sParam.substr(_pos + 1);
            }

            if (sKey == L"--use-system")
            {
                sValue = CorrectValue(sValue);
                if (sValue == L"1" || sValue == L"true")
                    bIsUseSystemFonts = true;
            }
            else if (sKey == L"--allfonts-web")
            {
                strAllFontsWebPath = CorrectDir(sValue);
            }
            else if (sKey == L"--allfonts")
            {
                strAllFontsPath = CorrectDir(sValue);
            }
            else if (sKey == L"--images")
            {
                strThumbnailsFolder = CorrectDir(sValue);
            }
            else if (sKey == L"--selection")
            {
                strFontsSelectionBin = CorrectDir(sValue);
            }
            else if (sKey == L"--input")
            {
                const wchar_t* src = sValue.c_str();
                const wchar_t* limit = src + sValue.length();

                const wchar_t* srcPrev = src;
                while (src < limit)
                {
                    if (*src == ';')
                    {
                        if (srcPrev != src)
                        {
                            arFontsDirs.push_back(CorrectDir(std::wstring(srcPrev, src - srcPrev)));
                        }
                        src++;
                        srcPrev = src;
                    }
                    else
                        src++;
                }

                if (src > srcPrev)
                {
                    arFontsDirs.push_back(CorrectDir(std::wstring(srcPrev, src - srcPrev)));
                }
            }
            else if (sKey == L"--output-web")
            {
                strOutputDir = CorrectDir(sValue);
            }
            else if (sKey == L"--font-format")
            {
                // first byte => isSupportCFF
                // second byte => isUnsupport DFont (mac)

                int nFlag = std::stoi(sValue);
                if (nFlag > 0)
                    nFontFlag = nFlag;
            }
        }
    }

    /*
    --input="D:\OO_FONTS" --allfonts="D:\123\gen\AllFonts.js" --allfonts-web="D:\123\gen\AllFonts2.js" --images="D:\123\gen" --selection="D:\123\gen\font_selection.bin" --output-web="D:\123" --use-system="true"
    */

    NSFonts::IApplicationFonts* pApplicationF = NSFonts::NSApplication::Create();

    std::vector<std::wstring> arFontFiles;
    if (bIsUseSystemFonts)
        arFontFiles = pApplicationF->GetSetupFontFiles();

    for (std::vector<std::wstring>::iterator i = arFontsDirs.begin(); i != arFontsDirs.end(); i++)
    {
        NSDirectory::GetFiles2(*i, arFontFiles, true);
    }

    pApplicationF->InitializeFromArrayFiles(arFontFiles, nFontFlag);

    NSCommon::SaveAllFontsJS(pApplicationF, strAllFontsPath, strAllFontsWebPath, strThumbnailsFolder, strFontsSelectionBin, strOutputDir);

#ifdef _GENERATE_FONT_MAP_

    NSCommon::DumpFontsDictionary(L"D:\\fonts_dictionary.txt");

#endif

    RELEASEOBJECT(pApplicationF);

    return 0;
}

