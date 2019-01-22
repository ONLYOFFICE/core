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
// FontMaps.cpp : Defines the entry point for the console application.
//

#include "../../common/File.h"
#include <algorithm>
#include "../../graphics/pro/Fonts.h"
#include "../../common/StringBuilder.h"

class CFontInfoJS
{
public:		
    std::wstring	m_sName;

	LONG	m_lIndexR;
	LONG	m_lFaceIndexR;
    std::vector<std::wstring> namesR;

	LONG	m_lIndexI;
	LONG	m_lFaceIndexI;
    std::vector<std::wstring> namesI;

	LONG	m_lIndexB;
	LONG	m_lFaceIndexB;
    std::vector<std::wstring> namesB;

	LONG	m_lIndexBI;
	LONG	m_lFaceIndexBI;
    std::vector<std::wstring> namesBI;

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

        namesR.clear();
        namesR = oSrc.namesR;

        namesI.clear();
        namesI = oSrc.namesI;

        namesB.clear();
        namesB = oSrc.namesB;

        namesBI.clear();
        namesBI = oSrc.namesBI;

		return *this;
	}
};

int main(int argc, char* argv[])
{
    CApplicationFonts oFonts;
    oFonts.InitializeFromFolder(L"D:\\onlyoffice_trunk\\fonts\\all");
    //oFonts.Initialize();

    std::wstring strFontDictionaryPath = L"D:\\GIT\\core\\DesktopEditor\\freetype_names\\FontMaps\\FontDictionary.h";

    int nCount = oFonts.GetList()->GetFonts()->GetCount();

	// теперь строим массив всех шрифтов по имени
    std::map<std::wstring, CFontInfoJS> mapFonts;
    std::map<std::wstring, CFontInfoJS> mapFontsUnicodes;
    std::vector<std::wstring> arrFonts;
    std::vector<std::wstring> arrFontsUnicodes;

	int nError = 0;

    std::map<std::wstring, bool> mapMainAscii;
	for (int i = 0; i < nCount; ++i)
	{
        CFontInfo* pInfo = oFonts.GetList()->GetFonts()->operator [](i);
        std::wstring strPath = pInfo->m_wsFontPath;
        std::wstring strName = pInfo->m_wsFontName;

		LONG lFontIndex = 0;
		LONG lFaceIndex = 0;

		lFontIndex = (LONG)i;

		if (pInfo->m_lIndex >= 0)
			lFaceIndex = pInfo->m_lIndex;

        mapMainAscii.insert(std::pair<std::wstring, bool>(pInfo->m_wsFontName, true));

        std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(pInfo->m_wsFontName);
        if (mapFonts.end() != pPair)
		{
            pPair->second.m_sName = pInfo->m_wsFontName;

			if (pInfo->m_bBold && pInfo->m_bItalic)
			{
                if (-1 != pPair->second.m_lIndexBI)
					nError++;

                pPair->second.m_lIndexBI = lFontIndex;
                pPair->second.m_lFaceIndexBI = lFaceIndex;

                pPair->second.namesBI.clear();
                pPair->second.namesBI = pInfo->names;
			}
			else if (pInfo->m_bBold)
			{
                if (-1 != pPair->second.m_lIndexB)
					nError++;

                pPair->second.m_lIndexB = lFontIndex;
                pPair->second.m_lFaceIndexB = lFaceIndex;

                pPair->second.namesB.clear();
                pPair->second.namesB = pInfo->names;
			}
			else if (pInfo->m_bItalic)
			{
                if (-1 != pPair->second.m_lIndexI)
					nError++;

                pPair->second.m_lIndexI = lFontIndex;
                pPair->second.m_lFaceIndexI = lFaceIndex;

                pPair->second.namesI.clear();
                pPair->second.namesI = pInfo->names;
			}
			else
			{
                if (-1 != pPair->second.m_lIndexR)
					nError++;

                pPair->second.m_lIndexR = lFontIndex;
                pPair->second.m_lFaceIndexR = lFaceIndex;

                pPair->second.namesR.clear();
                pPair->second.namesR = pInfo->names;
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

                fontInfo.namesBI.clear();
                fontInfo.namesBI = pInfo->names;
			}
			else if (pInfo->m_bBold)
			{
				fontInfo.m_lIndexB = lFontIndex;
				fontInfo.m_lFaceIndexB = lFaceIndex;

                fontInfo.namesB.clear();
                fontInfo.namesB = pInfo->names;
			}
			else if (pInfo->m_bItalic)
			{
				fontInfo.m_lIndexI = lFontIndex;
				fontInfo.m_lFaceIndexI = lFaceIndex;

                fontInfo.namesI.clear();
                fontInfo.namesI = pInfo->names;
			}
			else
			{
				fontInfo.m_lIndexR = lFontIndex;
				fontInfo.m_lFaceIndexR = lFaceIndex;

                fontInfo.namesR.clear();
                fontInfo.namesR = pInfo->names;
			}

            mapFonts.insert(std::pair<std::wstring, CFontInfoJS>(fontInfo.m_sName, fontInfo));
            arrFonts.push_back(fontInfo.m_sName);
		}
	}

	// additional names
	for (int i = 0; i < nCount; ++i)
	{
        CFontInfo* pInfo = oFonts.GetList()->GetFonts()->operator [](i);
        std::wstring strPath = pInfo->m_wsFontPath;
        std::wstring strName = pInfo->m_wsFontName;

		LONG lFontIndex = 0;
		LONG lFaceIndex = 0;

		lFontIndex = (LONG)i;

		if (pInfo->m_lIndex >= 0)
			lFaceIndex = pInfo->m_lIndex;

        int nNamesAdditional = pInfo->names.size();
		for (int j = 0; j < nNamesAdditional; ++j)
		{
            std::wstring strNameA = pInfo->names[j];

            std::map<std::wstring, bool>::iterator pPairMain = mapMainAscii.find(strNameA);
            if (mapMainAscii.end() != pPairMain)
				continue;

            const wchar_t* pBufferA = strNameA.c_str();
            int len = strNameA.length();
			
            std::map<std::wstring, CFontInfoJS>* pMap = &mapFonts;
            std::vector<std::wstring>* pArrFonts = &arrFonts;

			for (int k = 0; k < len; ++k)
			{
				if (pBufferA[k] > 255)
				{
					pMap = &mapFontsUnicodes;
					pArrFonts = &arrFontsUnicodes;
					break;
				}
			}
			
            std::map<std::wstring, CFontInfoJS>::iterator pPair = pMap->find(strNameA);
            if (pMap->end() != pPair)
			{
                pPair->second.m_sName = strNameA;

				if (pInfo->m_bBold && pInfo->m_bItalic)
				{
                    if (-1 != pPair->second.m_lIndexBI)
						nError++;

                    pPair->second.m_lIndexBI = lFontIndex;
                    pPair->second.m_lFaceIndexBI = lFaceIndex;

                    pPair->second.namesBI.clear();
                    pPair->second.namesBI = pInfo->names;
				}
				else if (pInfo->m_bBold)
				{
                    if (-1 != pPair->second.m_lIndexB)
						nError++;

                    pPair->second.m_lIndexB = lFontIndex;
                    pPair->second.m_lFaceIndexB = lFaceIndex;

                    pPair->second.namesB.clear();
                    pPair->second.namesB = pInfo->names;
				}
				else if (pInfo->m_bItalic)
				{
                    if (-1 != pPair->second.m_lIndexI)
						nError++;

                    pPair->second.m_lIndexI = lFontIndex;
                    pPair->second.m_lFaceIndexI = lFaceIndex;

                    pPair->second.namesI.clear();
                    pPair->second.namesI = pInfo->names;
				}
				else
				{
                    if (-1 != pPair->second.m_lIndexR)
						nError++;

                    pPair->second.m_lIndexR = lFontIndex;
                    pPair->second.m_lFaceIndexR = lFaceIndex;

                    pPair->second.namesR.clear();
                    pPair->second.namesR = pInfo->names;
				}
			}
			else
			{
				CFontInfoJS fontInfo;

				fontInfo.m_sName = strNameA;

				if (pInfo->m_bBold && pInfo->m_bItalic)
				{
					fontInfo.m_lIndexBI = lFontIndex;
					fontInfo.m_lFaceIndexBI = lFaceIndex;

                    fontInfo.namesBI.clear();
                    fontInfo.namesBI = pInfo->names;
				}
				else if (pInfo->m_bBold)
				{
					fontInfo.m_lIndexB = lFontIndex;
					fontInfo.m_lFaceIndexB = lFaceIndex;

                    fontInfo.namesB.clear();
                    fontInfo.namesB = pInfo->names;
				}
				else if (pInfo->m_bItalic)
				{
					fontInfo.m_lIndexI = lFontIndex;
					fontInfo.m_lFaceIndexI = lFaceIndex;

                    fontInfo.namesI.clear();
                    fontInfo.namesI = pInfo->names;
				}
				else
				{
					fontInfo.m_lIndexR = lFontIndex;
					fontInfo.m_lFaceIndexR = lFaceIndex;

                    fontInfo.namesR.clear();
                    fontInfo.namesR = pInfo->names;
				}

                pMap->insert(std::pair<std::wstring, CFontInfoJS>(fontInfo.m_sName, fontInfo));
                pArrFonts->push_back(fontInfo.m_sName);
			}
		}
	}
	// -------------------------------------------

	// теперь сортируем шрифты по имени ----------
    size_t nCountFonts = arrFonts.size();
    std::sort(arrFonts.begin(), arrFonts.end());

    size_t nCountFontsU = arrFontsUnicodes.size();
    std::sort(arrFontsUnicodes.begin(), arrFontsUnicodes.end());

    NSFile::CFileBinary oFileW;
	oFileW.CreateFile(strFontDictionaryPath);

	BYTE bom[3];
	bom[0] = 0xEF;
	bom[1] = 0xBB;
	bom[2] = 0xBF;
    oFileW.WriteFile(bom, 3);

    NSStringUtils::CStringBuilder oBuilder;

    oBuilder.WriteString(L"\
/*\n\
 * (c) Copyright Ascensio System SIA 2010-2019\n\
 *\n\
 * This program is a free software product. You can redistribute it and/or\n\
 * modify it under the terms of the GNU Affero General Public License (AGPL)\n\
 * version 3 as published by the Free Software Foundation. In accordance with\n\
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect\n\
 * that Ascensio System SIA expressly excludes the warranty of non-infringement\n\
 * of any third-party rights.\n\
 *\n\
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied\n\
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For\n\
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html\n\
 *\n\
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha\n\
 * street, Riga, Latvia, EU, LV-1050.\n\
 *\n\
 * The  interactive user interfaces in modified source and object code versions\n\
 * of the Program must display Appropriate Legal Notices, as required under\n\
 * Section 5 of the GNU AGPL version 3.\n\
 *\n\
 * Pursuant to Section 7(b) of the License you must retain the original Product\n\
 * logo when distributing the program. Pursuant to Section 7(e) we decline to\n\
 * grant you any rights under trademark law for use of our trademarks.\n\
 *\n\
 * All the Product's GUI elements, including illustrations and icon sets, as\n\
 * well as technical writing content are licensed under the terms of the\n\
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License\n\
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode\n\
 *\n\
 */\n");

    oBuilder.WriteString(L"#ifndef _FONT_DICTIONARY_H\n\n\
typedef struct FD_FontMapRec_\n\
{\n\
	const char*		m_name;\n\
\n\
	int				m_index_r;\n\
	int				m_index_i;\n\
	int				m_index_b;\n\
	int				m_index_bi;\n\
} FD_FontMapRec;\n\n\
typedef struct FD_FontMapRecW_\n\
{\n\
	const wchar_t*	m_name;\n\
\n\
	int				m_index_r;\n\
	int				m_index_i;\n\
	int				m_index_b;\n\
	int				m_index_bi;\n\
} FD_FontMapRecW;\n\n");

    int nAsciiNamesCount = (int)arrFonts.size();
    oBuilder.WriteString(L"#define FONTS_DICT_ASCII_NAMES_COUNT ");
    oBuilder.AddInt(nAsciiNamesCount);
    oBuilder.WriteString(L"\n");
    oBuilder.WriteString(L"static const FD_FontMapRec FD_Ascii_Names[FONTS_DICT_ASCII_NAMES_COUNT] = \n{\n");

	for (int k = 0; k < nAsciiNamesCount; ++k)
	{
        std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFonts.find(arrFonts[k]);

        oBuilder.WriteString(L"\t{ \"");
        oBuilder.WriteString(pPair->second.m_sName);
        oBuilder.WriteString(L"\", ");

		if (k != (nAsciiNamesCount - 1))
		{
            oBuilder.AddInt(pPair->second.m_lIndexR);
            oBuilder.WriteString(L", ");
            oBuilder.AddInt(pPair->second.m_lIndexI);
            oBuilder.WriteString(L", ");
            oBuilder.AddInt(pPair->second.m_lIndexB);
            oBuilder.WriteString(L", ");
            oBuilder.AddInt(pPair->second.m_lIndexBI);
            oBuilder.WriteString(L" },\n");
		}
		else
		{
            oBuilder.AddInt(pPair->second.m_lIndexR);
            oBuilder.WriteString(L", ");
            oBuilder.AddInt(pPair->second.m_lIndexI);
            oBuilder.WriteString(L", ");
            oBuilder.AddInt(pPair->second.m_lIndexB);
            oBuilder.WriteString(L", ");
            oBuilder.AddInt(pPair->second.m_lIndexBI);
            oBuilder.WriteString(L" }\n");
		}
	}

    oBuilder.WriteString(L"};\n\n");

	int _offsets[256];
	for (int t = 0; t < 256; ++t)
		_offsets[t] = -1;

	int nCurChar = -1;
	for (int k = 0; k < nAsciiNamesCount; ++k)
	{
        int nChar = (int)arrFonts[k].c_str()[0];
		nChar = max(0, min(nChar, 255));

		if (nChar != nCurChar)
		{
			_offsets[nChar] = k;
		}
		nCurChar = nChar;
	}

    oBuilder.WriteString(L"static const int FD_Ascii_Names_Offsets[256] =\n{\n");

	for (int k = 0; k < 256; ++k)
	{
        std::wstring sMem = std::to_wstring(_offsets[k]);

        while (sMem.length() < 4)
            sMem = (L" " + sMem);
		
		if (0 == k % 32)
            sMem = L"\t" + sMem;

		if (k != 255)
            sMem += L",";
		
		if (0 == (k + 1) % 32)
            sMem += L"\n";

        oBuilder.WriteString(sMem);
	}

    oBuilder.WriteString(L"};\n\n");

    int nUnicodeNamesCount = (int)arrFontsUnicodes.size();
    oBuilder.WriteString(L"#define FONTS_DICT_UNICODE_NAMES_COUNT ");
    oBuilder.AddInt(nUnicodeNamesCount);
    oBuilder.WriteString(L"\n");
    oBuilder.WriteString(L"static const FD_FontMapRecW FD_Unicode_Names[FONTS_DICT_UNICODE_NAMES_COUNT] = \n{\n");

	for (int k = 0; k < nUnicodeNamesCount; ++k)
	{
        std::map<std::wstring, CFontInfoJS>::iterator pPair = mapFontsUnicodes.find(arrFontsUnicodes[k]);

        oBuilder.WriteString(L"\t{ L\"");
        oBuilder.WriteString(pPair->second.m_sName);
        oBuilder.WriteString(L"\", ");

		if (k != (nAsciiNamesCount - 1))
		{
            oBuilder.AddInt(pPair->second.m_lIndexR);
            oBuilder.WriteString(L", ");
            oBuilder.AddInt(pPair->second.m_lIndexI);
            oBuilder.WriteString(L", ");
            oBuilder.AddInt(pPair->second.m_lIndexB);
            oBuilder.WriteString(L", ");
            oBuilder.AddInt(pPair->second.m_lIndexBI);
            oBuilder.WriteString(L" },\n");
		}
		else
		{
            oBuilder.AddInt(pPair->second.m_lIndexR);
            oBuilder.WriteString(L", ");
            oBuilder.AddInt(pPair->second.m_lIndexI);
            oBuilder.WriteString(L", ");
            oBuilder.AddInt(pPair->second.m_lIndexB);
            oBuilder.WriteString(L", ");
            oBuilder.AddInt(pPair->second.m_lIndexBI);
            oBuilder.WriteString(L" }\n");
		}
	}

    oBuilder.WriteString(L"};\n\n");

    oBuilder.WriteString(L"typedef struct FD_Font_Rec\n\
{\n\
	const char*		m_name;\n\
\n\
	long			m_lIndex;\n\
\n\
	unsigned char	m_bBold;\n\
	unsigned char	m_bItalic;\n\
	unsigned char	m_bIsFixed;\n\
\n\
	unsigned char	m_aPanose[10];\n\
	unsigned long	m_ulUnicodeRange1;\n\
	unsigned long	m_ulUnicodeRange2;\n\
	unsigned long	m_ulUnicodeRange3;\n\
	unsigned long	m_ulUnicodeRange4;\n\
\n\
	unsigned long	m_ulCodePageRange1;\n\
	unsigned long	m_ulCodePageRange2;\n\
\n\
	unsigned short	m_usWeigth;\n\
	unsigned short	m_usWidth;\n\
\n\
	short			m_sFamilyClass;\n\
	unsigned char	m_eFontFormat;\n\
\n\
	short			m_shAvgCharWidth;\n\
	short			m_shAscent;\n\
	short			m_shDescent;\n\
	short			m_shLineGap;\n\
	short			m_shXHeight;\n\
	short			m_shCapHeight;\n\
} FD_Font;\n\n");

	int nAllFontsCount = (int)nCount;
    oBuilder.WriteString(L"#define FONTS_DICT_ASCII_FONTS_COUNT ");
    oBuilder.AddInt(nCount);
    oBuilder.WriteString(L"\n");

    oBuilder.WriteString(L"static const FD_Font FD_Ascii_Files[FONTS_DICT_ASCII_FONTS_COUNT] = \n{\n");

	for (int k = 0; k < nCount; ++k)
	{
        CFontInfo* pInfo = oFonts.GetList()->GetFonts()->operator [](k);

#if 1
		// CORRECT!!!
        if (pInfo->m_wsFontName == L"Monotype Sorts")
			pInfo->m_aPanose[0] = 5;
#endif

        oBuilder.WriteString(L"\t{\"");
        oBuilder.WriteString(pInfo->m_wsFontName);
		
        oBuilder.WriteString(L"\", ");
        oBuilder.AddInt(pInfo->m_lIndex);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_bBold);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_bItalic);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_bIsFixed);

        oBuilder.WriteString(L", { ");
        oBuilder.AddInt(pInfo->m_aPanose[0]);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_aPanose[1]);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_aPanose[2]);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_aPanose[3]);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_aPanose[4]);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_aPanose[5]);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_aPanose[6]);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_aPanose[7]);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_aPanose[8]);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_aPanose[9]);
        oBuilder.WriteString(L" }, ");

        oBuilder.AddInt64((__int64)pInfo->m_ulUnicodeRange1);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt64((__int64)pInfo->m_ulUnicodeRange2);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt64((__int64)pInfo->m_ulUnicodeRange3);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt64((__int64)pInfo->m_ulUnicodeRange4);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt64((__int64)pInfo->m_ulCodePageRange1);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt64((__int64)pInfo->m_ulCodePageRange2);
        oBuilder.WriteString(L", ");

        oBuilder.AddInt(pInfo->m_usWeigth);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_usWidth);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_sFamilyClass);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_eFontFormat);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_shAvgCharWidth);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_shAscent);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_shDescent);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_shLineGap);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_shXHeight);
        oBuilder.WriteString(L", ");
        oBuilder.AddInt(pInfo->m_shCapHeight);
        oBuilder.WriteString(L" }");

		if (k != (nCount - 1))
            oBuilder.WriteString(L",\n");
		else
            oBuilder.WriteString(L"\n");
	}

    oBuilder.WriteString(L"};\n\n");

    oBuilder.WriteString(L"// error : ");
    oBuilder.AddInt(nError);
    oBuilder.WriteString(L"\n\n");
    oBuilder.WriteString(L"#endif /* _FONT_DICTIONARY_H */");

    oFileW.WriteStringUTF8(oBuilder.GetData());
	oFileW.CloseFile();

    return 0;
}

