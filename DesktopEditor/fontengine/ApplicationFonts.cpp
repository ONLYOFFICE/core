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
#include "ApplicationFonts.h"
#include "../common/File.h"
#include "../common/Directory.h"
#include FT_SFNT_NAMES_H
#include "fontdictionaryworker.h"
#include "../common/ByteBuilder.h"

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

namespace NSFonts
{
    void makeLower(std::wstring& name)
    {
        int nLen = (int)name.length();
        wchar_t* pStr = (wchar_t*)name.c_str();

        for (int i = 0; i < nLen; ++i)
        {
            if (pStr[i] >= 'A' && pStr[i] <= 'Z')
                pStr[i] = pStr[i] + 'a' - 'A';
        }
    }
    std::wstring prepareFont3000(std::wstring& name)
    {
        std::wstring sRet;
        int nLen = (int)name.length();
        wchar_t* pStr = (wchar_t*)name.c_str();

        for (int i = 0; i < nLen; ++i)
        {
            if (pStr[i] == ' ' || pStr[i] == '-')
                continue;
            sRet.append(pStr + i, 1);
        }
        return sRet;
    }

    CFontInfo* FromBuffer(BYTE*& pBuffer, std::wstring strDir)
    {
        // name
        int lLen = *((int*)pBuffer);
        pBuffer += sizeof(int);

        int len2 = lLen >> 1;
        wchar_t* sName = new wchar_t[len2 + 1];
        for (int i = 0; i < len2; ++i)
        {
            sName[i] = (wchar_t)(pBuffer[2 * i] | (pBuffer[2 * i + 1] << 8));
            if (sName[i] == wchar_t('\\'))
                sName[i] = wchar_t('/');
            if (0 == sName[i])
            {
                len2 = i;
                break;
            }
        }
        sName[len2] = 0;

        std::wstring strName(sName, len2);
        pBuffer += lLen;

        RELEASEARRAYOBJECTS(sName);

        // path
        lLen = *((int*)pBuffer);
        pBuffer += sizeof(int);

        len2 = lLen >> 1;
        sName = new wchar_t[len2 + 1];
        for (int i = 0; i < len2; ++i)
        {
            sName[i] = (wchar_t)(pBuffer[2 * i] | (pBuffer[2 * i + 1] << 8));
            if (sName[i] == wchar_t('\\'))
                sName[i] = wchar_t('/');
            if (0 == sName[i])
            {
                len2 = i;
                break;
            }
        }
        sName[len2] = 0;

        std::wstring strPath(sName, len2);
        pBuffer += lLen;

        RELEASEARRAYOBJECTS(sName);

        // index
        LONG lIndex = *((int*)pBuffer);
        pBuffer += sizeof(int);

        // italic
        INT bItalic = *((INT*)pBuffer);
        pBuffer += sizeof(INT);

        // bold
        INT bBold = *((INT*)pBuffer);
        pBuffer += sizeof(INT);

        // FixedWidth
        INT bFixedWidth = *((INT*)pBuffer);
        pBuffer += sizeof(INT);

        // Panose
        lLen = *((int*)pBuffer); // должно быть равно 10
        pBuffer += sizeof(int);

        BYTE pPanose[10];
        memcpy( (void *)pPanose, (const void *)pBuffer, 10 );
        pBuffer += lLen;

        // ulUnicodeRange1
        UINT ulRange1 = *((UINT*)pBuffer);
        pBuffer += sizeof(UINT);

        // ulUnicodeRange2
        UINT ulRange2 = *((UINT*)pBuffer);
        pBuffer += sizeof(UINT);

        // ulUnicodeRange3
        UINT ulRange3 = *((UINT*)pBuffer);
        pBuffer += sizeof(UINT);

        // ulUnicodeRange4
        UINT ulRange4 = *((UINT*)pBuffer);
        pBuffer += sizeof(UINT);

        // ulCodePageRange1
        UINT ulCodeRange1 = *((UINT*)pBuffer);
        pBuffer += sizeof(UINT);

        // ulCodePageRange2
        ULONG ulCodeRange2 = *((UINT*)pBuffer);
        pBuffer += sizeof(UINT);

        // usWeightClass
        USHORT usWeight = *((USHORT*)pBuffer);
        pBuffer += sizeof(USHORT);

        // usWidthClass
        USHORT usWidth = *((USHORT*)pBuffer);
        pBuffer += sizeof(USHORT);

        // sFamilyClass
        SHORT sFamilyClass = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // FontFormat
        SHORT sFormat = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // AvgCharWidth
        SHORT shAvgCharWidth = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // Ascent
        SHORT shAscent = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // Descent
        SHORT shDescent = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // LineGap
        SHORT shLineGap = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // XHeight
        SHORT shXHeight = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

        // CapHeight
        SHORT shCapHeight = *((SHORT*)pBuffer);
        pBuffer += sizeof(SHORT);

            if (strPath.find(wchar_t('/')) == std::wstring::npos && strPath.find(wchar_t('\\')) == std::wstring::npos)
                strPath = strDir + strPath;

        CFontInfo* pInfo = new CFontInfo(strName,
            L"",
            strPath,
            lIndex,
            bBold,
            bItalic,
            bFixedWidth,
            (BYTE*)pPanose,
            ulRange1,
            ulRange2,
            ulRange3,
            ulRange4,
            ulCodeRange1,
            ulCodeRange2,
            usWeight,
            usWidth,
            sFamilyClass,
            (EFontFormat)sFormat,
            shAvgCharWidth,
            shAscent,
            shDescent,
            shLineGap,
            shXHeight,
            shCapHeight );

        return pInfo;
    }
    LONG GetBufferLen(CFontInfo* pInfo, std::wstring strDirectory, bool bIsOnlyFileName)
    {
        std::wstring sPath = pInfo->m_wsFontPath;
        if (0 != strDirectory.length())
        {
            if (0 == sPath.find(strDirectory))
            {
                sPath = sPath.substr(strDirectory.length());
            }
        }
        else if (bIsOnlyFileName)
        {
            size_t pos1 = sPath.find_last_of(wchar_t('/'));
            size_t pos2 = sPath.find_last_of(wchar_t('\\'));

            size_t pos = std::wstring::npos;
            if (pos1 != std::wstring::npos)
                pos = pos1;

            if (pos2 != std::wstring::npos)
            {
                if (pos == std::wstring::npos)
                    pos = pos2;
                else if (pos2 > pos)
                    pos = pos2;
            }

            if (pos != std::wstring::npos)
            {
                sPath = sPath.substr(pos + 1);
            }
        }
        //return 4 * g_lSizeofLONG + 3 * g_lSizeofBOOL + (m_wsFontName.GetLength() + sPath.GetLength() + 2) * g_lSizeofWCHAR + 2 * g_lSizeofUSHORT + 6 * g_lSizeofULONG + 10 + 8 * g_lSizeofSHORT;
        if (2 == sizeof(wchar_t))
        {
            return 4 * 4 + 3 * 4 + (pInfo->m_wsFontName.length() + sPath.length() + 2) * 2 + 2 * 2 + 6 * 4 + 10 + 8 * 2;
        }

        NSFile::CStringUtf16 s1;
        NSFile::CUtf8Converter::GetUtf16StringFromUnicode_4bytes2(pInfo->m_wsFontName.c_str(), pInfo->m_wsFontName.length(), s1);

        NSFile::CStringUtf16 s2;
        NSFile::CUtf8Converter::GetUtf16StringFromUnicode_4bytes2(sPath.c_str(), sPath.length(), s2);

        return 4 * 4 + 3 * 4 + (s1.Length + s2.Length + 2) * 2 + 2 * 2 + 6 * 4 + 10 + 8 * 2;
    }

    void ToBuffer(CFontInfo* pInfo, BYTE*& pBuffer, std::wstring strDirectory, bool bIsOnlyFileName)
    {
        // name
        int lLen = 0;

        if (2 == sizeof(wchar_t))
        {
            lLen = (pInfo->m_wsFontName.length() + 1) * 2;

            *((int*)(pBuffer))	= lLen;
            pBuffer += 4;

            memcpy(pBuffer, pInfo->m_wsFontName.c_str(), lLen);
            pBuffer += lLen;

            // path
            std::wstring sPath = pInfo->m_wsFontPath;
            if (0 != strDirectory.length())
            {
                if (0 == sPath.find(strDirectory))
                {
                    sPath = sPath.substr(strDirectory.length());
                }
            }
            else if (bIsOnlyFileName)
            {
                size_t pos1 = sPath.find_last_of(wchar_t('/'));
                size_t pos2 = sPath.find_last_of(wchar_t('\\'));

                size_t pos = std::wstring::npos;
                if (pos1 != std::wstring::npos)
                    pos = pos1;

                if (pos2 != std::wstring::npos)
                {
                    if (pos == std::wstring::npos)
                        pos = pos2;
                    else if (pos2 > pos)
                        pos = pos2;
                }

                if (pos != std::wstring::npos)
                {
                    sPath = sPath.substr(pos + 1);
                }
            }

            lLen = (sPath.length() + 1) * 2;

            *((INT*)(pBuffer))	= lLen;
            pBuffer += sizeof(INT);

            memcpy(pBuffer, sPath.c_str(), lLen);
            pBuffer += lLen;
        }
        else
        {
            NSFile::CStringUtf16 s1;
            NSFile::CUtf8Converter::GetUtf16StringFromUnicode_4bytes2(pInfo->m_wsFontName.c_str(), pInfo->m_wsFontName.length(), s1);

            lLen = s1.Length + 2;

            *((int*)(pBuffer))	= lLen;
            pBuffer += 4;

            memcpy(pBuffer, s1.Data, lLen);
            pBuffer += lLen;

            // path
            std::wstring sPath = pInfo->m_wsFontPath;
            if (0 != strDirectory.length())
            {
                if (0 == sPath.find(strDirectory))
                {
                    sPath = sPath.substr(strDirectory.length());
                }
            }
            else if (bIsOnlyFileName)
            {
                size_t pos1 = sPath.find_last_of(wchar_t('/'));
                size_t pos2 = sPath.find_last_of(wchar_t('\\'));

                size_t pos = std::wstring::npos;
                if (pos1 != std::wstring::npos)
                    pos = pos1;

                if (pos2 != std::wstring::npos)
                {
                    if (pos == std::wstring::npos)
                        pos = pos2;
                    else if (pos2 > pos)
                        pos = pos2;
                }

                if (pos != std::wstring::npos)
                {
                    sPath = sPath.substr(pos + 1);
                }
            }

            NSFile::CStringUtf16 s2;
            NSFile::CUtf8Converter::GetUtf16StringFromUnicode_4bytes2(sPath.c_str(), sPath.length(), s2);

            lLen = s2.Length + 2;

            *((INT*)(pBuffer))	= lLen;
            pBuffer += sizeof(INT);

            memcpy(pBuffer, s2.Data, lLen);
            pBuffer += lLen;
        }

        // index
        *((INT*)(pBuffer))	= (INT)pInfo->m_lIndex;
        pBuffer += sizeof(INT);

        // italic
        *((INT*)(pBuffer))	= pInfo->m_bItalic;
        pBuffer += sizeof(INT);

        // bold
        *((INT*)(pBuffer))	= pInfo->m_bBold;
        pBuffer += sizeof(INT);

        // FixedWidth
        *((INT*)pBuffer) = pInfo->m_bIsFixed;
        pBuffer += sizeof(INT);

        // Panose
        lLen = 10;

        *((INT*)(pBuffer))	= lLen;
        pBuffer += sizeof(INT);

        memcpy( (void *)pBuffer, (const void *)pInfo->m_aPanose, lLen );
        pBuffer += lLen;

        // ulUnicodeRange1
        *((UINT*)pBuffer) = (UINT)pInfo->m_ulUnicodeRange1;
        pBuffer += sizeof(UINT);

        // ulUnicodeRange2
        *((UINT*)pBuffer) = (UINT)pInfo->m_ulUnicodeRange2;
        pBuffer += sizeof(UINT);

        // ulUnicodeRange3
        *((UINT*)pBuffer) = (UINT)pInfo->m_ulUnicodeRange3;
        pBuffer += sizeof(UINT);

        // ulUnicodeRange4
        *((UINT*)pBuffer) = (UINT)pInfo->m_ulUnicodeRange4;
        pBuffer += sizeof(UINT);

        // ulCodePageRange1
        *((UINT*)pBuffer) = (UINT)pInfo->m_ulCodePageRange1;
        pBuffer += sizeof(UINT);

        // ulCodePageRange2
        *((UINT*)pBuffer) = (UINT)pInfo->m_ulCodePageRange2;
        pBuffer += sizeof(UINT);

        // usWeightClass
        *((USHORT*)pBuffer) = pInfo->m_usWeigth;
        pBuffer += sizeof(USHORT);

        // usWidthClass
        *((USHORT*)pBuffer) = pInfo->m_usWidth;
        pBuffer += sizeof(USHORT);

        // sFamilyClass
        *((SHORT*)pBuffer) = pInfo->m_sFamilyClass;
        pBuffer += sizeof(SHORT);

        // FontFormat
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_eFontFormat;
        pBuffer += sizeof(SHORT);

        // AvgCharWidth
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_shAvgCharWidth;
        pBuffer += sizeof(SHORT);

        // Ascent
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_shAscent;
        pBuffer += sizeof(SHORT);

        // Descent
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_shDescent;
        pBuffer += sizeof(SHORT);

        // LineGap
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_shLineGap;
        pBuffer += sizeof(SHORT);

        // XHeight
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_shXHeight;
        pBuffer += sizeof(SHORT);

        // CapHeight
        *((SHORT*)pBuffer) = (SHORT)pInfo->m_shCapHeight;
        pBuffer += sizeof(SHORT);
    }
}

namespace NSCharsets
{
    static int GetDefaultCharset(INT bUseDefCharset = TRUE)
    {
        if ( !bUseDefCharset )
            return UNKNOWN_CHARSET;

        /*
        LOCALESIGNATURE LocSig;
        GetLocaleInfo( GetSystemDefaultLCID(), LOCALE_FONTSIGNATURE, (LPWSTR)&LocSig, sizeof(LocSig) / sizeof(TCHAR) );

        if ( LocSig.lsCsbDefault[0] & 1 )
            return 0;
        else if ( LocSig.lsCsbDefault[0] & 2 )
            return 238;
        else if ( LocSig.lsCsbDefault[0] & 4 )
            return 204;
        else if ( LocSig.lsCsbDefault[0] & 8 )
            return 161;
        else if ( LocSig.lsCsbDefault[0] & 16 )
            return 162;
        else if ( LocSig.lsCsbDefault[0] & 32 )
            return 177;
        else if ( LocSig.lsCsbDefault[0] & 64 )
            return 178;
        else if ( LocSig.lsCsbDefault[0] & 128 )
            return 186;
        else if ( LocSig.lsCsbDefault[0] & 256 )
            return 163;
        else if ( LocSig.lsCsbDefault[0] & 0x10000 )
            return 222;
        else if ( LocSig.lsCsbDefault[0] & 0x20000 )
            return 128;
        else if ( LocSig.lsCsbDefault[0] & 0x40000 )
            return 134;
        else if ( LocSig.lsCsbDefault[0] & 0x80000 )
            return 129;
        else if ( LocSig.lsCsbDefault[0] & 0x100000 )
            return 136;
        else if ( LocSig.lsCsbDefault[0] & 0x200000 )
            return 130;
        else if ( LocSig.lsCsbDefault[0] & 0x20000000 )
            return 77;
        else if ( LocSig.lsCsbDefault[0] & 0x40000000 )
            return 255;
        else if ( LocSig.lsCsbDefault[0] & 0x80000000 )
            return 2;
        */

        return 0;
    }

    static void GetCodePageByCharset(unsigned char unCharset, unsigned long *pulBit, unsigned int *punLongIndex)
    {
        // Данная функция возвращает параметры, которые нужно посылать на вход
        // функции AVSFontManager::IsUnicodeRangeAvailable


        // Соответствие Charset -> Codepage: http://support.microsoft.com/kb/165478
        // http://msdn.microsoft.com/en-us/library/cc194829.aspx

        //  Charset Name       Charset Value(hex)  Codepage number
        //  ------------------------------------------------------
        //
        //  DEFAULT_CHARSET           1 (x01)
        //  SYMBOL_CHARSET            2 (x02)
        //  OEM_CHARSET             255 (xFF)
        //  ANSI_CHARSET              0 (x00)            1252
        //  RUSSIAN_CHARSET         204 (xCC)            1251
        //  EASTEUROPE_CHARSET      238 (xEE)            1250
        //  GREEK_CHARSET           161 (xA1)            1253
        //  TURKISH_CHARSET         162 (xA2)            1254
        //  BALTIC_CHARSET          186 (xBA)            1257
        //  HEBREW_CHARSET          177 (xB1)            1255
        //  ARABIC _CHARSET         178 (xB2)            1256
        //  SHIFTJIS_CHARSET        128 (x80)             932
        //  HANGEUL_CHARSET         129 (x81)             949
        //  GB2313_CHARSET          134 (x86)             936
        //  CHINESEBIG5_CHARSET     136 (x88)             950
        //  THAI_CHARSET            222 (xDE)             874
        //  JOHAB_CHARSET	        130 (x82)            1361
        //  VIETNAMESE_CHARSET      163 (xA3)            1258
        //  MAC_CHARSET              77 (x4D)

        // Соответсвие CodePage -> ulCodePageRange1 : http://www.microsoft.com/Typography/otspec/os2.htm#cpr

        if ( punLongIndex )
            *punLongIndex = 4;

        if ( unCharset == 1 )
            unCharset = GetDefaultCharset();

        if ( pulBit )
        {
            switch( unCharset )
            {
            case 0x00: *pulBit =  0; break;
            case 0xEE: *pulBit =  1; break;
            case 0xCC: *pulBit =  2; break;
            case 0xA1: *pulBit =  3; break;
            case 0xA2: *pulBit =  4; break;
            case 0xB1: *pulBit =  5; break;
            case 0xB2: *pulBit =  6; break;
            case 0xBA: *pulBit =  7; break;
            case 0xA3: *pulBit =  8; break;
            case 0xDE: *pulBit = 16; break;
            case 0x80: *pulBit = 17; break;
            case 0x86: *pulBit = 18; break;
            case 0x81: *pulBit = 19; break;
            case 0x88: *pulBit = 20; break;
            case 0x82: *pulBit = 21; break;
            case 0x4D: *pulBit = 29; break;
            case 0x02: *pulBit = 31; break;
            case 0xFF: *pulBit = 30; break;
            default:   *pulBit =  0; break;
            }
        }
    }
}

#ifdef BUILD_FONT_NAMES_DICTIONARY
#include "ftsnames.h"
#include "tttables.h"
#include "ftxf86.h"
#include "internal/internal.h"
#include "internal/tttypes.h"

void ReadNames(NSFonts::CFontInfo* pInfo, FT_Face pFace)
{
    TT_Face pTT_Face = (TT_Face)(pFace);

    pInfo->names.clear();
    if (NULL != pTT_Face)
    {
        for (int i = 0; i < pTT_Face->utf16_len; ++i)
        {
            std::wstring s(pTT_Face->utf16_names[i]);

            if (s == pInfo->m_wsFontName)
                continue;

            bool bIsPresent = false;
            for (std::vector<std::wstring>::iterator i = pInfo->names.begin(); i != pInfo->names.end(); i++)
            {
                if (*i == s)
                {
                    bIsPresent = true;
                    break;
                }
            }

            if (!bIsPresent)
                pInfo->names.push_back(s);
        }
    }
}
#endif

std::wstring CFontList::GetFontBySymbol(int symbol)
{
    for (std::list<CFontRange>::iterator iter = m_listRanges.begin(); iter != m_listRanges.end() && !m_listRanges.empty(); iter++)
    {
        CFontRange& range = *iter;
        if (symbol >= range.Start && symbol <= range.End)
        {
            return range.Name;
        }
    }

    // search range by symbol
    int _start = 0;
    int _end = m_nRangesCount - 1;

    int _center = 0;

    if (_start > _end || m_pRanges == NULL)
        return L"";

    while (_start < _end)
    {
        _center = (_start + _end) >> 1;
        CFontRange& _range = m_pRanges[_center];

        if (_range.Start > symbol)
            _end = _center - 1;
        else if (_range.End < symbol)
            _start = _center + 1;
        else
        {
            m_listRanges.push_front(_range);
            return m_pRanges[_center].Name;
        }
    }

    if (_start > _end)
        return L"";

    CFontRange& _range = m_pRanges[_start];
    if (_range.Start > symbol || _range.End < symbol)
        return L"";

    m_listRanges.push_front(_range);
    return m_pRanges[_start].Name;
}

///////////////////////////////////////////////////////////////////////////////////
int CFontList::GetCharsetPenalty(ULONG ulCandRanges[6], unsigned char unReqCharset)
{
	// Penalty = 65000 (это самый весомый параметр)

	if ( UNKNOWN_CHARSET == unReqCharset )
		return 0;

	unsigned long ulBit = 0;
	unsigned int unLongIndex = 0;
	NSCharsets::GetCodePageByCharset( unReqCharset, &ulBit, &unLongIndex );

	int nMult = 1;
	for ( int nIndex = 0; nIndex < (int)ulBit; nIndex++ )
		nMult <<= 1;

	if ( !(ulCandRanges[unLongIndex] & nMult) )
		return 65000;

	return 0;
}
int CFontList::GetSigPenalty(ULONG ulCandRanges[6], ULONG ulReqRanges[6], double dRangeWeight, double dRangeWeightSuferflouous)
{
	double dPenalty = 0;

	// Для начала просматриваем сколько вообще различных пространств надо.
	// Исходя из их общего количества, находим вес 1 пропущеного пространства.

	unsigned char arrCandidate[192], arrRequest[192];
	memset( arrCandidate, 0x00, 192 );
	memset( arrRequest, 0x00, 192 );

	int nRangesCount = 0; // Количество необходимых пространств
	int nAddCount    = 0; // количество дополнительных(ненужных) пространств у кандидата

	for ( int nIndex = 0; nIndex < 6; nIndex++ )
	{
		for ( unsigned long nBitCount = 0, nBit = 1; nBitCount < 32; nBitCount++, nBit *= 2 )
		{
            INT bReqAdd = FALSE;

			if ( ulReqRanges[nIndex] & nBit )
			{
				arrRequest[ nIndex * 32 + nBitCount ] = 1;
				nRangesCount++;
				bReqAdd = TRUE;
			}

			if ( ulCandRanges[nIndex] & nBit )
			{
				arrCandidate[ nIndex * 32 + nBitCount ] = 1;
				if ( !bReqAdd )
					nAddCount++;
			}
		}
	}

	if ( 0 == nRangesCount )
		return 0;

	//double dRangeWeight = 1;//1000.0 / nRangesCount;

	for ( int nIndex = 0; nIndex < 192; nIndex++ )
	{
		if ( 1 == arrRequest[nIndex] && 0 == arrCandidate[nIndex] )
			dPenalty += dRangeWeight;
		else if ( dRangeWeightSuferflouous != 0 && 0 == arrRequest[nIndex] && 1 == arrCandidate[nIndex] )
			dPenalty += dRangeWeightSuferflouous; 
	}

	return (int)dPenalty;
}
int CFontList::GetFixedPitchPenalty(INT bCandFixed, INT bReqFixed)
{
	int nPenalty = 0;

	// Если запрашивается моноширинный, а кандидат не моноширинный, то вес 15000
	// Если запрашивается не моноширинный, а кандидат моноширинный, то вес 350
	if ( bReqFixed && !bCandFixed )
		nPenalty = 15000;
	if ( !bReqFixed && bCandFixed )
		nPenalty = 350;

	return nPenalty;
}

CFontListNamePicker CFontList::m_oPicker;
int CFontList::GetFaceNamePenalty(std::wstring sCandName, std::wstring sReqName, bool bIsUseNamePicker)
{
    if ( 0 == sReqName.length() )
		return 0;

    if ( 0 == sCandName.length() )
		return 10000;

	if ( sReqName == sCandName )
		return 0;

    if ( std::wstring::npos != sReqName.find( sCandName ) || std::wstring::npos != sCandName.find( sReqName ) )
    {
        if (m_oPicker.IsLikeFonts(sCandName, sReqName))
            return 700;
		return 1000;
    }

    if (bIsUseNamePicker)
	{
        if (m_oPicker.IsLikeFonts(sCandName, sReqName))
            return 1000;

        return m_oPicker.CheckEqualsFonts(sCandName, sReqName);
    }

	return 10000;
}

int CFontList::GetFamilyUnlikelyPenalty(SHORT nCandFamilyClass, SHORT nReqFamilyClass)
{
	// Requested a roman/modern/swiss family, but the candidate is 
	// decorative/script. Or requested decorative/script, and the 
	// candidate is roman/modern/swiss. Penalty = 50.

	int nReqClassID  = nReqFamilyClass  >> 8;
	int nCandClassID = nCandFamilyClass >> 8;

	if ( 0 == nReqClassID ) // Unknown
		return 0;

	if ( 0 == nCandClassID ) // Unknown
		return 50;

	if ( ( nReqClassID <= 8 && nCandClassID > 8 ) || ( nReqClassID > 8 && nCandClassID <= 8 ) )
		return 50;

	return 0;
}
int CFontList::GetFamilyUnlikelyPenalty(int nCandFamilyClass, std::wstring sReqFamilyClass)
{
	// Requested a roman/modern/swiss family, but the candidate is 
	// decorative/script. Or requested decorative/script, and the 
	// candidate is roman/modern/swiss. Penalty = 50.

	int nCandClassID = nCandFamilyClass >> 8;

	//sReqFamilyClass.MakeLower(); TODO:
	if ( L"any" == sReqFamilyClass || L"unknown" == sReqFamilyClass )
		return 0;
	else if ( 0 == nCandClassID )
		return 50;
	else if ( ( ( L"swiss" == sReqFamilyClass || 
		L"roman" == sReqFamilyClass || 
		L"modern" == sReqFamilyClass ) && nCandClassID > 8 ) || 
		( (L"decorative" == sReqFamilyClass || L"script" == sReqFamilyClass ) && nCandClassID <= 8 ) )
		return 50;

	return 0;
}
int CFontList::GetWidthPenalty(USHORT usCandWidth, USHORT usReqWidth)
{
	// Penalty * width difference (Penalty = 50)

	return abs( (int)usCandWidth - (int)usReqWidth ) * 50;
}
int CFontList::GetWeightPenalty(USHORT usCandWeight, USHORT usReqWeight)
{
	// Penalty * ( weight difference / 10 )  (Penalty = 3)

	return (3 * ( abs( (int)usCandWeight - (int)usReqWeight ) / 10 ));
}

int CFontList::GetItalicPenalty(INT bCandItalic, INT bReqItalic)
{
	// Penalty = 4

	if ( bCandItalic != bReqItalic )
		return 4;

	return 0;
}

int CFontList::GetBoldPenalty(INT bCandBold, INT bReqBold)
{
	// SmallPenalty
	// Penalty = 1

	if ( bCandBold != bReqBold )
		return 1;

	return 0;
}

int CFontList::GetFontFormatPenalty(EFontFormat eCandFormat, EFontFormat eReqFormat)
{
	// Вообще, на МSDN написано только про TrueType. Но мы будем сравнивать
	// все типы форматов и при несовпадении даем вес = 4. Если формат не задан
	// то по умолчанию считаем его TrueType.

	if ( eReqFormat == fontUnknown )
	{
		// Считаем, что когда формат не известен, значит это 100% не TrueType.
		if ( eCandFormat == fontTrueType )
			return 4;
		else
			return 0;
	}

	if ( eCandFormat != eReqFormat )
		return 4;

	return 0;
}
int CFontList::GetPanosePenalty(BYTE *pCandPanose, BYTE *pReqPanose)
{
	int nPenalty = 0;
	for ( int nIndex = 0; nIndex < 10; nIndex++ )
	{
		if ( pCandPanose[nIndex] != pReqPanose[nIndex] && 0 != pReqPanose[nIndex] )
		{
			int nKoef = abs(pCandPanose[nIndex] - pReqPanose[nIndex]);
			switch(nIndex)
			{
			case 0: nPenalty += 1000 * nKoef; break;
			case 1: nPenalty += 100  * nKoef; break;
			case 2: nPenalty += 100  * nKoef; break;
			case 3: nPenalty += 100  * nKoef; break;
			case 4: nPenalty += 100  * nKoef; break;
			case 5: nPenalty += 100  * nKoef; break;
			case 6: nPenalty += 100  * nKoef; break;
			case 7: nPenalty += 100  * nKoef; break;
			case 8: nPenalty += 100  * nKoef; break;
			case 9: nPenalty += 100  * nKoef; break;
			}
		}
	}

	return nPenalty;
}

int CFontList::GetAvgWidthPenalty(SHORT shCandWidth, SHORT shReqWidth)
{
	if ( 0 == shCandWidth && 0 != shReqWidth )
		return 4000;

	return abs( shCandWidth - shReqWidth ) * 4;
}
int CFontList::GetAscentPenalty(SHORT shCandAscent, SHORT shReqAscent)
{
	if ( 0 == shCandAscent && 0 != shReqAscent )
		return 100;

	return abs( shCandAscent - shReqAscent ) / 10;
}
int CFontList::GetDescentPenalty(SHORT shCandDescent, SHORT shReqDescent)
{
	if ( 0 == shCandDescent && 0 != shReqDescent )
		return 100;

	return abs( shCandDescent - shReqDescent ) / 10;
}
int CFontList::GetLineGapPenalty(SHORT shCandLineGap, SHORT shReqLineGap)
{
	if ( 0 == shCandLineGap && 0 != shReqLineGap )
		return 100;

	return abs( shCandLineGap - shReqLineGap ) / 10;
}
int CFontList::GetXHeightPenalty(SHORT shCandXHeight, SHORT shReqXHeight)
{
	if ( 0 == shCandXHeight && 0 != shReqXHeight )
		return 50;

	return abs( shCandXHeight - shReqXHeight ) / 20;
}
int CFontList::GetCapHeightPenalty(SHORT shCandCapHeight, SHORT shReqCapHeight)
{
	if ( 0 == shCandCapHeight && 0 != shReqCapHeight )
		return 50;

	return abs( shCandCapHeight - shReqCapHeight ) / 20;
}

EFontFormat CFontList::GetFontFormat(FT_Face pFace)
{
	std::string wsFormat( FT_Get_X11_Font_Format( pFace ) );

	if ( "Windows FNT" == wsFormat )
        return fontWindowsFNT;
	else if ( "TrueType" == wsFormat ) 
        return fontTrueType;
	else if ( "CFF" == wsFormat )
        return fontOpenType;

    return fontUnknown;
}

void CFontList::ToBuffer(BYTE** pDstData, LONG* pLen, std::wstring strDirectory, bool bIsOnlyFileName)
{
    LONG lDataSize = sizeof(INT);
    size_t nFontsCount = (size_t)m_pList.size();
    for (std::vector<NSFonts::CFontInfo*>::iterator iter = m_pList.begin(); iter != m_pList.end(); iter++)
    {
        lDataSize += NSFonts::GetBufferLen(*iter, strDirectory, bIsOnlyFileName);
    }

    BYTE* pData = new BYTE[lDataSize];
    BYTE* pDataMem = pData;

    *(INT*)pDataMem = (INT)nFontsCount;
    pDataMem += sizeof(INT);

    for (std::vector<NSFonts::CFontInfo*>::iterator iter = m_pList.begin(); iter != m_pList.end(); iter++)
    {
        NSFonts::ToBuffer(*iter, pDataMem, strDirectory, bIsOnlyFileName);
    }

    *pDstData = pData;
    *pLen = (LONG)(pDataMem - pData);
}

NSFonts::CFontInfo* CFontList::GetByParams(NSFonts::CFontSelectFormat& oSelect, bool bIsDictionaryUse)
{
    int nFontsCount = m_pList.size();
	if (0 == nFontsCount)
		return NULL;

	if (bIsDictionaryUse)
	{
		// дубликат не делаем!!! Серега создает объект только для подбора и дальше его не использует
		NSFontDictionary::CorrectParamsFromDictionary(oSelect);
	}

	int nMinIndex   = 0; // Номер шрифта в списке с минимальным весом
    int nMinPenalty = -1; // Минимальный вес

	int nDefPenalty = 2147483647;

    NSFonts::CFontInfo* pInfoMin = NULL;
    for (std::vector<NSFonts::CFontInfo*>::iterator iter = m_pList.begin(); iter != m_pList.end(); iter++)
	{
		int nCurPenalty = 0;
        NSFonts::CFontInfo* pInfo = *iter;

		if ( NULL != oSelect.pPanose )
		{
			nCurPenalty += GetPanosePenalty( pInfo->m_aPanose, oSelect.pPanose );
		}

		ULONG arrCandRanges[6] = { pInfo->m_ulUnicodeRange1, pInfo->m_ulUnicodeRange2, pInfo->m_ulUnicodeRange3, pInfo->m_ulUnicodeRange4, pInfo->m_ulCodePageRange1, pInfo->m_ulCodePageRange2 };
		
		if (true)
		{
			if (NULL != oSelect.ulRange1 &&
				NULL != oSelect.ulRange2 &&
				NULL != oSelect.ulRange3 &&
				NULL != oSelect.ulRange4 &&
				NULL != oSelect.ulCodeRange1 &&
				NULL != oSelect.ulCodeRange2)
			{
				ULONG arrReqRanges[6]  = { *oSelect.ulRange1, *oSelect.ulRange2, *oSelect.ulRange3, *oSelect.ulRange4, *oSelect.ulCodeRange1, *oSelect.ulCodeRange2 };
				nCurPenalty += GetSigPenalty( arrCandRanges, arrReqRanges, nCurPenalty >= 1000 ? 50 : 10, 10 );
			}
		}

		unsigned char unCharset = UNKNOWN_CHARSET; 
		if (NULL != oSelect.unCharset)
			unCharset = *oSelect.unCharset;

		if ( NULL != oSelect.bFixedWidth )
			nCurPenalty += GetFixedPitchPenalty( pInfo->m_bIsFixed, *oSelect.bFixedWidth );

		if ( oSelect.wsName != NULL && oSelect.wsAltName != NULL )
		{
            nCurPenalty += min( GetFaceNamePenalty( pInfo->m_wsFontName, *oSelect.wsName, true ),
                                GetFaceNamePenalty( pInfo->m_wsFontName, *oSelect.wsAltName, true ) );
		}
		else if ( oSelect.wsName != NULL )
            nCurPenalty += GetFaceNamePenalty( pInfo->m_wsFontName, *oSelect.wsName, true );
		else if ( oSelect.wsAltName != NULL )
            nCurPenalty += GetFaceNamePenalty( pInfo->m_wsFontName, *oSelect.wsAltName, true );

		if ( NULL != oSelect.usWidth )
			nCurPenalty += GetWidthPenalty( pInfo->m_usWidth, *oSelect.usWidth );

		if ( NULL != oSelect.usWeight )
			nCurPenalty += GetWeightPenalty( pInfo->m_usWeigth, *oSelect.usWeight );

		if ( NULL != oSelect.bBold )
			nCurPenalty += GetBoldPenalty( pInfo->m_bBold, *oSelect.bBold );

		if ( NULL != oSelect.bItalic )
			nCurPenalty += GetItalicPenalty( pInfo->m_bItalic, *oSelect.bItalic );

		if ( NULL != oSelect.wsFamilyClass )
			nCurPenalty += GetFamilyUnlikelyPenalty( pInfo->m_sFamilyClass, *oSelect.wsFamilyClass );
		else if (NULL != oSelect.sFamilyClass)
			nCurPenalty += GetFamilyUnlikelyPenalty( pInfo->m_sFamilyClass, *oSelect.sFamilyClass );
		
		//nCurPenalty += GetFontFormatPenalty( pInfo->m_eFontFormat, fontTrueType );
		nCurPenalty += GetCharsetPenalty( arrCandRanges, unCharset );

		if ( NULL != oSelect.shAvgCharWidth )
			nCurPenalty += GetAvgWidthPenalty( pInfo->m_shAvgCharWidth, *oSelect.shAvgCharWidth );

		if ( NULL != oSelect.shAscent )
			nCurPenalty += GetAscentPenalty( pInfo->m_shAscent, *oSelect.shAscent );

		if ( NULL != oSelect.shDescent )
			nCurPenalty += GetDescentPenalty( pInfo->m_shDescent, *oSelect.shDescent );

		if ( NULL != oSelect.shLineGap )
			nCurPenalty += GetLineGapPenalty( pInfo->m_shLineGap, *oSelect.shLineGap );

		if ( NULL != oSelect.shXHeight )
			nCurPenalty += GetXHeightPenalty( pInfo->m_shXHeight, *oSelect.shXHeight );

		if ( NULL != oSelect.shCapHeight )
			nCurPenalty += GetCapHeightPenalty( pInfo->m_shCapHeight, *oSelect.shCapHeight );

        if ( nMinPenalty < 0 )
		{
            pInfoMin   = pInfo;
			nMinPenalty = nCurPenalty;
		}
		else if ( nCurPenalty < nMinPenalty )
		{
            pInfoMin   = pInfo;
			nMinPenalty = nCurPenalty;
		}

		// Нашелся шрифт, удовлетворяющий всем параметрам, дальше искать нет смысла
		if ( 0 == nCurPenalty )
			break;
	}

    return pInfoMin;
}

std::vector<NSFonts::CFontInfo*> CFontList::GetAllByName(const std::wstring& strFontName)
{
    std::vector<NSFonts::CFontInfo*> aRes;
    for (std::vector<NSFonts::CFontInfo*>::iterator iter = m_pList.begin(); iter != m_pList.end(); iter++)
	{
        NSFonts::CFontInfo* pInfo = *iter;
        if (pInfo->m_wsFontName == strFontName)
            aRes.push_back(pInfo);
	}
	return aRes;
}

void CFontList::LoadFromArrayFiles(std::vector<std::wstring>& oArray, int nFlag)
{
	size_t nCount = oArray.size();

	FT_Library pLibrary = NULL;
	if (FT_Init_FreeType(&pLibrary))
		return;

	FT_Parameter *pParams = (FT_Parameter *)::malloc( sizeof(FT_Parameter) * 4 );
	pParams[0].tag  = FT_MAKE_TAG( 'i', 'g', 'p', 'f' );
	pParams[0].data = NULL;
	pParams[1].tag  = FT_MAKE_TAG( 'i', 'g', 'p', 's' );
	pParams[1].data = NULL; 
	pParams[2].tag  = FT_PARAM_TAG_IGNORE_PREFERRED_FAMILY;
	pParams[2].data = NULL; 
	pParams[3].tag  = FT_PARAM_TAG_IGNORE_PREFERRED_SUBFAMILY;
	pParams[3].data = NULL; 

    // определяем размер буфера, чтобы не выделять много кусков, а обойтись одним
    int nMaxFontSize = 0;
    for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
    {
        NSFile::CFileBinary oFile;
        if (oFile.OpenFile(oArray[nIndex]))
        {
            int nSizeTmp = (int)oFile.GetFileSize();
            if (nSizeTmp > 100000000)
            {
                // такие огромные шрифты не учитываем
                oArray.erase(oArray.begin() + nIndex, oArray.begin() + nIndex + 1);
                nIndex--;
                nCount--;
            }
            
            if (nMaxFontSize < nSizeTmp)
                nMaxFontSize = nSizeTmp;
        }
    }
    
    BYTE* pDataFontFile = new BYTE[nMaxFontSize];
    
	for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
	{
        if ((nFlag & 2) != 0)
        {
            std::wstring::size_type _pos_dfont = oArray[nIndex].find(L".dfont");
            if (_pos_dfont != std::wstring::npos && _pos_dfont == (oArray[nIndex].length() - 6))
                continue;
        }

		// open file
		CFontStream oStream;
		if (!oStream.CreateFromFile(oArray[nIndex], pDataFontFile))
			continue;

		FT_Open_Args oOpenArgs;
		oOpenArgs.flags			= FT_OPEN_MEMORY | FT_OPEN_PARAMS;
		oOpenArgs.memory_base	= oStream.m_pData;
		oOpenArgs.memory_size	= oStream.m_lSize;

		oOpenArgs.num_params = 4;
		oOpenArgs.params     = pParams;

		FT_Face pFace = NULL;
		if (FT_Open_Face( pLibrary, &oOpenArgs, 0, &pFace ))
			continue;

		// TO DO: Шрифты, которые нельзя скейлить (т.е. изменять размер 
		// произвольно) мы не грузим. Возможно в будущем надо будет
		// сделать, чтобы работал и такой вариант. (в Word такие шрифты
		// не используются)
		if ( !( pFace->face_flags & FT_FACE_FLAG_SCALABLE ) )
		{
			FT_Done_Face( pFace );
			continue;
		}

		int nFacesCount = pFace->num_faces;
		if ( FT_Done_Face( pFace ) )
			continue;
		
		for ( int nIndexFace = 0; nIndexFace < nFacesCount; nIndexFace++ )
		{
			if (FT_Open_Face( pLibrary, &oOpenArgs, nIndexFace, &pFace))
				continue;

            INT bBold   = (pFace->style_flags & FT_STYLE_FLAG_BOLD ? 1 : 0);
            INT bItalic = (pFace->style_flags & FT_STYLE_FLAG_ITALIC) ? 1 : 0;

			const char* pPostName = FT_Get_Postscript_Name(pFace);
			std::string sPostscriptName = "";
			if (NULL != pPostName)
				sPostscriptName = FT_Get_Postscript_Name(pFace);

			int nNamesCount = FT_Get_Sfnt_Name_Count(pFace);

			for (int nNameIndex = 0; nNameIndex < nNamesCount; ++nNameIndex)
			{
				FT_SfntName_ oSfntName;
				FT_Get_Sfnt_Name( pFace, nNameIndex, &oSfntName );

				if ( oSfntName.language_id == 0x0409 && oSfntName.name_id == 1 && oSfntName.platform_id == 3 )
				{
					// Family
					int k= 10;
				}
				if ( oSfntName.language_id == 0x0409 && oSfntName.name_id == 16 && oSfntName.platform_id == 3 )
				{
					// Preffered family
					int k= 10;
				}
			}

            INT bFixedWidth = FT_IS_FIXED_WIDTH( pFace );

			TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );

			BYTE* pPanose = NULL;
			ULONG ulRange1 = 0, ulRange2 = 0, ulRange3 = 0, ulRange4 = 0, ulCodeRange1 = 0, ulCodeRange2 = 0;
			USHORT usWidth = 0, usWeight = 0;
			SHORT sFamilyClass = 0;

			SHORT shAvgCharWidth = 0, shAscent = 0, shDescent = 0, shLineGap = 0, shXHeight = 0, shCapHeight = 0;
			if ( NULL != pOs2 )
			{
				pPanose        = (BYTE *)pOs2->panose;

				ulRange1       = pOs2->ulUnicodeRange1;
				ulRange2       = pOs2->ulUnicodeRange2;
				ulRange3       = pOs2->ulUnicodeRange3;
				ulRange4       = pOs2->ulUnicodeRange4;
				ulCodeRange1   = pOs2->ulCodePageRange1;
				ulCodeRange2   = pOs2->ulCodePageRange2;

				usWeight       = pOs2->usWeightClass;
				usWidth        = pOs2->usWidthClass;

				sFamilyClass   = pOs2->sFamilyClass;

				if ( 0 != pFace->units_per_EM )
				{
					double dKoef = ( 1000 / (double)pFace->units_per_EM );
					shAvgCharWidth = (SHORT)(pOs2->xAvgCharWidth  * dKoef);
					shAscent       = (SHORT)(pOs2->sTypoAscender  * dKoef);
					shDescent      = (SHORT)(pOs2->sTypoDescender * dKoef);
					shLineGap      = (SHORT)(pOs2->sTypoLineGap   * dKoef);
					shXHeight      = (SHORT)(pOs2->sxHeight       * dKoef);
					shCapHeight    = (SHORT)(pOs2->sCapHeight     * dKoef);
				}
				else
				{
					shAvgCharWidth = (SHORT)pOs2->xAvgCharWidth;
					shAscent       = (SHORT)pOs2->sTypoAscender;
					shDescent      = (SHORT)pOs2->sTypoDescender;
					shLineGap      = (SHORT)pOs2->sTypoLineGap;
					shXHeight      = (SHORT)pOs2->sxHeight;
					shCapHeight    = (SHORT)pOs2->sCapHeight;
				}
			}

			if ( true )
			{
				// Специальная ветка для случаев, когда charset может быть задан не через значения
				// ulCodePageRange, а непосредственно через тип Cmap.

				//  Charset Name       Charset Value(hex)  Codepage number   Platform_ID   Encoding_ID   Description
				//  -------------------------------------------------------------------------------------------------
				//
				//  SYMBOL_CHARSET            2 (x02)                             3            0           Symbol
				//  SHIFTJIS_CHARSET        128 (x80)             932             3            2           ShiftJIS
				//  GB2313_CHARSET          134 (x86)             936             3            3           PRC
				//  CHINESEBIG5_CHARSET     136 (x88)             950             3            4           Big5
				//  HANGEUL_CHARSET         129 (x81)             949             3            5           Wansung
				//  JOHAB_CHARSET	        130 (x82)            1361             3            6           Johab

				for( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
				{
					// Symbol
					if ( !( ulCodeRange1 & 0x80000000 ) && 0 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
						ulCodeRange1 |= 0x80000000;

					// ShiftJIS
					if ( !( ulCodeRange1 & 0x00020000 ) && 2 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
						ulCodeRange1 |= 0x00020000;

					// PRC
					if ( !( ulCodeRange1 & 0x00040000 ) && 3 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
						ulCodeRange1 |= 0x00040000;

					// Big5
					if ( !( ulCodeRange1 & 0x00100000 ) && 4 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
						ulCodeRange1 |= 0x00100000;

					// Wansung
					if ( !( ulCodeRange1 & 0x00080000 ) && 5 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
						ulCodeRange1 |= 0x00080000;

					// Johab
					if ( !( ulCodeRange1 & 0x00200000 ) && 6 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
						ulCodeRange1 |= 0x00200000;
				}
			}

			EFontFormat eFormat = GetFontFormat( pFace );

            bool bSupportFont = ((eFormat == fontTrueType) || ((nFlag & 1) && (eFormat == fontOpenType)));
            if (!bSupportFont)
            {
                FT_Done_Face( pFace );
                continue;
            }

            std::wstring wsFamilyName = GetCorrectSfntName(pFace->family_name);
            std::wstring wsStyleName = GetCorrectSfntName(pFace->style_name);

#ifdef _MAC
            if (wsFamilyName.find(L".") == 0)
            {
                FT_Done_Face( pFace );
                continue;
            }
#endif

            NSFonts::CFontInfo* pFontInfo = new NSFonts::CFontInfo( wsFamilyName,
				wsStyleName, 
				oArray[nIndex], 
				nIndexFace, 
				bBold, 
				bItalic, 
				bFixedWidth, 
				pPanose, 
				ulRange1, 
				ulRange2, 
				ulRange3, 
				ulRange4, 
				ulCodeRange1, 
				ulCodeRange2, 
				usWeight, 
				usWidth, 
				sFamilyClass, 
				eFormat, 
				shAvgCharWidth, 
				shAscent, 
				shDescent, 
				shLineGap, 
				shXHeight, 
				shCapHeight );

#ifdef BUILD_FONT_NAMES_DICTIONARY
            ReadNames(pFontInfo, pFace);
#endif

			Add(pFontInfo);

			FT_Done_Face( pFace );
		}
	}

    RELEASEARRAYOBJECTS(pDataFontFile);

	::free( pParams );
	FT_Done_FreeType(pLibrary);
}
void CFontList::LoadFromFolder(const std::wstring& strDirectory)
{
	std::vector<std::wstring> oArray = NSDirectory::GetFiles(strDirectory, true);
	this->LoadFromArrayFiles(oArray);
}

void CFontList::InitializeRanges(unsigned char* data)
{
    RELEASEARRAYOBJECTS(m_pRanges)

    NSMemoryUtils::CByteReader oReader(data);
    m_nRangesCount = oReader.GetInt();

    if (m_nRangesCount > 0)
        m_pRanges = new CFontRange[m_nRangesCount];

    for (int nIndex = 0; nIndex < m_nRangesCount; ++nIndex)
    {
        m_pRanges[nIndex].Name = oReader.GetStringUTF8();
        m_pRanges[nIndex].Start = oReader.GetInt();
        m_pRanges[nIndex].End = oReader.GetInt();
    }
}

bool CFontList::CheckLoadFromFolderBin(const std::wstring& strDirectory)
{
	std::wstring strPath = strDirectory + L"/font_selection.bin";

	NSFile::CFileBinary oFile;
	if (!oFile.OpenFile(strPath))
		return false;

	DWORD dwLen1 = (DWORD)oFile.GetFileSize();
	DWORD dwLen2 = 0;
	BYTE* pBuffer = new BYTE[dwLen1];
	oFile.ReadFile(pBuffer, dwLen1, dwLen2);

	BYTE* _pBuffer = pBuffer;

    int lCount = *((int*)_pBuffer);
    _pBuffer += sizeof(int);

    for (int nIndex = 0; nIndex < lCount; ++nIndex)
	{
        NSFonts::CFontInfo *pFontInfo = NSFonts::FromBuffer(_pBuffer, strDirectory);
		Add(pFontInfo);
	}

    if ((_pBuffer - pBuffer) < dwLen1)
    {
        InitializeRanges(_pBuffer);
    }

	RELEASEARRAYOBJECTS(pBuffer);

	return true;
}

void CFontList::Add(NSFonts::CFontInfo* pInfo)
{
    int nCount = m_pList.size();
	for ( int nIndex = 0; nIndex < nCount; ++nIndex ) 
	{
        if (m_pList[nIndex]->IsEquals(pInfo))
		{
			RELEASEOBJECT(pInfo);
			return;
		}
	}

    m_pList.push_back(pInfo);
}

// ApplicationFonts
CApplicationFonts::CApplicationFonts() : NSFonts::IApplicationFonts()
{
	m_oCache.m_pApplicationFontStreams = &m_oStreams;
}
CApplicationFonts::~CApplicationFonts()
{
}

NSFonts::IFontsCache* CApplicationFonts::GetCache()
{
	return &m_oCache;
}
NSFonts::IFontList* CApplicationFonts::GetList()
{
	return &m_oList;
}
NSFonts::IApplicationFontStreams* CApplicationFonts::GetStreams()
{
	return &m_oStreams;
}

void CApplicationFonts::InitializeFromFolder(std::wstring strFolder, bool bIsCheckSelection)
{
    if (bIsCheckSelection)
    {
        if (m_oList.CheckLoadFromFolderBin(strFolder))
            return;

        if (m_oList.CheckLoadFromFolderBin(NSFile::GetProcessDirectory()))
            return;
    }

    m_oList.LoadFromFolder(strFolder);
    m_oCache.m_pApplicationFontStreams = &m_oStreams;
}
void CApplicationFonts::Initialize(bool bIsCheckSelection)
{
    if (bIsCheckSelection)
    {
        if (m_oList.CheckLoadFromFolderBin(NSFile::GetProcessDirectory()))
            return;
    }

#if defined(_WIN32) || defined (_WIN64)
	//m_oList.LoadFromFolder(L"C:/Windows/Fonts");
	InitFromReg();
#endif

#if defined(_LINUX) && !defined(_MAC) && !defined(__ANDROID__)
	m_oList.LoadFromFolder(L"/usr/share/fonts");
#endif

#if defined(_MAC) && !defined(_IOS)
	m_oList.LoadFromFolder(L"/Library/Fonts/");
#endif

#ifdef __ANDROID__
    m_oList.LoadFromFolder(L"/system/fonts");
#endif

	m_oCache.m_pApplicationFontStreams = &m_oStreams;
}

void CApplicationFonts::InitializeRanges(unsigned char* data)
{
    m_oList.InitializeRanges(data);
}

NSFonts::IFontManager* CApplicationFonts::GenerateFontManager()
{
	CFontManager* pManager = new CFontManager();
	pManager->m_pApplication = this;
	return pManager;
}

std::wstring CApplicationFonts::GetFontBySymbol(int symbol)
{
    return m_oList.GetFontBySymbol(symbol);
}

#if defined(_WIN32) || defined (_WIN64)

#include <shlobj.h>

static long GetNextNameValue(HKEY key, const std::wstring& sSubkey, std::wstring& sName, std::wstring& sData)
{
	static HKEY hkey = NULL;	// registry handle, kept open between calls
	static DWORD dwIndex = 0;	// count of values returned
	long retval;

	// if all parameters are NULL then close key
	if (sSubkey.length() == 0 && sName.length() == 0 && sData.length() == 0)
	{
		if (hkey)
			RegCloseKey(hkey);
		hkey = NULL;
		return ERROR_SUCCESS;
	}

	// if subkey is specified then open key (first time)
	if (sSubkey.length() != 0)
	{
		retval = RegOpenKeyExW(key, sSubkey.c_str(), 0, KEY_READ, &hkey);
		if (retval != ERROR_SUCCESS)
		{
			return retval;
		}
		dwIndex = 0;
	}
	else
	{
		dwIndex++;
	}

	wchar_t szValueName[MAX_PATH];
	DWORD dwValueNameSize = sizeof(szValueName)-1;
	BYTE szValueData[MAX_PATH];
	DWORD dwValueDataSize = sizeof(szValueData)-1;
	DWORD dwType = 0;

	retval = RegEnumValueW(hkey, dwIndex, szValueName, &dwValueNameSize, NULL,
		&dwType, szValueData, &dwValueDataSize);
	if (retval == ERROR_SUCCESS)
	{
		sName = std::wstring(szValueName);
		sData = std::wstring((wchar_t*)szValueData);
	}

	return retval;
}

#endif

std::vector<std::wstring> CApplicationFonts::GetSetupFontFiles()
{
#if defined(_WIN32) || defined (_WIN64)
    // Ищем директорию с фонтами (обычно это C:\Windows\Fonts)
    wchar_t wsWinFontDir[MAX_PATH];
    wsWinFontDir[0] = (wchar_t)'\0';

    if ( !SHGetSpecialFolderPathW( NULL, wsWinFontDir, CSIDL_FONTS, FALSE ) )
        wsWinFontDir[0] = '\0';

    std::wstring sWinFontDir(wsWinFontDir);

    OSVERSIONINFO oVersion;
    oVersion.dwOSVersionInfoSize = sizeof(oVersion);
    GetVersionEx( &oVersion );

    std::wstring wsPath = L"";

    if ( oVersion.dwPlatformId == VER_PLATFORM_WIN32_NT )
        wsPath = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts\\";
    else
        wsPath = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Fonts\\";

    std::wstring sName;
    std::wstring sData;

    std::map<std::wstring, bool> map_files;	
	std::vector<std::wstring> oArray;

    while (GetNextNameValue( HKEY_LOCAL_MACHINE, wsPath, sName, sData ) == ERROR_SUCCESS)
    {
        if (wsPath.length())
            wsPath = L"";

        NSFile::CFileBinary oFile;
        if (oFile.OpenFile(sData))
        {
            oFile.CloseFile();

            if (map_files.find(sData) == map_files.end())
            {
                oArray.push_back(sData);
                map_files.insert(map_files.begin(), std::pair<std::wstring,bool>(sData,true));
            }
            continue;
        }

        oFile.CloseFile();

        std::wstring sFileInDir = sWinFontDir + L"\\" + sData;
        if (oFile.OpenFile(sFileInDir))
        {
            oFile.CloseFile();

            if (map_files.find(sFileInDir) == map_files.end())
            {
                oArray.push_back(sFileInDir);
                map_files.insert(map_files.begin(), std::pair<std::wstring,bool>(sFileInDir,true));
            }
            continue;
        }
    }

    if (true)
    {
        std::vector<std::wstring> oArray2 = NSDirectory::GetFiles(L"C:/Windows/Fonts", true);

        wchar_t sUserName[1000];
        DWORD nUserNameLen = 1000 + 1;
        GetUserNameW(sUserName, &nUserNameLen);
        std::wstring strUserName(sUserName, nUserNameLen - 1);

        NSDirectory::GetFiles2(L"C:\\Users\\" + strUserName + L"\\AppData\\Local\\Microsoft\\Windows\\Fonts", oArray2, false);

        for (std::vector<std::wstring>::iterator i = oArray2.begin(); i != oArray2.end(); i++)
        {
            if (map_files.end() == map_files.find(*i))
                oArray.push_back(*i);
        }
    }

    return oArray;
#endif

#if defined(__linux__) && !defined(_MAC) && !defined(__ANDROID__)
     std::vector<std::wstring> _array = NSDirectory::GetFiles(L"/usr/share/fonts", true);
     NSDirectory::GetFiles2(L"/usr/share/X11/fonts", _array, true);
     NSDirectory::GetFiles2(L"/usr/X11R6/lib/X11/fonts", _array, true);
     NSDirectory::GetFiles2(L"/usr/local/share/fonts", _array, true);
     return _array;
#endif

#if defined(_MAC) && !defined(_IOS)
	std::vector<std::wstring> _array = NSDirectory::GetFiles(L"/Library/Fonts", true);
	NSDirectory::GetFiles2(L"/System/Library/Fonts", _array, true);
	return _array;
#endif
    
#ifdef _IOS
    // own realization (objective c code)
    return GetSetupFontFiles_ios();
#endif

#ifdef __ANDROID__
    std::vector<std::wstring> _array = NSDirectory::GetFiles(L"/system/fonts", true);
    return _array;
#endif

	std::vector<std::wstring> ret;
    return ret;
}

void CApplicationFonts::InitializeFromArrayFiles(std::vector<std::wstring>& files, int nFlag)
{
    m_oList.LoadFromArrayFiles(files, nFlag);
}

#if defined(_WIN32) || defined (_WIN64)

void CApplicationFonts::InitFromReg()
{
	std::vector<std::wstring> oArray = GetSetupFontFiles();
	m_oList.LoadFromArrayFiles(oArray);
}

#endif

namespace NSFonts
{
    // Symbols
    class CApplicationFontsSymbols_Private
    {
    public:
        FT_Library m_library;
        FT_Parameter* m_params;
        BYTE* m_pData;

        CApplicationFontsSymbols_Private()
        {
            m_library = NULL;
            m_pData = NULL;
            m_params = NULL;

            if (FT_Init_FreeType(&m_library))
                return;

            m_params = (FT_Parameter *)::malloc( sizeof(FT_Parameter) * 4 );
            m_params[0].tag  = FT_MAKE_TAG( 'i', 'g', 'p', 'f' );
            m_params[0].data = NULL;
            m_params[1].tag  = FT_MAKE_TAG( 'i', 'g', 'p', 's' );
            m_params[1].data = NULL;
            m_params[2].tag  = FT_PARAM_TAG_IGNORE_PREFERRED_FAMILY;
            m_params[2].data = NULL;
            m_params[3].tag  = FT_PARAM_TAG_IGNORE_PREFERRED_SUBFAMILY;
            m_params[3].data = NULL;

            int nSize = 100000000;
            m_pData = new BYTE[nSize];
        }
        ~CApplicationFontsSymbols_Private()
        {
            RELEASEARRAYOBJECTS(m_pData);

            if (m_params)
                ::free( m_params );

            if (m_library)
                FT_Done_FreeType(m_library);
        }
    };

    CApplicationFontsSymbols::CApplicationFontsSymbols()
    {
        m_internal = new CApplicationFontsSymbols_Private();
    }
    CApplicationFontsSymbols::~CApplicationFontsSymbols()
    {
        RELEASEOBJECT(m_internal);
    }

    void CApplicationFontsSymbols::CheckSymbols(const std::wstring& sFile, const int& nFaceIndex, CApplicationFontsSymbolsChecker* pChecker)
    {
        CFontStream oStream;
        if (!oStream.CreateFromFile(sFile, m_internal->m_pData))
            return;;

        FT_Open_Args oOpenArgs;
        oOpenArgs.flags			= FT_OPEN_MEMORY | FT_OPEN_PARAMS;
        oOpenArgs.memory_base	= oStream.m_pData;
        oOpenArgs.memory_size	= oStream.m_lSize;

        oOpenArgs.num_params = 4;
        oOpenArgs.params     = m_internal->m_params;

        FT_Face pFace = NULL;
        if (FT_Open_Face(m_internal->m_library, &oOpenArgs, nFaceIndex, &pFace))
            return;

        for (int nCharMap = 0; nCharMap < pFace->num_charmaps; nCharMap++)
        {
            FT_Set_Charmap(pFace, pFace->charmaps[nCharMap]);

            FT_UInt indexG;
            FT_ULong character = FT_Get_First_Char(pFace, &indexG);

            while (indexG)
            {
                pChecker->Check((int)character, indexG);
                character = FT_Get_Next_Char(pFace, character, &indexG);
            }
        }

        FT_Done_Face( pFace );
    }
}
//
