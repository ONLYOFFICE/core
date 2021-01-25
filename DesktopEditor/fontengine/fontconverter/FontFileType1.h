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
#ifndef _ASC_FONTCONVERTER_FONT_FILE_TYPE1_H
#define _ASC_FONTCONVERTER_FONT_FILE_TYPE1_H

#include "Utils.h"
#include "FontFileBase.h"
#include "FontFileEncodings.h"
#include "../../common/Array.h"

namespace NSFontConverter
{
    #define type1MaxBlueValues 14 // 7 пар
    #define type1MaxOtherBlues 10 // 5 пар
    #define type1MaxStemSnap   12

    struct Type1PrivateDict
    {
        int    arrnBlueValues[type1MaxBlueValues];
        int    nBlueValues;
        int    arrnOtherBlues[type1MaxOtherBlues];
        int    nOtherBlues;
        int    arrnFamilyBlues[type1MaxBlueValues];
        int    nFamilyBlues;
        int    arrnFamilyOtherBlues[type1MaxOtherBlues];
        int    nFamilyOtherBlues;
        double dBlueScale;
        int    nBlueShift;
        int    nBlueFuzz;
        double dStdHW;
        bool   bHasStdHW;
        double dStdVW;
        bool   bHasStdVW;
        double arrdStemSnapH[type1MaxStemSnap];
        int    nStemSnapH;
        double arrdStemSnapV[type1MaxStemSnap];
        int    nStemSnapV;
        bool   bHasForceBold;
        bool   bForceBold;
        int    nLanguageGroup;
        int    nLenIV;
        double dExpansionFactor;
    };

    struct Type1TopDict
    {
        // TO DO: дополнить данную структуру
        double arrdFontMatrix[6];
        double arrdFontBBox[4];
    };

    // команды
    const int c_nType1hstem				= 0x0001; // 'hstem'
    const int c_nType1vstem				= 0x0003; // 'vstem'
    const int c_nType1vmoveto			= 0x0004; // 'vmoveto'
    const int c_nType1rlineto			= 0x0005; // 'rlineto'
    const int c_nType1hlineto			= 0x0006; // 'hlineto'
    const int c_nType1vlineto			= 0x0007; // 'vlineto'
    const int c_nType1rrcurveto			= 0x0008; // 'rrcurveto'
    const int c_nType1closepath			= 0x0009; // 'closepath' не используется в Type2
    const int c_nType1callsubr			= 0x000A; // 'callsubr
    const int c_nType1return			= 0x000B; // 'return'
    const int c_nType1dotsection		= 0x000C; // 'dotsection' не используется в Type2
    const int c_nType1vstem3			= 0x010C; // 'vstem'
    const int c_nType1hstem3			= 0x020C; // 'hstem'
    const int c_nType1seac				= 0x060C; // 'seac' Type1 only
    const int c_nType1sbw				= 0x070C; // 'sbw'  Type1 only
    const int c_nType1sub				= 0x0B0C; // 'sub'
    const int c_nType1div				= 0x0C0C; // 'div'
    const int c_nType1callothersubr		= 0x100C; // 'callothersubr'
    const int c_nType1pop				= 0x110C; // 'pop'
    const int c_nType1setcurrentpoint	= 0x210C; // 'setcurrentpoint' не используется в Type2
    const int c_nType1hsbw				= 0x000D; // 'hsbw'
    const int c_nType1endchar			= 0x000E; // 'endchar'
    const int c_nType1rmoveto			= 0x0015; // 'rmoveto'
    const int c_nType1hmoveto			= 0x0016; // 'hmoveto'
    const int c_nType1vhcurveto			= 0x001E; // 'vhcurveto'
    const int c_nType1hvcurveto			= 0x001F; // 'hvcurveto'

    struct Type1CharstringItem
    {
        int  nValue;   // Значение
        bool bCommand; // TRUE: значение - номер команды, FALSE: значение - параметр команды

        Type1CharstringItem(int nVal, bool bCom)
        {
            nValue   = nVal;
            bCommand = bCom;
        }
        Type1CharstringItem()
        {
            nValue   = 0;
            bCommand = false;
        }
    };

    struct Type1Charstring
    {
        std::vector<Type1CharstringItem> arrCharstring;
        int                               nWidth;
        int                               nLSB;

        Type1Charstring()
        {
            nWidth    = 0;
            nLSB      = 0;
        }

        Type1Charstring(std::vector<Type1CharstringItem> &arrCs, int nW, int nL)
        {
            arrCharstring = arrCs;
            nWidth        = nW;
            nLSB          = nL;
        }
    };


    struct Type1Glyph
    {
        std::wstring    sGlyph;    // Type1 имя глифа
        int             nUnicode;  // Юникодное значение глифа
        Type1Charstring oData;
        int             nReserved; // Используем для SID при конвертации Type1->Type2

        Type1Glyph(std::wstring& sGlyf, int nUni, Type1Charstring &oCharstring)
        {
            sGlyph    = sGlyf;
            nUnicode  = nUni;
            oData     = oCharstring;
            nReserved = 0;
        }
        Type1Glyph()
        {
            nUnicode  = 0;
            nReserved = 0;
        }
    };

    static int CompareType1Glyph(const void *pGlyph1, const void *pGlyph2)
    {
        Type1Glyph *pGlyf1 = (Type1Glyph *)pGlyph1;
        Type1Glyph *pGlyf2 = (Type1Glyph *)pGlyph2;

        return pGlyf1->nUnicode - pGlyf2->nUnicode;
    }


    //------------------------------------------------------------------------
    // CFontFileType1
    //------------------------------------------------------------------------

    class CFontFileType1: public CFontFileBase
    {
    public:

        static CFontFileType1 *LoadFromBuffer(char *sBuffer, int nLen);
        static CFontFileType1 *LoadFromFile(const wchar_t *wsFileName);

        virtual ~CFontFileType1();

        char *GetName();

        char **GetEncoding();

        void WriteEncoded(char **ppNewEncoding, FontFileOutputFunc pOutputFunc, void *pOutputStream);
        void ToCFF(FontFileOutputFunc pOutputFunc, void *pOutputStream);

    private:

        CFontFileType1(char *sBuffer, int nLen, bool bFreeData);
        void Parse();
        void DecryptEexec(unsigned char** ppEexecBuffer, int nLen);
        Type1Charstring DecodeCharString(unsigned char *sString, int nLen);
        char *GetNextLine(char *sLine);
        bool RemovePfbMarkers();
        template<int nMax>
        int    ReadDoubleArray(unsigned char *sString, int nLen, double (&pArray)[nMax])
        {
            int nStart = 0;
            while( sString[nStart] != '[' )
            {
                nStart++;
                if ( nStart >= nLen )
                    return 0;
            }

            int nEnd = ++nStart;
            while ( sString[nEnd] != ']' )
            {
                nEnd++;
                if ( nEnd >= nLen )
                    return 0;
            }

            sString = sString + nStart;
            nLen    = nEnd - nStart;


            int nCount = 0;
            const int c_nNumLimit = 32;
            unsigned char sBuffer[c_nNumLimit];
            int nBufPos = 0;
            bool bNewItem = true;

            for ( int nIndex = 0; nIndex < nLen; ++nIndex )
            {
                unsigned char unChar = sString[nIndex];
                if ( ' ' == unChar )
                {
                    if ( !bNewItem )
                        bNewItem = true;

                    continue;
                }

                if ( bNewItem )
                {
                    if ( nCount >= nMax )
                        break;

                    // Добавляем предыдущее число в массив
                    if ( nCount > 0 )
                        pArray[nCount - 1] = Utils::GetDouble( (const char *)sBuffer );

                    memset( sBuffer, 0x00, c_nNumLimit );
                    nBufPos = 0;

                    bNewItem = false;
                    nCount++;
                }

                sBuffer[nBufPos++] = unChar;
                if (nBufPos >= c_nNumLimit)
                    break;
            }

            if ( 0 != sBuffer[0] && nCount > 0 )
                pArray[nCount - 1] = Utils::GetDouble( (const char *)sBuffer );

            return nCount;
        }

        template<int nMax>
        int    ReadIntArray   (unsigned char *sString, int nLen, int (&pArray)[nMax])
        {
            int nStart = 0;
            while( sString[nStart] != '[' )
            {
                nStart++;
                if ( nStart >= nLen )
                    return 0;
            }

            int nEnd = ++nStart;
            while ( sString[nEnd] != ']' )
            {
                nEnd++;
                if ( nEnd >= nLen )
                    return 0;
            }

            sString = sString + nStart;
            nLen    = nEnd - nStart;

            int nCount = 0;
            const int c_nNumLimit = 32;
            unsigned char sBuffer[c_nNumLimit];
            int nBufPos = 0;
            bool bNewItem = true;

            for ( int nIndex = 0; nIndex < nLen; ++nIndex )
            {
                unsigned char unChar = sString[nIndex];
                if ( ' ' == unChar )
                {
                    if ( !bNewItem )
                        bNewItem = true;

                    continue;
                }

                if ( bNewItem )
                {
                    if ( nCount >= nMax )
                        break;

                    // Добавляем предыдущее число в массив
                    if ( nCount > 0 )
                        pArray[nCount - 1] = Utils::GetInteger( (const char *)sBuffer );

                    memset( sBuffer, 0x00, c_nNumLimit );
                    nBufPos = 0;

                    bNewItem = false;
                    nCount++;
                }

                sBuffer[nBufPos++] = unChar;
            }

            if ( 0 != sBuffer[0] && nCount > 0 )
                pArray[nCount - 1] = Utils::GetInteger( (const char *)sBuffer );

            return nCount;
        }

        double ReadDouble     (unsigned char *sString, int nMaxLen)
        {
            // Смещаемся к первому пробелу (после него идет значение)
            int nPos = 0;
            while ( ' ' == sString[nPos] && nPos < nMaxLen )
                nPos++;

            return Utils::GetDouble( (const char*)( sString + nPos ) );
        }
        int    ReadInt        (unsigned char *sString, int nMaxLen)
        {
            int nPos = 0;
            while ( ' ' == sString[nPos] && nPos < nMaxLen )
                nPos++;

            return Utils::GetInteger( (const char*)( sString + nPos ) );
        }

        bool   ReadBool       (unsigned char *sString, int nMaxLen)
        {
            int nStartPos = 0;
            while ( ' ' == sString[nStartPos] && nStartPos < nMaxLen )
                nStartPos++;

            if ( nStartPos >= nMaxLen - 4 )
                return false;

            if ( 't' == sString[nStartPos + 0] &&
                 'r' == sString[nStartPos + 1] &&
                 'u' == sString[nStartPos + 2] &&
                 'e' == sString[nStartPos + 3] )
                return true;

            return false;
        }

        Type1Charstring FlattenCharstring(Type1Charstring& oCharstring, int nBias = 0);
        void   CFFCreateIndexHeader(FontFileOutputFunc pOutputFunc, void *pOutputStream, std::vector<std::wstring> aObjects);
        void   CFFCreateIndexHeader(FontFileOutputFunc pOutputFunc, void *pOutputStream, std::vector<Type1Charstring> aObjects);
        void   CFFEncodeNumber     (FontFileOutputFunc pOutputFunc, void *pOutputStream, int nValue, bool bForceLong = false);
        void   CFFEncodeNumber     (FontFileOutputFunc pOutputFunc, void *pOutputStream, double dValue);

    private:

        char                         *m_sName;
        char                        **m_arrEncoding;
        bool                          m_bParsed;

        Type1PrivateDict              m_oPrivateDict;
        Type1TopDict                  m_oTopDict;

        std::vector<Type1Glyph>      m_arrCharstrings;
        std::vector<Type1Charstring> m_arrSubrs;
    };
}

#endif /* _ASC_FONTCONVERTER_FONT_FILE_TYPE1_H */
