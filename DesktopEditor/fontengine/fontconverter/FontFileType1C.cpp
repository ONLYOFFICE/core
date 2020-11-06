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
#include <math.h>
#include "MemoryUtils.h"
#include "StringExt.h"
#include "FontFileEncodings.h"
#include "FontFileType1C.h"
#include "FontFileTrueType.h"
#include "Utils.h"

namespace NSFontConverter
{
    #include FT_TRUETYPE_TABLES_H
    #include FT_ADVANCES_H

    static char c_sHexChars[17] = "0123456789ABCDEF";

    //------------------------------------------------------------------------
    // CFontFileType1C
    //------------------------------------------------------------------------

    CFontFileType1C *CFontFileType1C::LoadFromBuffer(char *sBuffer, int nLen)
    {
        CFontFileType1C *pT1CFF = new CFontFileType1C( sBuffer, nLen, false);
        if ( !pT1CFF->Parse() )
        {
            delete pT1CFF;
            return NULL;
        }
        return pT1CFF;
    }

    CFontFileType1C *CFontFileType1C::LoadFromFile(const wchar_t *wsFileName)
    {
        char *sBuffer;
        int nLen = 0;

        if ( !( sBuffer = CFontFileBase::ReadFile( wsFileName, &nLen ) ) )
            return NULL;

        CFontFileType1C *pT1CFF = new CFontFileType1C( sBuffer, nLen, true);

        if ( !pT1CFF->Parse() )
        {
            delete pT1CFF;
            return NULL;
        }
        return pT1CFF;
    }

    CFontFileType1C::CFontFileType1C(char *sBuffer, int nLen, bool bFreeData):
    CFontFileBase( sBuffer, nLen, bFreeData)
    {
        m_seName        = NULL;
        m_arrEncoding   = NULL;
        m_pPrivateDicts = NULL;
        m_pnFDSelect    = NULL;
        m_pnCharset     = NULL;
    }

    CFontFileType1C::~CFontFileType1C()
    {
        if ( m_seName )
            delete m_seName;

        if ( m_arrEncoding && m_arrEncoding != c_arrsFontFileType1StandardEncoding && m_arrEncoding != c_arrsFontFileType1ExpertEncoding )
        {
            for ( int nIndex = 0; nIndex < 256; ++nIndex )
            {
                MemUtilsFree( m_arrEncoding[ nIndex ] );
            }
            MemUtilsFree( m_arrEncoding );
        }

        if ( m_pPrivateDicts )
        {
            MemUtilsFree(m_pPrivateDicts);
        }
        if ( m_pnFDSelect )
        {
            MemUtilsFree(m_pnFDSelect);
        }

        if ( m_pnCharset && m_pnCharset != c_arrnFontFileType1CISOAdobeCharset && m_pnCharset != c_arrnFontFileType1CExpertCharset && m_pnCharset != c_arrnFontFileType1CExpertSubsetCharset )
        {
            MemUtilsFree(m_pnCharset);
        }
    }

    char *CFontFileType1C::GetName()
    {
        return m_seName ? m_seName->GetBuffer() : (char *)NULL;
    }

    char **CFontFileType1C::GetEncoding()
    {
        return m_arrEncoding;
    }

    unsigned short *CFontFileType1C::GetCIDToGIDMap(int *arrCIDs)
    {
        if ( m_oTopDict.nFirstOperator != 0x0c1e )
        {
            *arrCIDs = 0;
            return NULL;
        }

        int nCount = 0;
        for (int nIndex = 0; nIndex < m_nGlyphsCount; ++nIndex )
        {
            if ( m_pnCharset[nIndex] > nCount )
            {
                nCount = m_pnCharset[nIndex];
            }
        }
        ++nCount;
        unsigned short *pMap = (unsigned short *)MemUtilsMallocArray( nCount, sizeof(unsigned short));
        memset( pMap, 0, nCount * sizeof(unsigned short) );
        for (int nIndex = 0; nIndex < m_nGlyphsCount; ++nIndex )
        {
            pMap[m_pnCharset[nIndex]] = nIndex;
        }
        *arrCIDs = nCount;
        return pMap;
    }

    void CFontFileType1C::ToType1(char *sPSName, char **ppNewEncoding, bool bASKII, FontFileOutputFunc pOutputFunc, void *pOutputStream)
    {
        int nPSNameLen;
        StringExt *seBuffer;
        char sBuf[256];

        if ( sPSName )
        {
            nPSNameLen = strlen( sPSName );
        }
        else
        {
            sPSName    = m_seName->GetBuffer();
            nPSNameLen = m_seName->GetLength();
        }

        bool bSuccess = true;
        (*pOutputFunc)( pOutputStream, "%!FontType1-1.0: ", 17);
        (*pOutputFunc)( pOutputStream, sPSName, nPSNameLen);
        if ( m_oTopDict.nVersionSID != 0)
        {
            GetString(m_oTopDict.nVersionSID, sBuf, &bSuccess);
            (*pOutputFunc)( pOutputStream, sBuf, strlen( sBuf ));
        }
        (*pOutputFunc)( pOutputStream, "\n", 1);
        // Для словаря нужно место для 12 полей: следующие 9 +
        // Private и CharStrings (в eexec секции) и FID.
        (*pOutputFunc)( pOutputStream, "12 dict begin\n", 14);
        (*pOutputFunc)( pOutputStream, "/FontInfo 10 dict dup begin\n", 28);
        if ( m_oTopDict.nVersionSID != 0 )
        {
            (*pOutputFunc)( pOutputStream, "/version (", 10);
            (*pOutputFunc)( pOutputStream, sBuf, strlen(sBuf));
            (*pOutputFunc)( pOutputStream, ") readonly def\n", 15);
        }
        if ( m_oTopDict.nNoticeSID != 0 )
        {
            GetString(m_oTopDict.nNoticeSID, sBuf, &bSuccess);
            (*pOutputFunc)( pOutputStream, "/Notice (", 9);
            (*pOutputFunc)( pOutputStream, sBuf, strlen(sBuf));
            (*pOutputFunc)( pOutputStream, ") readonly def\n", 15);
        }
        if ( m_oTopDict.nCopyrightSID != 0)
        {
            GetString(m_oTopDict.nCopyrightSID, sBuf, &bSuccess);
            (*pOutputFunc)( pOutputStream, "/Copyright (", 12);
            (*pOutputFunc)( pOutputStream, sBuf, strlen(sBuf));
            (*pOutputFunc)( pOutputStream, ") readonly def\n", 15);
        }
        if ( m_oTopDict.nFullNameSID != 0 )
        {
            GetString(m_oTopDict.nFullNameSID, sBuf, &bSuccess);
            (*pOutputFunc)( pOutputStream, "/FullName (", 11);
            (*pOutputFunc)( pOutputStream, sBuf, strlen(sBuf));
            (*pOutputFunc)( pOutputStream, ") readonly def\n", 15);
        }
        if ( m_oTopDict.nFamilyNameSID != 0 )
        {
            GetString(m_oTopDict.nFamilyNameSID, sBuf, &bSuccess);
            (*pOutputFunc)( pOutputStream, "/FamilyName (", 13);
            (*pOutputFunc)( pOutputStream, sBuf, strlen(sBuf));
            (*pOutputFunc)( pOutputStream, ") readonly def\n", 15);
        }
        if ( m_oTopDict.nWeightSID != 0 )
        {
            GetString(m_oTopDict.nWeightSID, sBuf, &bSuccess);
            (*pOutputFunc)( pOutputStream, "/Weight (", 9);
            (*pOutputFunc)( pOutputStream, sBuf, strlen(sBuf));
            (*pOutputFunc)( pOutputStream, ") readonly def\n", 15);
        }
        if (m_oTopDict.nIsFixedPitch)
        {
            (*pOutputFunc)( pOutputStream, "/isFixedPitch true def\n", 23);
        }
        else
        {
            (*pOutputFunc)( pOutputStream, "/isFixedPitch false def\n", 24);
        }

        seBuffer = StringExt::Format("/ItalicAngle {0:.4g} def\n", m_oTopDict.dItalicAngle);
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        seBuffer = StringExt::Format("/UnderlinePosition {0:.4g} def\n", m_oTopDict.dUnderlinePosition);
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        seBuffer = StringExt::Format("/UnderlineThickness {0:.4g} def\n", m_oTopDict.dUnderlineThickness);
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        (*pOutputFunc)( pOutputStream, "end readonly def\n", 17);
        (*pOutputFunc)( pOutputStream, "/FontName /", 11);
        (*pOutputFunc)( pOutputStream, sPSName, nPSNameLen);
        (*pOutputFunc)( pOutputStream, " def\n", 5);

        seBuffer = StringExt::Format("/PaintType {0:d} def\n", m_oTopDict.nPaintType);
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        (*pOutputFunc)( pOutputStream, "/FontType 1 def\n", 16);
        seBuffer = StringExt::Format("/FontMatrix [{0:.8g} {1:.8g} {2:.8g} {3:.8g} {4:.8g} {5:.8g}] readonly def\n", m_oTopDict.arrdFontMatrix[0], m_oTopDict.arrdFontMatrix[1], m_oTopDict.arrdFontMatrix[2], m_oTopDict.arrdFontMatrix[3], m_oTopDict.arrdFontMatrix[4], m_oTopDict.arrdFontMatrix[5]);
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        seBuffer = StringExt::Format("/FontBBox [{0:.4g} {1:.4g} {2:.4g} {3:.4g}] readonly def\n", m_oTopDict.arrdFontBBox[0], m_oTopDict.arrdFontBBox[1], m_oTopDict.arrdFontBBox[2], m_oTopDict.arrdFontBBox[3]);
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        seBuffer = StringExt::Format("/StrokeWidth {0:.4g} def\n", m_oTopDict.dStrokeWidth);
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        if ( m_oTopDict.nUniqueID != 0 )
        {
            seBuffer = StringExt::Format("/UniqueID {0:d} def\n", m_oTopDict.nUniqueID);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
            delete seBuffer;
        }

        // Запись кодировки
        (*pOutputFunc)( pOutputStream, "/Encoding ", 10);
        if ( !ppNewEncoding && m_arrEncoding == c_arrsFontFileType1ExpertEncoding )
        {
            (*pOutputFunc)(  pOutputStream, "StandardEncoding def\n", 21);
        }
        else
        {
            (*pOutputFunc)( pOutputStream, "256 array\n", 10);
            (*pOutputFunc)( pOutputStream, "0 1 255 {1 index exch /.notdef put} for\n", 40);
            char **ppEnc = ppNewEncoding ? ppNewEncoding : m_arrEncoding;
            for (int nIndex = 0; nIndex < 256; ++nIndex )
            {
                if ( ppEnc[nIndex] )
                {
                    seBuffer = StringExt::Format("dup {0:d} /{1:s} put\n", nIndex, ppEnc[nIndex] );
                    (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                    delete seBuffer;
                }
            }
            (*pOutputFunc)( pOutputStream, "readonly def\n", 13);
        }
        (*pOutputFunc)( pOutputStream, "currentdict end\n", 16);

        // Бинарная секция (eexec)
        Type1CEexecBuf oEexecBuffer;
        (*pOutputFunc)( pOutputStream, "currentfile eexec\n", 18);
        oEexecBuffer.pOutputFunc   = pOutputFunc;
        oEexecBuffer.pOutputStream =  pOutputStream;
        oEexecBuffer.bASKII = bASKII;
        oEexecBuffer.unEncryptionKey = 55665;
        oEexecBuffer.nLine = 0;

        // Private Dictionary
        EexecWrite( &oEexecBuffer, "\x83\xca\x73\xd5");
        EexecWrite( &oEexecBuffer, "dup /Private 32 dict dup begin\n");
        EexecWrite( &oEexecBuffer, "/RD {string currentfile exch readstring pop} executeonly def\n");
        EexecWrite( &oEexecBuffer, "/ND {noaccess def} executeonly def\n");
        EexecWrite( &oEexecBuffer, "/NP {noaccess put} executeonly def\n");
        EexecWrite( &oEexecBuffer, "/MinFeature {16 16} def\n");
        EexecWrite( &oEexecBuffer, "/password 5839 def\n");

        if ( m_pPrivateDicts[0].nBlueValues )
        {
            EexecWrite( &oEexecBuffer, "/BlueValues [");
            for ( int nIndex = 0; nIndex < m_pPrivateDicts[0].nBlueValues; ++nIndex )
            {
                seBuffer = StringExt::Format("{0:s}{1:d}", nIndex > 0 ? " " : "", m_pPrivateDicts[0].arrnBlueValues[ nIndex ]);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            EexecWrite( &oEexecBuffer, "] def\n");
        }
        if ( m_pPrivateDicts[0].nOtherBlues )
        {
            EexecWrite( &oEexecBuffer, "/OtherBlues [");

            for ( int nIndex = 0; nIndex < m_pPrivateDicts[0].nOtherBlues; ++nIndex)
            {
                seBuffer = StringExt::Format("{0:s}{1:d}", nIndex > 0 ? " " : "", m_pPrivateDicts[0].arrnOtherBlues[nIndex]);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            EexecWrite( &oEexecBuffer, "] def\n");
        }
        if ( m_pPrivateDicts[0].nFamilyBlues )
        {
            EexecWrite( &oEexecBuffer, "/FamilyBlues [");
            for ( int nIndex = 0; nIndex < m_pPrivateDicts[0].nFamilyBlues; ++nIndex )
            {
                seBuffer = StringExt::Format("{0:s}{1:d}", nIndex > 0 ? " " : "", m_pPrivateDicts[0].arrnFamilyBlues[nIndex]);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            EexecWrite( &oEexecBuffer, "] def\n");
        }

        if (m_pPrivateDicts[0].nFamilyOtherBlues)
        {
            EexecWrite( &oEexecBuffer, "/FamilyOtherBlues [");
            for ( int nIndex = 0; nIndex < m_pPrivateDicts[0].nFamilyOtherBlues; ++nIndex )
            {
                seBuffer = StringExt::Format("{0:s}{1:d}", nIndex > 0 ? " " : "", m_pPrivateDicts[0].arrnFamilyOtherBlues[nIndex]);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer() );
                delete seBuffer;
            }
            EexecWrite( &oEexecBuffer, "] def\n");
        }
        if ( m_pPrivateDicts[0].dBlueScale != 0.039625 )
        {
            seBuffer = StringExt::Format("/BlueScale {0:.4g} def\n", m_pPrivateDicts[0].dBlueScale);
            EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
            delete seBuffer;
        }
        if ( m_pPrivateDicts[0].nBlueShift != 7 )
        {
            seBuffer = StringExt::Format("/BlueShift {0:d} def\n", m_pPrivateDicts[0].nBlueShift);
            EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
            delete seBuffer;
        }
        if ( m_pPrivateDicts[0].nBlueFuzz != 1 )
        {
            seBuffer = StringExt::Format("/BlueFuzz {0:d} def\n", m_pPrivateDicts[0].nBlueFuzz);
            EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
            delete seBuffer;
        }
        if ( m_pPrivateDicts[0].bHasStdHW )
        {
            seBuffer = StringExt::Format("/StdHW [{0:.4g}] def\n", m_pPrivateDicts[0].dStdHW);
            EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
            delete seBuffer;
        }
        if ( m_pPrivateDicts[0].bHasStdVW )
        {
            seBuffer = StringExt::Format("/StdVW [{0:.4g}] def\n", m_pPrivateDicts[0].dStdVW);
            EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
            delete seBuffer;
        }
        if ( m_pPrivateDicts[0].nStemSnapH )
        {
            EexecWrite( &oEexecBuffer, "/StemSnapH [");
            for ( int nIndex = 0; nIndex < m_pPrivateDicts[0].nStemSnapH; ++nIndex )
            {
                seBuffer = StringExt::Format("{0:s}{1:.4g}", nIndex > 0 ? " " : "", m_pPrivateDicts[0].arrdStemSnapH[nIndex]);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            EexecWrite( &oEexecBuffer, "] def\n");
        }
        if ( m_pPrivateDicts[0].nStemSnapV )
        {
            EexecWrite( &oEexecBuffer, "/StemSnapV [");
            for ( int nIndex = 0; nIndex < m_pPrivateDicts[0].nStemSnapV; ++nIndex )
            {
                seBuffer = StringExt::Format("{0:s}{1:.4g}", nIndex > 0 ? " " : "", m_pPrivateDicts[0].arrdStemSnapV[nIndex]);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            EexecWrite( &oEexecBuffer, "] def\n");
        }
        if ( m_pPrivateDicts[0].bHasForceBold )
        {
            seBuffer = StringExt::Format("/ForceBold {0:s} def\n", m_pPrivateDicts[0].bForceBold ? "true" : "false");
            EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
            delete seBuffer;
        }
        if ( m_pPrivateDicts[0].dForceBoldThreshold != 0 )
        {
            seBuffer = StringExt::Format("/ForceBoldThreshold {0:.4g} def\n", m_pPrivateDicts[0].dForceBoldThreshold);
            EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
            delete seBuffer;
        }
        if ( m_pPrivateDicts[0].nLanguageGroup != 0 )
        {
            seBuffer = StringExt::Format("/LanguageGroup {0:d} def\n", m_pPrivateDicts[0].nLanguageGroup);
            EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
            delete seBuffer;
        }
        if ( m_pPrivateDicts[0].dExpansionFactor != 0.06 )
        {
            seBuffer = StringExt::Format("/ExpansionFactor {0:.4g} def\n", m_pPrivateDicts[0].dExpansionFactor);
            EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
            delete seBuffer;
        }

        bSuccess = true;
        Type1CIndex oSubrIndex;
        GetIndex( m_pPrivateDicts[0].nSubrsOffset, &oSubrIndex, &bSuccess);
        if ( !bSuccess)
        {
            oSubrIndex.nPos = -1;
        }

        // CharStrings
        seBuffer = StringExt::Format("2 index /CharStrings {0:d} dict dup begin\n", m_nGlyphsCount);
        EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
        delete seBuffer;
        Type1CIndexVal oIndexVal;
        for (int nIndex = 0; nIndex < m_nGlyphsCount; ++nIndex )
        {
            bSuccess = true;
            GetIndexVal( &m_oCharStringsIndex, nIndex, &oIndexVal, &bSuccess);
            if (bSuccess)
            {
                GetString( m_pnCharset[nIndex], sBuf, &bSuccess);
                if (bSuccess)
                {
                    EexecConvertGlyph( &oEexecBuffer, sBuf, oIndexVal.nPos, oIndexVal.nLen, &oSubrIndex, &m_pPrivateDicts[0]);
                }
            }
        }
        EexecWrite( &oEexecBuffer, "end\n");
        EexecWrite( &oEexecBuffer, "end\n");
        EexecWrite( &oEexecBuffer, "readonly put\n");
        EexecWrite( &oEexecBuffer, "noaccess put\n");
        EexecWrite( &oEexecBuffer, "dup /FontName get exch definefont pop\n");
        EexecWrite( &oEexecBuffer, "mark currentfile closefile\n");

        // Trailer
        if ( bASKII && oEexecBuffer.nLine > 0 )
        {
            (*pOutputFunc)( pOutputStream, "\n", 1);
        }
        for ( int nIndex = 0; nIndex < 8; ++nIndex )
        {
            (*pOutputFunc)( pOutputStream, "0000000000000000000000000000000000000000000000000000000000000000\n", 65);
        }
        (*pOutputFunc)( pOutputStream, "cleartomark\n", 12);
    }

    void CFontFileType1C::ToOpenTypeCFF(FontFileOutputFunc pOutputFunc, void *pOutputStream, FT_Face pFace)
    {
        // CFF Table
        TCharBuffer oCFFTable;
        oCFFTable.Write( (char*)m_sFile, m_nLen );

        // Os2 Table
        TCharBuffer oOS2Table;
        oOS2Table.Write( "\x00\x03", 2 ); // version
        oOS2Table.Write( "\x02\x24", 2 ); // xAvgCharWidth
        oOS2Table.Write( "\x01\xF4", 2 ); // usWeightClass
        oOS2Table.Write( "\x00\x05", 2 ); // usWidthClass
        oOS2Table.Write( "\x00\x00", 2 ); // fsType
        oOS2Table.Write( "\x02\x8A", 2 ); // ySubscriptXSize
        oOS2Table.Write( "\x02\xBB", 2 ); // ySubscriptYSize
        oOS2Table.Write( "\x00\x00", 2 ); // ySubscriptXOffset
        oOS2Table.Write( "\x00\x8C", 2 ); // ySubscriptYOffset
        oOS2Table.Write( "\x02\x8A", 2 ); // ySuperscriptXSize
        oOS2Table.Write( "\x02\xBB", 2 ); // ySuperscriptYSize
        oOS2Table.Write( "\x00\x00", 2 ); // ySuperscriptXOffset
        oOS2Table.Write( "\x01\xDF", 2 ); // ySuperscriptYOffset
        oOS2Table.Write( "\x00\x31", 2 ); // yStrikeoutSize
        oOS2Table.Write( "\x01\x02", 2 ); // yStrikeoutPosition
        oOS2Table.Write( "\x00\x00", 2 ); // sFamilyClass
        oOS2Table.Write( "\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00", 10 ); // panose
        oOS2Table.Write( "\x00\x00\x00\x00", 4 ); // ulUnicodeRange1
        oOS2Table.Write( "\x00\x00\x00\x00", 4 ); // ulUnicodeRange2
        oOS2Table.Write( "\x00\x00\x00\x00", 4 ); // ulUnicodeRange3
        oOS2Table.Write( "\x00\x00\x00\x00", 4 ); // ulUnicodeRange4
        oOS2Table.Write( "\x2A\x32\x31\x2A", 4 ); // achVendID
        oOS2Table.Write( "\x00\x00", 2 ); // fsSelection
        oOS2Table.Write( "\x00\x00", 2 ); // usFirstCharIndex
        oOS2Table.Write( "\x00\x00", 2 ); // usLastCharIndex
        oOS2Table.Write16( pFace->ascender ); // sTypoAscender
        oOS2Table.Write16( pFace->descender); // sTypoDescender
        oOS2Table.Write( "\x00\x64", 2 ); // sTypoLineGap
        oOS2Table.Write( "\x00\x00", 2 ); // usWinAscent
        oOS2Table.Write( "\x00\x00", 2 ); // usWinDescent
        oOS2Table.Write( "\x00\x00\x00\x00", 4 ); // ulCodePageRange1
        oOS2Table.Write( "\x00\x00\x00\x00", 4 ); // ulCodePageRange2
        oOS2Table.Write( "\x00\x00", 2 ); // sxHeight
        oOS2Table.Write( "\x00\x00", 2 ); // sCapHeight
        oOS2Table.Write( "\x00\x00", 2 ); // usDefaultChar
        oOS2Table.Write( "\x00\x00", 2 ); // usBreakChar
        oOS2Table.Write( "\x00\x03", 2 ); // usMaxContext

        /*
        // Cmap Table
        int nNumCmapTables = 1;
        int nStartCmapTable = 4 + nNumCmapTables * 8; // заголовок Cmap + заголовок подтаблицы

        TCharBuffer oCmapTable;
        oCmapTable.Write( "\x00\x00", 2 );     // version
        oCmapTable.Write16( nNumCmapTables );  // numTables
        oCmapTable.Write( "\x00\x03", 2 );     // platformID
        oCmapTable.Write( "\x00\x01", 2 );     // encodingID
        oCmapTable.Write32( nStartCmapTable ); // Byte offset from beginning of table to the subtable for this encoding.

        for ( int nIndex = 0; nIndex < 45; nIndex++ )
        {
            arrCmapTable[nIndex * 2 + 0] = 0x00;
            arrCmapTable[nIndex * 2 + 1] = nIndex;
        }
        */


        // Head Table
        TCharBuffer oHeadTable;
        oHeadTable.Write( "\x00\x01\x00\x00", 4 ); // Version number
        oHeadTable.Write( "\x00\x00\x10\x00", 4 );	// fontRevision
        oHeadTable.Write( "\x00\x00\x00\x00", 4 );	// checksumAdjustement
        oHeadTable.Write( "\x5F\x0F\x3C\xF5", 4 ); // magicNumber
        oHeadTable.Write( "\x00\x00", 2 ); // flags
        oHeadTable.Write( "\x03\xE8", 2 ); // unitsPerEM (defaulting to 1000)
        oHeadTable.Write( "\x00\x00\x00\x00\x9e\x0b\x7e\x27", 8 ); // creation date
        oHeadTable.Write( "\x00\x00\x00\x00\x9e\x0b\x7e\x27", 8 ); // modifification date
        oHeadTable.Write16( (int)m_oTopDict.arrdFontBBox[0] ); // xMin
        oHeadTable.Write16( (int)m_oTopDict.arrdFontBBox[1] ); // yMin
        oHeadTable.Write16( (int)m_oTopDict.arrdFontBBox[2] ); // xMax
        oHeadTable.Write16( (int)m_oTopDict.arrdFontBBox[3] ); // yMax
        oHeadTable.Write( "\x00\x00", 2 ); // macStyle
        oHeadTable.Write( "\x00\x11", 2 ); // lowestRecPPEM
        oHeadTable.Write( "\x00\x00", 2 ); // fontDirectionHint
        oHeadTable.Write( "\x00\x00", 2 ); // indexToLocFormat
        oHeadTable.Write( "\x00\x00", 2 ); // glyphDataFormat

        // Hheaa Table
        TCharBuffer oHheaTable;
        oHheaTable.Write( "\x00\x01\x00\x00", 4 ); // Version number
        oHheaTable.Write16( pFace->ascender ); // Typographic Ascent
        oHheaTable.Write16( pFace->descender); //Typographic Descent
        oHheaTable.Write( "\x00\x00", 2 ); // Line Gap
        oHheaTable.Write( "\xFF\xFF", 2 ); // advanceWidthMax
        oHheaTable.Write( "\x00\x00", 2 ); // minLeftSidebearing
        oHheaTable.Write( "\x00\x00", 2 ); // minRightSidebearing
        oHheaTable.Write( "\x00\x00", 2 ); // xMaxExtent
        oHheaTable.Write( "\x00\x00", 2 ); // caretSlopeRise
        oHheaTable.Write( "\x00\x00", 2 ); // caretSlopeRun
        oHheaTable.Write( "\x00\x00", 2 ); // caretOffset
        oHheaTable.Write( "\x00\x00", 2 ); // -reserved-
        oHheaTable.Write( "\x00\x00", 2 ); // -reserved-
        oHheaTable.Write( "\x00\x00", 2 ); // -reserved-
        oHheaTable.Write( "\x00\x00", 2 ); // -reserved-
        oHheaTable.Write( "\x00\x00", 2 ); // metricDataFormat
        oHheaTable.Write16( pFace->num_glyphs + 1 ); // Number of HMetrics


        // Hmtx Table
        TCharBuffer oHmtxTable;
        oHmtxTable.Write( "\x00\x00\x00\x00", 4 ); // .notdef

        for ( int nIndex = 1; nIndex <= pFace->num_glyphs; nIndex++ )
        {
            FT_Load_Glyph( pFace, nIndex - 1, FT_LOAD_LINEAR_DESIGN | FT_ADVANCE_FLAG_FAST_ONLY );
            FT_Fixed fAdvance = pFace->glyph->linearHoriAdvance;

            oHmtxTable.Write16( fAdvance );
            oHmtxTable.Write16( 0 );
        }

        // Maxp Table
        TCharBuffer oMaxpTable;
        oMaxpTable.Write( "\x00\x00\x50\x00", 4 );   // Version number
        oMaxpTable.Write16( pFace->num_glyphs + 1 ); // Num of glyphs

        // Name Table
        TCharBuffer oNameTable;

        // Пока таблицу с именами сделаем пустой
        oNameTable.Write( "\x00\x00", 2 ); // format
        oNameTable.Write( "\x00\x00", 2 ); // number of name records
        oNameTable.Write( "\x00\x06", 2 ); // offset to start of string storage
        oNameTable.Write( "\x00\x00", 2 ); // pad to multiple of four bytes

        // Post Table
        TCharBuffer oPostTable;
        oPostTable.Write( "\x00\x03\x00\x00", 4 ); // format
        oPostTable.Write( "\x00\x00\x00\x00", 4 ); // italic angle
        oPostTable.Write( "\x00\x00", 2 );         // underline position
        oPostTable.Write( "\x00\x00", 2 );         // underline thickness
        oPostTable.Write( "\x00\x00\x00\x00", 4 ); // fixed pitch
        oPostTable.Write( "\x00\x00\x00\x00", 4 ); // min Type 42 memory
        oPostTable.Write( "\x00\x00\x00\x00", 4 ); // max Type 42 memory
        oPostTable.Write( "\x00\x00\x00\x00", 4 ); // min Type 1 memory
        oPostTable.Write( "\x00\x00\x00\x00", 4 ); // max Type 1 memory

        // Создаем каталог с описанием всех таблиц:
        // - сортируем талицы по тэгу
        // - вычисляем позиции таблиц, учитывая 4-byte alignment
        // - вычисляем чексуммы таблиц

        int nRequiredTablesCount = 8;
        TrueTypeTable *pTables = (TrueTypeTable *)MemUtilsMallocArray( nRequiredTablesCount, sizeof(TrueTypeTable) );

        int nTableIndex = 0;

        pTables[nTableIndex].unTag      = cffTag;
        pTables[nTableIndex].unChecksum = ComputeTTTableChecksum((unsigned char *)oCFFTable.sBuffer, oCFFTable.nLen );
        pTables[nTableIndex].nLen       = oCFFTable.nLen;
        nTableIndex++;

        pTables[nTableIndex].unTag      = os2Tag;
        pTables[nTableIndex].unChecksum = ComputeTTTableChecksum((unsigned char *)oOS2Table.sBuffer, oOS2Table.nLen );
        pTables[nTableIndex].nLen       = oOS2Table.nLen;
        nTableIndex++;

        //pTables[nTableIndex].unTag      = cmapTag;
        //pTables[nTableIndex].unChecksum = ComputeTTTableChecksum((unsigned char *)oCmapTable.sBuffer, oCmapTable.nLen );
        //pTables[nTableIndex].nLen       = oCmapTable.nLen;
        //nTableIndex++;

        pTables[nTableIndex].unTag      = headTag;
        pTables[nTableIndex].unChecksum = ComputeTTTableChecksum((unsigned char *)oHeadTable.sBuffer, oHeadTable.nLen );
        pTables[nTableIndex].nLen       = oHeadTable.nLen;
        nTableIndex++;

        pTables[nTableIndex].unTag      = hheaTag;
        pTables[nTableIndex].unChecksum = ComputeTTTableChecksum((unsigned char *)oHheaTable.sBuffer, oHheaTable.nLen );
        pTables[nTableIndex].nLen       = oHheaTable.nLen;
        nTableIndex++;

        pTables[nTableIndex].unTag      = hmtxTag;
        pTables[nTableIndex].unChecksum = ComputeTTTableChecksum((unsigned char *)oHmtxTable.sBuffer, oHmtxTable.nLen );
        pTables[nTableIndex].nLen       = oHmtxTable.nLen;
        nTableIndex++;

        pTables[nTableIndex].unTag      = maxpTag;
        pTables[nTableIndex].unChecksum = ComputeTTTableChecksum((unsigned char *)oMaxpTable.sBuffer, oMaxpTable.nLen );
        pTables[nTableIndex].nLen       = oMaxpTable.nLen;
        nTableIndex++;

        pTables[nTableIndex].unTag      = nameTag;
        pTables[nTableIndex].unChecksum = ComputeTTTableChecksum((unsigned char *)oNameTable.sBuffer, oNameTable.nLen );
        pTables[nTableIndex].nLen       = oNameTable.nLen;
        nTableIndex++;

        pTables[nTableIndex].unTag      = postTag;
        pTables[nTableIndex].unChecksum = ComputeTTTableChecksum((unsigned char *)oPostTable.sBuffer, oPostTable.nLen );
        pTables[nTableIndex].nLen       = oPostTable.nLen;
        nTableIndex++;

        qsort( pTables, nRequiredTablesCount, sizeof(TrueTypeTable), CompareTrueTypeTableTag );
        int nPos = 12 + nRequiredTablesCount * 16;
        for (int nIndex = 0; nIndex < nRequiredTablesCount; ++nIndex )
        {
            pTables[nIndex].nOffset = nPos;
            nPos += pTables[nIndex].nLen;
            if ( nPos & 3)
            {
                nPos += 4 - (nPos & 3);
            }
        }

        // Записываем каталог таблиц
        TCharBuffer oTableDir; // 12 + nReqTabCount * 16
        oTableDir.Write( "\x4F\x54\x54\x4F", 4 ); // sfnt version
        oTableDir.Write16( nRequiredTablesCount); // numTables

        int nI;
        unsigned int unT;
        for ( nI = -1, unT = (unsigned int)nRequiredTablesCount; unT; ++nI, unT >>= 1 );
        unT = 1 << (4 + nI);

        oTableDir.Write16( unT );		// searchRange
        oTableDir.Write16( nI );		// entrySelector

        unT = nRequiredTablesCount * 16 - unT;
        oTableDir.Write16( unT );		// rangeShift

        for ( int nIndex = 0; nIndex < nRequiredTablesCount; ++nIndex )
        {
            oTableDir.Write32( pTables[nIndex].unTag      );
            oTableDir.Write32( pTables[nIndex].unChecksum );
            oTableDir.Write32( pTables[nIndex].nOffset    );
            oTableDir.Write32( pTables[nIndex].nLen       );
        }
        (*pOutputFunc)( pOutputStream, oTableDir.sBuffer, oTableDir.nLen );

        // Вычислим чексумму файла
        int nFileChecksum = ComputeTTTableChecksum( (unsigned char *)oTableDir.sBuffer, oTableDir.nLen );
        for ( int nIndex = 0; nIndex < nRequiredTablesCount; ++nIndex )
        {
            nFileChecksum += pTables[nIndex].unChecksum;
        }
        nFileChecksum = 0xb1b0afba - nFileChecksum;

        // Записываем таблицы
        for ( int nIndex = 0; nIndex < nRequiredTablesCount; ++nIndex )
        {
            switch( pTables[nIndex].unTag )
            {
            case  cffTag: (*pOutputFunc)( pOutputStream, oCFFTable.sBuffer,  pTables[nIndex].nLen ); break;
            case  os2Tag: (*pOutputFunc)( pOutputStream, oOS2Table.sBuffer,  pTables[nIndex].nLen ); break;
            //case cmapTag: (*pOutputFunc)( pOutputStream, oCmapTable.sBuffer, pTables[nIndex].nLen ); break;
            case headTag: (*pOutputFunc)( pOutputStream, oHeadTable.sBuffer, pTables[nIndex].nLen ); break;
            case hheaTag: (*pOutputFunc)( pOutputStream, oHheaTable.sBuffer, pTables[nIndex].nLen ); break;
            case hmtxTag: (*pOutputFunc)( pOutputStream, oHmtxTable.sBuffer, pTables[nIndex].nLen ); break;
            case maxpTag: (*pOutputFunc)( pOutputStream, oMaxpTable.sBuffer, pTables[nIndex].nLen ); break;
            case nameTag: (*pOutputFunc)( pOutputStream, oNameTable.sBuffer, pTables[nIndex].nLen ); break;
            case postTag: (*pOutputFunc)( pOutputStream, oPostTable.sBuffer, pTables[nIndex].nLen ); break;
            }

            if ( pTables[nIndex].nLen & 3 )
            {
                (*pOutputFunc)( pOutputStream, "\0\0\0", 4 - ( pTables[nIndex].nLen & 3) );
            }
        }

        delete pTables;
    }

    void CFontFileType1C::ToCIDType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream)
    {
        StringExt *charStrings;
        int *charStringOffsets;
        StringExt *seBuffer;
        char sBuf[256];
        bool bSuccess = true;
        int nGID = 0, i, j, k;

        // Вычислим количество CID и построим отображение CID->GID
        int nCIDsCount = 0;
        for ( i = 0; i < m_nGlyphsCount; ++i )
        {
            if (m_pnCharset[i] >= nCIDsCount)
            {
                nCIDsCount = m_pnCharset[i] + 1;
            }
        }
        int *arrCIDdMap = (int *)MemUtilsMallocArray( nCIDsCount, sizeof(int));
        for ( i = 0; i < nCIDsCount; ++i )
        {
            arrCIDdMap[i] = -1;
        }
        for (i = 0; i < m_nGlyphsCount; ++i)
        {
            arrCIDdMap[m_pnCharset[i]] = i;
        }

        // Build Charstrings
        charStrings = new StringExt();
        charStringOffsets = (int *)MemUtilsMallocArray( nCIDsCount + 1, sizeof(int));
        for ( i = 0; i < nCIDsCount; ++i )
        {
            charStringOffsets[i] = charStrings->GetLength();
            if ( ( nGID = arrCIDdMap[i] ) >= 0 )
            {
                Type1CIndexVal oIndexVal;
                bSuccess = true;
                GetIndexVal( &m_oCharStringsIndex, nGID, &oIndexVal, &bSuccess);
                if ( bSuccess )
                {
                    Type1CIndex oSubrIndex;
                    GetIndex( m_pPrivateDicts[m_pnFDSelect[ nGID ]].nSubrsOffset, &oSubrIndex, &bSuccess);
                    if ( !bSuccess )
                    {
                        oSubrIndex.nPos = -1;
                    }
                    ConvertGlyph( oIndexVal.nPos, oIndexVal.nLen, charStrings, &oSubrIndex, &m_pPrivateDicts[m_pnFDSelect[nGID]], true);
                }
            }
        }
        charStringOffsets[ nCIDsCount ] = charStrings->GetLength();

        // Вычислим nNeedBytes = количество байт, необходимых для charstring offsets
        int nNeedBytes = 0;
        i = ( nCIDsCount + 1) * 5 + charStrings->GetLength();
        if (i < 0x100)
        {
            nNeedBytes = 1;
        }
        else if (i < 0x10000)
        {
            nNeedBytes = 2;
        }
        else if (i < 0x1000000)
        {
            nNeedBytes = 3;
        }
        else
        {
            nNeedBytes = 4;
        }

        // Начинаем запись Font Dictionary
        (*pOutputFunc)( pOutputStream, "/CIDInit /ProcSet findresource begin\n", 37);
        (*pOutputFunc)( pOutputStream, "20 dict begin\n", 14);
        (*pOutputFunc)( pOutputStream, "/CIDFontName /", 14);
        (*pOutputFunc)( pOutputStream, sPSName, strlen( sPSName ));
        (*pOutputFunc)( pOutputStream, " def\n", 5);
        (*pOutputFunc)( pOutputStream, "/CIDFontType 0 def\n", 19);
        (*pOutputFunc)( pOutputStream, "/CIDSystemInfo 3 dict dup begin\n", 32);

        if (m_oTopDict.nRegistrySID > 0 && m_oTopDict.nOrderingSID > 0)
        {
            bSuccess = true;
            GetString(m_oTopDict.nRegistrySID, sBuf, &bSuccess);
            if ( bSuccess )
            {
                (*pOutputFunc)( pOutputStream, "  /Registry (", 13);
                (*pOutputFunc)( pOutputStream, sBuf, strlen(sBuf));
                (*pOutputFunc)( pOutputStream, ") def\n", 6);
            }
            bSuccess = true;
            GetString(m_oTopDict.nOrderingSID, sBuf, &bSuccess);
            if ( bSuccess )
            {
                (*pOutputFunc)( pOutputStream, "  /Ordering (", 13);
                (*pOutputFunc)( pOutputStream, sBuf, strlen(sBuf));
                (*pOutputFunc)( pOutputStream, ") def\n", 6);
            }
        }
        else
        {
            (*pOutputFunc)( pOutputStream, "  /Registry (Adobe) def\n", 24);
            (*pOutputFunc)( pOutputStream, "  /Ordering (Identity) def\n", 27);
        }
        seBuffer = StringExt::Format("  /Supplement {0:d} def\n", m_oTopDict.nSupplement);
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        (*pOutputFunc)( pOutputStream, "end def\n", 8);

        if (m_oTopDict.bHasFontMatrix)
        {
            seBuffer = StringExt::Format("/FontMatrix [{0:.8g} {1:.8g} {2:.8g} {3:.8g} {4:.8g} {5:.8g}] def\n", m_oTopDict.arrdFontMatrix[0], m_oTopDict.arrdFontMatrix[1], m_oTopDict.arrdFontMatrix[2], m_oTopDict.arrdFontMatrix[3], m_oTopDict.arrdFontMatrix[4], m_oTopDict.arrdFontMatrix[5]);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
            delete seBuffer;
        }
        else if (m_pPrivateDicts[0].bHasFontMatrix)
        {
            (*pOutputFunc)( pOutputStream, "/FontMatrix [1 0 0 1 0 0] def\n", 30);
        }
        else
        {
            (*pOutputFunc)( pOutputStream, "/FontMatrix [0.001 0 0 0.001 0 0] def\n", 38);
        }
        seBuffer = StringExt::Format("/FontBBox [{0:.4g} {1:.4g} {2:.4g} {3:.4g}] def\n", m_oTopDict.arrdFontBBox[0], m_oTopDict.arrdFontBBox[1], m_oTopDict.arrdFontBBox[2], m_oTopDict.arrdFontBBox[3]);
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        (*pOutputFunc)( pOutputStream, "/FontInfo 1 dict dup begin\n", 27);
        (*pOutputFunc)( pOutputStream, "  /FSType 8 def\n", 16);
        (*pOutputFunc)( pOutputStream, "end def\n", 8);

        // CIDFont-specific
        seBuffer = StringExt::Format("/CIDCount {0:d} def\n", nCIDsCount);
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        (*pOutputFunc)( pOutputStream, "/FDBytes 1 def\n", 15);
        seBuffer = StringExt::Format("/GDBytes {0:d} def\n", nNeedBytes);
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        (*pOutputFunc)( pOutputStream, "/CIDMapOffset 0 def\n", 20);

        if (m_oTopDict.nPaintType != 0)
        {
            seBuffer = StringExt::Format("/PaintType {0:d} def\n", m_oTopDict.nPaintType);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
            delete seBuffer;

            seBuffer = StringExt::Format("/StrokeWidth {0:.4g} def\n", m_oTopDict.dStrokeWidth);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
            delete seBuffer;
        }

        // FDArray
        seBuffer = StringExt::Format("/FDArray {0:d} array\n", m_nFDsCount);
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        for (i = 0; i < m_nFDsCount; ++i)
        {
            seBuffer = StringExt::Format("dup {0:d} 10 dict begin\n", i);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
            delete seBuffer;

            (*pOutputFunc)( pOutputStream, "/FontType 1 def\n", 16);

            if (m_pPrivateDicts[i].bHasFontMatrix)
            {
                seBuffer = StringExt::Format("/FontMatrix [{0:.8g} {1:.8g} {2:.8g} {3:.8g} {4:.8g} {5:.8g}] def\n", m_pPrivateDicts[i].arrdFontMatrix[0], m_pPrivateDicts[i].arrdFontMatrix[1], m_pPrivateDicts[i].arrdFontMatrix[2], m_pPrivateDicts[i].arrdFontMatrix[3], m_pPrivateDicts[i].arrdFontMatrix[4], m_pPrivateDicts[i].arrdFontMatrix[5]);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            else
            {
                (*pOutputFunc)( pOutputStream, "/FontMatrix [1 0 0 1 0 0] def\n", 30);
            }
            seBuffer = StringExt::Format("/PaintType {0:d} def\n", m_oTopDict.nPaintType);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
            delete seBuffer;

            (*pOutputFunc)( pOutputStream, "/Private 32 dict begin\n", 23);
            if (m_pPrivateDicts[i].nBlueValues)
            {
                (*pOutputFunc)( pOutputStream, "/BlueValues [", 13);
                for (j = 0; j < m_pPrivateDicts[i].nBlueValues; ++j)
                {
                    seBuffer = StringExt::Format("{0:s}{1:d}", j > 0 ? " " : "", m_pPrivateDicts[i].arrnBlueValues[j]);
                    (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                    delete seBuffer;
                }
                (*pOutputFunc)( pOutputStream, "] def\n", 6);
            }
            if (m_pPrivateDicts[i].nOtherBlues)
            {
                (*pOutputFunc)( pOutputStream, "/OtherBlues [", 13);
                for (j = 0; j < m_pPrivateDicts[i].nOtherBlues; ++j)
                {
                    seBuffer = StringExt::Format("{0:s}{1:d}", j > 0 ? " " : "", m_pPrivateDicts[i].arrnOtherBlues[j]);
                    (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                    delete seBuffer;
                }
                (*pOutputFunc)( pOutputStream, "] def\n", 6);
            }
            if (m_pPrivateDicts[i].nFamilyBlues)
            {
                (*pOutputFunc)( pOutputStream, "/FamilyBlues [", 14);
                for (j = 0; j < m_pPrivateDicts[i].nFamilyBlues; ++j)
                {
                    seBuffer = StringExt::Format("{0:s}{1:d}", j > 0 ? " " : "", m_pPrivateDicts[i].arrnFamilyBlues[j]);
                    (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                    delete seBuffer;
                }
                (*pOutputFunc)( pOutputStream, "] def\n", 6);
            }
            if (m_pPrivateDicts[i].nFamilyOtherBlues)
            {
                (*pOutputFunc)( pOutputStream, "/FamilyOtherBlues [", 19);
                for (j = 0; j < m_pPrivateDicts[i].nFamilyOtherBlues; ++j)
                {
                    seBuffer = StringExt::Format("{0:s}{1:d}", j > 0 ? " " : "", m_pPrivateDicts[i].arrnFamilyOtherBlues[j]);
                    (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                    delete seBuffer;
                }
                (*pOutputFunc)( pOutputStream, "] def\n", 6);
            }
            if (m_pPrivateDicts[i].dBlueScale != 0.039625)
            {
                seBuffer = StringExt::Format("/BlueScale {0:.4g} def\n",	m_pPrivateDicts[i].dBlueScale);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            if (m_pPrivateDicts[i].nBlueShift != 7)
            {
                seBuffer = StringExt::Format("/BlueShift {0:d} def\n", m_pPrivateDicts[i].nBlueShift);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            if (m_pPrivateDicts[i].nBlueFuzz != 1)
            {
                seBuffer = StringExt::Format("/BlueFuzz {0:d} def\n", m_pPrivateDicts[i].nBlueFuzz);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            if (m_pPrivateDicts[i].bHasStdHW)
            {
                seBuffer = StringExt::Format("/StdHW [{0:.4g}] def\n", m_pPrivateDicts[i].dStdHW);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            if (m_pPrivateDicts[i].bHasStdVW)
            {
                seBuffer = StringExt::Format("/StdVW [{0:.4g}] def\n", m_pPrivateDicts[i].dStdVW);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            if (m_pPrivateDicts[i].nStemSnapH)
            {
                (*pOutputFunc)( pOutputStream, "/StemSnapH [", 12);
                for (j = 0; j < m_pPrivateDicts[i].nStemSnapH; ++j)
                {
                    seBuffer = StringExt::Format("{0:s}{1:.4g}", j > 0 ? " " : "", m_pPrivateDicts[i].arrdStemSnapH[j]);
                    (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                    delete seBuffer;
                }
                (*pOutputFunc)( pOutputStream, "] def\n", 6);
            }
            if (m_pPrivateDicts[i].nStemSnapV)
            {
                (*pOutputFunc)( pOutputStream, "/StemSnapV [", 12);
                for (j = 0; j < m_pPrivateDicts[i].nStemSnapV; ++j)
                {
                    seBuffer = StringExt::Format("{0:s}{1:.4g}", j > 0 ? " " : "", m_pPrivateDicts[i].arrdStemSnapV[j]);
                    (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                    delete seBuffer;
                }
                (*pOutputFunc)( pOutputStream, "] def\n", 6);
            }
            if (m_pPrivateDicts[i].bHasForceBold)
            {
                seBuffer = StringExt::Format("/ForceBold {0:s} def\n", m_pPrivateDicts[i].bForceBold ? "true" : "false");
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            if (m_pPrivateDicts[i].dForceBoldThreshold != 0)
            {
                seBuffer = StringExt::Format("/ForceBoldThreshold {0:.4g} def\n", m_pPrivateDicts[i].dForceBoldThreshold);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            if (m_pPrivateDicts[i].nLanguageGroup != 0)
            {
                seBuffer = StringExt::Format("/LanguageGroup {0:d} def\n", m_pPrivateDicts[i].nLanguageGroup);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            if (m_pPrivateDicts[i].dExpansionFactor != 0.06)
            {
                seBuffer = StringExt::Format("/ExpansionFactor {0:.4g} def\n", m_pPrivateDicts[i].dExpansionFactor);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            (*pOutputFunc)( pOutputStream, "currentdict end def\n", 20);
            (*pOutputFunc)( pOutputStream, "currentdict end put\n", 20);
        }
        (*pOutputFunc)( pOutputStream, "def\n", 4);

        // Binary section
        int nOffset = ( nCIDsCount + 1 ) * ( 1 + nNeedBytes );
        seBuffer = StringExt::Format("(Hex) {0:d} StartData\n", nOffset + charStrings->GetLength());
        (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
        delete seBuffer;

        int nLen = 0;

        // write the charstring offset (CIDMap) table
        for ( i = 0; i <= nCIDsCount; i += 6 )
        {
            for (j = 0; j < 6 && i+j <= nCIDsCount; ++j)
            {
                if (i+j < nCIDsCount && arrCIDdMap[i+j] >= 0)
                {
                    sBuf[0] = (char)m_pnFDSelect[ arrCIDdMap[i+j] ];
                }
                else
                {
                    sBuf[0] = (char)0;
                }
                nLen = nOffset + charStringOffsets[i+j];
                for (k = nNeedBytes; k >= 1; --k)
                {
                    sBuf[k] = (char)(nLen & 0xff);
                    nLen >>= 8;
                }
                for (k = 0; k <= nNeedBytes; ++k)
                {
                    seBuffer = StringExt::Format("{0:02x}", sBuf[k] & 0xff);
                    (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                    delete seBuffer;
                }
            }
            (*pOutputFunc)( pOutputStream, "\n", 1);
        }

        // Charstring
        nLen = charStrings->GetLength();
        for (i = 0; i < nLen; i += 32)
        {
            for (j = 0; j < 32 && i+j < nLen; ++j)
            {
                seBuffer = StringExt::Format("{0:02x}", charStrings->GetAt(i+j) & 0xff);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            if (i + 32 >= nLen)
            {
                (*pOutputFunc)( pOutputStream, ">", 1);
            }
            (*pOutputFunc)( pOutputStream, "\n", 1);
        }

        MemUtilsFree(charStringOffsets);
        delete charStrings;
        MemUtilsFree( arrCIDdMap );
    }

    void CFontFileType1C::ToType0(char *sPSName, FontFileOutputFunc pOutputFunc, void *pOutputStream)
    {
        StringExt *seBuffer;
        bool bSuccess = true;
        int i, j, k;

        int nCurFD = 0;

        // Вычислим количество CID и построим отображение CID->GID
        int nCIDsCount = 0;
        for (int nIndex = 0; nIndex < m_nGlyphsCount; ++nIndex )
        {
            if ( m_pnCharset[nIndex] >= nCIDsCount )
            {
                nCIDsCount = m_pnCharset[nIndex] + 1;
            }
        }
        int *arrCIDMap = (int *)MemUtilsMallocArray( nCIDsCount, sizeof(int));
        for ( int nIndex = 0; nIndex < nCIDsCount; ++nIndex )
        {
            arrCIDMap[nIndex] = -1;
        }
        for (int nIndex = 0; nIndex < m_nGlyphsCount; ++nIndex )
        {
            arrCIDMap[m_pnCharset[ nIndex ]] = nIndex;
        }

        // Запись Type 1 фонта
        for (i = 0; i < nCIDsCount; i += 256)
        {
            // Игнорируем CID = 0, т.е. ".notdef"
            nCurFD = 0;
            for ( j = ((i == 0) ? 1 : 0); j < 256 && i+j < nCIDsCount; ++j )
            {
                if (arrCIDMap[i+j] >= 0)
                {
                    nCurFD = m_pnFDSelect[arrCIDMap[i+j]];
                    break;
                }
            }

            // Font Dictionary (незашифрованная часть)
            (*pOutputFunc)( pOutputStream, "16 dict begin\n", 14);
            (*pOutputFunc)( pOutputStream, "/FontName /", 11);
            (*pOutputFunc)( pOutputStream, sPSName, strlen(sPSName));

            seBuffer = StringExt::Format("_{0:02x} def\n", i >> 8);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
            delete seBuffer;

            (*pOutputFunc)( pOutputStream, "/FontType 1 def\n", 16);
            if (m_pPrivateDicts[nCurFD].bHasFontMatrix)
            {
                seBuffer = StringExt::Format("/FontMatrix [{0:.8g} {1:.8g} {2:.8g} {3:.8g} {4:.8g} {5:.8g}] def\n", m_pPrivateDicts[nCurFD].arrdFontMatrix[0], m_pPrivateDicts[nCurFD].arrdFontMatrix[1], m_pPrivateDicts[nCurFD].arrdFontMatrix[2], m_pPrivateDicts[nCurFD].arrdFontMatrix[3], m_pPrivateDicts[nCurFD].arrdFontMatrix[4], m_pPrivateDicts[nCurFD].arrdFontMatrix[5]);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            else if (m_oTopDict.bHasFontMatrix)
            {
                (*pOutputFunc)( pOutputStream, "/FontMatrix [1 0 0 1 0 0] def\n", 30);
            }
            else
            {
                (*pOutputFunc)( pOutputStream, "/FontMatrix [0.001 0 0 0.001 0 0] def\n", 38);
            }
            seBuffer = StringExt::Format("/FontBBox [{0:.4g} {1:.4g} {2:.4g} {3:.4g}] def\n", m_oTopDict.arrdFontBBox[0], m_oTopDict.arrdFontBBox[1], m_oTopDict.arrdFontBBox[2], m_oTopDict.arrdFontBBox[3]);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
            delete seBuffer;

            seBuffer = StringExt::Format("/PaintType {0:d} def\n", m_oTopDict.nPaintType);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
            delete seBuffer;

            if ( m_oTopDict.nPaintType != 0 )
            {
                seBuffer = StringExt::Format("/StrokeWidth {0:.4g} def\n", m_oTopDict.dStrokeWidth);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            (*pOutputFunc)( pOutputStream, "/Encoding 256 array\n", 20);
            for (j = 0; j < 256 && i+j < nCIDsCount; ++j)
            {
                seBuffer = StringExt::Format("dup {0:d} /c{1:02x} put\n", j, j);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            if (j < 256)
            {
                seBuffer = StringExt::Format("{0:d} 1 255 {{ 1 index exch /.notdef put }} for\n", j);
                (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
                delete seBuffer;
            }
            (*pOutputFunc)( pOutputStream, "readonly def\n", 13);
            (*pOutputFunc)( pOutputStream, "currentdict end\n", 16);

            // start the binary section
            Type1CEexecBuf oEexecBuffer;
            (*pOutputFunc)( pOutputStream, "currentfile eexec\n", 18);
            oEexecBuffer.pOutputFunc = pOutputFunc;
            oEexecBuffer.pOutputStream =  pOutputStream;
            oEexecBuffer.bASKII = true;
            oEexecBuffer.unEncryptionKey = 55665;
            oEexecBuffer.nLine = 0;

            // start the private dictionary
            EexecWrite( &oEexecBuffer, "\x83\xca\x73\xd5");
            EexecWrite( &oEexecBuffer, "dup /Private 32 dict dup begin\n");
            EexecWrite( &oEexecBuffer, "/RD {string currentfile exch readstring pop} executeonly def\n");
            EexecWrite( &oEexecBuffer, "/ND {noaccess def} executeonly def\n");
            EexecWrite( &oEexecBuffer, "/NP {noaccess put} executeonly def\n");
            EexecWrite( &oEexecBuffer, "/MinFeature {16 16} def\n");
            EexecWrite( &oEexecBuffer, "/password 5839 def\n");
            if (m_pPrivateDicts[nCurFD].nBlueValues)
            {
                EexecWrite( &oEexecBuffer, "/BlueValues [");
                for (k = 0; k < m_pPrivateDicts[nCurFD].nBlueValues; ++k)
                {
                    seBuffer = StringExt::Format("{0:s}{1:d}", k > 0 ? " " : "", m_pPrivateDicts[nCurFD].arrnBlueValues[k]);
                    EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                    delete seBuffer;
                }
                EexecWrite( &oEexecBuffer, "] def\n");
            }
            if (m_pPrivateDicts[nCurFD].nOtherBlues)
            {
                EexecWrite( &oEexecBuffer, "/OtherBlues [");
                for (k = 0; k < m_pPrivateDicts[nCurFD].nOtherBlues; ++k)
                {
                    seBuffer = StringExt::Format("{0:s}{1:d}", k > 0 ? " " : "", m_pPrivateDicts[nCurFD].arrnOtherBlues[k]);
                    EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                    delete seBuffer;
                }
                EexecWrite( &oEexecBuffer, "] def\n");
            }
            if ( m_pPrivateDicts[nCurFD].nFamilyBlues )
            {
                EexecWrite( &oEexecBuffer, "/FamilyBlues [");
                for (k = 0; k < m_pPrivateDicts[nCurFD].nFamilyBlues; ++k)
                {
                    seBuffer = StringExt::Format("{0:s}{1:d}", k > 0 ? " " : "", m_pPrivateDicts[nCurFD].arrnFamilyBlues[k]);
                    EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                    delete seBuffer;
                }
                EexecWrite( &oEexecBuffer, "] def\n");
            }
            if (m_pPrivateDicts[nCurFD].nFamilyOtherBlues)
            {
                EexecWrite( &oEexecBuffer, "/FamilyOtherBlues [");
                for (k = 0; k < m_pPrivateDicts[nCurFD].nFamilyOtherBlues; ++k)
                {
                    seBuffer = StringExt::Format("{0:s}{1:d}", k > 0 ? " " : "", m_pPrivateDicts[nCurFD].arrnFamilyOtherBlues[k]);
                    EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                    delete seBuffer;
                }
                EexecWrite( &oEexecBuffer, "] def\n");
            }
            if (m_pPrivateDicts[nCurFD].dBlueScale != 0.039625)
            {
                seBuffer = StringExt::Format("/BlueScale {0:.4g} def\n", m_pPrivateDicts[nCurFD].dBlueScale);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            if (m_pPrivateDicts[nCurFD].nBlueShift != 7)
            {
                seBuffer = StringExt::Format("/BlueShift {0:d} def\n", m_pPrivateDicts[nCurFD].nBlueShift);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            if (m_pPrivateDicts[nCurFD].nBlueFuzz != 1)
            {
                seBuffer = StringExt::Format("/BlueFuzz {0:d} def\n", m_pPrivateDicts[nCurFD].nBlueFuzz);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            if (m_pPrivateDicts[nCurFD].bHasStdHW)
            {
                seBuffer = StringExt::Format("/StdHW [{0:.4g}] def\n", m_pPrivateDicts[nCurFD].dStdHW);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            if (m_pPrivateDicts[nCurFD].bHasStdVW)
            {
                seBuffer = StringExt::Format("/StdVW [{0:.4g}] def\n", m_pPrivateDicts[nCurFD].dStdVW);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            if (m_pPrivateDicts[nCurFD].nStemSnapH)
            {
                EexecWrite( &oEexecBuffer, "/StemSnapH [");
                for (k = 0; k < m_pPrivateDicts[nCurFD].nStemSnapH; ++k)
                {
                    seBuffer = StringExt::Format("{0:s}{1:.4g}", k > 0 ? " " : "", m_pPrivateDicts[nCurFD].arrdStemSnapH[k]);
                    EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                    delete seBuffer;
                }
                EexecWrite( &oEexecBuffer, "] def\n");
            }
            if (m_pPrivateDicts[nCurFD].nStemSnapV)
            {
                EexecWrite( &oEexecBuffer, "/StemSnapV [");
                for (k = 0; k < m_pPrivateDicts[nCurFD].nStemSnapV; ++k)
                {
                    seBuffer = StringExt::Format("{0:s}{1:.4g}", k > 0 ? " " : "", m_pPrivateDicts[nCurFD].arrdStemSnapV[k]);
                    EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                    delete seBuffer;
                }
                EexecWrite( &oEexecBuffer, "] def\n");
            }
            if (m_pPrivateDicts[nCurFD].bHasForceBold)
            {
                seBuffer = StringExt::Format("/ForceBold {0:s} def\n", m_pPrivateDicts[nCurFD].bForceBold ? "true" : "false");
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            if (m_pPrivateDicts[nCurFD].dForceBoldThreshold != 0)
            {
                seBuffer = StringExt::Format("/ForceBoldThreshold {0:.4g} def\n", m_pPrivateDicts[nCurFD].dForceBoldThreshold);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            if (m_pPrivateDicts[nCurFD].nLanguageGroup != 0)
            {
                seBuffer = StringExt::Format("/LanguageGroup {0:d} def\n", m_pPrivateDicts[nCurFD].nLanguageGroup);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }
            if (m_pPrivateDicts[nCurFD].dExpansionFactor != 0.06)
            {
                seBuffer = StringExt::Format("/ExpansionFactor {0:.4g} def\n", m_pPrivateDicts[nCurFD].dExpansionFactor);
                EexecWrite( &oEexecBuffer, seBuffer->GetBuffer());
                delete seBuffer;
            }

            bSuccess = true;
            Type1CIndex oSubrIndex;
            GetIndex( m_pPrivateDicts[nCurFD].nSubrsOffset, &oSubrIndex, &bSuccess);
            if (!bSuccess)
            {
                oSubrIndex.nPos = -1;
            }

            // CharStrings
            EexecWrite( &oEexecBuffer, "2 index /CharStrings 256 dict dup begin\n");

            // .notdef CharString
            Type1CIndexVal oIndexVal;
            bSuccess = true;
            GetIndexVal( &m_oCharStringsIndex, 0, &oIndexVal, &bSuccess);
            if ( bSuccess )
            {
                EexecConvertGlyph( &oEexecBuffer, ".notdef", oIndexVal.nPos, oIndexVal.nLen, &oSubrIndex, &m_pPrivateDicts[nCurFD]);
            }

            // CharStrings
            for (j = 0; j < 256 && i+j < nCIDsCount; ++j)
            {
                if (arrCIDMap[i+j] >= 0)
                {
                    bSuccess = true;
                    GetIndexVal( &m_oCharStringsIndex, arrCIDMap[i+j], &oIndexVal, &bSuccess);
                    if ( bSuccess )
                    {
                        seBuffer = StringExt::Format("c{0:02x}", j);
                        EexecConvertGlyph( &oEexecBuffer, seBuffer->GetBuffer(), oIndexVal.nPos, oIndexVal.nLen, &oSubrIndex, &m_pPrivateDicts[nCurFD]);
                        delete seBuffer;
                    }
                }
            }
            EexecWrite( &oEexecBuffer, "end\n");
            EexecWrite( &oEexecBuffer, "end\n");
            EexecWrite( &oEexecBuffer, "readonly put\n");
            EexecWrite( &oEexecBuffer, "noaccess put\n");
            EexecWrite( &oEexecBuffer, "dup /FontName get exch definefont pop\n");
            EexecWrite( &oEexecBuffer, "mark currentfile closefile\n");

            // trailer
            if ( oEexecBuffer.nLine > 0 )
            {
                (*pOutputFunc)( pOutputStream, "\n", 1);
            }
            for (j = 0; j < 8; ++j)
            {
                (*pOutputFunc)( pOutputStream, "0000000000000000000000000000000000000000000000000000000000000000\n", 65);
            }
            (*pOutputFunc)( pOutputStream, "cleartomark\n", 12);
        }

        // write the Type 0 parent font
        (*pOutputFunc)( pOutputStream, "16 dict begin\n", 14);
        (*pOutputFunc)( pOutputStream, "/FontName /", 11);
        (*pOutputFunc)( pOutputStream, sPSName, strlen(sPSName));
        (*pOutputFunc)( pOutputStream, " def\n", 5);
        (*pOutputFunc)( pOutputStream, "/FontType 0 def\n", 16);
        if (m_oTopDict.bHasFontMatrix)
        {
            seBuffer = StringExt::Format("/FontMatrix [{0:.8g} {1:.8g} {2:.8g} {3:.8g} {4:.8g} {5:.8g}] def\n", m_oTopDict.arrdFontMatrix[0], m_oTopDict.arrdFontMatrix[1], m_oTopDict.arrdFontMatrix[2], m_oTopDict.arrdFontMatrix[3], m_oTopDict.arrdFontMatrix[4], m_oTopDict.arrdFontMatrix[5]);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
            delete seBuffer;
        }
        else
        {
            (*pOutputFunc)( pOutputStream, "/FontMatrix [1 0 0 1 0 0] def\n", 30);
        }
        (*pOutputFunc)( pOutputStream, "/FMapType 2 def\n", 16);
        (*pOutputFunc)( pOutputStream, "/Encoding [\n", 12);

        for (i = 0; i < nCIDsCount; i += 256)
        {
            seBuffer = StringExt::Format("{0:d}\n", i >> 8);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
            delete seBuffer;
        }

        (*pOutputFunc)( pOutputStream, "] def\n", 6);
        (*pOutputFunc)( pOutputStream, "/FDepVector [\n", 14);

        for (i = 0; i < nCIDsCount; i += 256)
        {
            (*pOutputFunc)( pOutputStream, "/", 1);
            (*pOutputFunc)( pOutputStream, sPSName, strlen(sPSName));
            seBuffer = StringExt::Format("_{0:02x} findfont\n", i >> 8);
            (*pOutputFunc)( pOutputStream, seBuffer->GetBuffer(), seBuffer->GetLength());
            delete seBuffer;
        }

        (*pOutputFunc)( pOutputStream, "] def\n", 6);
        (*pOutputFunc)( pOutputStream, "FontName currentdict end definefont pop\n", 40);

        MemUtilsFree(arrCIDMap);
    }

    void CFontFileType1C::EexecConvertGlyph( Type1CEexecBuf *pEexecBuffer, char *sGlyphName, int nOffset, int nBytes, Type1CIndex *pSubrIndex, Type1CPrivateDict *pDict)
    {
        StringExt *seCharBuffer = new StringExt();
        ConvertGlyph( nOffset, nBytes, seCharBuffer, pSubrIndex, pDict, true);

        StringExt *seBuffer = StringExt::Format("/{0:s} {1:d} RD ", sGlyphName, seCharBuffer->GetLength());
        EexecWrite( pEexecBuffer, seBuffer->GetBuffer() );
        delete seBuffer;
        EexecWriteCharString( pEexecBuffer, (unsigned char *)seCharBuffer->GetBuffer(), seCharBuffer->GetLength() );
        EexecWrite( pEexecBuffer, " ND\n");

        delete seCharBuffer;
    }

    void CFontFileType1C::ConvertGlyph(int nOffset, int nBytes, StringExt *seCharBuffer, Type1CIndex *pSubrIndex, Type1CPrivateDict *pDict, bool bTop)
    {
        Type1CIndexVal oIndexVal;
        double dTemp = 0, dX = 0, dY = 0;
        unsigned char unByte = 0;
        int nSubrBias = 0, k = 0;
        bool bDouble = false;

        int nStart = seCharBuffer->GetLength();

        if ( bTop )
        {
            seCharBuffer->Append((char)73);
            seCharBuffer->Append((char)58);
            seCharBuffer->Append((char)147);
            seCharBuffer->Append((char)134);
            m_nOperatorsCount = 0;
            m_nHints = 0;
            m_bFirstOperator = true;
            m_bOpenPath = false;
        }

        int nPos = nOffset;
        while ( nPos < nOffset + nBytes )
        {
            bool bSuccess = true;
            nPos = GetOperator( nPos, true, &bSuccess );
            if ( !bSuccess )
                break;

            if (!m_arrOperators[m_nOperatorsCount - 1].bIsNumber)
            {
                --m_nOperatorsCount;
                switch (m_arrOperators[m_nOperatorsCount].nOperator)
                {
                case 0x0001:		// hstem
                    if (m_bFirstOperator)
                    {
                        ConvertGlyphWidth(m_nOperatorsCount & 1, seCharBuffer, pDict);
                        m_bFirstOperator = false;
                    }
                    if (m_nOperatorsCount & 1)
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 hstem"
                    }
                    dTemp = 0;
                    bDouble = false;
                    for ( k = 0; k < m_nOperatorsCount; k += 2)
                    {
                        // convert Type 2 edge hints (-20 or -21) to Type 1 ghost hints
                        if (m_arrOperators[k+1].dNumber < 0)
                        {
                            dTemp += m_arrOperators[k].dNumber + m_arrOperators[k+1].dNumber;
                            bDouble |= m_arrOperators[k].bIsFloat | m_arrOperators[k+1].bIsFloat;
                            ConvertNum( dTemp, bDouble, seCharBuffer);
                            ConvertNum(-m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                        }
                        else
                        {
                            dTemp += m_arrOperators[k].dNumber;
                            bDouble |= m_arrOperators[k].bIsFloat;
                            ConvertNum( dTemp, bDouble, seCharBuffer);
                            ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                            dTemp += m_arrOperators[k+1].dNumber;
                            bDouble |= m_arrOperators[k+1].bIsFloat;
                        }
                        seCharBuffer->Append((char)1);
                    }
                    m_nHints += m_nOperatorsCount / 2;
                    m_nOperatorsCount = 0;
                    break;
                case 0x0003:		// vstem
                    if (m_bFirstOperator)
                    {
                        ConvertGlyphWidth(m_nOperatorsCount & 1, seCharBuffer, pDict);
                        m_bFirstOperator = false;
                    }
                    if (m_nOperatorsCount & 1)
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 vstem"
                    }
                    dTemp = 0;
                    bDouble = false;
                    for ( k = 0; k < m_nOperatorsCount; k += 2 )
                    {
                        // convert Type 2 edge hints (-20 or -21) to Type 1 ghost hints
                        if (m_arrOperators[k+1].dNumber < 0)
                        {
                            dTemp += m_arrOperators[k].dNumber + m_arrOperators[k+1].dNumber;
                            bDouble |= m_arrOperators[k].bIsFloat | m_arrOperators[k+1].bIsFloat;
                            ConvertNum( dTemp, bDouble, seCharBuffer);
                            ConvertNum(-m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                        }
                        else
                        {
                            dTemp += m_arrOperators[k].dNumber;
                            bDouble |= m_arrOperators[k].bIsFloat;
                            ConvertNum( dTemp, bDouble, seCharBuffer);
                            ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                            dTemp += m_arrOperators[k+1].dNumber;
                            bDouble |= m_arrOperators[k+1].bIsFloat;
                        }
                        seCharBuffer->Append((char)3);
                    }
                    m_nHints += m_nOperatorsCount / 2;
                    m_nOperatorsCount = 0;
                    break;
                case 0x0004:		// vmoveto
                    if (m_bFirstOperator)
                    {
                        ConvertGlyphWidth(m_nOperatorsCount == 2, seCharBuffer, pDict);
                        m_bFirstOperator = false;
                    }
                    if (m_bOpenPath)
                    {
                        seCharBuffer->Append((char)9);
                        m_bOpenPath = false;
                    }
                    if (m_nOperatorsCount != 1)
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 vmoveto"
                    }
                    ConvertNum(m_arrOperators[0].dNumber, m_arrOperators[0].bIsFloat, seCharBuffer);
                    seCharBuffer->Append((char)4);
                    m_nOperatorsCount = 0;
                    break;
                case 0x0005:		// rlineto
                    if (m_nOperatorsCount < 2 || m_nOperatorsCount % 2 != 0)
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 rlineto"
                    }
                    for ( k = 0; k < m_nOperatorsCount; k += 2 )
                    {
                        ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                        seCharBuffer->Append((char)5);
                    }
                    m_nOperatorsCount = 0;
                    m_bOpenPath = true;
                    break;
                case 0x0006:		// hlineto
                    if (m_nOperatorsCount < 1)
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 hlineto"
                    }
                    for (k = 0; k < m_nOperatorsCount; ++k)
                    {
                        ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                        seCharBuffer->Append((char)((k & 1) ? 7 : 6));
                    }
                    m_nOperatorsCount = 0;
                    m_bOpenPath = true;
                    break;
                case 0x0007:		// vlineto
                    if (m_nOperatorsCount < 1)
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 vlineto"
                    }
                    for ( k = 0; k < m_nOperatorsCount; ++k )
                    {
                        ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                        seCharBuffer->Append((char)((k & 1) ? 6 : 7));
                    }
                    m_nOperatorsCount = 0;
                    m_bOpenPath = true;
                    break;
                case 0x0008:		// rrcurveto
                    if (m_nOperatorsCount < 6 || m_nOperatorsCount % 6 != 0)
                    {
                        // TO DO: error "Wrong number of args (%d) to Type 2 rrcurveto", m_nOperatorsCount);
                    }
                    for (k = 0; k < m_nOperatorsCount; k += 6)
                    {
                        ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+4].dNumber, m_arrOperators[k+4].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+5].dNumber, m_arrOperators[k+5].bIsFloat, seCharBuffer);
                        seCharBuffer->Append((char)8);
                    }
                    m_nOperatorsCount = 0;
                    m_bOpenPath = true;
                    break;
                case 0x000a:		// callsubr
                    if (m_nOperatorsCount >= 1)
                    {
                        nSubrBias = (pSubrIndex->nCount < 1240) ? 107 : (pSubrIndex->nCount < 33900) ? 1131 : 32768;
                        k = nSubrBias + (int)m_arrOperators[m_nOperatorsCount - 1].dNumber;
                        --m_nOperatorsCount;
                        bSuccess = true;
                        GetIndexVal(pSubrIndex, k, &oIndexVal, &bSuccess);
                        if ( bSuccess )
                        {
                            ConvertGlyph( oIndexVal.nPos, oIndexVal.nLen, seCharBuffer, pSubrIndex, pDict, false);
                        }
                    }
                    else
                    {
                        // TO DO: error "Too few args to Type 2 callsubr"
                    }
                    // не очищаем стек
                    break;
                case 0x000b:		// return
                    // не очищаем стек
                    break;
                case 0x000e:		// endchar / seac
                    if (m_bFirstOperator)
                    {
                        ConvertGlyphWidth(m_nOperatorsCount == 1 || m_nOperatorsCount == 5, seCharBuffer, pDict);
                        m_bFirstOperator = false;
                    }
                    if (m_bOpenPath)
                    {
                        seCharBuffer->Append((char)9);
                        m_bOpenPath = false;
                    }
                    if (m_nOperatorsCount == 4)
                    {
                        ConvertNum(0, false, seCharBuffer);
                        ConvertNum(m_arrOperators[0].dNumber, m_arrOperators[0].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[1].dNumber, m_arrOperators[1].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[2].dNumber, m_arrOperators[2].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[3].dNumber, m_arrOperators[3].bIsFloat, seCharBuffer);
                        seCharBuffer->Append((char)12)->Append((char)6);
                    }
                    else if (m_nOperatorsCount == 0)
                    {
                        seCharBuffer->Append((char)14);

                    }
                    else
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 endchar"
                    }
                    m_nOperatorsCount = 0;
                    break;
                case 0x000f:		// (obsolete)
                    // Данная операция игнорируется, но нам нужна ширина символа
                    if (m_bFirstOperator)
                    {
                        ConvertGlyphWidth(m_nOperatorsCount > 0, seCharBuffer, pDict);
                        m_bFirstOperator = false;
                    }
                    m_nOperatorsCount = 0;
                    break;
                case 0x0010:		// blend
                    // TO DO: error "Unimplemented Type 2 charstring op: file[i]"
                    m_nOperatorsCount = 0;
                    break;
                case 0x0012:		// hstemhm
                    // Данная операция игнорируется
                    if (m_bFirstOperator)
                    {
                        ConvertGlyphWidth(m_nOperatorsCount & 1, seCharBuffer, pDict);
                        m_bFirstOperator = false;
                    }
                    if (m_nOperatorsCount & 1)
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 hstemhm"
                    }
                    m_nHints += m_nOperatorsCount / 2;
                    m_nOperatorsCount = 0;
                    break;
                case 0x0013:		// hintmask
                    // Данная операция игнорируется
                    if (m_bFirstOperator)
                    {
                        ConvertGlyphWidth(m_nOperatorsCount & 1, seCharBuffer, pDict);
                        m_bFirstOperator = false;
                    }
                    if (m_nOperatorsCount > 0)
                    {
                        if (m_nOperatorsCount & 1)
                        {
                            // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 hintmask/vstemhm"
                        }
                        m_nHints += m_nOperatorsCount / 2;
                    }
                    nPos += ( m_nHints + 7) >> 3;
                    m_nOperatorsCount = 0;
                    break;
                case 0x0014:		// cntrmask
                    // Данная операция игнорируется
                    if (m_bFirstOperator)
                    {
                        ConvertGlyphWidth(m_nOperatorsCount & 1, seCharBuffer, pDict);
                        m_bFirstOperator = false;
                    }
                    if (m_nOperatorsCount > 0)
                    {
                        if (m_nOperatorsCount & 1)
                        {
                            // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 cntrmask/vstemhm"
                        }
                        m_nHints += m_nOperatorsCount / 2;
                    }
                    nPos += ( m_nHints + 7) >> 3;
                    m_nOperatorsCount = 0;
                    break;
                case 0x0015:		// rmoveto
                    if (m_bFirstOperator)
                    {
                        ConvertGlyphWidth(m_nOperatorsCount == 3, seCharBuffer, pDict);
                        m_bFirstOperator = false;
                    }
                    if (m_bOpenPath)
                    {
                        seCharBuffer->Append((char)9);
                        m_bOpenPath = false;
                    }
                    if (m_nOperatorsCount != 2)
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 rmoveto"
                    }
                    ConvertNum(m_arrOperators[0].dNumber, m_arrOperators[0].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[1].dNumber, m_arrOperators[1].bIsFloat, seCharBuffer);
                    seCharBuffer->Append((char)21);
                    m_nOperatorsCount = 0;
                    break;
                case 0x0016:		// hmoveto
                    if (m_bFirstOperator)
                    {
                        ConvertGlyphWidth(m_nOperatorsCount == 2, seCharBuffer, pDict);
                        m_bFirstOperator = false;
                    }
                    if (m_bOpenPath)
                    {
                        seCharBuffer->Append((char)9);
                        m_bOpenPath = false;
                    }
                    if (m_nOperatorsCount != 1)
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 hmoveto"
                    }
                    ConvertNum(m_arrOperators[0].dNumber, m_arrOperators[0].bIsFloat, seCharBuffer);
                    seCharBuffer->Append((char)22);
                    m_nOperatorsCount = 0;
                    break;
                case 0x0017:		// vstemhm
                    // Данная операция игнорируется
                    if (m_bFirstOperator)
                    {
                        ConvertGlyphWidth(m_nOperatorsCount & 1, seCharBuffer, pDict);
                        m_bFirstOperator = false;
                    }
                    if (m_nOperatorsCount & 1)
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 vstemhm"
                    }
                    m_nHints += m_nOperatorsCount / 2;
                    m_nOperatorsCount = 0;
                    break;
                case 0x0018:		// rcurveline
                    if (m_nOperatorsCount < 8 || (m_nOperatorsCount - 2) % 6 != 0)
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 rcurveline"
                    }
                    for (k = 0; k < m_nOperatorsCount - 2; k += 6)
                    {
                        ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+4].dNumber, m_arrOperators[k+4].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+5].dNumber, m_arrOperators[k+5].bIsFloat, seCharBuffer);
                        seCharBuffer->Append((char)8);
                    }
                    ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                    seCharBuffer->Append((char)5);
                    m_nOperatorsCount = 0;
                    m_bOpenPath = true;
                    break;
                case 0x0019:		// rlinecurve
                    if (m_nOperatorsCount < 8 || (m_nOperatorsCount - 6) % 2 != 0)
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 rlinecurve"
                    }
                    for (k = 0; k < m_nOperatorsCount - 6; k += 2)
                    {
                        ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                        seCharBuffer->Append((char)5);
                    }
                    ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[k+4].dNumber, m_arrOperators[k+4].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[k+5].dNumber, m_arrOperators[k+5].bIsFloat, seCharBuffer);
                    seCharBuffer->Append((char)8);
                    m_nOperatorsCount = 0;
                    m_bOpenPath = true;
                    break;
                case 0x001a:		// vvcurveto
                    if (m_nOperatorsCount < 4 || !(m_nOperatorsCount % 4 == 0 || (m_nOperatorsCount-1) % 4 == 0))
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 vvcurveto"
                    }
                    if (m_nOperatorsCount % 2 == 1)
                    {
                        ConvertNum(m_arrOperators[0].dNumber, m_arrOperators[0].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[1].dNumber, m_arrOperators[1].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[2].dNumber, m_arrOperators[2].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[3].dNumber, m_arrOperators[3].bIsFloat, seCharBuffer);
                        ConvertNum(0, false, seCharBuffer);
                        ConvertNum(m_arrOperators[4].dNumber, m_arrOperators[4].bIsFloat, seCharBuffer);
                        seCharBuffer->Append((char)8);
                        k = 5;
                    }
                    else
                    {
                        k = 0;
                    }
                    for (; k < m_nOperatorsCount; k += 4)
                    {
                        ConvertNum(0, false, seCharBuffer);
                        ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                        ConvertNum(0, false, seCharBuffer);
                        ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                        seCharBuffer->Append((char)8);
                    }
                    m_nOperatorsCount = 0;
                    m_bOpenPath = true;
                    break;
                case 0x001b:		// hhcurveto
                    if (m_nOperatorsCount < 4 || !(m_nOperatorsCount % 4 == 0 || (m_nOperatorsCount-1) % 4 == 0))
                    {
                        // TO DO: error "Wrong number of args (m_nOperatorsCount) to Type 2 hhcurveto"
                    }
                    if (m_nOperatorsCount % 2 == 1)
                    {
                        ConvertNum(m_arrOperators[1].dNumber, m_arrOperators[1].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[0].dNumber, m_arrOperators[0].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[2].dNumber, m_arrOperators[2].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[3].dNumber, m_arrOperators[3].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[4].dNumber, m_arrOperators[4].bIsFloat, seCharBuffer);
                        ConvertNum(0, false, seCharBuffer);
                        seCharBuffer->Append((char)8);
                        k = 5;
                    }
                    else
                    {
                        k = 0;
                    }
                    for (; k < m_nOperatorsCount; k += 4)
                    {
                        ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                        ConvertNum(0, false, seCharBuffer);
                        ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                        ConvertNum(0, false, seCharBuffer);
                        seCharBuffer->Append((char)8);
                    }
                    m_nOperatorsCount = 0;
                    m_bOpenPath = true;
                    break;
                case 0x001d:		// callgsubr
                    if (m_nOperatorsCount >= 1)
                    {
                        k = m_nGsubrBias + (int)m_arrOperators[m_nOperatorsCount - 1].dNumber;
                        --m_nOperatorsCount;
                        bSuccess = true;
                        GetIndexVal( &m_oGsubrIndex, k, &oIndexVal, &bSuccess );
                        if ( bSuccess )
                        {
                            ConvertGlyph( oIndexVal.nPos, oIndexVal.nLen, seCharBuffer, pSubrIndex, pDict, false);
                        }
                    }
                    else
                    {
                        // TO DO: error "Too few args to Type 2 callgsubr"
                    }
                    // не очищаем стек
                    break;
                case 0x001e:		// vhcurveto
                    if (m_nOperatorsCount < 4 || !(m_nOperatorsCount % 4 == 0 || (m_nOperatorsCount-1) % 4 == 0))
                    {
                        // TO DO: "Wrong number of args (m_nOperatorsCount) to Type 2 vhcurveto"
                    }
                    for (k = 0; k < m_nOperatorsCount && k != m_nOperatorsCount-5; k += 4)
                    {
                        if (k % 8 == 0)
                        {
                            ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                            seCharBuffer->Append((char)30);
                        }
                        else
                        {
                            ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                            seCharBuffer->Append((char)31);
                        }
                    }
                    if (k == m_nOperatorsCount-5)
                    {
                        if (k % 8 == 0)
                        {
                            ConvertNum(0, false, seCharBuffer);
                            ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+4].dNumber, m_arrOperators[k+4].bIsFloat, seCharBuffer);
                        }
                        else
                        {
                            ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                            ConvertNum(0, false, seCharBuffer);
                            ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+4].dNumber, m_arrOperators[k+4].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                        }
                        seCharBuffer->Append((char)8);
                    }
                    m_nOperatorsCount = 0;
                    m_bOpenPath = true;
                    break;
                case 0x001f:		// hvcurveto
                    if (m_nOperatorsCount < 4 || !(m_nOperatorsCount % 4 == 0 || (m_nOperatorsCount-1) % 4 == 0))
                    {
                        // TO DO: "Wrong number of args (m_nOperatorsCount) to Type 2 hvcurveto"
                    }
                    for (k = 0; k < m_nOperatorsCount && k != m_nOperatorsCount-5; k += 4)
                    {
                        if (k % 8 == 0)
                        {
                            ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                            seCharBuffer->Append((char)31);
                        }
                        else
                        {
                            ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                            seCharBuffer->Append((char)30);
                        }
                    }
                    if (k == m_nOperatorsCount-5)
                    {
                        if (k % 8 == 0)
                        {
                            ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                            ConvertNum(0, false, seCharBuffer);
                            ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+4].dNumber, m_arrOperators[k+4].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                        }
                        else
                        {
                            ConvertNum(0, false, seCharBuffer);
                            ConvertNum(m_arrOperators[k].dNumber, m_arrOperators[k].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+1].dNumber, m_arrOperators[k+1].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+2].dNumber, m_arrOperators[k+2].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+3].dNumber, m_arrOperators[k+3].bIsFloat, seCharBuffer);
                            ConvertNum(m_arrOperators[k+4].dNumber, m_arrOperators[k+4].bIsFloat, seCharBuffer);
                        }
                        seCharBuffer->Append((char)8);
                    }
                    m_nOperatorsCount = 0;
                    m_bOpenPath = true;
                    break;
                case 0x0c00:		// dotsection (should be Type 1 only?)
                    // игнорируем
                    m_nOperatorsCount = 0;
                    break;
                case 0x0c03:		// and
                case 0x0c04:		// or
                case 0x0c05:		// not
                case 0x0c08:		// store
                case 0x0c09:		// abs
                case 0x0c0a:		// add
                case 0x0c0b:		// sub
                case 0x0c0c:		// div
                case 0x0c0d:		// load
                case 0x0c0e:		// neg
                case 0x0c0f:		// eq
                case 0x0c12:		// drop
                case 0x0c14:		// put
                case 0x0c15:		// get
                case 0x0c16:		// ifelse
                case 0x0c17:		// random
                case 0x0c18:		// mul
                case 0x0c1a:		// sqrt
                case 0x0c1b:		// dup
                case 0x0c1c:		// exch
                case 0x0c1d:		// index
                case 0x0c1e:		// roll
                    // TO DO: "Unimplemented Type 2 charstring op"
                    m_nOperatorsCount = 0;
                    break;
                case 0x0c22:		// hflex
                    if (m_nOperatorsCount != 7)
                    {
                        // TO DO: "Wrong number of args (m_nOperatorsCount) to Type 2 hflex"
                    }
                        ConvertNum(m_arrOperators[0].dNumber, m_arrOperators[0].bIsFloat, seCharBuffer);
                        ConvertNum(0, false, seCharBuffer);
                        ConvertNum(m_arrOperators[1].dNumber, m_arrOperators[1].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[2].dNumber, m_arrOperators[2].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[3].dNumber, m_arrOperators[3].bIsFloat, seCharBuffer);
                        ConvertNum(0, false, seCharBuffer);
                        seCharBuffer->Append((char)8);
                        ConvertNum(m_arrOperators[4].dNumber, m_arrOperators[4].bIsFloat, seCharBuffer);
                        ConvertNum(0, false, seCharBuffer);
                        ConvertNum(m_arrOperators[5].dNumber, m_arrOperators[5].bIsFloat, seCharBuffer);
                        ConvertNum(-m_arrOperators[2].dNumber, m_arrOperators[2].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[6].dNumber, m_arrOperators[6].bIsFloat, seCharBuffer);
                        ConvertNum(0, false, seCharBuffer);
                        seCharBuffer->Append((char)8);
                        m_nOperatorsCount = 0;
                        m_bOpenPath = true;
                        break;
                case 0x0c23:		// flex
                    if (m_nOperatorsCount != 13)
                    {
                        // TO DO: "Wrong number of args (m_nOperatorsCount) to Type 2 flex"
                    }
                        ConvertNum(m_arrOperators[0].dNumber, m_arrOperators[0].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[1].dNumber, m_arrOperators[1].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[2].dNumber, m_arrOperators[2].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[3].dNumber, m_arrOperators[3].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[4].dNumber, m_arrOperators[4].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[5].dNumber, m_arrOperators[5].bIsFloat, seCharBuffer);
                        seCharBuffer->Append((char)8);
                        ConvertNum(m_arrOperators[6].dNumber, m_arrOperators[6].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[7].dNumber, m_arrOperators[7].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[8].dNumber, m_arrOperators[8].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[9].dNumber, m_arrOperators[9].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[10].dNumber, m_arrOperators[10].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[11].dNumber, m_arrOperators[11].bIsFloat, seCharBuffer);
                        seCharBuffer->Append((char)8);
                        m_nOperatorsCount = 0;
                        m_bOpenPath = true;
                        break;
                case 0x0c24:		// hflex1
                    if (m_nOperatorsCount != 9)
                    {
                        // TO DO: "Wrong number of args (m_nOperatorsCount) to Type 2 hflex1"
                    }
                        ConvertNum(m_arrOperators[0].dNumber, m_arrOperators[0].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[1].dNumber, m_arrOperators[1].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[2].dNumber, m_arrOperators[2].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[3].dNumber, m_arrOperators[3].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[4].dNumber, m_arrOperators[4].bIsFloat, seCharBuffer);
                        ConvertNum(0, false, seCharBuffer);
                        seCharBuffer->Append((char)8);
                        ConvertNum(m_arrOperators[5].dNumber, m_arrOperators[5].bIsFloat, seCharBuffer);
                        ConvertNum(0, false, seCharBuffer);
                        ConvertNum(m_arrOperators[6].dNumber, m_arrOperators[6].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[7].dNumber, m_arrOperators[7].bIsFloat, seCharBuffer);
                        ConvertNum(m_arrOperators[8].dNumber, m_arrOperators[8].bIsFloat, seCharBuffer);
                        ConvertNum(-(m_arrOperators[1].dNumber + m_arrOperators[3].dNumber + m_arrOperators[7].dNumber), m_arrOperators[1].bIsFloat | m_arrOperators[3].bIsFloat | m_arrOperators[7].bIsFloat, seCharBuffer);
                        seCharBuffer->Append((char)8);
                        m_nOperatorsCount = 0;
                        m_bOpenPath = true;
                        break;
                case 0x0c25:		// flex1
                    if (m_nOperatorsCount != 11)
                    {
                        // TO DO: "Wrong number of args (m_nOperatorsCount) to Type 2 flex1"
                    }
                    ConvertNum(m_arrOperators[0].dNumber, m_arrOperators[0].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[1].dNumber, m_arrOperators[1].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[2].dNumber, m_arrOperators[2].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[3].dNumber, m_arrOperators[3].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[4].dNumber, m_arrOperators[4].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[5].dNumber, m_arrOperators[5].bIsFloat, seCharBuffer);
                    seCharBuffer->Append((char)8);
                    ConvertNum(m_arrOperators[6].dNumber, m_arrOperators[6].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[7].dNumber, m_arrOperators[7].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[8].dNumber, m_arrOperators[8].bIsFloat, seCharBuffer);
                    ConvertNum(m_arrOperators[9].dNumber, m_arrOperators[9].bIsFloat, seCharBuffer);
                    dX = m_arrOperators[0].dNumber + m_arrOperators[2].dNumber + m_arrOperators[4].dNumber + m_arrOperators[6].dNumber + m_arrOperators[8].dNumber;
                    dY = m_arrOperators[1].dNumber + m_arrOperators[3].dNumber + m_arrOperators[5].dNumber + m_arrOperators[7].dNumber + m_arrOperators[9].dNumber;
                    if ( fabs(dX) > fabs(dY) )
                    {
                        ConvertNum( m_arrOperators[10].dNumber, m_arrOperators[10].bIsFloat, seCharBuffer);
                        ConvertNum( -dY, m_arrOperators[1].bIsFloat | m_arrOperators[3].bIsFloat | m_arrOperators[5].bIsFloat | m_arrOperators[7].bIsFloat | m_arrOperators[9].bIsFloat, seCharBuffer);
                    }
                    else
                    {
                        ConvertNum( -dX, m_arrOperators[0].bIsFloat | m_arrOperators[2].bIsFloat | m_arrOperators[4].bIsFloat | m_arrOperators[6].bIsFloat | m_arrOperators[8].bIsFloat, seCharBuffer);
                        ConvertNum( m_arrOperators[10].dNumber, m_arrOperators[10].bIsFloat, seCharBuffer);
                    }
                    seCharBuffer->Append((char)8);
                    m_nOperatorsCount = 0;
                    m_bOpenPath = true;
                    break;
                default:
                    // TO DO: "Illegal Type 2 charstring Operation"
                    m_nOperatorsCount = 0;
                    break;
                }
            }
        }

        // CharString Encryption
        if ( bTop )
        {
            unsigned short nR2 = 4330;
            for ( int nIndex = nStart; nIndex < seCharBuffer->GetLength(); ++nIndex )
            {
                unByte = seCharBuffer->GetAt( nIndex ) ^ ( nR2 >> 8 );
                seCharBuffer->SetAt( nIndex, unByte);
                nR2 = ( unByte + nR2) * 52845 + 22719;
            }
        }
    }

    void CFontFileType1C::ConvertGlyphWidth(bool bUseOperation, StringExt *seCharBuffer, Type1CPrivateDict *pDict)
    {
        double dWidth = 0;
        bool bWidthDouble = false;

        if ( bUseOperation )
        {
            dWidth = pDict->dNominalWidthX + m_arrOperators[0].dNumber;
            bWidthDouble = pDict->bNominalWidthXFP | m_arrOperators[0].bIsFloat;
            for (int nIndex = 1; nIndex < m_nOperatorsCount; ++nIndex)
            {
                m_arrOperators[nIndex - 1] = m_arrOperators[nIndex];
            }
            --m_nOperatorsCount;
        }
        else
        {
            dWidth = pDict->dDefaultWidthX;
            bWidthDouble = pDict->bDefaultWidthXFP;
        }
        ConvertNum( 0, false, seCharBuffer);
        ConvertNum( dWidth, bWidthDouble, seCharBuffer);
        seCharBuffer->Append((char)13);
    }

    void CFontFileType1C::ConvertNum(double dValue, bool bIsFloat, StringExt *seCharBuffer)
    {
        unsigned char sBuffer[12];

        int nLen = 0;
        int nTemp = 0;
        if (bIsFloat)
        {

            if ( dValue >= -32768 && dValue < 32768 )
            {
                nTemp = (int)(dValue * 256.0);
                sBuffer[0] = 255;
                sBuffer[1] = (unsigned char)(nTemp >> 24);
                sBuffer[2] = (unsigned char)(nTemp >> 16);
                sBuffer[3] = (unsigned char)(nTemp >> 8);
                sBuffer[4] = (unsigned char)nTemp;
                sBuffer[5] = 255;
                sBuffer[6] = 0;
                sBuffer[7] = 0;
                sBuffer[8] = 1;
                sBuffer[9] = 0;
                sBuffer[10] = 12;
                sBuffer[11] = 12;
                nLen = 12;
            }
            else
            {
                // TO DO: "Type 2 fixed point constant out of range"
            }
        }
        else
        {
            nTemp = (int)dValue;
            if ( nTemp >= -107 && nTemp <= 107 )
            {
                sBuffer[0] = (unsigned char)(nTemp + 139);
                nLen = 1;
            }
            else if ( nTemp > 107 && nTemp <= 1131 )
            {
                nTemp -= 108;
                sBuffer[0] = (unsigned char)((nTemp >> 8) + 247);
                sBuffer[1] = (unsigned char)(nTemp & 0xff);
                nLen = 2;
            }
            else if ( nTemp < -107 && nTemp >= -1131 )
            {
                nTemp = -nTemp - 108;
                sBuffer[0] = (unsigned char)((nTemp >> 8) + 251);
                sBuffer[1] = (unsigned char)(nTemp & 0xff);
                nLen = 2;
            }
            else
            {
                sBuffer[0] = 255;
                sBuffer[1] = (unsigned char)(nTemp >> 24);
                sBuffer[2] = (unsigned char)(nTemp >> 16);
                sBuffer[3] = (unsigned char)(nTemp >> 8);
                sBuffer[4] = (unsigned char)nTemp;
                nLen = 5;
            }
        }
        seCharBuffer->Append( (char *)sBuffer, nLen);
    }

    void CFontFileType1C::EexecWrite(Type1CEexecBuf *pEexecBuffer, char *sBuffer)
    {
        unsigned char *pCharPtr;

        for ( pCharPtr = (unsigned char *)sBuffer; *pCharPtr; ++pCharPtr )
        {
            unsigned char nCurChar = *pCharPtr ^ ( pEexecBuffer->unEncryptionKey >> 8);
            pEexecBuffer->unEncryptionKey = ( nCurChar + pEexecBuffer->unEncryptionKey ) * 52845 + 22719;
            if ( pEexecBuffer->bASKII )
            {
                (*pEexecBuffer->pOutputFunc)( pEexecBuffer->pOutputStream, &c_sHexChars[ nCurChar >> 4 ], 1);
                (*pEexecBuffer->pOutputFunc)( pEexecBuffer->pOutputStream, &c_sHexChars[ nCurChar & 0x0f ], 1);
                pEexecBuffer->nLine += 2;
                if ( pEexecBuffer->nLine == 64 )
                {
                    (*pEexecBuffer->pOutputFunc)( pEexecBuffer->pOutputStream, "\n", 1);
                    pEexecBuffer->nLine = 0;
                }
            }
            else
            {
                (*pEexecBuffer->pOutputFunc)( pEexecBuffer->pOutputStream, (char *)&nCurChar, 1);
            }
        }
    }

    void CFontFileType1C::EexecWriteCharString(Type1CEexecBuf *pEexecBuffer, unsigned char *sBuffer, int nLen)
    {
        // Eexec шифрование
        for (int nIndex = 0; nIndex < nLen; ++nIndex )
        {
            unsigned nCurChar = sBuffer[nIndex] ^ ( pEexecBuffer->unEncryptionKey >> 8);
            pEexecBuffer->unEncryptionKey = ( nCurChar + pEexecBuffer->unEncryptionKey) * 52845 + 22719;
            if ( pEexecBuffer->bASKII )
            {
                (*pEexecBuffer->pOutputFunc)( pEexecBuffer->pOutputStream, &c_sHexChars[ nCurChar >> 4 ], 1);
                (*pEexecBuffer->pOutputFunc)( pEexecBuffer->pOutputStream, &c_sHexChars[ nCurChar & 0x0f ], 1);
                pEexecBuffer->nLine += 2;
                if ( pEexecBuffer->nLine == 64)
                {
                    (*pEexecBuffer->pOutputFunc)( pEexecBuffer->pOutputStream, "\n", 1);
                    pEexecBuffer->nLine = 0;
                }
            }
            else
            {
                (*pEexecBuffer->pOutputFunc)( pEexecBuffer->pOutputStream, (char *)&nCurChar, 1);
            }
        }
    }

    bool CFontFileType1C::Parse()
    {
        Type1CIndex    oFDIndex;
        Type1CIndexVal oIndexVal;

        m_bSuccessParsed = true;

        // некоторые программы включают фонты Type 1C в пробелами в начале
        if ( m_nLen > 0 && m_sFile[0] != '\x01' )
        {
            ++m_sFile;
            --m_nLen;
        }

        GetIndex( GetU8(2, &m_bSuccessParsed), &m_oNameIndex,    &m_bSuccessParsed);
        GetIndex( m_oNameIndex.nEndPos,        &m_oTopDictIndex, &m_bSuccessParsed);
        GetIndex( m_oTopDictIndex.nEndPos,     &m_oStringIndex,  &m_bSuccessParsed);
        GetIndex( m_oStringIndex.nEndPos,      &m_oGsubrIndex,   &m_bSuccessParsed);

        if (!m_bSuccessParsed)
            return false;

        m_nGsubrBias = (m_oGsubrIndex.nCount < 1240) ? 107 : (m_oGsubrIndex.nCount < 33900) ? 1131 : 32768;

        // считываем первое имя фонта
        GetIndexVal( &m_oNameIndex, 0, &oIndexVal, &m_bSuccessParsed);

        if ( !m_bSuccessParsed )
            return false;

        m_seName = new StringExt((char *)&m_sFile[ oIndexVal.nPos ], oIndexVal.nLen);

        // Считываем самый верхний словарь для первого фонта
        ReadTopDict();

        if ( m_oTopDict.nFirstOperator == 0x0c1e )
        {
            if ( m_oTopDict.nFDArrayOffset == 0 )
            {
                m_nFDsCount = 1;
                m_pPrivateDicts = (Type1CPrivateDict *)MemUtilsMalloc(sizeof(Type1CPrivateDict));
                ReadPrivateDict(0, 0, &m_pPrivateDicts[0]);
            }
            else
            {
                GetIndex( m_oTopDict.nFDArrayOffset, &oFDIndex, &m_bSuccessParsed);
                if ( !m_bSuccessParsed )
                    return false;
                m_nFDsCount = oFDIndex.nCount;
                m_pPrivateDicts = (Type1CPrivateDict *) MemUtilsMallocArray(m_nFDsCount, sizeof(Type1CPrivateDict));
                for (int nIndex = 0; nIndex < m_nFDsCount; ++nIndex)
                {
                    GetIndexVal( &oFDIndex, nIndex, &oIndexVal, &m_bSuccessParsed);
                    if (!m_bSuccessParsed)
                        return false;
                    ReadFD( oIndexVal.nPos, oIndexVal.nLen, &m_pPrivateDicts[ nIndex ]);
                }
            }
        }
        else
        {
            m_pPrivateDicts = (Type1CPrivateDict *)MemUtilsMalloc(sizeof(Type1CPrivateDict));
            ReadPrivateDict( m_oTopDict.nPrivateOffset, m_oTopDict.nPrivateSize, &m_pPrivateDicts[0]);
        }

        if ( !m_bSuccessParsed )
            return false;

        // get the charstrings index
        if ( m_oTopDict.nCharStringsOffset <= 0 )
        {
            m_bSuccessParsed = false;
            return false;
        }

        GetIndex( m_oTopDict.nCharStringsOffset, &m_oCharStringsIndex, &m_bSuccessParsed);
        if ( !m_bSuccessParsed )
            return false;

        m_nGlyphsCount = m_oCharStringsIndex.nCount;

        if ( m_oTopDict.nFirstOperator == 0x0c1e )
        {
            ReadFDSelect();
            if (!m_bSuccessParsed)
                return false;
        }

        if ( !ReadCharset() )
        {
            m_bSuccessParsed = false;
            return false;
        }

        if ( m_oTopDict.nFirstOperator != 0x0c14 && m_oTopDict.nFirstOperator != 0x0c1e )
        {
            BuildEncoding();
            if ( !m_bSuccessParsed )
                return false;
        }

        return m_bSuccessParsed;
    }

    void CFontFileType1C::ReadTopDict()
    {
        Type1CIndexVal oTopDictPointer;

        m_oTopDict.nFirstOperator  = -1;
        m_oTopDict.nVersionSID    = 0;
        m_oTopDict.nNoticeSID     = 0;
        m_oTopDict.nCopyrightSID  = 0;
        m_oTopDict.nFullNameSID   = 0;
        m_oTopDict.nFamilyNameSID = 0;
        m_oTopDict.nWeightSID     = 0;
        m_oTopDict.nIsFixedPitch = 0;
        m_oTopDict.dItalicAngle  = 0;
        m_oTopDict.dUnderlinePosition  = -100;
        m_oTopDict.dUnderlineThickness = 50;
        m_oTopDict.nPaintType      = 0;
        m_oTopDict.nCharStringType = 2;
        m_oTopDict.arrdFontMatrix[0] = 0.001;
        m_oTopDict.arrdFontMatrix[1] = 0;
        m_oTopDict.arrdFontMatrix[2] = 0;
        m_oTopDict.arrdFontMatrix[3] = 0.001;
        m_oTopDict.arrdFontMatrix[4] = 0;
        m_oTopDict.arrdFontMatrix[5] = 0;
        m_oTopDict.bHasFontMatrix = false;
        m_oTopDict.nUniqueID = 0;
        m_oTopDict.arrdFontBBox[0] = 0;
        m_oTopDict.arrdFontBBox[1] = 0;
        m_oTopDict.arrdFontBBox[2] = 0;
        m_oTopDict.arrdFontBBox[3] = 0;
        m_oTopDict.dStrokeWidth       = 0;
        m_oTopDict.nCharsetOffset     = 0;
        m_oTopDict.nEncodingOffset    = 0;
        m_oTopDict.nCharStringsOffset = 0;
        m_oTopDict.nPrivateSize       = 0;
        m_oTopDict.nPrivateOffset     = 0;
        m_oTopDict.nRegistrySID       = 0;
        m_oTopDict.nOrderingSID       = 0;
        m_oTopDict.nSupplement        = 0;
        m_oTopDict.nFDArrayOffset     = 0;
        m_oTopDict.nFDSelectOffset    = 0;

        GetIndexVal( &m_oTopDictIndex, 0, &oTopDictPointer, &m_bSuccessParsed);
        int nPos = oTopDictPointer.nPos;
        m_nOperatorsCount = 0;
        while ( nPos < oTopDictPointer.nPos + oTopDictPointer.nLen )
        {
            nPos = GetOperator( nPos, false, &m_bSuccessParsed);
            if (!m_bSuccessParsed)
            {
                break;
            }
            if (!m_arrOperators[m_nOperatorsCount - 1].bIsNumber)
            {
                --m_nOperatorsCount;
                if (m_oTopDict.nFirstOperator < 0)
                {
                    m_oTopDict.nFirstOperator = m_arrOperators[m_nOperatorsCount].nOperator;
                }
                switch (m_arrOperators[m_nOperatorsCount].nOperator)
                {
                case 0x0000: m_oTopDict.nVersionSID         = (int)m_arrOperators[0].dNumber; break;
                case 0x0001: m_oTopDict.nNoticeSID          = (int)m_arrOperators[0].dNumber; break;
                case 0x0c00: m_oTopDict.nCopyrightSID       = (int)m_arrOperators[0].dNumber; break;
                case 0x0002: m_oTopDict.nFullNameSID        = (int)m_arrOperators[0].dNumber; break;
                case 0x0003: m_oTopDict.nFamilyNameSID      = (int)m_arrOperators[0].dNumber; break;
                case 0x0004: m_oTopDict.nWeightSID          = (int)m_arrOperators[0].dNumber; break;
                case 0x0c01: m_oTopDict.nIsFixedPitch       = (int)m_arrOperators[0].dNumber; break;
                case 0x0c02: m_oTopDict.dItalicAngle        = m_arrOperators[0].dNumber;      break;
                case 0x0c03: m_oTopDict.dUnderlinePosition  = m_arrOperators[0].dNumber;      break;
                case 0x0c04: m_oTopDict.dUnderlineThickness = m_arrOperators[0].dNumber;      break;
                case 0x0c05: m_oTopDict.nPaintType          = (int)m_arrOperators[0].dNumber; break;
                case 0x0c06: m_oTopDict.nCharStringType     = (int)m_arrOperators[0].dNumber; break;
                case 0x0c07: m_oTopDict.arrdFontMatrix[0]   = m_arrOperators[0].dNumber;
                             m_oTopDict.arrdFontMatrix[1]   = m_arrOperators[1].dNumber;
                             m_oTopDict.arrdFontMatrix[2]   = m_arrOperators[2].dNumber;
                             m_oTopDict.arrdFontMatrix[3]   = m_arrOperators[3].dNumber;
                             m_oTopDict.arrdFontMatrix[4]   = m_arrOperators[4].dNumber;
                             m_oTopDict.arrdFontMatrix[5]   = m_arrOperators[5].dNumber;
                             m_oTopDict.bHasFontMatrix      = true;                           break;
                case 0x000d: m_oTopDict.nUniqueID           = (int)m_arrOperators[0].dNumber; break;
                case 0x0005: m_oTopDict.arrdFontBBox[0]     = m_arrOperators[0].dNumber;
                             m_oTopDict.arrdFontBBox[1]     = m_arrOperators[1].dNumber;
                             m_oTopDict.arrdFontBBox[2]     = m_arrOperators[2].dNumber;
                             m_oTopDict.arrdFontBBox[3]     = m_arrOperators[3].dNumber;      break;
                case 0x0c08: m_oTopDict.dStrokeWidth        = m_arrOperators[0].dNumber;      break;
                case 0x000f: m_oTopDict.nCharsetOffset      = (int)m_arrOperators[0].dNumber; break;
                case 0x0010: m_oTopDict.nEncodingOffset     = (int)m_arrOperators[0].dNumber; break;
                case 0x0011: m_oTopDict.nCharStringsOffset  = (int)m_arrOperators[0].dNumber; break;
                case 0x0012: m_oTopDict.nPrivateSize        = (int)m_arrOperators[0].dNumber;
                             m_oTopDict.nPrivateOffset      = (int)m_arrOperators[1].dNumber; break;
                case 0x0c1e: m_oTopDict.nRegistrySID        = (int)m_arrOperators[0].dNumber;
                             m_oTopDict.nOrderingSID        = (int)m_arrOperators[1].dNumber;
                             m_oTopDict.nSupplement         = (int)m_arrOperators[2].dNumber; break;
                case 0x0c24: m_oTopDict.nFDArrayOffset      = (int)m_arrOperators[0].dNumber; break;
                case 0x0c25: m_oTopDict.nFDSelectOffset     = (int)m_arrOperators[0].dNumber; break;
                }
                m_nOperatorsCount = 0;
            }
        }
    }

    // Читаем словарь шрифта (CID Font Dict (FD)). Отсюда вытаскиваем указатель на private dict,
    // и читаем private dict. Также вытаксиваем FontMatrix.
    void CFontFileType1C::ReadFD(int nOffset, int nLength, Type1CPrivateDict *pDict)
    {
        double arrdFontMatrix[6];
        bool bHasFontMatrix = false;
        int nPrivateSize = 0, nPrivateOffset = 0;
        int nPos = nOffset;
        m_nOperatorsCount = 0;
        while ( nPos < nOffset + nLength )
        {
            nPos = GetOperator( nPos, false, &m_bSuccessParsed);
            if ( !m_bSuccessParsed )
                return;

            if ( !m_arrOperators[m_nOperatorsCount - 1].bIsNumber )
            {
                if ( m_arrOperators[m_nOperatorsCount - 1].nOperator == 0x0012 )
                {
                    if ( m_nOperatorsCount < 3 )
                    {
                        m_bSuccessParsed = false;
                        return;
                    }
                    nPrivateSize = (int)m_arrOperators[0].dNumber;
                    nPrivateOffset = (int)m_arrOperators[1].dNumber;
                    break;
                }
                else if ( m_arrOperators[m_nOperatorsCount - 1].nOperator == 0x0c07 )
                {
                    arrdFontMatrix[0] = m_arrOperators[0].dNumber;
                    arrdFontMatrix[1] = m_arrOperators[1].dNumber;
                    arrdFontMatrix[2] = m_arrOperators[2].dNumber;
                    arrdFontMatrix[3] = m_arrOperators[3].dNumber;
                    arrdFontMatrix[4] = m_arrOperators[4].dNumber;
                    arrdFontMatrix[5] = m_arrOperators[5].dNumber;
                    bHasFontMatrix = true;
                }
                m_nOperatorsCount = 0;
            }
        }
        ReadPrivateDict( nPrivateOffset, nPrivateSize, pDict);
        if ( bHasFontMatrix )
        {
            pDict->arrdFontMatrix[0] = arrdFontMatrix[0];
            pDict->arrdFontMatrix[1] = arrdFontMatrix[1];
            pDict->arrdFontMatrix[2] = arrdFontMatrix[2];
            pDict->arrdFontMatrix[3] = arrdFontMatrix[3];
            pDict->arrdFontMatrix[4] = arrdFontMatrix[4];
            pDict->arrdFontMatrix[5] = arrdFontMatrix[5];
            pDict->bHasFontMatrix = true;
        }
    }

    void CFontFileType1C::ReadPrivateDict(int nOffset, int nLength, Type1CPrivateDict *pDict)
    {
        pDict->bHasFontMatrix = false;
        pDict->nBlueValues       = 0;
        pDict->nOtherBlues       = 0;
        pDict->nFamilyBlues      = 0;
        pDict->nFamilyOtherBlues = 0;
        pDict->dBlueScale        = 0.039625;
        pDict->nBlueShift        = 7;
        pDict->nBlueFuzz         = 1;
        pDict->bHasStdHW = false;
        pDict->bHasStdVW = false;
        pDict->nStemSnapH = 0;
        pDict->nStemSnapV = 0;
        pDict->bHasForceBold       = false;
        pDict->dForceBoldThreshold = 0;
        pDict->nLanguageGroup      = 0;
        pDict->dExpansionFactor    = 0.06;
        pDict->nInitialRandomSeed  = 0;
        pDict->nSubrsOffset        = 0;
        pDict->dDefaultWidthX   = 0;
        pDict->bDefaultWidthXFP = false;
        pDict->dNominalWidthX   = 0;
        pDict->bNominalWidthXFP = false;

        if ( nOffset == 0 || nLength == 0 )
            return;

        int nPos = nOffset;
        m_nOperatorsCount = 0;

        while ( nPos < nOffset + nLength )
        {
            nPos = GetOperator( nPos, false, &m_bSuccessParsed);
            if (!m_bSuccessParsed)
                break;

            if (!m_arrOperators[m_nOperatorsCount - 1].bIsNumber)
            {
                --m_nOperatorsCount;

                switch ( m_arrOperators[m_nOperatorsCount].nOperator)
                {
                case 0x0006:
                    pDict->nBlueValues = GetDeltaIntArray( pDict->arrnBlueValues, type1CMaxBlueValues);
                    break;
                case 0x0007:
                    pDict->nOtherBlues = GetDeltaIntArray(pDict->arrnOtherBlues, type1CMaxOtherBlues);
                    break;
                case 0x0008:
                    pDict->nFamilyBlues = GetDeltaIntArray(pDict->arrnFamilyBlues, type1CMaxBlueValues);
                    break;
                case 0x0009:
                    pDict->nFamilyOtherBlues = GetDeltaIntArray(pDict->arrnFamilyOtherBlues, type1CMaxOtherBlues);
                    break;
                case 0x0c09:
                    pDict->dBlueScale = m_arrOperators[0].dNumber;
                    break;
                case 0x0c0a:
                    pDict->nBlueShift = (int)m_arrOperators[0].dNumber;
                    break;
                case 0x0c0b:
                    pDict->nBlueFuzz = (int)m_arrOperators[0].dNumber;
                    break;
                case 0x000a:
                    pDict->dStdHW = m_arrOperators[0].dNumber;
                    pDict->bHasStdHW = true;
                    break;
                case 0x000b:
                    pDict->dStdVW = m_arrOperators[0].dNumber;
                    pDict->bHasStdVW = true;
                    break;
                case 0x0c0c:
                    pDict->nStemSnapH = GetDeltaDoubleArray(pDict->arrdStemSnapH, type1CMaxStemSnap);
                    break;
                case 0x0c0d:
                    pDict->nStemSnapV = GetDeltaDoubleArray(pDict->arrdStemSnapV, type1CMaxStemSnap);
                    break;
                case 0x0c0e:
                    pDict->bForceBold = m_arrOperators[0].dNumber != 0;
                    pDict->bHasForceBold = true;
                    break;
                case 0x0c0f:
                    pDict->dForceBoldThreshold = m_arrOperators[0].dNumber;
                    break;
                case 0x0c11:
                    pDict->nLanguageGroup = (int)m_arrOperators[0].dNumber;
                    break;
                case 0x0c12:
                    pDict->dExpansionFactor = m_arrOperators[0].dNumber;
                    break;
                case 0x0c13:
                    pDict->nInitialRandomSeed = (int)m_arrOperators[0].dNumber;
                    break;
                case 0x0013:
                    pDict->nSubrsOffset = nOffset + (int)m_arrOperators[0].dNumber;
                    break;
                case 0x0014:
                    pDict->dDefaultWidthX = m_arrOperators[0].dNumber;
                    pDict->bDefaultWidthXFP = m_arrOperators[0].bIsFloat;
                    break;
                case 0x0015:
                    pDict->dNominalWidthX = m_arrOperators[0].dNumber;
                    pDict->bNominalWidthXFP = m_arrOperators[0].bIsFloat;
                    break;
                }
                m_nOperatorsCount = 0;
            }
        }
    }

    void CFontFileType1C::ReadFDSelect()
    {
        int nRanges = 0;

        m_pnFDSelect = (unsigned char *)MemUtilsMalloc( m_nGlyphsCount );

        if (m_oTopDict.nFDSelectOffset == 0)
        {
            for (int nIndex = 0; nIndex < m_nGlyphsCount; ++nIndex )
            {
                m_pnFDSelect[nIndex] = 0;
            }
        }
        else
        {
            int nPos = m_oTopDict.nFDSelectOffset;
            int nFDSelectFormat = GetU8( nPos++, &m_bSuccessParsed);
            if (!m_bSuccessParsed)
            {
                return;
            }
            if ( nFDSelectFormat == 0 )
            {
                if (!CheckRegion( nPos, m_nGlyphsCount))
                {
                    m_bSuccessParsed = false;
                    return;
                }
                memcpy( m_pnFDSelect, m_sFile + nPos, m_nGlyphsCount);
            }
            else if ( nFDSelectFormat == 3 )
            {
                nRanges = GetU16BE( nPos, &m_bSuccessParsed);
                nPos += 2;
                int nGID0 = GetU16BE( nPos, &m_bSuccessParsed);
                nPos += 2;
                for (int nIndex = 1; nIndex <= nRanges; ++nIndex )
                {
                    int nCurFD = GetU8( nPos++, &m_bSuccessParsed);
                    int nGID1 = GetU16BE( nPos, &m_bSuccessParsed);
                    if ( !m_bSuccessParsed )
                        return;

                    nPos += 2;
                    if ( nGID0 > nGID1 || nGID1 > m_nGlyphsCount)
                    {
                        // TO DO: error "Bad FDSelect table in CID font"
                        m_bSuccessParsed = false;
                        return;
                    }
                    for (int nJ = nGID0; nJ < nGID1; ++nJ )
                    {
                        m_pnFDSelect[ nJ ] = nCurFD;
                    }
                    nGID0 = nGID1;
                }
            }
            else
            {
                // TO DO: error "Unknown FDSelect table format in CID font"
                for (int nIndex = 0; nIndex < m_nGlyphsCount; ++nIndex )
                {
                    m_pnFDSelect[nIndex] = 0;
                }
            }
        }
    }

    void CFontFileType1C::BuildEncoding()
    {
        char sBuffer[256];
        int nCodes, nRanges;
        int nChar = 0, nLeft, nSups;

        if (m_oTopDict.nEncodingOffset == 0)
        {
            m_arrEncoding = c_arrsFontFileType1StandardEncoding;
        }
        else if (m_oTopDict.nEncodingOffset == 1)
        {
            m_arrEncoding = c_arrsFontFileType1ExpertEncoding;
        }
        else
        {
            m_arrEncoding = (char **)MemUtilsMallocArray(256, sizeof(char *));
            for (int nIndex = 0; nIndex < 256; ++nIndex)
            {
                m_arrEncoding[nIndex] = NULL;
            }
            int nPos = m_oTopDict.nEncodingOffset;
            int nEncodingFormat = GetU8( nPos++, &m_bSuccessParsed);

            if (!m_bSuccessParsed)
                return;

            if ( ( nEncodingFormat & 0x7f ) == 0 )
            {
                nCodes = 1 + GetU8( nPos++, &m_bSuccessParsed);
                if (!m_bSuccessParsed)
                    return;
                if (nCodes > m_nGlyphsCount)
                {
                    nCodes = m_nGlyphsCount;
                }
                for (int nIndex = 1; nIndex < nCodes; ++nIndex )
                {
                    nChar = GetU8( nPos++, &m_bSuccessParsed);
                    if (!m_bSuccessParsed)
                        return;
                    if (m_arrEncoding[ nChar ])
                    {
                        MemUtilsFree(m_arrEncoding[ nChar ]);
                    }
                    m_arrEncoding[ nChar ] = CopyString( GetString( m_pnCharset[ nIndex ], sBuffer, &m_bSuccessParsed));
                }
            }
            else if ( (nEncodingFormat & 0x7f) == 1 )
            {
                nRanges = GetU8( nPos++, &m_bSuccessParsed);
                if (!m_bSuccessParsed)
                    return;

                nCodes = 1;
                for (int nIndex = 0; nIndex < nRanges; ++nIndex )
                {
                    nChar = GetU8( nPos++, &m_bSuccessParsed);
                    nLeft = GetU8( nPos++, &m_bSuccessParsed);
                    if (!m_bSuccessParsed)
                        return;

                    for (int nJ = 0; nJ <= nLeft && nCodes < m_nGlyphsCount; ++nJ )
                    {
                        if ( nChar < 256 )
                        {
                            if (m_arrEncoding[ nChar ])
                            {
                                MemUtilsFree(m_arrEncoding[ nChar ]);
                            }
                            m_arrEncoding[ nChar ] = CopyString( GetString( m_pnCharset[nCodes], sBuffer, &m_bSuccessParsed ) );
                        }
                        ++nCodes;
                        ++nChar;
                    }
                }
            }
            if ( nEncodingFormat & 0x80 )
            {
                nSups = GetU8( nPos++, &m_bSuccessParsed);
                if (!m_bSuccessParsed)
                    return;

                for (int nIndex = 0; nIndex < nSups; ++nIndex )
                {
                    nChar = GetU8( nPos++, &m_bSuccessParsed);
                    if (!m_bSuccessParsed)
                        return;

                    int nSID = GetU16BE( nPos, &m_bSuccessParsed);
                    nPos += 2;

                    if (!m_bSuccessParsed)
                        return;

                    if (m_arrEncoding[ nChar ])
                    {
                        MemUtilsFree(m_arrEncoding[ nChar ]);
                    }
                    m_arrEncoding[ nChar ] = CopyString( GetString( nSID, sBuffer, &m_bSuccessParsed ) );
                }
            }
        }
    }

    bool CFontFileType1C::ReadCharset()
    {
        int nChar = 0;
        int nLeft, nIndex = 0;

        if (m_oTopDict.nCharsetOffset == 0)
        {
            m_pnCharset = c_arrnFontFileType1CISOAdobeCharset;
			if (m_nGlyphsCount > 229)
				m_nGlyphsCount = 229;
        }
        else if (m_oTopDict.nCharsetOffset == 1)
        {
            m_pnCharset = c_arrnFontFileType1CExpertCharset;
			if (m_nGlyphsCount > 166)
				m_nGlyphsCount = 166;
		}
        else if (m_oTopDict.nCharsetOffset == 2)
        {
            m_pnCharset = c_arrnFontFileType1CExpertSubsetCharset;
			if (m_nGlyphsCount > 87)
				m_nGlyphsCount = 87;
		}
        else
        {
            m_pnCharset = (unsigned short *)MemUtilsMallocArray( m_nGlyphsCount, sizeof(unsigned short));
            for (nIndex = 0; nIndex < m_nGlyphsCount; ++nIndex)
            {
                m_pnCharset[nIndex] = 0;
            }
            int nPos = m_oTopDict.nCharsetOffset;
            int nCharsetFormat = GetU8( nPos++, &m_bSuccessParsed);

            if ( nCharsetFormat == 0 )
            {
                for (nIndex = 1; nIndex < m_nGlyphsCount; ++nIndex)
                {
                    m_pnCharset[nIndex] = (unsigned short)GetU16BE( nPos, &m_bSuccessParsed);
                    nPos += 2;
                    if ( !m_bSuccessParsed )
                        break;
                }
            }
            else if ( nCharsetFormat == 1 )
            {
                nIndex = 1;
                while ( nIndex < m_nGlyphsCount )
                {
                    nChar = GetU16BE( nPos, &m_bSuccessParsed);
                    nPos += 2;
                    nLeft = GetU8( nPos++, &m_bSuccessParsed);
                    if (!m_bSuccessParsed)
                        break;
                    for (int nJ = 0; nJ <= nLeft && nIndex < m_nGlyphsCount; ++nJ )
                    {
                        m_pnCharset[nIndex++] = (unsigned short)nChar++;
                    }
                }
            }
            else if ( nCharsetFormat == 2 )
            {
                nIndex = 1;
                while ( nIndex < m_nGlyphsCount )
                {
                    nChar = GetU16BE( nPos, &m_bSuccessParsed);
                    nPos += 2;
                    nLeft = GetU16BE( nPos, &m_bSuccessParsed);
                    nPos += 2;
                    if ( !m_bSuccessParsed )
                        break;

                    for (int nJ = 0; nJ <= nLeft && nIndex < m_nGlyphsCount; ++nJ )
                    {
                        m_pnCharset[nIndex++] = (unsigned short)nChar++;
                    }
                }
            }
            if (!m_bSuccessParsed)
            {
                MemUtilsFree(m_pnCharset);
                m_pnCharset = NULL;
                return false;
            }
        }
        return true;
    }

    int CFontFileType1C::GetOperator(int nPos, bool bCharString, bool *pbSuccess)
    {
        static char c_sNumChars[16] = "0123456789.ee -";
        Type1COperator oOperator;
        char sBuffer[65];
        int nFirstChar, nSecondChar, nFC, nSC, nTempValue = 0;

        nFirstChar = GetU8( nPos++, pbSuccess);
        oOperator.bIsNumber = true;
        oOperator.bIsFloat = false;

        if ( nFirstChar == 28 )
        {
            nTempValue = GetU8( nPos++, pbSuccess);
            nTempValue = (nTempValue << 8) | GetU8( nPos++, pbSuccess);
            if ( nTempValue & 0x8000 )
            {
                nTempValue |= ~0xffff;
            }
            oOperator.dNumber = nTempValue;

        }
        else if ( !bCharString && nFirstChar == 29 )
        {
            nTempValue = GetU8( nPos++, pbSuccess);
            nTempValue = (nTempValue << 8) | GetU8( nPos++, pbSuccess);
            nTempValue = (nTempValue << 8) | GetU8( nPos++, pbSuccess);
            nTempValue = (nTempValue << 8) | GetU8( nPos++, pbSuccess);
            if ( nTempValue & 0x80000000 )
            {
                nTempValue |= ~0xffffffff;
            }
            oOperator.dNumber = nTempValue;

        }
        else if ( !bCharString && nFirstChar == 30 )
        {
            int nIndex = 0;
            do
            {
                nSecondChar = GetU8( nPos++, pbSuccess);
                nFC = nSecondChar >> 4;
                nSC = nSecondChar & 0x0f;
                if ( nFC == 0xf )
                {
                    break;
                }
                sBuffer[ nIndex++ ] = c_sNumChars[ nFC ];
                if ( nIndex == 64 )
                {
                    break;
                }
                if ( nFC == 0xc )
                {
                    sBuffer[nIndex++] = '-';
                }
                if ( nIndex == 64 )
                {
                    break;
                }
                if ( nSC == 0xf )
                {
                    break;
                }
                sBuffer[nIndex++] = c_sNumChars[ nSC ];
                if ( nIndex == 64 )
                {
                    break;
                }
                if ( nSC == 0xc )
                {
                    sBuffer[nIndex++] = '-';
                }
            } while (nIndex < 64);
            sBuffer[nIndex] = '\0';
            oOperator.dNumber = atof( sBuffer );
            oOperator.bIsFloat = true;

        }
        else if ( nFirstChar >= 32 && nFirstChar <= 246)
        {
            oOperator.dNumber = nFirstChar - 139;
        }
        else if ( nFirstChar >= 247 && nFirstChar <= 250)
        {
            oOperator.dNumber = (( nFirstChar - 247) << 8) + GetU8( nPos++, pbSuccess) + 108;
        }
        else if ( nFirstChar >= 251 && nFirstChar <= 254)
        {
            oOperator.dNumber = -(( nFirstChar - 251) << 8) - GetU8( nPos++, pbSuccess) - 108;
        }
        else if ( bCharString && nFirstChar == 255 )
        {
            nTempValue = GetU8( nPos++, pbSuccess);
            nTempValue = (nTempValue << 8) | GetU8( nPos++, pbSuccess);
            nTempValue = (nTempValue << 8) | GetU8( nPos++, pbSuccess);
            nTempValue = (nTempValue << 8) | GetU8( nPos++, pbSuccess);
            if ( nTempValue & 0x80000000 )
            {
                nTempValue |= ~0xffffffff;
            }
            oOperator.dNumber = (double)nTempValue / 65536.0;
            oOperator.bIsFloat = true;

        }
        else if ( nFirstChar == 12 )
        {
            oOperator.bIsNumber = false;
            oOperator.nOperator = 0x0c00 + GetU8( nPos++, pbSuccess);
        }
        else
        {
            oOperator.bIsNumber = false;
            oOperator.nOperator = nFirstChar;
        }

        if ( m_nOperatorsCount < 49 )
        {
            m_arrOperators[m_nOperatorsCount++] = oOperator;
        }

        return nPos;
    }

    // Конвертируем delta-encoded массив операторов в массив ints
    int CFontFileType1C::GetDeltaIntArray(int *pArray, int nMaxLen)
    {
        int nCount = 0;

        if ( ( nCount = m_nOperatorsCount ) > nMaxLen )
        {
            nCount = nMaxLen;
        }
        int nTemp = 0;
        for ( int nIndex = 0; nIndex < nCount; ++nIndex )
        {
            nTemp += (int)m_arrOperators[nIndex].dNumber;
            pArray[nIndex] = nTemp;
        }
        return nCount;
    }

    // Конвертируем delta-encoded массив операторов в массив doubles
    int CFontFileType1C::GetDeltaDoubleArray(double *pArray, int nMaxLen)
    {
        int nCount = 0;

        if ( ( nCount = m_nOperatorsCount ) > nMaxLen )
        {
            nCount = nMaxLen;
        }
        double dTemp = 0;
        for (int nIndex = 0; nIndex < nCount; ++nIndex)
        {
            dTemp += m_arrOperators[nIndex].dNumber;
            pArray[nIndex] = dTemp;
        }
        return nCount;
    }

    void CFontFileType1C::GetIndex(int nPos, Type1CIndex *pIndex, bool *bSuccess)
    {
        pIndex->nPos = nPos;
        pIndex->nCount = GetU16BE( nPos, bSuccess);
        if ( 0 == pIndex->nCount )
        {
            // возможны пустые индексы, они содержат только поле length
            pIndex->nOffsetSize = 0;
            pIndex->nStartPos = pIndex->nEndPos = nPos + 2;
        }
        else
        {
            pIndex->nOffsetSize = GetU8( nPos + 2, bSuccess);
            if ( pIndex->nOffsetSize < 1 || pIndex->nOffsetSize > 4 )
            {
                *bSuccess = false;
            }
            pIndex->nStartPos = nPos + 3 + ( pIndex->nCount + 1 ) * pIndex->nOffsetSize - 1;
            if ( pIndex->nStartPos < 0 || pIndex->nStartPos >= m_nLen )
            {
                *bSuccess = false;
            }
            pIndex->nEndPos = pIndex->nStartPos + GetUVarBE( nPos + 3 + pIndex->nCount * pIndex->nOffsetSize, pIndex->nOffsetSize, bSuccess);
            if ( pIndex->nEndPos < pIndex->nStartPos || pIndex->nEndPos > m_nLen )
            {
                *bSuccess = false;
            }
        }
    }

    void CFontFileType1C::GetIndexVal(Type1CIndex *pIndex, int nIndex, Type1CIndexVal *pIndexVal, bool *bSuccess)
    {
        if ( nIndex < 0 || nIndex >= pIndex->nCount )
        {
            *bSuccess = false;
            return;
        }
        int nPos0 = pIndex->nStartPos + GetUVarBE( pIndex->nPos + 3 + nIndex * pIndex->nOffsetSize, pIndex->nOffsetSize, bSuccess);
        int nPos1 = pIndex->nStartPos + GetUVarBE( pIndex->nPos + 3 + (nIndex + 1) * pIndex->nOffsetSize, pIndex->nOffsetSize, bSuccess);

        if ( nPos0 < pIndex->nStartPos || nPos0 > pIndex->nEndPos || nPos1 <= pIndex->nStartPos || nPos1 > pIndex->nEndPos || nPos1 < nPos0 )
        {
            *bSuccess = false;
        }
        pIndexVal->nPos = nPos0;
        pIndexVal->nLen = nPos1 - nPos0;
    }

    char *CFontFileType1C::GetString(int nSID, char *sBuffer, bool *pbSuccess)
    {
        Type1CIndexVal oIndexVal;
        int nCount = 0;

        if ( nSID < 391 )
        {
            strcpy( sBuffer, c_arrsFontFileType1CStandardStrings[ nSID ] );
        }
        else
        {
            nSID -= 391;
            GetIndexVal(&m_oStringIndex, nSID, &oIndexVal, pbSuccess);
            if ( *pbSuccess )
            {
                if ( ( nCount = oIndexVal.nLen ) > 255 )
                {
                    nCount = 255;
                }
                strncpy( sBuffer, (char *)&m_sFile[ oIndexVal.nPos ], nCount);
                sBuffer[ nCount ] = '\0';
            }
            else
            {
                sBuffer[0] = '\0';
            }
        }
        return sBuffer;
    }
}
