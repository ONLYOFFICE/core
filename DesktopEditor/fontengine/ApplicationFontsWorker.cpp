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

#include <map>
#include <set>

#include "../common/File.h"
#include "../common/Directory.h"
#include "../common/Array.h"
#include "../common/StringBuilder.h"
#include "../common/ByteBuilder.h"
#include "../graphics/pro/Fonts.h"
#include "../raster/BgraFrame.h"
#include "../graphics/pro/Graphics.h"

#define ONLYOFFICE_FONTS_VERSION_ 5

class CApplicationFontsWorker_private
{
public:
    CApplicationFontsWorker_private(CApplicationFontsWorker* pMain)
    {
        m_pMain = pMain;
        m_pBreaker = NULL;

        m_bIsCheckThumbnailsMode = false;
    }
    ~CApplicationFontsWorker_private()
    {
    }

public:

    class CFontThumbnail
    {
    public:
        std::wstring m_sFile;
        std::wstring m_sName;
        int m_lFaceIndex;

    public:
        CFontThumbnail()
        {
            m_sFile = L"";
            m_sName = L"";
            m_lFaceIndex = 0;
        }
        CFontThumbnail(const CFontThumbnail& oSrc)
        {
            m_sFile = oSrc.m_sFile;
            m_sName = oSrc.m_sName;
            m_lFaceIndex = oSrc.m_lFaceIndex;
        }

        CFontThumbnail& operator=(const CFontThumbnail& oSrc)
        {
            m_sFile = oSrc.m_sFile;
            m_sName = oSrc.m_sName;
            m_lFaceIndex = oSrc.m_lFaceIndex;
            return *this;
        }
    };

public:
    CApplicationFontsWorker* m_pMain;
    CApplicationFontsWorkerBreaker* m_pBreaker;

    std::vector<CFontThumbnail> m_arFonts;
    bool m_bIsCheckThumbnailsMode;

    class CFontInfoJS
    {
    public:
        std::wstring m_sName;

        int	m_lIndexR;
        int	m_lFaceIndexR;
        std::wstring m_sStyleR;

        int	m_lIndexI;
        int	m_lFaceIndexI;
        std::wstring m_sStyleI;

        int	m_lIndexB;
        int	m_lFaceIndexB;
        std::wstring m_sStyleB;

        int	m_lIndexBI;
        int	m_lFaceIndexBI;
        std::wstring m_sStyleBI;

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

            m_sStyleR       = L"";
            m_sStyleI       = L"";
            m_sStyleB       = L"";
            m_sStyleBI      = L"";
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

            m_sStyleR   = oSrc.m_sStyleR;
            m_sStyleI   = oSrc.m_sStyleI;
            m_sStyleB   = oSrc.m_sStyleB;
            m_sStyleBI  = oSrc.m_sStyleBI;

            return *this;
        }

        bool IsNeedUpdateR()
        {
            if (m_sStyleR == L"Regular" ||
                m_sStyleR == L"Roman")
                return false;
            return true;
        }
        bool IsNeedUpdateI()
        {
            if (m_sStyleI == L"Italic" ||
                m_sStyleI == L"Oblique")
                return false;
            return true;
        }
        bool IsNeedUpdateB()
        {
            if (m_sStyleB == L"Bold")
                return false;
            return true;
        }
        bool IsNeedUpdateBI()
        {
            if (m_sStyleBI == L"Bold Italic" ||
                m_sStyleBI == L"Bold Oblique" ||
                m_sStyleBI == L"BoldItalic" ||
                m_sStyleBI == L"BoldOblique")
                return false;
            return true;
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

                while (tmp < tmpLast && nMask == *tmp)
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

public:
    inline bool CheckBreak()
    {
        if (m_pBreaker && !m_pBreaker->IsFontsWorkerRunned())
            return true;
        return false;
    }

    void SaveAllFontsJS(NSFonts::IApplicationFonts* applicationFonts, int nVersion = -1)
    {
        if (CheckBreak()) return;

        std::vector<NSFonts::CFontInfo*>* pList = applicationFonts->GetList()->GetFonts();
        int nCount = (int)pList->size();

        // сначала строим массив всех файлов шрифтов
        std::map<std::wstring, LONG> mapFontFiles;
        std::map<LONG, std::wstring> mapFontFiles2;
        LONG lFontFiles = 0;
        for (std::vector<NSFonts::CFontInfo*>::iterator i = pList->begin(); i != pList->end(); i++)
        {
            NSFonts::CFontInfo* pInfo = *i;

            if (mapFontFiles.find(pInfo->m_wsFontPath) == mapFontFiles.end())
            {
                mapFontFiles.insert(std::pair<std::wstring, LONG>(pInfo->m_wsFontPath, lFontFiles));
                mapFontFiles2.insert(std::pair<LONG, std::wstring>(lFontFiles, pInfo->m_wsFontPath));
                ++lFontFiles;
            }
        }
        // -----------------------------------------

        if (CheckBreak()) return;

        // теперь строим массив всех шрифтов по имени
        std::map<std::wstring, CFontInfoJS> mapFonts;
        std::vector<std::wstring> arrFonts;

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
                    if (pPair->second.IsNeedUpdateBI())
                    {
                        pPair->second.m_lIndexBI = lFontIndex;
                        pPair->second.m_lFaceIndexBI = lFaceIndex;
                        pPair->second.m_sStyleBI = pInfo->m_wsStyle;
                    }
                }
                else if (pInfo->m_bBold)
                {
                    if (pPair->second.IsNeedUpdateB())
                    {
                        pPair->second.m_lIndexB = lFontIndex;
                        pPair->second.m_lFaceIndexB = lFaceIndex;
                        pPair->second.m_sStyleB = pInfo->m_wsStyle;
                    }
                }
                else if (pInfo->m_bItalic)
                {
                    if (pPair->second.IsNeedUpdateI())
                    {
                        pPair->second.m_lIndexI = lFontIndex;
                        pPair->second.m_lFaceIndexI = lFaceIndex;
                        pPair->second.m_sStyleI = pInfo->m_wsStyle;
                    }
                }
                else
                {
                    if (pPair->second.IsNeedUpdateR())
                    {
                        pPair->second.m_lIndexR = lFontIndex;
                        pPair->second.m_lFaceIndexR = lFaceIndex;
                        pPair->second.m_sStyleR = pInfo->m_wsStyle;
                    }
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
                    fontInfo.m_sStyleBI = pInfo->m_wsStyle;
                }
                else if (pInfo->m_bBold)
                {
                    fontInfo.m_lIndexB = lFontIndex;
                    fontInfo.m_lFaceIndexB = lFaceIndex;
                    fontInfo.m_sStyleB = pInfo->m_wsStyle;
                }
                else if (pInfo->m_bItalic)
                {
                    fontInfo.m_lIndexI = lFontIndex;
                    fontInfo.m_lFaceIndexI = lFaceIndex;
                    fontInfo.m_sStyleI = pInfo->m_wsStyle;
                }
                else
                {
                    fontInfo.m_lIndexR = lFontIndex;
                    fontInfo.m_lFaceIndexR = lFaceIndex;
                    fontInfo.m_sStyleR = pInfo->m_wsStyle;
                }

                mapFonts.insert(std::pair<std::wstring, CFontInfoJS>(fontInfo.m_sName, fontInfo));
                arrFonts.push_back(fontInfo.m_sName);
            }
        }
        // -------------------------------------------

        if (CheckBreak()) return;

        // теперь сортируем шрифты по имени ----------
        size_t nCountFonts = arrFonts.size();
        for (size_t i = 0; i < nCountFonts; ++i)
        {
            for (size_t j = i + 1; j < nCountFonts; ++j)
            {
                if (arrFonts[i] > arrFonts[j])
                {
                    std::wstring temp = arrFonts[i];
                    arrFonts[i] = arrFonts[j];
                    arrFonts[j] = temp;
                }
            }
        }

        std::wstring strFontSelectionBin = L"";
        // нужно ли скидывать font_selection.bin
        if (-1 == nVersion && !m_bIsCheckThumbnailsMode)
        {
            strFontSelectionBin = m_pMain->m_sDirectory + L"/font_selection.bin";
        }

        NSMemoryUtils::CByteBuilder* pRangeBuilder = NULL;
        int nRangeBuilderCount = 0;
        if (!strFontSelectionBin.empty())
        {
            pRangeBuilder = new NSMemoryUtils::CByteBuilder();
            pRangeBuilder->WriteInt(0);
        }

        // и самое главное. Здесь должен скидываться скрипт для работы со всеми шрифтами.
        // все объекты, которые позволят не знать о существующих фонтах
        std::wstring sAllFontsPath = m_pMain->m_sDirectory + L"/AllFonts.js";
        if (nVersion != -1)
            sAllFontsPath += (L"." + std::to_wstring((int)(nVersion + 1)));

        if (m_bIsCheckThumbnailsMode)
            sAllFontsPath = L"";

        if (!sAllFontsPath.empty())
        {
            NSStringUtils::CStringBuilder oWriterJS;
            int nAllFontsVersion = nVersion;
            if (-1 == nAllFontsVersion)
                nAllFontsVersion = 1;
            oWriterJS.WriteString(L"window[\"__all_fonts_js_version__\"] = ");
            oWriterJS.AddInt(nAllFontsVersion);
            oWriterJS.WriteString(L";\n\n");

            // сначала все файлы
            size_t nCountFiles = mapFontFiles.size();
            if (nCountFiles == 0)
                oWriterJS.WriteString(L"window[\"__fonts_files\"] = []; \n\n");
            else
            {
                std::wstring* pMassFiles = new std::wstring[nCountFiles];
                for ( std::map<std::wstring, LONG>::iterator pos = mapFontFiles.begin(); pos != mapFontFiles.end(); ++pos)
                {
                    std::wstring strFontId = pos->first;

                    NSStringUtils::string_replace(strFontId, L"\\\\", L"\\");
                    NSStringUtils::string_replace(strFontId, L"\\", L"/");

                    pMassFiles[pos->second] = strFontId;
                }

                oWriterJS.WriteString(L"window[\"__fonts_files\"] = [\n");
                for (size_t nIndex = 0; nIndex < nCountFiles; ++nIndex)
                {
                    oWriterJS.WriteString(L"\"");
                    oWriterJS.WriteString(pMassFiles[nIndex]);
                    if (nIndex != (nCountFiles - 1))
                        oWriterJS.WriteString(L"\",\n");
                    else
                        oWriterJS.WriteString(L"\"");
                }
                oWriterJS.WriteString(L"\n];\n\n");

                delete [] pMassFiles;
            }

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
                        ++nRangeBuilderCount;
                        oWriterJS.AddInt(nFontPriorityStart);
                        oWriterJS.AddCharSafe(',');
                        oWriterJS.AddInt(i - 1);
                        oWriterJS.AddCharSafe(',');
                        oWriterJS.AddInt(mapFontIndexes.find(arrFontsPriority[nFontPriority - 1].name)->second);
                        oWriterJS.AddCharSafe(',');

                        if (pRangeBuilder)
                        {
                            pRangeBuilder->WriteStringUTF8(arrFontsPriority[nFontPriority - 1].name);
                            pRangeBuilder->WriteInt(nFontPriorityStart);
                            pRangeBuilder->WriteInt(i - 1);
                        }
                    }
                    nFontPriority = nFontPriorityTest;
                    nFontPriorityStart = i;
                }

                if (nFontPriority != 0)
                {
                    ++nRangeBuilderCount;
                    oWriterJS.AddInt(nFontPriorityStart);
                    oWriterJS.AddCharSafe(',');
                    oWriterJS.AddInt(nMaxSymbol - 1);
                    oWriterJS.AddCharSafe(',');
                    oWriterJS.AddInt(mapFontIndexes.find(arrFontsPriority[nFontPriority - 1].name)->second);
                    oWriterJS.AddCharSafe(',');

                    if (pRangeBuilder)
                    {
                        pRangeBuilder->WriteStringUTF8(arrFontsPriority[nFontPriority - 1].name);
                        pRangeBuilder->WriteInt(nFontPriorityStart);
                        pRangeBuilder->WriteInt(nMaxSymbol - 1);
                    }
                }

                oWriterJS.SetCurSize(oWriterJS.GetCurSize() - 1);

                oWriterJS += (L"];\n\n");

                delete[] arSymbolsAll;
            }

            if (true)
            {
                BYTE* pData = NULL;
                LONG lLen = 0;
                applicationFonts->GetList()->ToBuffer(&pData, &lLen, L"", false, nVersion);

                char* cData64 = NULL;
                int nData64Dst = 0;
                NSFile::CBase64Converter::Encode(pData, (int)lLen, cData64, nData64Dst, NSBase64::B64_BASE64_FLAG_NOCRLF);

                std::wstring sData64 = NSFile::CUtf8Converter::GetUnicodeFromCharPtr(cData64, (LONG)nData64Dst, FALSE);

                oWriterJS.WriteString(L"window[\"g_fonts_selection_bin\"] = \"");
                oWriterJS.WriteString(sData64);
                oWriterJS.WriteString(L"\";\n");

                RELEASEARRAYOBJECTS(pData);
                RELEASEARRAYOBJECTS(cData64);
            }

            NSFile::CFileBinary oFile;
            oFile.CreateFileW(sAllFontsPath);
            oFile.WriteStringUTF8(oWriterJS.GetData(), true);
            oFile.CloseFile();
        }

        if (!strFontSelectionBin.empty())
        {
            BYTE* pData = NULL;
            LONG lLen = 0;
            applicationFonts->GetList()->ToBuffer(&pData, &lLen, L"", false);

            NSFile::CFileBinary oFile;
            oFile.CreateFileW(strFontSelectionBin);
            oFile.WriteFile(pData, (DWORD)lLen);

            if (pRangeBuilder)
            {
                size_t nPosCur = pRangeBuilder->GetCurSize();
                pRangeBuilder->SetCurSize(0);
                pRangeBuilder->WriteInt(nRangeBuilderCount);
                pRangeBuilder->SetCurSize(nPosCur);
                oFile.WriteFile(pRangeBuilder->GetData(), (DWORD)pRangeBuilder->GetCurSize());

                // init ranges
                applicationFonts->InitializeRanges(pRangeBuilder->GetData());
            }

            oFile.CloseFile();

            RELEASEARRAYOBJECTS(pData);
        }

        RELEASEOBJECT(pRangeBuilder);

        if (CheckBreak()) return;

        if (m_pMain->m_bIsNeedThumbnails)
        {
            m_arFonts.clear();
            m_arFonts.resize(nCountFonts);
            for (int index = 0; index < nCountFonts; ++index)
            {
                std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(arrFonts[index]);

                // thumbnail
                int lFontIndex = 0;
                int lFaceIndex = 0;
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

                std::map<LONG, std::wstring>::iterator pPair2 = mapFontFiles2.find(lFontIndex);
                std::wstring strFontPath = L"";
                if (mapFontFiles2.end() != pPair2)
                    strFontPath = pPair2->second;

                m_arFonts[index].m_sFile = strFontPath;
                m_arFonts[index].m_sName = pPair->second.m_sName;
                m_arFonts[index].m_lFaceIndex = lFaceIndex;
            }
        }

        if (m_pMain->m_bSeparateThumbnails)
            return;

        if (m_pMain->m_bIsNeedThumbnails)
        {
            SaveThumbnails(applicationFonts);
        }
    }

    void PrepareForThumbnails(NSFonts::IApplicationFonts* applicationFonts)
    {
        if (m_arFonts.empty())
        {
            m_bIsCheckThumbnailsMode = true;
            SaveAllFontsJS(applicationFonts, -1);
            m_bIsCheckThumbnailsMode = false;
        }
    }

    void SaveThumbnails(NSFonts::IApplicationFonts* applicationFonts)
    {
        std::vector<std::wstring> arrFiles;
        bool bIsNeedCheck = false;

        int nCountScales = m_pMain->m_arThumbnailsScales.size();
        for (int iX = 0; iX < nCountScales; ++iX)
        {
            if (CheckBreak()) return;

            double dScale = m_pMain->m_arThumbnailsScales[iX];

            std::wstring strThumbnailPath = m_pMain->m_sDirectory + L"/fonts_thumbnail";
            int nScaleOut = (int)(dScale * 10 + 0.5);

            if (nScaleOut == 10)
                strThumbnailPath += L".png";
            else if ((nScaleOut % 10) == 0)
                strThumbnailPath += L"@" + std::to_wstring((int)(nScaleOut / 10)) + L"x.png";
            else
                strThumbnailPath += L"@" + std::to_wstring((int)(nScaleOut / 10)) + L"." + std::to_wstring((int)(nScaleOut % 10)) + L"x.png";

            arrFiles.push_back(strThumbnailPath);

            if (!NSFile::CFileBinary::Exists(strThumbnailPath))
                bIsNeedCheck = true;
        }

        if (!bIsNeedCheck)
            return;

        NSFonts::IApplicationFonts* applicationFontsGood = applicationFonts;
        if (NULL == applicationFontsGood)
        {
            applicationFontsGood = NSFonts::NSApplication::Create();
            applicationFontsGood->InitializeFromFolder(m_pMain->m_sDirectory);
        }

        NSFonts::IFontManager* pManager = applicationFontsGood->GenerateFontManager();
        NSFonts::IFontsCache* pCache = NSFonts::NSFontCache::Create();
        pCache->SetStreams(applicationFontsGood->GetStreams());
        pManager->SetOwnerCache(pCache);

        PrepareForThumbnails(applicationFontsGood);

        int nCountFonts = (int)m_arFonts.size();
        for (int iX = 0; iX < nCountScales; ++iX)
        {
            if (CheckBreak()) return;

            double dScale = m_pMain->m_arThumbnailsScales[iX];
            std::wstring strThumbnailPath = arrFiles[iX];

            if (NSFile::CFileBinary::Exists(strThumbnailPath))
                continue;

            // создаем картинку для табнейлов
            double dDpi = 96 * dScale;
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
                if (CheckBreak()) return;

                CFontThumbnail& font = m_arFonts[index];

                pRenderer->put_FontPath(font.m_sFile);
                pRenderer->put_FontFaceIndex(font.m_lFaceIndex);
                pManager->LoadFontFromFile(font.m_sFile, font.m_lFaceIndex, 14, dDpi, dDpi);

                bool bIsSymbol = false;
                NSFonts::IFontFile* pFile = pManager->GetFile();

                if (pFile)
                    bIsSymbol = pFile->IsSymbolic(false);

                std::wstring sFontName = font.m_sName;

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
                    pRenderer->put_FontFaceIndex(0);
                }
                else if (pFile)
                {
                    // у нас режим "без квадратов"
                    // но есть шрифты, в которых символы есть, но нулевой ширины.
                    // только из-за таких шрифтов делаем заглушку
                    int nFontNameLen = (int)sFontName.length();
                    bool bIsExistEmpty = false;

                    for (int nC = 0; nC < nFontNameLen; nC++)
                    {
                        int nCMapIndex = 0;
                        int nGid = pFile->SetCMapForCharCode(sFontName.at(nC), &nCMapIndex);
                        if (0 < nGid && 0.0001 > pFile->GetCharWidth(nGid))
                        {
                            bIsExistEmpty = true;
                            break;
                        }
                    }

                    if (bIsExistEmpty)
                    {
                        NSFonts::CFontSelectFormat oSelectFormat;
                        oSelectFormat.wsName = new std::wstring(L"Arial");
                        NSFonts::CFontInfo* pInfoCur = pManager->GetFontInfoByParams(oSelectFormat);
                        if (NULL != pInfoCur)
                        {
                            pManager->LoadFontFromFile(pInfoCur->m_wsFontPath, 0, 14, dDpi, dDpi);
                        }
                        pRenderer->put_FontPath(pInfoCur->m_wsFontPath);
                        pRenderer->put_FontFaceIndex(0);
                    }
                }

                pRenderer->PathCommandStart();
                pRenderer->BeginCommand(c_nClipType);
                pRenderer->PathCommandRect(0, 25.4 * (index * lH1_px) / dDpi, dW_mm, 25.4 * lH1_px / dDpi);
                pRenderer->EndCommand(c_nClipType);
                pRenderer->PathCommandEnd();

                pRenderer->put_FontStringGID(FALSE);
                pRenderer->put_FontCharSpace(0);
                pRenderer->put_FontSize(14);

                pRenderer->CommandDrawText(sFontName, 5, 25.4 * (index * lH1_px + lH1_px) / dDpi - 2, 0, 0);

                pRenderer->BeginCommand(c_nResetClipType);
                pRenderer->EndCommand(c_nResetClipType);

                pRenderer->CloseFont();
                pCache->Clear();
                applicationFontsGood->GetStreams()->Clear();
            }

            RELEASEOBJECT(pRenderer);            

            oFrame.SaveFile(strThumbnailPath, 4);
        }

        RELEASEOBJECT(pManager);

        if (applicationFonts == NULL)
            RELEASEOBJECT(applicationFontsGood);
    }
};

CApplicationFontsWorker::CApplicationFontsWorker()
{
    m_bIsUseSystemFonts = true;
    m_bIsNeedThumbnails = true;
    m_bIsUseOpenType    = true;
    m_bIsUseAllVersions = false;
    m_bSeparateThumbnails = false;

    m_arThumbnailsScales.clear();
    m_arThumbnailsScales.push_back(1);
    m_arThumbnailsScales.push_back(1.5);
    m_arThumbnailsScales.push_back(2);

    m_pInternal = new CApplicationFontsWorker_private(this);
}
CApplicationFontsWorker::~CApplicationFontsWorker()
{
    RELEASEOBJECT(m_pInternal);
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
    
    std::sort(strFontsW_Cur.begin(), strFontsW_Cur.end());
    
    bool bIsEqual = true;
    if (strFonts.size() != strFontsW_Cur.size())
        bIsEqual = false;
    
    if (bIsEqual)
    {
        int nCount = (int)strFonts.size();
        for (int i = 0; i < nCount; ++i)
        {
            if (strFonts[i] != NSFile::CUtf8Converter::GetUtf8StringFromUnicode2(strFontsW_Cur[i].c_str(), (LONG)strFontsW_Cur[i].length()))
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
        std::vector<std::wstring> arFiles = NSDirectory::GetFiles(m_sDirectory, false);
        for (std::vector<std::wstring>::iterator fileIter = arFiles.begin(); fileIter != arFiles.end(); fileIter++)
        {
            NSFile::CFileBinary::Remove(*fileIter);
        }
        
        int nFlag = 3;
        if (!m_bIsUseOpenType)
            nFlag = 2;

        NSStringUtils::CStringBuilder oFontsLog;
#ifdef ONLYOFFICE_FONTS_VERSION_
        oFontsLog.WriteString(L"ONLYOFFICE_FONTS_VERSION_");
        oFontsLog.WriteString(std::to_wstring(ONLYOFFICE_FONTS_VERSION_));
        oFontsLog.WriteString(L"\n");
#endif
        int nCount = (int)strFontsW_Cur.size();
        for (int i = 0; i < nCount; ++i)
        {
            oFontsLog.WriteString(strFontsW_Cur[i]);
            oFontsLog.WriteString(L"\n");
        }
        
        pApplicationF->InitializeFromArrayFiles(strFontsW_Cur, nFlag);

        m_pInternal->SaveAllFontsJS(pApplicationF, -1);

        if (m_bIsUseAllVersions)
            m_pInternal->SaveAllFontsJS(pApplicationF, 0);

        if (!m_pInternal->CheckBreak())
            NSFile::CFileBinary::SaveToFile(strFontsCheckPath, oFontsLog.GetData());
    }
    
    pApplicationF->Release();
    pApplicationF = NULL;

    if (!m_pInternal->m_pBreaker || m_pInternal->m_pBreaker->IsFontsWorkerRunned())
    {
        pApplicationF = NSFonts::NSApplication::Create();
        pApplicationF->InitializeFromFolder(m_sDirectory);
    }
    else
    {
        std::vector<std::wstring> arFiles = NSDirectory::GetFiles(m_sDirectory, false);
        for (std::vector<std::wstring>::iterator fileIter = arFiles.begin(); fileIter != arFiles.end(); fileIter++)
        {
            NSFile::CFileBinary::Remove(*fileIter);
        }
    }
    
    return pApplicationF;
}

void CApplicationFontsWorker::CheckThumbnails()
{
    m_pInternal->SaveThumbnails(NULL);
}

void CApplicationFontsWorker::SetBreaker(CApplicationFontsWorkerBreaker* pChecker)
{
    m_pInternal->m_pBreaker = pChecker;
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

std::vector<std::wstring> CApplicationFontsWorker::GetFontNamesWithExcludes(NSFonts::IApplicationFonts* pFonts, std::vector<std::wstring> excludes)
{
    std::vector<std::wstring> arNames;
    
    if (!pFonts || !pFonts->GetList())
        return arNames;
    
    std::vector<NSFonts::CFontInfo*>* arInfos = pFonts->GetList()->GetFonts();
    
    std::map<std::wstring, bool> map;
    
    for (std::vector<NSFonts::CFontInfo*>::iterator iter = arInfos->begin(); iter != arInfos->end(); iter++)
    {
        std::wstring fontName = (*iter)->m_wsFontName;
        
        bool isExclude = false;
        for (size_t i = 0; i < excludes.size(); ++i) {
            if (fontName.find(excludes[i]) != std::string::npos) {
                isExclude = true;
                break;
            }
        }
        
        if (isExclude) {
            continue;
        }
        
        if (map.find(fontName) == map.end()) {
            arNames.push_back(fontName);
            map[fontName] = true;
        }
    }
    
    std::sort(arNames.begin(), arNames.end());
    
    return arNames;
}
