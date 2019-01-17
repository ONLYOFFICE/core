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
#include <stdlib.h>
#include <string.h>
#include "MemoryUtils.h"
#include "StringExt.h"
#include "Hash.h"
#include "FontFileType1C.h"
#include "FontFileTrueType.h"

namespace NSFontConverter
{
    //
    // Терминология
    // -----------
    //
    // character code = номер, используемый как элемент текстовой строки
    //
    // character name = glyph name = имя определенного символа фонта
    //
    // glyph index = GID = позиция (в предела некоторой внутренней таблицы
    //               в фонте), где приведены инструкции как рисовать
    //               данный символ
    //
    // Type 1 fonts
    // ------------
    //
    // Type 1 fonts contain:
    //
    // Encoding: char code -> glyph name
    //
    //           Encoding[charCode] = charName
    //
    // CharStrings: dictionary of instructions, keyed by character names,
    //              maps character name to glyph data
    //
    //              CharStrings[charName] = glyphData
    //
    // TrueType fonts
    // --------------
    //
    // TrueType fonts contain:
    //
    // 'cmap' table: character code -> GID;
    //
    //               cmap[charCode] = gid
    //
    // 'post' table: GID -> glyph name
    //
    //               post[gid] = glyphName
    //
    // Type 42 fonts
    // -------------
    //
    // Type 42 fonts contain:
    //
    // Encoding: char code -> glyph name
    //
    //           Encoding[charCode] = charName
    //
    // CharStrings: dictionary of glyph indexes, keyed by character names,
    //              maps character name to glyph index
    //
    //              CharStrings[charName] = gid
    //
    //------------------------------------------------------------------------

    // Названия символов в стандартном порядке, который использует Apple для
    // своих TrueType фонтов.
    static char *c_arrAppleGlyphNames[258] =
    {
      ".notdef",        "null",           "CR",             "space",
      "exclam",         "quotedbl",       "numbersign",     "dollar",
      "percent",        "ampersand",      "quotesingle",    "parenleft",
      "parenright",     "asterisk",       "plus",           "comma",
      "hyphen",         "period",         "slash",          "zero",
      "one",            "two",            "three",          "four",
      "five",           "six",            "seven",          "eight",
      "nine",           "colon",          "semicolon",      "less",
      "equal",          "greater",        "question",       "at",
      "A",              "B",              "C",              "D",
      "E",              "F",              "G",              "H",
      "I",              "J",              "K",              "L",
      "M",              "N",              "O",              "P",
      "Q",              "R",              "S",              "T",
      "U",              "V",              "W",              "X",
      "Y",              "Z",              "bracketleft",    "backslash",
      "bracketright",   "asciicircum",    "underscore",     "grave",
      "a",              "b",              "c",              "d",
      "e",              "f",              "g",              "h",
      "i",              "j",              "k",              "l",
      "m",              "n",              "o",              "p",
      "q",              "r",              "s",              "t",
      "u",              "v",              "w",              "x",
      "y",              "z",              "braceleft",      "bar",
      "braceright",     "asciitilde",     "Adieresis",      "Aring",
      "Ccedilla",       "Eacute",         "Ntilde",         "Odieresis",
      "Udieresis",      "aacute",         "agrave",         "acircumflex",
      "adieresis",      "atilde",         "aring",          "ccedilla",
      "eacute",         "egrave",         "ecircumflex",    "edieresis",
      "iacute",         "igrave",         "icircumflex",    "idieresis",
      "ntilde",         "oacute",         "ograve",         "ocircumflex",
      "odieresis",      "otilde",         "uacute",         "ugrave",
      "ucircumflex",    "udieresis",      "dagger",         "degree",
      "cent",           "sterling",       "section",        "bullet",
      "paragraph",      "germandbls",     "registered",     "copyright",
      "trademark",      "acute",          "dieresis",       "notequal",
      "AE",             "Oslash",         "infinity",       "plusminus",
      "lessequal",      "greaterequal",   "yen",            "mu1",
      "partialdiff",    "summation",      "product",        "pi",
      "integral",       "ordfeminine",    "ordmasculine",   "Ohm",
      "ae",             "oslash",         "questiondown",   "exclamdown",
      "logicalnot",     "radical",        "florin",         "approxequal",
      "increment",      "guillemotleft",  "guillemotright", "ellipsis",
      "nbspace",        "Agrave",         "Atilde",         "Otilde",
      "OE",             "oe",             "endash",         "emdash",
      "quotedblleft",   "quotedblright",  "quoteleft",      "quoteright",
      "divide",         "lozenge",        "ydieresis",      "Ydieresis",
      "fraction",       "currency",       "guilsinglleft",  "guilsinglright",
      "fi",             "fl",             "daggerdbl",      "periodcentered",
      "quotesinglbase", "quotedblbase",   "perthousand",    "Acircumflex",
      "Ecircumflex",    "Aacute",         "Edieresis",      "Egrave",
      "Iacute",         "Icircumflex",    "Idieresis",      "Igrave",
      "Oacute",         "Ocircumflex",    "applelogo",      "Ograve",
      "Uacute",         "Ucircumflex",    "Ugrave",         "dotlessi",
      "circumflex",     "tilde",          "overscore",      "breve",
      "dotaccent",      "ring",           "cedilla",        "hungarumlaut",
      "ogonek",         "caron",          "Lslash",         "lslash",
      "Scaron",         "scaron",         "Zcaron",         "zcaron",
      "brokenbar",      "Eth",            "eth",            "Yacute",
      "yacute",         "Thorn",          "thorn",          "minus",
      "multiply",       "onesuperior",    "twosuperior",    "threesuperior",
      "onehalf",        "onequarter",     "threequarters",  "franc",
      "Gbreve",         "gbreve",         "Idot",           "Scedilla",
      "scedilla",       "Cacute",         "cacute",         "Ccaron",
      "ccaron",         "dmacron"
    };

    //------------------------------------------------------------------------
    // CFontFileTrueType
    //------------------------------------------------------------------------

    CFontFileTrueType *CFontFileTrueType::LoadFromBuffer(char *sBuffer, int nLen)
    {
        CFontFileTrueType *pTTF = new CFontFileTrueType( sBuffer, nLen, false );
        if ( !pTTF->m_bSuccess )
        {
            delete pTTF;
            return NULL;
        }
        return pTTF;
    }

    CFontFileTrueType *CFontFileTrueType::LoadFromFile(const wchar_t *wsFileName)
    {
        char *sBuffer;
        int nLen = 0;

        if ( !( sBuffer = CFontFileBase::ReadFile( wsFileName, &nLen ) ) )
            return NULL;

        CFontFileTrueType *pTTF = new CFontFileTrueType( sBuffer, nLen, true );
        if ( !pTTF->m_bSuccess )
        {
            delete pTTF;
            return NULL;
        }
        return pTTF;
    }

    CFontFileTrueType::CFontFileTrueType(char *sBuffer, int nLen, bool bFreeFileData):
      CFontFileBase( sBuffer, nLen, bFreeFileData )
    {
        m_pTables = NULL;
        m_nTablesCount = 0;
        m_pCMaps = NULL;
        m_nCMapsCount = 0;
        m_pNameToGID = NULL;
        m_bSuccess = false;

        Parse();
    }

    CFontFileTrueType::~CFontFileTrueType()
    {
        MemUtilsFree( m_pTables );
        MemUtilsFree( m_pCMaps );
        if ( m_pNameToGID )
            delete m_pNameToGID;
    }

    int CFontFileTrueType::GetCmapsCount()
    {
        return m_nCMapsCount;
    }

    int CFontFileTrueType::GetCmapPlatform(int nIndex)
    {
        return m_pCMaps[nIndex].nPlatform;
    }

    int CFontFileTrueType::GetCmapEncoding(int nIndex)
    {
        return m_pCMaps[nIndex].nEncoding;
    }

    int CFontFileTrueType::FindCmap(int nPlatform, int nEncoding)
    {
        for ( int nIndex = 0; nIndex < m_nCMapsCount; ++nIndex)
        {
            if ( m_pCMaps[nIndex].nPlatform == nPlatform && m_pCMaps[nIndex].nEncoding == nEncoding )
                return nIndex;
        }
        return -1;
    }

    unsigned short CFontFileTrueType::MapCodeToGID(int nCMapIndex, int nChar)
    {
        unsigned short unGid = 0;
        int nSegmentCount = 0, nSegmentEnd = 0, nSegmentStart = 0, nSegmentDelta = 0, nSegmentOffset = 0;
        int nCMapFirst = 0, nCMapLen = 0;
        int  a, b, m;
        bool bSuccess = true;

        if ( nCMapIndex < 0 || nCMapIndex >= m_nCMapsCount)
            return 0;

        int nPos = m_pCMaps[ nCMapIndex ].nOffset;
        switch (m_pCMaps[ nCMapIndex ].nFormat)
        {
        case 0:
            if ( nChar < 0 || nChar >= m_pCMaps[ nCMapIndex ].nLen - 6 )
                return 0;
            unGid = GetU8( m_pCMaps[ nCMapIndex ].nOffset + 6 + nChar, &bSuccess );
            break;
        case 4:
            nSegmentCount = GetU16BE( nPos + 6, &bSuccess ) / 2;
            a = -1;
            b = nSegmentCount - 1;
            nSegmentEnd = GetU16BE( nPos + 14 + 2 * b, &bSuccess );
            if ( nChar > nSegmentEnd )
                return 0;

            while ( b - a > 1 && bSuccess )
            {
                m = (a + b) / 2;
                nSegmentEnd = GetU16BE( nPos + 14 + 2*m, &bSuccess );
                if ( nSegmentEnd < nChar )
                {
                    a = m;
                }
                else
                {
                    b = m;
                }
            }
            nSegmentStart  = GetU16BE( nPos + 16 + 2 * nSegmentCount + 2 * b, &bSuccess );
            nSegmentDelta  = GetU16BE( nPos + 16 + 4 * nSegmentCount + 2 * b, &bSuccess );
            nSegmentOffset = GetU16BE( nPos + 16 + 6 * nSegmentCount + 2 * b, &bSuccess );
            if ( nChar < nSegmentStart )
                return 0;
            if ( 0 == nSegmentOffset )
            {
                unGid = ( nChar + nSegmentDelta ) & 0xffff;
            }
            else
            {
                unGid = GetU16BE( nPos + 16 + 6 * nSegmentCount + 2*b + nSegmentOffset + 2 * ( nChar - nSegmentStart ), &bSuccess );
                if ( 0 != unGid )
                    unGid = ( unGid + nSegmentDelta ) & 0xffff;
            }
            break;
        case 6:
            nCMapFirst = GetU16BE( nPos + 6, &bSuccess );
            nCMapLen   = GetU16BE( nPos + 8, &bSuccess );
            if ( nChar < nCMapFirst || nChar >= nCMapFirst + nCMapLen )
                return 0;
            unGid = GetU16BE( nPos + 10 + 2 * ( nChar - nCMapFirst ), &bSuccess );
            break;
        default:
            return 0;
        }

        if ( !bSuccess )
            return 0;

        return unGid;
    }

    int CFontFileTrueType::MapNameToGID(char *sName)
    {
        if ( !m_pNameToGID )
            return 0;

        return m_pNameToGID->LookupInt( sName );
    }

    unsigned short *CFontFileTrueType::GetCIDToGIDMap(int *pnCIDs)
    {
        CFontFileType1C *pC1FF;

        *pnCIDs = 0;
        if ( !m_bOpenTypeCFF )
            return NULL;

        int nIndex = SeekTable("CFF ");

        if ( !CheckRegion( m_pTables[nIndex].nOffset, m_pTables[nIndex].nLen ) )
            return NULL;

        if ( !( pC1FF = CFontFileType1C::LoadFromBuffer( (char *)m_sFile + m_pTables[nIndex].nOffset, m_pTables[nIndex].nLen ) ) )
            return NULL;

        unsigned short *pMap = pC1FF->GetCIDToGIDMap( pnCIDs );
        delete pC1FF;
        return pMap;
    }

    int CFontFileTrueType::GetEmbeddingRestrictions()
    {
        int nIndex = -1;

        if ( ( nIndex = SeekTable("OS/2" ) ) < 0)
            return 4;

        bool bSuccess = true;
        int nType = GetU16BE( m_pTables[ nIndex ].nOffset + 8, &bSuccess );

        if ( !bSuccess )
            return 4;

        if ( 0x0008 & nType )
            return 2;

        if ( 0x0004 & nType )
            return 1;
        if ( 0x0002 & nType )
            return 0;

        return 3;
    }

    void CFontFileTrueType::ToType42(char *sPSName, char **ppEncoding, unsigned short *pCodeToGID, FontFileOutputFunc pOutputFunc, void *pOutputStream )
    {
        StringExt *seBuffer;

        if ( m_bOpenTypeCFF )
            return;

        // пишем заголовок
        bool bSuccess = true;
        seBuffer = StringExt::Format("%!PS-TrueTypeFont-{0:2g}\n", (double)GetS32BE( 0, &bSuccess ) / 65536.0 );
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
        delete seBuffer;

        (*pOutputFunc)( pOutputStream, "10 dict begin\n", 14);
        (*pOutputFunc)( pOutputStream, "/FontName /", 11);
        (*pOutputFunc)( pOutputStream, sPSName, strlen( sPSName ) );
        (*pOutputFunc)( pOutputStream, " def\n", 5);
        (*pOutputFunc)( pOutputStream, "/FontType 42 def\n", 17);
        (*pOutputFunc)( pOutputStream, "/FontMatrix [1 0 0 1 0 0] def\n", 30);
        seBuffer = StringExt::Format("/FontBBox [{0:d} {1:d} {2:d} {3:d}] def\n", m_arrBBox[0], m_arrBBox[1], m_arrBBox[2], m_arrBBox[3] );
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;
        (*pOutputFunc)( pOutputStream, "/PaintType 0 def\n", 17);

        // записываем содержимое библиотеки шрифта
        ConvertEncoding( ppEncoding, pOutputFunc, pOutputStream );
        ConvertCharStrings( ppEncoding, pCodeToGID, pOutputFunc, pOutputStream);
        ConvertSfnts( pOutputFunc, pOutputStream, NULL, false );

        // закончили запись библиотеки
        (*pOutputFunc)( pOutputStream, "FontName currentdict end definefont pop\n", 40);
    }

    void CFontFileTrueType::ToType1(char *sPSName, char **ppNewEncoding, bool bASKII, FontFileOutputFunc pOutputFunc, void *pOutputStream)
    {
        CFontFileType1C *pT1CFF;

        if (!m_bOpenTypeCFF)
            return;

        int nTableIndex = SeekTable("CFF ");
        if ( !CheckRegion( m_pTables[nTableIndex].nOffset, m_pTables[nTableIndex].nLen ) )
            return;

        if ( !( pT1CFF = CFontFileType1C::LoadFromBuffer((char *)m_sFile + m_pTables[nTableIndex].nOffset, m_pTables[nTableIndex].nLen ) ) )
            return;

        pT1CFF->ToType1( sPSName, ppNewEncoding, bASKII, pOutputFunc, pOutputStream);
        delete pT1CFF;
    }

    void CFontFileTrueType::ToCIDType2(char *sPSName, unsigned short *pCIDMap, int nCIDCount, bool bNeedVerticalMetrics, FontFileOutputFunc pOutputFunc, void *pOutputStream)
    {
        unsigned short nCID = 0;

        if (m_bOpenTypeCFF)
            return;

        // write the header
        bool bSuccess = true;
        StringExt *seBuffer = StringExt::Format("%!PS-TrueTypeFont-{0:2g}\n", (double)GetS32BE( 0, &bSuccess ) / 65536.0 );
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
        delete seBuffer;

        // begin the font dictionary
        (*pOutputFunc)( pOutputStream, "20 dict begin\n", 14);
        (*pOutputFunc)( pOutputStream, "/CIDFontName /", 14);
        (*pOutputFunc)( pOutputStream, sPSName, strlen( sPSName ) );
        (*pOutputFunc)( pOutputStream, " def\n", 5);
        (*pOutputFunc)( pOutputStream, "/CIDFontType 2 def\n", 19);
        (*pOutputFunc)( pOutputStream, "/FontType 42 def\n", 17);
        (*pOutputFunc)( pOutputStream, "/CIDSystemInfo 3 dict dup begin\n", 32);
        (*pOutputFunc)( pOutputStream, "  /Registry (Adobe) def\n", 24);
        (*pOutputFunc)( pOutputStream, "  /Ordering (Identity) def\n", 27);
        (*pOutputFunc)( pOutputStream, "  /Supplement 0 def\n", 20);
        (*pOutputFunc)( pOutputStream, "  end def\n", 10);
        (*pOutputFunc)( pOutputStream, "/GDBytes 2 def\n", 15);
        if ( pCIDMap )
        {
            seBuffer = StringExt::Format("/CIDCount {0:d} def\n", nCIDCount);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
            delete seBuffer;
            if ( nCIDCount > 32767 )
            {
                (*pOutputFunc)( pOutputStream, "/CIDMap [", 9);
                for (int nI = 0; nI < nCIDCount; nI += 32768 - 16)
                {
                    (*pOutputFunc)( pOutputStream, "<\n", 2);
                    for (int nJ = 0; nJ < 32768 - 16 && nI + nJ < nCIDCount; nJ += 16)
                    {
                        (*pOutputFunc)( pOutputStream, "  ", 2);
                        for (int nK = 0; nK < 16 && nI + nJ + nK < nCIDCount; ++nK)
                        {
                            nCID = pCIDMap[nI + nJ + nK];
                            seBuffer = StringExt::Format("{0:02x}{1:02x}", (nCID >> 8) & 0xff, nCID & 0xff);
                            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
                            delete seBuffer;
                        }
                        (*pOutputFunc)( pOutputStream, "\n", 1);
                    }
                    (*pOutputFunc)( pOutputStream, "  >", 3);
                }
                (*pOutputFunc)( pOutputStream, "\n", 1);
                (*pOutputFunc)( pOutputStream, "] def\n", 6);
            }
            else
            {
                (*pOutputFunc)( pOutputStream, "/CIDMap <\n", 10);
                for (int nI = 0; nI < nCIDCount; nI += 16)
                {
                    (*pOutputFunc)( pOutputStream, "  ", 2);
                    for (int nJ = 0; nJ < 16 && nI + nJ < nCIDCount; ++nJ)
                    {
                        nCID = pCIDMap[nI + nJ];
                        seBuffer = StringExt::Format("{0:02x}{1:02x}", (nCID >> 8) & 0xff, nCID & 0xff);
                        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
                        delete seBuffer;
                    }
                    (*pOutputFunc)( pOutputStream, "\n", 1);
                }
                (*pOutputFunc)( pOutputStream, "> def\n", 6);
            }
        }
        else
        {
            // direct mapping - just fill the string(s) with s[i]=i
            seBuffer = StringExt::Format("/CIDCount {0:d} def\n", m_nGlyphs);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
            delete seBuffer;
            if (m_nGlyphs > 32767)
            {
                (*pOutputFunc)( pOutputStream, "/CIDMap [\n", 10);
                for (int nI = 0; nI < m_nGlyphs; nI += 32767)
                {
                    int nTemp = m_nGlyphs - nI < 32767 ? m_nGlyphs - nI : 32767;
                    seBuffer = StringExt::Format("  {0:d} string 0 1 {1:d} {{\n", 2 * nTemp, nTemp - 1);
                    (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
                    delete seBuffer;
                    seBuffer = StringExt::Format("    2 copy dup 2 mul exch {0:d} add -8 bitshift put\n", nI);
                    (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
                    delete seBuffer;
                    seBuffer = StringExt::Format("    1 index exch dup 2 mul 1 add exch {0:d} add 255 and put\n", nI);
                    (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
                    delete seBuffer;
                    (*pOutputFunc)( pOutputStream, "  } for\n", 8);
                }
                (*pOutputFunc)( pOutputStream, "] def\n", 6);
            }
            else
            {
                seBuffer = StringExt::Format("/CIDMap {0:d} string\n", 2 * m_nGlyphs);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
                delete seBuffer;
                seBuffer = StringExt::Format("  0 1 {0:d} {{\n", m_nGlyphs - 1);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
                delete seBuffer;
                (*pOutputFunc)( pOutputStream, "    2 copy dup 2 mul exch -8 bitshift put\n", 42);
                (*pOutputFunc)( pOutputStream, "    1 index exch dup 2 mul 1 add exch 255 and put\n", 50);
                (*pOutputFunc)( pOutputStream, "  } for\n", 8);
                (*pOutputFunc)( pOutputStream, "def\n", 4);
            }
        }
        (*pOutputFunc)( pOutputStream, "/FontMatrix [1 0 0 1 0 0] def\n", 30);
        seBuffer = StringExt::Format("/FontBBox [{0:d} {1:d} {2:d} {3:d}] def\n", m_arrBBox[0], m_arrBBox[1], m_arrBBox[2], m_arrBBox[3] );
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
        delete seBuffer;
        (*pOutputFunc)( pOutputStream, "/PaintType 0 def\n", 17);
        (*pOutputFunc)( pOutputStream, "/Encoding [] readonly def\n", 26);
        (*pOutputFunc)( pOutputStream, "/CharStrings 1 dict dup begin\n", 30);
        (*pOutputFunc)( pOutputStream, "  /.notdef 0 def\n", 17);
        (*pOutputFunc)( pOutputStream, "  end readonly def\n", 19);

        // write the guts of the dictionary
        ConvertSfnts( pOutputFunc, pOutputStream, NULL, bNeedVerticalMetrics);

        // end the dictionary and define the font
        (*pOutputFunc)( pOutputStream, "CIDFontName currentdict end /CIDFont defineresource pop\n", 56);
    }

    void CFontFileTrueType::ToCIDType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream)
    {
        CFontFileType1C *pT1CFF;

        if ( !m_bOpenTypeCFF )
            return;

        int nTableIndex = SeekTable("CFF ");
        if ( !CheckRegion(m_pTables[nTableIndex].nOffset, m_pTables[nTableIndex].nLen) )
            return;

        if ( !( pT1CFF = CFontFileType1C::LoadFromBuffer((char *)m_sFile + m_pTables[nTableIndex].nOffset, m_pTables[nTableIndex].nLen) ) )
            return;

        pT1CFF->ToCIDType0( sPSName, pOutputFunc, pOutputStream);
        delete pT1CFF;
    }

    void CFontFileTrueType::ToType0(char *sPSName, unsigned short *pCIDMap, int nCIDCount, bool bNeedVerticalMetrics, FontFileOutputFunc pOutputFunc, void *pOutputStream)
    {
        StringExt *seBuffer;

        if ( m_bOpenTypeCFF )
            return;

        // write the Type 42 sfnts array
        StringExt *seSfntsName = (new StringExt( sPSName ))->Append("_sfnts");
        ConvertSfnts( pOutputFunc,  pOutputStream, seSfntsName, bNeedVerticalMetrics);
        delete seSfntsName;

        // write the descendant Type 42 fonts
        int nCount = pCIDMap ? nCIDCount : m_nGlyphs;
        for (int nIndex = 0; nIndex < nCount; nIndex += 256)
        {
            (*pOutputFunc)( pOutputStream, "10 dict begin\n", 14);
            (*pOutputFunc)( pOutputStream, "/FontName /", 11);
            (*pOutputFunc)( pOutputStream, sPSName, strlen( sPSName ) );
            seBuffer = StringExt::Format("_{0:02x} def\n", nIndex >> 8);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
            delete seBuffer;
            (*pOutputFunc)( pOutputStream, "/FontType 42 def\n", 17);
            (*pOutputFunc)( pOutputStream, "/FontMatrix [1 0 0 1 0 0] def\n", 30);
            seBuffer = StringExt::Format("/FontBBox [{0:d} {1:d} {2:d} {3:d}] def\n", m_arrBBox[0], m_arrBBox[1], m_arrBBox[2], m_arrBBox[3]);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
            delete seBuffer;
            (*pOutputFunc)( pOutputStream, "/PaintType 0 def\n", 17);
            (*pOutputFunc)( pOutputStream, "/sfnts ", 7);
            (*pOutputFunc)( pOutputStream, sPSName, strlen( sPSName ) );
            (*pOutputFunc)( pOutputStream, "_sfnts def\n", 11);
            (*pOutputFunc)( pOutputStream, "/Encoding 256 array\n", 20);
            for (int nJ = 0; nJ < 256 && nIndex + nJ < nCount; ++nJ )
            {
                seBuffer = StringExt::Format("dup {0:d} /c{1:02x} put\n", nJ, nJ);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
                delete seBuffer;
            }
            (*pOutputFunc)( pOutputStream, "readonly def\n", 13);
            (*pOutputFunc)( pOutputStream, "/CharStrings 257 dict dup begin\n", 32);
            (*pOutputFunc)( pOutputStream, "/.notdef 0 def\n", 15);
            for (int nJ = 0; nJ < 256 && nIndex + nJ < nCount; ++nJ )
            {
                seBuffer = StringExt::Format("/c{0:02x} {1:d} def\n", nJ, pCIDMap ? pCIDMap[nIndex + nJ] : nIndex + nJ);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
                delete seBuffer;
            }
            (*pOutputFunc)( pOutputStream, "end readonly def\n", 17);
            (*pOutputFunc)( pOutputStream,
                  "FontName currentdict end definefont pop\n", 40);
        }

        // write the Type 0 parent font
        (*pOutputFunc)( pOutputStream, "16 dict begin\n", 14);
        (*pOutputFunc)( pOutputStream, "/FontName /", 11);
        (*pOutputFunc)( pOutputStream, sPSName, strlen( sPSName ) );
        (*pOutputFunc)( pOutputStream, " def\n", 5);
        (*pOutputFunc)( pOutputStream, "/FontType 0 def\n", 16);
        (*pOutputFunc)( pOutputStream, "/FontMatrix [1 0 0 1 0 0] def\n", 30);
        (*pOutputFunc)( pOutputStream, "/FMapType 2 def\n", 16);
        (*pOutputFunc)( pOutputStream, "/Encoding [\n", 12);
        for (int nIndex = 0; nIndex < nCount; nIndex += 256)
        {
            seBuffer = StringExt::Format("{0:d}\n", nIndex >> 8);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
            delete seBuffer;
        }
        (*pOutputFunc)( pOutputStream, "] def\n", 6);
        (*pOutputFunc)( pOutputStream, "/FDepVector [\n", 14);
        for (int nIndex =0; nIndex < nCount; nIndex += 256)
        {
            (*pOutputFunc)( pOutputStream, "/", 1);
            (*pOutputFunc)( pOutputStream, sPSName, strlen( sPSName ) );
            seBuffer = StringExt::Format("_{0:02x} findfont\n", nIndex >> 8);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
            delete seBuffer;
        }
        (*pOutputFunc)( pOutputStream, "] def\n", 6);
        (*pOutputFunc)( pOutputStream, "FontName currentdict end definefont pop\n", 40);
    }

    void CFontFileTrueType::ToType0(char *psName, FontFileOutputFunc outputFunc, void *outputStream)
    {
        CFontFileType1C *pT1CFF;

        if ( !m_bOpenTypeCFF )
            return;

        int nTableIndex = SeekTable("CFF ");
        if ( !CheckRegion( m_pTables[nTableIndex].nOffset, m_pTables[nTableIndex].nLen ) )
            return;

        if ( !( pT1CFF = CFontFileType1C::LoadFromBuffer((char *)m_sFile + m_pTables[nTableIndex].nOffset, m_pTables[nTableIndex].nLen)) )
            return;

        pT1CFF->ToType0(psName, outputFunc, outputStream);
        delete pT1CFF;
    }

    void CFontFileTrueType::WriteTTF(FontFileOutputFunc pOutputFunc, void *pOutputStream, const char *sName, unsigned short *pCodeToGID, unsigned char *pUseGlyfs, long lGlyfsCount)
    {
        static char arrCMapTab[36] =
        {
            0, 0,                   // table version number
            0, 1,                   // number of encoding tables
            0, 1,                   // platform ID
            0, 0,                   // encoding ID
            0, 0, 0, 12,            // offset of subtable
            0, 4,                   // subtable format
            0, 24,                  // subtable length
            0, 0,                   // subtable version
            0, 2,                   // segment count * 2
            0, 2,                   // 2 * 2 ^ floor(log2(segCount))
            0, 0,                   // floor(log2(segCount))
            0, 0,                   // 2*segCount - 2*2^floor(log2(segCount))
            (char)0xff, (char)0xff, // endCount[0]
            0, 0,			        // reserved
            0, 0,                   // startCount[0]
            0, 0,                   // idDelta[0]
            0, 0                    // pad to a mulitple of four bytes
        };

        static char arrNameTab[8] =
        {
            0, 0,			// format
            0, 0,			// number of name records
            0, 6,			// offset to start of string storage
            0, 0			// pad to multiple of four bytes
        };
        static char arrPostTab[32] =
        {
            0, 1, 0, 0,  // format
            0, 0, 0, 0,  // italic angle
            0, 0,        // underline position
            0, 0,        // underline thickness
            0, 0, 0, 0,  // fixed pitch
            0, 0, 0, 0,  // min Type 42 memory
            0, 0, 0, 0,  // max Type 42 memory
            0, 0, 0, 0,  // min Type 1 memory
            0, 0, 0, 0   // max Type 1 memory
        };
        static char arrOS2Tab[86] =
        {
            0, 1,			// version
            0, 1,			// xAvgCharWidth
            0, 0,			// usWeightClass
            0, 0,			// usWidthClass
            0, 0,			// fsType
            0, 0,			// ySubscriptXSize
            0, 0,			// ySubscriptYSize
            0, 0,			// ySubscriptXOffset
            0, 0,			// ySubscriptYOffset
            0, 0,			// ySuperscriptXSize
            0, 0,			// ySuperscriptYSize
            0, 0,			// ySuperscriptXOffset
            0, 0,			// ySuperscriptYOffset
            0, 0,			// yStrikeoutSize
            0, 0,			// yStrikeoutPosition
            0, 0,			// sFamilyClass
            0, 0, 0, 0, 0,  // panose
            0, 0, 0, 0, 0,
            0, 0, 0, 0,     // ulUnicodeRange1
            0, 0, 0, 0,	    // ulUnicodeRange2
            0, 0, 0, 0,     // ulUnicodeRange3
            0, 0, 0, 0,     // ulUnicodeRange4
            0, 0, 0, 0,     // achVendID
            0, 0,           // fsSelection
            0, 0,           // usFirstCharIndex
            0, 0,           // usLastCharIndex
            0, 0,           // sTypoAscender
            0, 0,           // sTypoDescender
            0, 0,           // sTypoLineGap
            0, 0,           // usWinAscent
            0, 0,           // usWinDescent
            0, 0, 0, 0,     // ulCodePageRange1
            0, 0, 0, 0      // ulCodePageRange2
        };
        bool badCmapLen, abbrevHMTX;

        int nZeroLengthTables;
        int nHMetrics, nAdvWidth, nLeftSideBearing;
        TrueTypeTable *pNewTables;
        char *arrNewNameTable, *arrNewCmapTable, *arrNewHHEATable, *arrNewHMTXTable;
        int nNewTables, nCmapIndex, nCmapLen, nGlyphLen, nNewNameLen, nNewCmapLen, nNext;
        int nNewHHEALen, nNewHMTXLen;
        unsigned int nLocaChecksum, nGlyphChecksum, nFileChecksum;
        char *arrTableDir;
        char arrLocaBuf[4], arrChecksumBuf[4];
        unsigned int t;
        int nPos = 0, i, j, k, n;

        // Такого не должно быть
        if ( m_bOpenTypeCFF )
            return;

        // Проверяем недостающие таблицы
        bool bMissingCmap = (nCmapIndex = SeekTable("cmap")) < 0;
        bool bMissingName = SeekTable("name") < 0;
        bool bMissingPost = SeekTable("post") < 0;
        bool bMissingOS2  = SeekTable("OS/2") < 0;

        TrueTypeLoca *pLocaTable = (TrueTypeLoca *)malloc( (m_nGlyphs + 1) * sizeof(TrueTypeLoca) );
        bool bUnsortedLoca = false;
        i = SeekTable("loca");
        nPos = m_pTables[i].nOffset;
        bool bSuccess = true;

        for (i = 0; i <= m_nGlyphs; ++i)
        {
            if (m_nLocaFormat)
            {
                pLocaTable[i].nOrigOffset = (int)GetU32BE( nPos + i*4, &bSuccess);
            }
            else
            {
                pLocaTable[i].nOrigOffset = 2 * GetU16BE( nPos + i*2, &bSuccess);
            }
            if (i > 0 && pLocaTable[i].nOrigOffset < pLocaTable[i-1].nOrigOffset)
            {
                bUnsortedLoca = true;
            }
            // Описание глифа должны быть как минимум 12 байт (nContours,
            // xMin, yMin, xMax, yMax, instructionLength - каждый по 2 байта);
            if (i > 0 && pLocaTable[i].nOrigOffset - pLocaTable[i-1].nOrigOffset > 0 && pLocaTable[i].nOrigOffset - pLocaTable[i-1].nOrigOffset < 12)
            {
                pLocaTable[i-1].nOrigOffset = pLocaTable[i].nOrigOffset;
                bUnsortedLoca = true;
            }
            pLocaTable[i].nIndex = i;
        }

        // Проверяем наличие нулевых таблиц
        nZeroLengthTables = 0;
        for (i = 0; i < m_nTablesCount; ++i)
        {
            if (m_pTables[i].nLen == 0)
                ++nZeroLengthTables;
        }

        // Проверяем длину таблицы Cmap
        badCmapLen = false;
        nCmapLen = 0;
        if ( !bMissingCmap )
        {
            nCmapLen = m_pCMaps[0].nOffset + m_pCMaps[0].nLen;
            for (i = 1; i < m_nCMapsCount; ++i)
            {
                if ( m_pCMaps[i].nOffset + m_pCMaps[i].nLen > nCmapLen )
                {
                    nCmapLen = m_pCMaps[i].nOffset + m_pCMaps[i].nLen;
                }
            }
            nCmapLen -= m_pTables[nCmapIndex].nOffset;
            if ( nCmapLen > m_pTables[nCmapIndex].nLen )
            {
                badCmapLen = true;
            }
        }

        // Проверяем, является ли таблица 'hmtx' сокращенной.
        i = SeekTable("hhea");
        nHMetrics = GetU16BE(m_pTables[i].nOffset + 34, &bSuccess);
        abbrevHMTX = nHMetrics < m_nGlyphs;

        // Если все впорядке, и нам не надо переписывать таблицы 'cmap' и 'name', тогда пишем файл TTF как он есть
        if (!bMissingCmap && !bMissingName && !bMissingPost && !bMissingOS2 && !bUnsortedLoca && !badCmapLen && !abbrevHMTX && nZeroLengthTables == 0 && !sName && !pCodeToGID && !pUseGlyfs )
        {
            (*pOutputFunc)( pOutputStream, (char *)m_sFile, m_nLen);
            free( pLocaTable );
            return;
        }

        // Сортируем таблицу 'loca': некоторые шрифты содержат неупорядоченную
        // таблицу 'loca'; а некоторые шрифты с нормальной таблицей 'loca'
        // содержат пустые элементы в середине таблицы, cmpTrueTypeLocaOffset
        // использует сдвиги как основной ключ для сортировки, а номера глифов
        // как второй ключ (чтобы элементы в таблице, которые имели одинаковую позицию
        // шли в том же порядке, как и в исходном шрифте)
        nGlyphLen = 0;
        if (bUnsortedLoca || pUseGlyfs)
        {
            qsort(pLocaTable, m_nGlyphs + 1, sizeof(TrueTypeLoca), &CompareTrueTypeLocaOffset);
            for (i = 0; i < m_nGlyphs; ++i)
            {
                pLocaTable[i].nLen = pLocaTable[i+1].nOrigOffset - pLocaTable[i].nOrigOffset;
            }
            pLocaTable[m_nGlyphs].nLen = 0;
            qsort(pLocaTable, m_nGlyphs + 1, sizeof(TrueTypeLoca), &CompareTrueTypeLocaIndex);
            nPos = 0;

            for (i = 0; i <= m_nGlyphs; ++i)
            {
                // TO DO: Протестировать тут запись только тех глифов, которые нам нужны

                if ( pUseGlyfs && lGlyfsCount == m_nGlyphs )
                {
                    pLocaTable[i].nNewOffset = nPos;
                    int nCurGlyfLen = pLocaTable[i].nLen;
                    pLocaTable[i].nLen = 0;

                    if ( 1 == pUseGlyfs[i] )
                    {
                        pLocaTable[i].nLen = nCurGlyfLen;

                        nPos += pLocaTable[i].nLen;
                        if ( nPos & 3 )
                        {
                            nPos += 4 - ( nPos & 3 );
                        }
                    }
                }
                else
                {
                    pLocaTable[i].nNewOffset = nPos;
                    nPos += pLocaTable[i].nLen;
                    if ( nPos & 3)
                    {
                        nPos += 4 - ( nPos & 3 );
                    }
                }
            }
            nGlyphLen = nPos;
        }

        // Вычисляем чексуммы таблиц 'loca' и 'glyf'
        nLocaChecksum = nGlyphChecksum = 0;
        if (bUnsortedLoca || pUseGlyfs)
        {
            if (m_nLocaFormat)
            {
                for (j = 0; j <= m_nGlyphs; ++j)
                {
                    nLocaChecksum += pLocaTable[j].nNewOffset;
                }
            }
            else
            {
                for (j = 0; j <= m_nGlyphs; j += 2)
                {
                    nLocaChecksum += pLocaTable[j].nNewOffset << 16;
                    if (j + 1 <= m_nGlyphs)
                    {
                        nLocaChecksum += pLocaTable[j+1].nNewOffset;
                    }
                }
            }
            nPos = m_pTables[SeekTable("glyf")].nOffset;
            for (j = 0; j < m_nGlyphs; ++j)
            {
                n = pLocaTable[j].nLen;
                if (n > 0)
                {
                    k = pLocaTable[j].nOrigOffset;
                    if (CheckRegion( nPos + k, n))
                    {
                        nGlyphChecksum += ComputeTableChecksum(m_sFile + nPos + k, n);
                    }
                }
            }
        }

        // Строим новую таблицу 'name'
        if ( sName )
        {
            n = strlen(sName);
            nNewNameLen = (6 + 4*12 + 2 * (3*n + 7) + 3) & ~3;
            arrNewNameTable = (char *)malloc(nNewNameLen);
            memset( arrNewNameTable, 0, nNewNameLen);
            arrNewNameTable[0] = 0;		// format selector
            arrNewNameTable[1] = 0;
            arrNewNameTable[2] = 0;		// number of name records
            arrNewNameTable[3] = 4;
            arrNewNameTable[4] = 0;		// offset to start of string storage
            arrNewNameTable[5] = 6 + 4*12;
            nNext = 0;
            for (i = 0; i < 4; ++i)
            {
                arrNewNameTable[6 + i*12 + 0] = 0;	// platform ID = Microsoft
                arrNewNameTable[6 + i*12 + 1] = 3;
                arrNewNameTable[6 + i*12 + 2] = 0;	// encoding ID = Unicode
                arrNewNameTable[6 + i*12 + 3] = 1;
                arrNewNameTable[6 + i*12 + 4] = 0x04;	// language ID = American English
                arrNewNameTable[6 + i*12 + 5] = 0x09;
                arrNewNameTable[6 + i*12 + 6] = 0;	// name ID
                arrNewNameTable[6 + i*12 + 7] = i + 1;
                arrNewNameTable[6 + i*12 + 8] = i+1 == 2 ? 0 : ((2*n) >> 8); // string length
                arrNewNameTable[6 + i*12 + 9] = i+1 == 2 ? 14 : ((2*n) & 0xff);
                arrNewNameTable[6 + i*12 + 10] = nNext >> 8;		    // string offset
                arrNewNameTable[6 + i*12 + 11] = nNext & 0xff;
                if (i+1 == 2)
                {
                    memcpy( arrNewNameTable + 6 + 4*12 + nNext, "\0R\0e\0g\0u\0l\0a\0r", 14);
                    nNext += 14;
                }
                else
                {
                    for (j = 0; j < n; ++j)
                    {
                        arrNewNameTable[6 + 4*12 + nNext + 2*j] = 0;
                        arrNewNameTable[6 + 4*12 + nNext + 2*j + 1] = sName[j];
                    }
                    nNext += 2 * n;
                }
            }
        }
        else
        {
            nNewNameLen = 0;
            arrNewNameTable = NULL;
        }

        // Строим новую таблицу 'cmap'
        if (pCodeToGID)
        {
            //nNewCmapLen = 44 + 256 * 2;
            //arrNewCmapTable = (char *)malloc(nNewCmapLen);
            //arrNewCmapTable[0] = 0;           // table version number = 0
            //arrNewCmapTable[1] = 0;           //
            //arrNewCmapTable[2] = 0;           // number of encoding tables = 1
            //arrNewCmapTable[3] = 1;           //
            //arrNewCmapTable[4] = 0;           // platform ID = 1 (MacOS) // Эти два поля обязательно должны
            //arrNewCmapTable[5] = 1;           //                         // иметь таки значения, иначе, Adobe
            //arrNewCmapTable[6] = 0;           // encoding ID = 0         // Acrobat может открыть данный шрифт.
            //arrNewCmapTable[7] = 0;           //                         //
            //arrNewCmapTable[8] = 0;           // offset of subtable
            //arrNewCmapTable[9] = 0;           //
            //arrNewCmapTable[10] = 0;          //
            //arrNewCmapTable[11] = 12;         //
            //arrNewCmapTable[12] = 0;          // subtable format = 4
            //arrNewCmapTable[13] = 4;          //
            //arrNewCmapTable[14] = 0x02;       // subtable length
            //arrNewCmapTable[15] = 0x20;       //
            //arrNewCmapTable[16] = 0;          // subtable version = 0
            //arrNewCmapTable[17] = 0;          //
            //arrNewCmapTable[18] = 0;          // segment count * 2
            //arrNewCmapTable[19] = 4;          //
            //arrNewCmapTable[20] = 0;          // 2 * 2 ^ floor(log2(segCount))
            //arrNewCmapTable[21] = 4;          //
            //arrNewCmapTable[22] = 0;          // floor(log2(segCount))
            //arrNewCmapTable[23] = 1;          //
            //arrNewCmapTable[24] = 0;          // 2*segCount - 2*2^floor(log2(segCount))
            //arrNewCmapTable[25] = 0;          //
            //arrNewCmapTable[26] = 0x00;       // endCount[0]
            //arrNewCmapTable[27] = (char)0xff; //
            //arrNewCmapTable[28] = (char)0xff; // endCount[1]
            //arrNewCmapTable[29] = (char)0xff; //
            //arrNewCmapTable[30] = 0;          // reserved
            //arrNewCmapTable[31] = 0;          //
            //arrNewCmapTable[32] = 0x00;       // startCount[0]
            //arrNewCmapTable[33] = 0x00;       //
            //arrNewCmapTable[34] = (char)0xff; // startCount[1]
            //arrNewCmapTable[35] = (char)0xff; //
            //arrNewCmapTable[36] = 0;          // idDelta[0]
            //arrNewCmapTable[37] = 0;          //
            //arrNewCmapTable[38] = 0;          // idDelta[1]
            //arrNewCmapTable[39] = 1;
            //arrNewCmapTable[40] = 0;          // idRangeOffset[0]
            //arrNewCmapTable[41] = 4;          //
            //arrNewCmapTable[42] = 0;          // idRangeOffset[1]
            //arrNewCmapTable[43] = 0;          //

            //for (i = 0; i < 256; ++i)
            //{
            //	arrNewCmapTable[44 + 2*i] = pCodeToGID[i] >> 8;
            //	arrNewCmapTable[44 + 2*i + 1] = pCodeToGID[i] & 0xff;
            //}
            nNewCmapLen = 22 + 256 * 2;
            arrNewCmapTable = (char *)malloc(nNewCmapLen);
            arrNewCmapTable[0] = 0;           // table version number = 0
            arrNewCmapTable[1] = 0;           //
            arrNewCmapTable[2] = 0;           // number of encoding tables = 1
            arrNewCmapTable[3] = 1;           //
            arrNewCmapTable[4] = 0;           // platform ID = 1 (MacOS) // Эти два поля обязательно должны
            arrNewCmapTable[5] = 1;           //                         // иметь таки значения, иначе, Adobe
            arrNewCmapTable[6] = 0;           // encoding ID = 0         // Acrobat может открыть данный шрифт.
            arrNewCmapTable[7] = 0;           //                         //
            arrNewCmapTable[8] = 0;           // offset of subtable
            arrNewCmapTable[9] = 0;           //
            arrNewCmapTable[10] = 0;          //
            arrNewCmapTable[11] = 12;         //
            arrNewCmapTable[12] = 0;          // subtable format = 6
            arrNewCmapTable[13] = 6;          //
            arrNewCmapTable[14] = 0x02;       // subtable length
            arrNewCmapTable[15] = 0x0A;       //
            arrNewCmapTable[16] = 0;          // subtable version = 0
            arrNewCmapTable[17] = 0;          //
            arrNewCmapTable[18] = 0;          // firstCode
            arrNewCmapTable[19] = 0;          //
            arrNewCmapTable[20] = 0x01;       // entryCount
            arrNewCmapTable[21] = 0x00;       //

            for (i = 0; i < 256; ++i)
            {
                arrNewCmapTable[22 + 2*i]     = pCodeToGID[i] >> 8;
                arrNewCmapTable[22 + 2*i + 1] = pCodeToGID[i] & 0xff;
            }
        }
        else
        {
            nNewCmapLen = 0;
            arrNewCmapTable = NULL;
        }

        // Генерируем новую таблицу 'hmtx' и обновляем таблицу 'hhea'
        if ( abbrevHMTX )
        {
            i = SeekTable("hhea");
            nPos = m_pTables[i].nOffset;
            nNewHHEALen = 36;
            arrNewHHEATable = (char *)malloc(nNewHHEALen);
            for (i = 0; i < nNewHHEALen; ++i)
            {
                arrNewHHEATable[i] = GetU8( nPos++, &bSuccess);
            }
            arrNewHHEATable[34] = m_nGlyphs >> 8;
            arrNewHHEATable[35] = m_nGlyphs & 0xff;
            i = SeekTable("hmtx");
            nPos = m_pTables[i].nOffset;
            nNewHMTXLen = 4 * m_nGlyphs;
            arrNewHMTXTable = (char *)malloc(nNewHMTXLen);
            nAdvWidth = 0;
            for (i = 0; i < nHMetrics; ++i)
            {
                nAdvWidth = GetU16BE( nPos, &bSuccess);
                nLeftSideBearing = GetU16BE( nPos + 2, &bSuccess);
                nPos += 4;
                arrNewHMTXTable[4*i    ] = nAdvWidth >> 8;
                arrNewHMTXTable[4*i + 1] = nAdvWidth & 0xff;
                arrNewHMTXTable[4*i + 2] = nLeftSideBearing >> 8;
                arrNewHMTXTable[4*i + 3] = nLeftSideBearing & 0xff;
            }
            for (; i < m_nGlyphs; ++i)
            {
                nLeftSideBearing = GetU16BE( nPos, &bSuccess);
                nPos += 2;
                arrNewHMTXTable[4*i    ] = nAdvWidth >> 8;
                arrNewHMTXTable[4*i + 1] = nAdvWidth & 0xff;
                arrNewHMTXTable[4*i + 2] = nLeftSideBearing >> 8;
                arrNewHMTXTable[4*i + 3] = nLeftSideBearing & 0xff;
            }
        }
        else
        {
            arrNewHHEATable = arrNewHMTXTable = NULL;
            nNewHHEALen = nNewHMTXLen = 0;
        }

        // Создаем список таблиц:
        // - сохраняем исходные ненулевые таблицы
        // - переписываем длину таблицы 'cmap', если необходимо
        // - добавляем недостающие таблицы
        // - сортируем таблицы по тэгам
        // - вычисляем новые позиции таблиц, с учетом 4-байтового выравнивания
        // - пересчитываем чексуммы таблиц
        nNewTables = m_nTablesCount - nZeroLengthTables + (bMissingCmap ? 1 : 0) + (bMissingName ? 1 : 0) + (bMissingPost ? 1 : 0) + (bMissingOS2 ? 1 : 0);
        pNewTables = (TrueTypeTable *)malloc( nNewTables * sizeof(TrueTypeTable) );
        j = 0;
        for (i = 0; i < m_nTablesCount; ++i)
        {
            if (m_pTables[i].nLen > 0)
            {
                pNewTables[j] = m_pTables[i];
                pNewTables[j].nOrigOffset = m_pTables[i].nOffset;
                if ( CheckRegion(m_pTables[i].nOffset, pNewTables[i].nLen) )
                {
                    pNewTables[j].unChecksum = ComputeTableChecksum(m_sFile + m_pTables[i].nOffset, m_pTables[i].nLen);
                    if (m_pTables[i].unTag == headTag)
                    {
                        // don't include the file checksum
                        pNewTables[j].unChecksum -= GetU32BE(m_pTables[i].nOffset + 8, &bSuccess);
                    }
                }
                if (pNewTables[j].unTag == cmapTag && pCodeToGID)
                {
                    pNewTables[j].nLen = nNewCmapLen;
                    pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNewCmapTable, nNewCmapLen);
                }
                else if (pNewTables[j].unTag == cmapTag && badCmapLen)
                {
                    pNewTables[j].nLen = nCmapLen;
                }
                else if (pNewTables[j].unTag == locaTag && (bUnsortedLoca || pCodeToGID || pUseGlyfs))
                {
                    pNewTables[j].nLen = (m_nGlyphs + 1) * (m_nLocaFormat ? 4 : 2);
                    pNewTables[j].unChecksum = nLocaChecksum;
                }
                else if (pNewTables[j].unTag == glyfTag && (bUnsortedLoca || pCodeToGID || pUseGlyfs))
                {
                    pNewTables[j].nLen = nGlyphLen;
                    pNewTables[j].unChecksum = nGlyphChecksum;
                }
                else if (pNewTables[j].unTag == nameTag && sName)
                {
                    pNewTables[j].nLen = nNewNameLen;
                    pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNewNameTable, nNewNameLen);
                }
                else if (pNewTables[j].unTag == hheaTag && abbrevHMTX)
                {
                    pNewTables[j].nLen = nNewHHEALen;
                    pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNewHHEATable, nNewHHEALen);
                }
                else if (pNewTables[j].unTag == hmtxTag && abbrevHMTX)
                {
                    pNewTables[j].nLen = nNewHMTXLen;
                    pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNewHMTXTable, nNewHMTXLen);
                }
                ++j;
            }
        }
        if (bMissingCmap)
        {
            pNewTables[j].unTag = cmapTag;
            if (pCodeToGID)
            {
                pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNewCmapTable, nNewCmapLen);
                pNewTables[j].nLen = nNewCmapLen;
            }
            else
            {
                pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrCMapTab, sizeof(arrCMapTab));
                pNewTables[j].nLen = sizeof(arrCMapTab);
            }
            ++j;
        }
        if (bMissingName)
        {
            pNewTables[j].unTag = nameTag;
            if (sName)
            {
                pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNewNameTable, nNewNameLen);
                pNewTables[j].nLen = nNewNameLen;
            }
            else
            {
                pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrNameTab, sizeof(arrNameTab));
                pNewTables[j].nLen = sizeof(arrNameTab);
            }
            ++j;
        }
        if (bMissingPost)
        {
            pNewTables[j].unTag = postTag;
            pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrPostTab, sizeof(arrPostTab));
            pNewTables[j].nLen = sizeof(arrPostTab);
            ++j;
        }
        if (bMissingOS2)
        {
            pNewTables[j].unTag = os2Tag;
            pNewTables[j].unChecksum = ComputeTableChecksum((unsigned char *)arrOS2Tab, sizeof(arrOS2Tab));
            pNewTables[j].nLen = sizeof(arrOS2Tab);
            ++j;
        }
        qsort(pNewTables, nNewTables, sizeof(TrueTypeTable), CompareTrueTypeTableTag);

        unsigned char *pUseTable = new unsigned char[nNewTables];
        if ( !pUseTable )
        {
            free( arrNewHMTXTable );
            free( arrNewHHEATable );
            free( arrNewCmapTable );
            free( arrNewNameTable );
            free( pNewTables );
            free( pLocaTable );
            return;
        }
        ::memset( pUseTable, 0, nNewTables * sizeof(unsigned char) );
        int nNewReqTables = 0;
        for ( int nIndex = 0; nIndex < nNewTables; nIndex++ )
        {
            if ( pNewTables[nIndex].unTag == cmapTag || pNewTables[nIndex].unTag == glyfTag ||
                pNewTables[nIndex].unTag == headTag || pNewTables[nIndex].unTag == hheaTag ||
                pNewTables[nIndex].unTag == hmtxTag || pNewTables[nIndex].unTag == locaTag ||
                pNewTables[nIndex].unTag == nameTag || pNewTables[nIndex].unTag ==  os2Tag ||
                pNewTables[nIndex].unTag == postTag || pNewTables[nIndex].unTag ==  cvtTag ||
                pNewTables[nIndex].unTag == fpgmTag || pNewTables[nIndex].unTag == maxpTag ||
                pNewTables[nIndex].unTag == prepTag )
            {
                pUseTable[nIndex] = 1;
                nNewReqTables++;
            }
        }

        nPos = 12 + nNewReqTables * 16;
        for (i = 0; i < nNewTables; ++i)
        {
            if ( 1 == pUseTable[i] )
            {
                pNewTables[i].nOffset = nPos;
                nPos += pNewTables[i].nLen;
                if ( nPos & 3)
                {
                    nPos += 4 - (nPos & 3);
                }
            }
        }

        // Записываем информацию о таблицах в файле
        arrTableDir = (char *)malloc(12 + nNewReqTables * 16);
        arrTableDir[0] = 0x00; // sfnt version
        arrTableDir[1] = 0x01; //
        arrTableDir[2] = 0x00; //
        arrTableDir[3] = 0x00; //
        arrTableDir[4] = (char)((nNewReqTables >> 8) & 0xff); // numTables
        arrTableDir[5] = (char)( nNewReqTables       & 0xff);
        for (i = -1, t = (unsigned int)nNewReqTables; t; ++i, t >>= 1) ;
        t = 1 << (4 + i);
        arrTableDir[6] = (char)((t >> 8) & 0xff);		// searchRange
        arrTableDir[7] = (char)(t & 0xff);
        arrTableDir[8] = (char)((i >> 8) & 0xff);		// entrySelector
        arrTableDir[9] = (char)(i & 0xff);
        t = nNewReqTables * 16 - t;
        arrTableDir[10] = (char)((t >> 8) & 0xff);		// rangeShift
        arrTableDir[11] = (char)(t & 0xff);
        nPos = 12;
        for (i = 0; i < nNewTables; ++i)
        {
            if ( 1 == pUseTable[i] )
            {
                arrTableDir[nPos     ] = (char)(pNewTables[i].unTag >> 24);
                arrTableDir[nPos +  1] = (char)(pNewTables[i].unTag >> 16);
                arrTableDir[nPos +  2] = (char)(pNewTables[i].unTag >>  8);
                arrTableDir[nPos +  3] = (char) pNewTables[i].unTag;
                arrTableDir[nPos +  4] = (char)(pNewTables[i].unChecksum >> 24);
                arrTableDir[nPos +  5] = (char)(pNewTables[i].unChecksum >> 16);
                arrTableDir[nPos +  6] = (char)(pNewTables[i].unChecksum >>  8);
                arrTableDir[nPos +  7] = (char) pNewTables[i].unChecksum;
                arrTableDir[nPos +  8] = (char)(pNewTables[i].nOffset >> 24);
                arrTableDir[nPos +  9] = (char)(pNewTables[i].nOffset >> 16);
                arrTableDir[nPos + 10] = (char)(pNewTables[i].nOffset >>  8);
                arrTableDir[nPos + 11] = (char) pNewTables[i].nOffset;
                arrTableDir[nPos + 12] = (char)(pNewTables[i].nLen >> 24);
                arrTableDir[nPos + 13] = (char)(pNewTables[i].nLen >> 16);
                arrTableDir[nPos + 14] = (char)(pNewTables[i].nLen >>  8);
                arrTableDir[nPos + 15] = (char) pNewTables[i].nLen;
                nPos += 16;
            }
        }
        (*pOutputFunc)( pOutputStream, arrTableDir, 12 + nNewReqTables * 16);

        // Вычисляем чексумму файла
        nFileChecksum = ComputeTableChecksum((unsigned char *)arrTableDir, 12 + nNewReqTables * 16);
        for (i = 0; i < nNewTables; ++i)
        {
            if ( 1 == pUseTable[i] )
            {
                nFileChecksum += pNewTables[i].unChecksum;
            }
        }
        nFileChecksum = 0xb1b0afba - nFileChecksum;

        // Записываем сами таблицы
        for (i = 0; i < nNewTables; ++i)
        {
            if ( 1 == pUseTable[i] )
            {
                if (pNewTables[i].unTag == headTag)
                {
                    if (CheckRegion(pNewTables[i].nOrigOffset, pNewTables[i].nLen))
                    {
                        (*pOutputFunc)( pOutputStream, (char *)m_sFile + pNewTables[i].nOrigOffset, 8);
                        arrChecksumBuf[0] = nFileChecksum >> 24;
                        arrChecksumBuf[1] = nFileChecksum >> 16;
                        arrChecksumBuf[2] = nFileChecksum >> 8;
                        arrChecksumBuf[3] = nFileChecksum;
                        (*pOutputFunc)( pOutputStream, arrChecksumBuf, 4);
                        (*pOutputFunc)( pOutputStream, (char *)m_sFile + pNewTables[i].nOrigOffset + 12, pNewTables[i].nLen - 12);
                    }
                    else
                    {
                        for (j = 0; j < pNewTables[i].nLen; ++j)
                        {
                            (*pOutputFunc)( pOutputStream, "\0", 1);
                        }
                    }
                }
                else if (pNewTables[i].unTag == cmapTag && pCodeToGID)
                    (*pOutputFunc)( pOutputStream, arrNewCmapTable, pNewTables[i].nLen);
                else if (pNewTables[i].unTag == cmapTag && bMissingCmap)
                    (*pOutputFunc)( pOutputStream, arrCMapTab, pNewTables[i].nLen);
                else if (pNewTables[i].unTag == nameTag && sName)
                    (*pOutputFunc)( pOutputStream, arrNewNameTable, pNewTables[i].nLen);
                else if (pNewTables[i].unTag == nameTag && bMissingName)
                    (*pOutputFunc)( pOutputStream, arrNameTab, pNewTables[i].nLen);
                else if (pNewTables[i].unTag == postTag && bMissingPost)
                    (*pOutputFunc)( pOutputStream, arrPostTab, pNewTables[i].nLen);
                else if (pNewTables[i].unTag == os2Tag && bMissingOS2)
                    (*pOutputFunc)( pOutputStream, arrOS2Tab, pNewTables[i].nLen);
                else if (pNewTables[i].unTag == hheaTag && abbrevHMTX)
                    (*pOutputFunc)( pOutputStream, arrNewHHEATable, pNewTables[i].nLen);
                else if (pNewTables[i].unTag == hmtxTag && abbrevHMTX)
                    (*pOutputFunc)( pOutputStream, arrNewHMTXTable, pNewTables[i].nLen);
                else if (pNewTables[i].unTag == locaTag && (bUnsortedLoca || pCodeToGID || pUseGlyfs))
                {
                    for (j = 0; j <= m_nGlyphs; ++j)
                    {
                        if (m_nLocaFormat)
                        {
                            arrLocaBuf[0] = (char)(pLocaTable[j].nNewOffset >> 24);
                            arrLocaBuf[1] = (char)(pLocaTable[j].nNewOffset >> 16);
                            arrLocaBuf[2] = (char)(pLocaTable[j].nNewOffset >>  8);
                            arrLocaBuf[3] = (char) pLocaTable[j].nNewOffset;
                            (*pOutputFunc)( pOutputStream, arrLocaBuf, 4);
                        }
                        else
                        {
                            arrLocaBuf[0] = (char)(pLocaTable[j].nNewOffset >> 9);
                            arrLocaBuf[1] = (char)(pLocaTable[j].nNewOffset >> 1);
                            (*pOutputFunc)( pOutputStream, arrLocaBuf, 2);
                        }
                    }
                }
                else if (pNewTables[i].unTag == glyfTag && (bUnsortedLoca || pCodeToGID || pUseGlyfs))
                {
                    nPos = m_pTables[SeekTable("glyf")].nOffset;

                    for (j = 0; j < m_nGlyphs; ++j)
                    {
                        n = pLocaTable[j].nLen;
                        if (n > 0)
                        {
                            k = pLocaTable[j].nOrigOffset;
                            if (CheckRegion( nPos + k, n))
                            {
                                (*pOutputFunc)( pOutputStream, (char *)m_sFile + nPos + k, n);
                            }
                            else
                            {
                                for (k = 0; k < n; ++k)
                                {
                                    (*pOutputFunc)( pOutputStream, "\0", 1);
                                }
                            }
                            if ((k = pLocaTable[j].nLen & 3))
                            {
                                (*pOutputFunc)( pOutputStream, "\0\0\0\0", 4 - k);
                            }
                        }
                    }
                }
                else
                {
                    if ( CheckRegion( pNewTables[i].nOrigOffset, pNewTables[i].nLen) )
                    {
                        (*pOutputFunc)( pOutputStream, (char *)m_sFile + pNewTables[i].nOrigOffset, pNewTables[i].nLen);
                    }
                    else
                    {
                        for (j = 0; j < pNewTables[i].nLen; ++j)
                        {
                            (*pOutputFunc)( pOutputStream, "\0", 1);
                        }
                    }
                }
                if ( pNewTables[i].nLen & 3 )
                {
                    (*pOutputFunc)( pOutputStream, "\0\0\0", 4 - ( pNewTables[i].nLen & 3) );
                }
            }
        }

        delete []pUseTable;
        free( arrNewHMTXTable );
        free( arrNewHHEATable );
        free( arrNewCmapTable );
        free( arrNewNameTable );
        free( arrTableDir );
        free( pNewTables );
        free( pLocaTable );
    }



    void CFontFileTrueType::ConvertEncoding   (char **ppEncoding, FontFileOutputFunc pOutputFunc, void *pOutputStream)
    {
        // конвертация кодировки в тип Type42
        char *sName;
        StringExt *seBuffer;

        (*pOutputFunc)( pOutputStream, "/Encoding 256 array\n", 20);
        if ( ppEncoding )
        {
            for (int nIndex = 0; nIndex < 256; ++nIndex )
            {
                if ( !( sName = ppEncoding[ nIndex ] ) )
                {
                    sName = ".notdef";
                }
                seBuffer = StringExt::Format("dup {0:d} /", nIndex );
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
                delete seBuffer;
                (*pOutputFunc)( pOutputStream, sName, strlen( sName ) );
                (*pOutputFunc)( pOutputStream, " put\n", 5);
            }
        }
        else
        {
            for (int nIndex = 0; nIndex < 256; ++nIndex)
            {
                seBuffer = StringExt::Format("dup {0:d} /c{1:02x} put\n", nIndex, nIndex );
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
        }
        (*pOutputFunc)( pOutputStream, "readonly def\n", 13);
    }

    void CFontFileTrueType::ConvertCharStrings(char **ppEncoding, unsigned short *pnCodeToGID, FontFileOutputFunc pOutputFunc, void *pOutputStream)
    {
        char *sName;
        StringExt *seBuffer;
        char sBuf[16];

        (*pOutputFunc)( pOutputStream, "/CharStrings 256 dict dup begin\n", 32);
        (*pOutputFunc)( pOutputStream, "/.notdef 0 def\n", 15);

        if (m_nCMapsCount == 0)
        {
            (*pOutputFunc)( pOutputStream, "end readonly def\n", 17);
            return;
        }

        // Ставим в соответствие названию символа glyph индекс:
        // 1. Используем ppEncoding для отображения имени символа в его номер
        // 2. Используем pnCodeToGID для отображения номера символа в glyph индекс
        int nGlyphIndex = 0;
        for (int nIndex = 255; nIndex >= 0; --nIndex)
        {
            if (ppEncoding)
            {
                sName = ppEncoding[nIndex];
            }
            else
            {
                sprintf( sBuf, "c%02x", nIndex);
                sName = sBuf;
            }
            if ( sName && strcmp( sName, ".notdef") )
            {
                nGlyphIndex = pnCodeToGID[nIndex];

                if ( nGlyphIndex > 0 && nGlyphIndex < m_nGlyphs )
                {
                    (*pOutputFunc)( pOutputStream, "/", 1);
                    (*pOutputFunc)( pOutputStream, sName, strlen( sName ) );
                    seBuffer = StringExt::Format(" {0:d} def\n", nGlyphIndex );
                    (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
                    delete seBuffer;
                }
            }
        }
    }

    void CFontFileTrueType::ConvertSfnts      (FontFileOutputFunc pOutputFunc, void *pOutputStream, StringExt *seName, bool bNeedVerticalMetrics)
    {
        unsigned char pHeadData[54];
        TrueTypeLoca *pLocaTable;
        unsigned char *pLocaData;
        TrueTypeTable arrNewTables[ nT42Tables ];
        unsigned char arrTableDir[12 + nT42Tables * 16];
        unsigned int nChecksum = 0;
        int nNewTables;
        int nLength = 0, nGlyphfPos = 0, nJ = 0;

        unsigned char arrVheaTable[36] =
        {
            0, 1, 0, 0,		// table version number
            0, 0,			// ascent
            0, 0,			// descent
            0, 0,			// reserved
            0, 0,			// max advance height
            0, 0,			// min top side bearing
            0, 0,			// min bottom side bearing
            0, 0,			// y max extent
            0, 0,			// caret slope rise
            0, 1,			// caret slope run
            0, 0,			// caret offset
            0, 0,			// reserved
            0, 0,			// reserved
            0, 0,			// reserved
            0, 0,			// reserved
            0, 0,			// metric data format
            0, 1			// number of advance heights in vmtx table
        };

        unsigned char *arrVmtxTable;
        bool bNeedVhea, bNeedVmtx;

        int nCurTable = SeekTable("head");
        int nPos = m_pTables[ nCurTable ].nOffset;
        if ( !CheckRegion( nPos, 54) )
            return;

        memcpy( pHeadData, m_sFile + nPos, 54 );
        pHeadData[8] = pHeadData[9] = pHeadData[10] = pHeadData[11] = (unsigned char)0;

        pLocaTable = (TrueTypeLoca *)MemUtilsMallocArray( m_nGlyphs + 1, sizeof(TrueTypeLoca) );
        nCurTable = SeekTable("loca");
        nPos = m_pTables[ nCurTable ].nOffset;
        bool bSuccess = true;
        for (int nIndex = 0; nIndex <= m_nGlyphs; ++nIndex )
        {
            pLocaTable[nIndex].nIndex = nIndex;
            if ( m_nLocaFormat )
                pLocaTable[nIndex].nOrigOffset = (int)GetU32BE( nPos + nIndex * 4, &bSuccess );
            else
                pLocaTable[nIndex].nOrigOffset = 2 * GetU16BE( nPos + nIndex * 2, &bSuccess );
        }
        qsort( pLocaTable, m_nGlyphs + 1, sizeof(TrueTypeLoca), &CompareTrueTypeLocaOffset );

        for (int nIndex = 0; nIndex < m_nGlyphs; ++nIndex)
        {
            pLocaTable[ nIndex ].nLen = pLocaTable[nIndex + 1].nOrigOffset - pLocaTable[nIndex].nOrigOffset;
        }

        pLocaTable[m_nGlyphs].nLen = 0;
        qsort( pLocaTable, m_nGlyphs + 1, sizeof(TrueTypeLoca), &CompareTrueTypeLocaIndex);
        nPos = 0;
        for (int nIndex = 0; nIndex <= m_nGlyphs; ++nIndex )
        {
            pLocaTable[nIndex].nNewOffset = nPos;
            nPos += pLocaTable[nIndex].nLen;
            if ( nPos & 3 )
            {
                nPos += 4 - ( nPos & 3);
            }
        }

        // Создаем новую таблицу 'loca'
        pLocaData = (unsigned char *)MemUtilsMallocArray( m_nGlyphs + 1, (m_nLocaFormat ? 4 : 2) );
        for (int nIndex = 0; nIndex <= m_nGlyphs; ++nIndex )
        {
            nPos = pLocaTable[nIndex].nNewOffset;
            if ( m_nLocaFormat )
            {
                pLocaData[ 4 * nIndex     ] = (unsigned char)( nPos >> 24);
                pLocaData[ 4 * nIndex + 1 ] = (unsigned char)( nPos >> 16);
                pLocaData[ 4 * nIndex + 2 ] = (unsigned char)( nPos >>  8);
                pLocaData[ 4 * nIndex + 3 ] = (unsigned char) nPos;
            }
            else
            {
                pLocaData[ 2 * nIndex     ] = (unsigned char)( nPos >> 9);
                pLocaData[ 2 * nIndex + 1 ] = (unsigned char)( nPos >> 1);
            }
        }

        // считаем число таблиц
        nNewTables = 0;
        for (int nIndex = 0; nIndex < nT42Tables; ++nIndex )
        {
            if (t42Tables[ nIndex ].bRequired || SeekTable( t42Tables[ nIndex ].sTag ) >= 0)
            {
                ++nNewTables;
            }
        }
        arrVmtxTable = NULL;
        int nAdvance = 0;
        if ( bNeedVerticalMetrics )
        {
            bNeedVhea = SeekTable("vhea") < 0;
            bNeedVmtx = SeekTable("vmtx") < 0;
            if ( bNeedVhea || bNeedVmtx )
            {
                nCurTable = SeekTable("head");
                nAdvance = GetU16BE( m_pTables[ nCurTable ].nOffset + 18, &bSuccess ); // units per em
                if ( bNeedVhea )
                {
                    ++nNewTables;
                }
                if ( bNeedVmtx )
                {
                    ++nNewTables;
                }
            }
        }

        nPos = 12 + nNewTables * 16;
        int nTableIndex = 0;
        for (int nIndex = 0; nIndex < nT42Tables; ++nIndex)
        {
            nLength = -1;
            nChecksum = 0;
            if ( t42HeadTable == nIndex )
            {
                nLength = 54;
                nChecksum = ComputeTableChecksum( pHeadData, 54);
            }
            else if ( t42LocaTable == nIndex )
            {
                nLength = (m_nGlyphs + 1) * ( m_nLocaFormat ? 4 : 2);
                nChecksum = ComputeTableChecksum( pLocaData, nLength);
            }
            else if ( t42GlyfTable == nIndex)
            {
                nLength = 0;
                nChecksum = 0;
                nGlyphfPos = m_pTables[SeekTable("glyf")].nOffset;
                for (nJ = 0; nJ < m_nGlyphs; ++nJ )
                {
                    nLength += pLocaTable[nJ].nLen;
                    if ( nLength & 3 )
                    {
                        nLength += 4 - (nLength & 3);
                    }
                    if ( CheckRegion( nGlyphfPos + pLocaTable[nJ].nOrigOffset, pLocaTable[nJ].nLen) )
                    {
                        nChecksum += ComputeTableChecksum( m_sFile + nGlyphfPos + pLocaTable[nJ].nOrigOffset, pLocaTable[nJ].nLen);
                    }
                }
            }
            else
            {
                if ( ( nJ = SeekTable(t42Tables[nIndex].sTag ) ) >= 0 )
                {
                    nLength = m_pTables[nJ].nLen;
                    if ( CheckRegion( m_pTables[nJ].nOffset, nLength ) )
                    {
                        nChecksum = ComputeTableChecksum(m_sFile + m_pTables[nJ].nOffset, nLength);
                    }
                }
                else if ( bNeedVerticalMetrics && nIndex == t42VheaTable )
                {
                    arrVheaTable[10] = nAdvance / 256;    // max advance height
                    arrVheaTable[11] = nAdvance % 256;
                    nLength = sizeof(arrVheaTable);
                    nChecksum = ComputeTableChecksum(arrVheaTable, nLength);
                }
                else if ( bNeedVerticalMetrics && nIndex == t42VmtxTable)
                {
                    nLength = 4 + (m_nGlyphs - 1) * 4;
                    arrVmtxTable = (unsigned char *)MemUtilsMalloc( nLength );
                    arrVmtxTable[0] = nAdvance / 256;
                    arrVmtxTable[1] = nAdvance % 256;
                    for (nJ = 2; nJ < nLength; nJ += 2)
                    {
                        arrVmtxTable[nJ] = 0;
                        arrVmtxTable[nJ + 1] = 0;
                    }
                    nChecksum = ComputeTableChecksum(arrVmtxTable, nLength);
                }
                else if ( t42Tables[ nIndex ].bRequired )
                {
                    // TO DO : "Embedded TrueType font is missing a required table  t42Tables[i].tag
                    nLength = 0;
                    nChecksum = 0;
                }
            }
            if ( nLength >= 0 )
            {
                arrNewTables[nTableIndex].unTag = ((t42Tables[ nIndex ].sTag[0] & 0xff) << 24) | ((t42Tables[ nIndex ].sTag[1] & 0xff) << 16) | ((t42Tables[ nIndex ].sTag[2] & 0xff) <<  8) | (t42Tables[ nIndex ].sTag[3] & 0xff);
                arrNewTables[nTableIndex].unChecksum = nChecksum;
                arrNewTables[nTableIndex].nOffset = nPos;
                arrNewTables[nTableIndex].nLen = nLength;
                nPos += nLength;
                if ( nPos & 3)
                {
                    nPos += 4 - ( nLength & 3 );
                }
                ++nTableIndex;
            }
        }

        // construct the table directory
        arrTableDir[0] = 0x00;		// sfnt version
        arrTableDir[1] = 0x01;
        arrTableDir[2] = 0x00;
        arrTableDir[3] = 0x00;
        arrTableDir[4] = 0;		// numTables
        arrTableDir[5] = nNewTables;
        arrTableDir[6] = 0;		// searchRange
        arrTableDir[7] = (unsigned char)128;
        arrTableDir[8] = 0;		// entrySelector
        arrTableDir[9] = 3;
        arrTableDir[10] = 0;		// rangeShift
        arrTableDir[11] = (unsigned char)(16 * nNewTables - 128);
        nPos = 12;
        for ( int nIndex = 0; nIndex < nNewTables; ++nIndex )
        {
            arrTableDir[ nPos     ] = (unsigned char)(arrNewTables[nIndex].unTag >> 24);
            arrTableDir[ nPos +  1] = (unsigned char)(arrNewTables[nIndex].unTag >> 16);
            arrTableDir[ nPos +  2] = (unsigned char)(arrNewTables[nIndex].unTag >>  8);
            arrTableDir[ nPos +  3] = (unsigned char) arrNewTables[nIndex].unTag;
            arrTableDir[ nPos +  4] = (unsigned char)(arrNewTables[nIndex].unChecksum >> 24);
            arrTableDir[ nPos +  5] = (unsigned char)(arrNewTables[nIndex].unChecksum >> 16);
            arrTableDir[ nPos +  6] = (unsigned char)(arrNewTables[nIndex].unChecksum >>  8);
            arrTableDir[ nPos +  7] = (unsigned char) arrNewTables[nIndex].unChecksum;
            arrTableDir[ nPos +  8] = (unsigned char)(arrNewTables[nIndex].nOffset >> 24);
            arrTableDir[ nPos +  9] = (unsigned char)(arrNewTables[nIndex].nOffset >> 16);
            arrTableDir[ nPos + 10] = (unsigned char)(arrNewTables[nIndex].nOffset >>  8);
            arrTableDir[ nPos + 11] = (unsigned char) arrNewTables[nIndex].nOffset;
            arrTableDir[ nPos + 12] = (unsigned char)(arrNewTables[nIndex].nLen >> 24);
            arrTableDir[ nPos + 13] = (unsigned char)(arrNewTables[nIndex].nLen >> 16);
            arrTableDir[ nPos + 14] = (unsigned char)(arrNewTables[nIndex].nLen >>  8);
            arrTableDir[ nPos + 15] = (unsigned char) arrNewTables[nIndex].nLen;
            nPos += 16;
        }

        // вычисляем checksum и сохраняем ее в заголовке таблицы
        nChecksum = ComputeTableChecksum( arrTableDir, 12 + nNewTables*16);
        for (int nIndex = 0; nIndex < nNewTables; ++nIndex)
        {
            nChecksum += arrNewTables[ nIndex ].unChecksum;
        }
        nChecksum = 0xb1b0afba - nChecksum; // по спецификации TrueType
        pHeadData[ 8] = (unsigned char)(nChecksum >> 24);
        pHeadData[ 9] = (unsigned char)(nChecksum >> 16);
        pHeadData[10] = (unsigned char)(nChecksum >>  8);
        pHeadData[11] = (unsigned char) nChecksum;

        // начинаем писать массив sfnts
        if ( seName )
        {
            (*pOutputFunc)( pOutputStream, "/", 1);
            (*pOutputFunc)( pOutputStream, seName->GetBuffer(), seName->GetLength() );
            (*pOutputFunc)( pOutputStream, " [\n", 3);
        }
        else
        {
            (*pOutputFunc)( pOutputStream, "/sfnts [\n", 9);
        }

        DumpString( arrTableDir, 12 + nNewTables*16, pOutputFunc, pOutputStream);

        // пишем таблицы
        for (int nIndex = 0; nIndex < nNewTables; ++nIndex)
        {
            if ( t42HeadTable == nIndex )
            {
                DumpString( pHeadData, 54, pOutputFunc, pOutputStream);
            }
            else if ( t42LocaTable == nIndex )
            {
                nLength = (m_nGlyphs + 1) * (m_nLocaFormat ? 4 : 2);
                DumpString(pLocaData, nLength, pOutputFunc, pOutputStream);
            }
            else if ( t42GlyfTable == nIndex )
            {
                nGlyphfPos = m_pTables[SeekTable("glyf")].nOffset;
                for (nJ = 0; nJ < m_nGlyphs; ++nJ)
                {
                    if ( pLocaTable[nJ].nLen > 0 && CheckRegion( nGlyphfPos + pLocaTable[nJ].nOrigOffset, pLocaTable[nJ].nLen ) )
                    {
                        DumpString( m_sFile + nGlyphfPos + pLocaTable[nJ].nOrigOffset, pLocaTable[nJ].nLen, pOutputFunc, pOutputStream);
                    }
                }
            }
            else
            {
                // nLength == 0 означает, что таблица не найдена, а ошибка уже была выдана
                // во время конструирования таблицы
                if ( ( nLength = arrNewTables[nIndex].nLen ) > 0 )
                {
                    if ( ( nJ = SeekTable( t42Tables[nIndex].sTag ) ) >= 0 && CheckRegion( m_pTables[nJ].nOffset, m_pTables[nJ].nLen ) )
                    {
                        DumpString(m_sFile + m_pTables[nJ].nOffset, m_pTables[nJ].nLen, pOutputFunc, pOutputStream);
                    }
                    else if ( bNeedVerticalMetrics && t42VheaTable == nIndex )
                    {
                        DumpString(arrVheaTable, nLength, pOutputFunc, pOutputStream);
                    }
                    else if ( bNeedVerticalMetrics && t42VmtxTable == nIndex )
                    {
                        DumpString(arrVmtxTable, nLength, pOutputFunc, pOutputStream);
                        MemUtilsFree(arrVmtxTable);
                    }
                }
            }
        }

        // закончили писать массив sfnts
        (*pOutputFunc)( pOutputStream, "] def\n", 6);

        MemUtilsFree( pLocaData );
        MemUtilsFree( pLocaTable );
    }

    void CFontFileTrueType::DumpString(unsigned char *sString, int nLength, FontFileOutputFunc pOutputFunc, void *pOutputStream)
    {
        StringExt *seBuffer;
        int nPad = 0 ;

        (*pOutputFunc)( pOutputStream, "<", 1);
        for ( int nIndex = 0; nIndex < nLength; nIndex += 32 )
        {
            for ( int nJ = 0; nJ < 32 && nIndex + nJ < nLength; ++nJ )
            {
                seBuffer = StringExt::Format("{0:02x}", sString[ nIndex + nJ ] & 0xff);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength() );
                delete seBuffer;
            }
            if ( nIndex % (65536 - 32) == 65536 - 64)
            {
                (*pOutputFunc)( pOutputStream, ">\n<", 3);
            }
            else if ( nIndex + 32 < nLength)
            {
                (*pOutputFunc)( pOutputStream, "\n", 1);
            }
        }
        if ( nLength & 3)
        {
            nPad = 4 - (nLength & 3);
            for ( int nIndex = 0; nIndex < nPad; ++nIndex )
            {
                (*pOutputFunc)( pOutputStream, "00", 2);
            }
        }
        // Добавляем дополнительный нулевой байт, по спецификации Adobe Type 42
        (*pOutputFunc)( pOutputStream, "00>\n", 4);
    }

    unsigned int CFontFileTrueType::ComputeTableChecksum(unsigned char *sData, int nLength)
    {
        unsigned int nWord = 0;

        unsigned int nChecksum = 0;
        for ( int nIndex = 0; nIndex + 3 < nLength; nIndex += 4 )
        {
            nWord = ( ( sData[ nIndex ] & 0xff) << 24) + ((sData[ nIndex + 1 ] & 0xff) << 16) + ((sData[ nIndex + 2 ] & 0xff) <<  8) + (sData[ nIndex + 3 ] & 0xff);
            nChecksum += nWord;
        }
        if ( nLength & 3 )
        {
            nWord = 0;
            int nTemp = nLength & ~3;
            switch ( nLength & 3 )
            {
            case 3:
                nWord |= (sData[nTemp + 2] & 0xff) <<  8;
            case 2:
                nWord |= (sData[nTemp + 1] & 0xff) << 16;
            case 1:
                nWord |= (sData[nTemp]     & 0xff) << 24;
                break;
            }
            nChecksum += nWord;
        }
        return nChecksum;
    }

    void CFontFileTrueType::Parse()
    {
        int nPos = 0, nIndex = 0, nJ;

        m_bSuccess = true;

        // Проверяем является ли данный файл (TTC)
        unsigned int usTopTag = GetU32BE(0, &m_bSuccess);
        if ( !m_bSuccess )
            return;

        if ( usTopTag == ttcfTag )
        {
            nPos = GetU32BE(12, &m_bSuccess);
            if (!m_bSuccess)
                return;
        }
        else
            nPos = 0;

        // Проверяем sfnt версию
        int nSfntVersion = GetU32BE( nPos, &m_bSuccess);
        if ( !m_bSuccess )
            return;

        // Проверяем на формат данных. CCF или нет?
        m_bOpenTypeCFF = ( nSfntVersion == 0x4f54544f ); // 'OTTO'

        m_nTablesCount = GetU16BE( nPos + 4, &m_bSuccess);
        if ( !m_bSuccess )
            return;

        m_pTables = (TrueTypeTable *)MemUtilsMallocArray( m_nTablesCount, sizeof(TrueTypeTable) );
        nPos += 12;

        for ( nIndex = 0; nIndex < m_nTablesCount; ++nIndex )
        {
            m_pTables[nIndex].unTag = GetU32BE( nPos, &m_bSuccess);
            m_pTables[nIndex].unChecksum = GetU32BE( nPos + 4, &m_bSuccess);
            m_pTables[nIndex].nOffset = (int)GetU32BE( nPos + 8, &m_bSuccess);
            m_pTables[nIndex].nLen = (int)GetU32BE( nPos + 12, &m_bSuccess);
            if ( m_pTables[nIndex].nOffset + m_pTables[nIndex].nLen < m_pTables[nIndex].nOffset || m_pTables[nIndex].nOffset + m_pTables[nIndex].nLen > m_nLen )
            {
                m_bSuccess = false;
            }
            nPos += 16;
        }
        if ( !m_bSuccess )
            return;

        // ищем таблицы необходимые как и для TrueType так и для Type 42
        if ( SeekTable("head") < 0 || SeekTable("hhea") < 0 || SeekTable("maxp") < 0 || SeekTable("hmtx") < 0 || ( !m_bOpenTypeCFF && SeekTable("loca") < 0 ) || ( !m_bOpenTypeCFF && SeekTable("glyf" ) < 0) || ( m_bOpenTypeCFF && SeekTable("CFF " ) < 0 ) )
        {
            m_bSuccess = false;
            return;
        }

        // читаем таблицы CMaps
        if ( ( nIndex = SeekTable("cmap") ) >= 0 )
        {
            nPos = m_pTables[nIndex].nOffset + 2;
            m_nCMapsCount = GetU16BE( nPos, &m_bSuccess);

            if (0 == m_nCMapsCount)
            {
                m_bSuccess = false;
                return;
            }

            nPos += 2;
            if ( !m_bSuccess )
                return;

            m_pCMaps = (TrueTypeCmap *)MemUtilsMallocArray( m_nCMapsCount, sizeof(TrueTypeCmap) );

            for ( nJ = 0; nJ < m_nCMapsCount; ++nJ )
            {
                m_pCMaps[nJ].nPlatform = GetU16BE( nPos, &m_bSuccess);
                m_pCMaps[nJ].nEncoding = GetU16BE( nPos + 2, &m_bSuccess);
                unsigned int nTemp = GetU32BE( nPos + 4, &m_bSuccess);
                m_pCMaps[nJ].nOffset = m_pTables[nIndex].nOffset + GetU32BE( nPos + 4, &m_bSuccess);
                nPos += 8;
                m_pCMaps[nJ].nFormat = GetU16BE(m_pCMaps[nJ].nOffset, &m_bSuccess);
                m_pCMaps[nJ].nLen = GetU16BE(m_pCMaps[nJ].nOffset + 2, &m_bSuccess);
            }
            if (!m_bSuccess)
                return;
        }
        else
            m_nCMapsCount = 0;

        nIndex = SeekTable("maxp");
        m_nGlyphs = GetU16BE( m_pTables[nIndex].nOffset + 4, &m_bSuccess );
        if (!m_bSuccess)
            return;

        nIndex = SeekTable("head");
        m_arrBBox[0] = GetS16BE(m_pTables[nIndex].nOffset + 36, &m_bSuccess);
        m_arrBBox[1] = GetS16BE(m_pTables[nIndex].nOffset + 38, &m_bSuccess);
        m_arrBBox[2] = GetS16BE(m_pTables[nIndex].nOffset + 40, &m_bSuccess);
        m_arrBBox[3] = GetS16BE(m_pTables[nIndex].nOffset + 42, &m_bSuccess);
        m_nLocaFormat = GetS16BE(m_pTables[nIndex].nOffset + 50, &m_bSuccess);
        if (!m_bSuccess)
            return;

        // Проверяем корректность таблицы loca
        if ( !m_bOpenTypeCFF )
        {
            nIndex = SeekTable("loca");
            if ( m_pTables[nIndex].nLen < 0 )
            {
                m_bSuccess = false;
                return;
            }
            if ( m_pTables[nIndex].nLen < ( m_nGlyphs + 1) * ( m_nLocaFormat ? 4 : 2 ) )
            {
                m_nGlyphs = m_pTables[nIndex].nLen / (m_nLocaFormat ? 4 : 2) - 1;
            }
            for ( nJ = 0; nJ <= m_nGlyphs; ++nJ )
            {
                if ( m_nLocaFormat )
                    nPos = (int)GetU32BE(m_pTables[nIndex].nOffset + nJ * 4, &m_bSuccess);
                else
                    nPos = GetU16BE(m_pTables[nIndex].nOffset + nJ * 2, &m_bSuccess);

                if ( nPos < 0 || nPos > m_nLen )
                    m_bSuccess = false;
            }
            if ( !m_bSuccess )
                return;
        }

        // Читаем таблицу post
        ReadPostTable();
    }

    void CFontFileTrueType::ReadPostTable()
    {
        StringExt *seName;
        int nStringIndex, nStringPos;
        bool bSuccess = true;
        int nTemp, nJ, nCount = 0, nTempLen = 0;

        if ( ( nTemp = SeekTable("post" ) ) < 0 )
            return;

        int nTablePos = m_pTables[ nTemp ].nOffset;
        int nPostFormat = GetU32BE( nTablePos, &bSuccess );

        if (! bSuccess )
            goto error;

        if ( 0x00010000 == nPostFormat )
        {
            m_pNameToGID = new CHash(true);
            for ( nTemp = 0; nTemp < 258; ++nTemp )
            {
                m_pNameToGID->Add(new StringExt( c_arrAppleGlyphNames[ nTemp ]), nTemp);
            }
        }
        else if ( 0x00020000 == nPostFormat )
        {
            m_pNameToGID = new CHash(true);
            nCount = GetU16BE( nTablePos + 32, &bSuccess );
            if ( !bSuccess )
                goto error;
            if ( nCount > m_nGlyphs )
            {
                nCount = m_nGlyphs;
            }
            nStringIndex = 0;
            nStringPos = nTablePos + 34 + 2 * nCount;
            for ( nTemp = 0; nTemp < nCount; ++nTemp )
            {
                nJ = GetU16BE( nTablePos + 34 + 2 * nTemp, &bSuccess );
                if ( nJ < 258 )
                {
                    m_pNameToGID->RemoveInt( c_arrAppleGlyphNames[nJ]);
                    m_pNameToGID->Add(new StringExt( c_arrAppleGlyphNames[nJ]), nTemp );
                }
                else
                {
                    nJ -= 258;
                    if ( nJ != nStringIndex )
                    {
                        for ( nStringIndex = 0, nStringPos = nTablePos + 34 + 2 * nCount; nStringIndex < nJ; ++nStringIndex, nStringPos += 1 + GetU8( nStringPos, &bSuccess ) ) ;
                        if ( !bSuccess )
                            goto error;
                    }
                    nTempLen = GetU8( nStringPos, &bSuccess );
                    if ( !bSuccess || !CheckRegion( nStringPos + 1, nTempLen ) )
                        goto error;
                    seName = new StringExt((char *)&m_sFile[ nStringPos + 1 ], nTempLen );
                    m_pNameToGID->RemoveInt( seName );
                    m_pNameToGID->Add( seName, nTemp );
                    ++nStringIndex;
                    nStringPos += 1 + nTempLen;
                }
            }
        }
        else if ( 0x00028000 == nPostFormat )
        {
            m_pNameToGID = new CHash(true);
            for ( nTemp = 0; nTemp < m_nGlyphs; ++nTemp )
            {
                nJ = GetU8( nTablePos + 32 + nTemp, &bSuccess );
                if ( !bSuccess )
                    goto error;
                if ( nJ < 258 )
                {
                    m_pNameToGID->RemoveInt( c_arrAppleGlyphNames[nJ]);
                    m_pNameToGID->Add(new StringExt( c_arrAppleGlyphNames[nJ]), nTemp );
                }
            }
        }

        return;

    error:
        if (m_pNameToGID)
        {
        delete m_pNameToGID;
        m_pNameToGID = NULL;
        }
    }

    int CFontFileTrueType::SeekTable(char *sTag)
    {
        unsigned int nTagIndex = ( ( sTag[0] & 0xff ) << 24 ) | ( ( sTag[1] & 0xff ) << 16 ) | ( ( sTag[2] & 0xff ) << 8 ) | ( sTag[3] & 0xff );
        for ( int nIndex = 0; nIndex < m_nTablesCount; ++nIndex )
        {
            if ( m_pTables[nIndex].unTag == nTagIndex )
            {
                return nIndex;
            }
        }
        return -1;
    }
}
