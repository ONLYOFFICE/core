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
#include "FontFileEncodings.h"
#include "FontFileType1.h"

namespace NSFontConverter
{
    #define LINESIZE 1024

    #define PFB_MARKER 128
    #define PFB_ASCII    1
    #define PFB_BINARY   2
    #define PFB_DONE     3


    #define IS_PS_NEWLINE( ch ) \
      ( (ch) == '\r' ||         \
        (ch) == '\n' )

    #define IS_PS_SPACE( ch )  \
      ( (ch) == ' '         || \
        IS_PS_NEWLINE( ch ) || \
        (ch) == '\t'        || \
        (ch) == '\f'        || \
        (ch) == '\0' )

    // Таблица для быстрого конвертирования цифр (десятичных и не десятичных) в числа
    static const signed char c_arrCharTable[128] =
    {
        /* 0x00 */
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
        25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
        25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1, -1,
    };

    // Ни один символ больше >= 0x80 не может представлять число
    #define OP  >=

    #define WriteChar(Value) \
        nChar = (char)(Value);\
        pOutputFunc( pOutputStream, &nChar, 1 );

    unsigned int EexecDecode   (unsigned char** cursor, unsigned char* limit, unsigned char* buffer, unsigned int n, unsigned short* seed )
    {
        unsigned char*  p;
        unsigned int r;
        unsigned int s = *seed;

        p = *cursor;
        if ( n > (unsigned int)(limit - p) )
          n = (unsigned int)(limit - p);

        for ( r = 0; r < n; r++ )
        {
          unsigned int  val = p[r];
          unsigned int  b   = ( val ^ ( s >> 8 ) );


          s         = ( (val + s)*52845U + 22719 ) & 0xFFFFU;
          buffer[r] = (unsigned char) b;
        }

        *cursor = p + n;
        *seed   = (unsigned short)s;

        return r;
      }

    unsigned int ASCIIHexDecode(unsigned char** cursor, unsigned char* limit, unsigned char* buffer, unsigned int n)
    {
        unsigned char*  p;
        unsigned int  r   = 0;
        unsigned int  w   = 0;
        unsigned int  pad = 0x01;


        n *= 2;

        p  = *cursor;
        if ( n > (unsigned int)( limit - p ) )
          n = (unsigned int)( limit - p );

        /* we try to process two nibbles at a time to be as fast as possible */
        for ( ; r < n; r++ )
        {
          FT_UInt  c = p[r];


          if ( IS_PS_SPACE( c ) )
            continue;

          if ( c OP 0x80 )
            break;

          c = c_arrCharTable[c & 0x7F];
          if ( (unsigned)c >= 16 )
            break;

          pad = ( pad << 4 ) | c;
          if ( pad & 0x100 )
          {
            buffer[w++] = (FT_Byte)pad;
            pad         = 0x01;
          }
        }

        if ( pad != 0x01 )
          buffer[w++] = (FT_Byte)( pad << 4 );

        *cursor = p + r;

        return w;
      }
    //------------------------------------------------------------------------
    // CFontFileType1
    //------------------------------------------------------------------------

    CFontFileType1 *CFontFileType1::LoadFromBuffer(char *sBuffer, int nLen)
    {
        return new CFontFileType1(sBuffer, nLen, false);
    }

    CFontFileType1 *CFontFileType1::LoadFromFile(const wchar_t *wsFileName)
    {
        char *sBuffer;
        int nLen = 0;

        if ( !( sBuffer = CFontFileBase::ReadFile(wsFileName, &nLen) ) )
            return NULL;

        return new CFontFileType1(sBuffer, nLen, true);
    }

    CFontFileType1::CFontFileType1(char *sBuffer, int nLen, bool bFreeData):
    CFontFileBase(sBuffer, nLen, bFreeData)
    {
        m_sName = NULL;
        m_arrEncoding = NULL;

        Parse();
        m_bParsed = false;
    }

    CFontFileType1::~CFontFileType1()
    {
        if (m_sName)
            MemUtilsFree(m_sName);

        if ( m_arrEncoding && m_arrEncoding != c_arrsFontFileType1StandardEncoding )
        {
            for (int nIndex = 0; nIndex < 256; ++nIndex )
            {
                MemUtilsFree( m_arrEncoding[nIndex] );
            }
            MemUtilsFree(m_arrEncoding);
        }
    }

    char *CFontFileType1::GetName()
    {
        if ( !m_bParsed )
            Parse();

        return m_sName;
    }

    char **CFontFileType1::GetEncoding()
    {
        if (!m_bParsed)
            Parse();

        return m_arrEncoding;
    }

    void CFontFileType1::WriteEncoded(char **ppNewEncoding, FontFileOutputFunc pOutputFunc, void *pOutputStream)
    {
        char sBuffer[512];
        char *sLine, *sLine2, *sCurChar;

        // копируем все до строчки /Encoding
        for ( sLine = (char *)m_sFile; sLine && strncmp( sLine, "/Encoding", 9); sLine = GetNextLine(sLine) );
        if ( !sLine )
        {
            // не нашли кодировку, тогда копируем целиком фонт файл
            (*pOutputFunc)( pOutputStream, (char *)m_sFile, m_nLen);
            return;
        }
        (*pOutputFunc)( pOutputStream, (char *)m_sFile, sLine - (char *)m_sFile);

        // пишем новую кодировку
        (*pOutputFunc)( pOutputStream, "/Encoding 256 array\n", 20);
        (*pOutputFunc)( pOutputStream, "0 1 255 {1 index exch /.notdef put} for\n", 40);
        for ( int nIndex = 0; nIndex < 256; ++nIndex )
        {
            if (ppNewEncoding[nIndex])
            {
                sprintf( sBuffer, "dup %d /%s put\n", nIndex, ppNewEncoding[nIndex]);
                (*pOutputFunc)( pOutputStream, sBuffer, strlen( sBuffer ));
            }
        }
        (*pOutputFunc)( pOutputStream, "readonly def\n", 13);

        if ( !strncmp( sLine, "/Encoding StandardEncoding def", 30) )
        {
            sLine = GetNextLine(sLine);
        }
        else
        {
            sCurChar = sLine + 10;
            sLine = NULL;
            for (; sCurChar < (char *)m_sFile + m_nLen; ++sCurChar)
            {
                if ((*sCurChar == ' ' || *sCurChar == '\t' || *sCurChar == '\x0a' || *sCurChar == '\x0d' || *sCurChar == '\x0c' || *sCurChar == '\0') && sCurChar + 4 <= (char *)m_sFile + m_nLen && !strncmp(sCurChar + 1, "def", 3) )
                {
                    sLine = sCurChar + 4;
                    break;
                }
            }
        }

        // У некоторых фонтов две записи /Encoding, поэтому проверяем наличие второй записи
        if ( sLine )
        {
            int nIndex;
            for ( sLine2 = sLine, nIndex = 0; nIndex < 20 && sLine2 && strncmp(sLine2, "/Encoding", 9); sLine2 = GetNextLine(sLine2), ++nIndex) ;
            if ( nIndex < 20 && sLine2 )
            {
                (*pOutputFunc)( pOutputStream, sLine, sLine2 - sLine);
                if ( !strncmp(sLine2, "/Encoding StandardEncoding def", 30) )
                {
                    sLine = GetNextLine( sLine2 );
                }
                else
                {
                    sCurChar = sLine2 + 10;
                    sLine = NULL;
                    for (; sCurChar < (char *)m_sFile + m_nLen; ++sCurChar)
                    {
                        if ((*sCurChar == ' ' || *sCurChar == '\t' || *sCurChar == '\x0a' || *sCurChar == '\x0d' || *sCurChar == '\x0c' || *sCurChar == '\0') && sCurChar + 4 <= (char *)m_sFile + m_nLen && !strncmp(sCurChar + 1, "def", 3) )
                        {
                            sLine = sCurChar + 4;
                            break;
                        }
                    }
                }
            }

            // копируем все после кодировки
            if ( sLine )
            {
                (*pOutputFunc)( pOutputStream, sLine, ((char *)m_sFile + m_nLen) - sLine );
            }
        }
    }

    char *CFontFileType1::GetNextLine(char *sLine)
    {
        while ( sLine < (char *)m_sFile + m_nLen && *sLine != '\x0a' && *sLine != '\x0d')
            ++sLine;

        if ( sLine < (char *)m_sFile + m_nLen && *sLine == '\x0d')
            ++sLine;

        if ( sLine < (char *)m_sFile + m_nLen && *sLine == '\x0a')
            ++sLine;

        if ( sLine >= (char *)m_sFile + m_nLen )
            return NULL;

        return sLine;
    }

    void CFontFileType1::Parse()
    {
        // Сначала проверим, если это pfb файл, тогда избавимся от всех его маркеров.
        Reset();

        while( m_nPos < m_nLen && ( ' ' == m_sFile[m_nPos] || '\t' == m_sFile[m_nPos] || '\r' == m_sFile[m_nPos] || '\n' == m_sFile[m_nPos] ) )
            ++m_nPos;

        bool bSuccess = true;
        int nChar = GetU8( m_nPos, &bSuccess );
        if ( !bSuccess || ( PFB_MARKER != nChar && '%' != nChar ) )
            return;
        else if ( PFB_MARKER == nChar )
        {
            if ( !RemovePfbMarkers() )
                return;
        }

        char *sLine, *sLine1, *pCur, *pTemp;
        char sBuffer[256];
        int nCount, nCode;
        int nIndex = 0;
        unsigned char *sEexec = NULL;

        m_oTopDict.arrdFontBBox[0] = 0; m_oTopDict.arrdFontBBox[1] = 0;
        m_oTopDict.arrdFontBBox[2] = 0; m_oTopDict.arrdFontBBox[3] = 0;

        m_oTopDict.arrdFontMatrix[0] = 0.001; m_oTopDict.arrdFontMatrix[1] = 0;
        m_oTopDict.arrdFontMatrix[2] = 0;     m_oTopDict.arrdFontMatrix[3] = 0.001;
        m_oTopDict.arrdFontMatrix[4] = 0;     m_oTopDict.arrdFontMatrix[5] = 0;

        for (nIndex = 1, sLine = (char *)m_sFile; nIndex <= 100 && sLine && (!m_sName || !m_arrEncoding); ++nIndex )
        {
            if ( !m_sName && !strncmp( sLine, "/FontName", 9) )
            {
                strncpy( sBuffer, sLine, 255);
                sBuffer[255] = '\0';
                if ( ( pCur = strchr( sBuffer + 9, '/' ) ) && ( pCur = strtok( pCur + 1, " \t\n\r" ) ) )
                {
                    m_sName = CopyString( pCur );
                }
                sLine = GetNextLine(sLine);

            }
            else if ( !strncmp( sLine, "/FontMatrix", 11 ) )
            {
                strncpy( sBuffer, sLine, 255);
                sBuffer[255] = '\0';

                ReadDoubleArray<6>( (unsigned char*)(sBuffer + 11), 244, m_oTopDict.arrdFontMatrix );
                sLine = GetNextLine( sLine );
            }
            else if ( !strncmp( sLine, "/FontBBox", 9 ) )
            {
                strncpy( sBuffer, sLine, 255);
                sBuffer[255] = '\0';

                ReadDoubleArray<4>( (unsigned char*)(sBuffer + 9), 246, m_oTopDict.arrdFontBBox );
                sLine = GetNextLine( sLine );
            }
            else if (!m_arrEncoding && !strncmp( sLine, "/Encoding StandardEncoding def", 30))
            {
                m_arrEncoding = c_arrsFontFileType1StandardEncoding;
            }
            else if (!m_arrEncoding && !strncmp( sLine, "/Encoding 256 array", 19))
            {
                m_arrEncoding = (char **)MemUtilsMallocArray(256, sizeof(char *));
                int nJ = 0;
                for (nJ = 0; nJ < 256; ++nJ )
                {
                    m_arrEncoding[nJ] = NULL;
                }
                for (nJ = 0, sLine = GetNextLine(sLine); nJ < 300 && sLine && ( sLine1 = GetNextLine( sLine )); ++nJ, sLine = sLine1)
                {
                    if ( ( nCount = sLine1 - sLine ) > 255 )
                    {
                        nCount = 255;
                    }
                    strncpy( sBuffer, sLine, nCount);
                    sBuffer[ nCount ] = '\0';
                    for ( pCur = sBuffer; *pCur == ' ' || *pCur == '\t'; ++pCur );
                    if ( !strncmp( pCur, "dup", 3 ) )
                    {
                        for ( pCur += 3; *pCur == ' ' || *pCur == '\t'; ++pCur ) ;
                        for ( pTemp = pCur; *pTemp >= '0' && *pTemp <= '9'; ++pTemp ) ;
                        if ( *pTemp )
                        {
                            char nChar = *pTemp;
                            *pTemp = '\0';
                            nCode = atoi( pCur );
							if (nCode < 0) nCode = 0;								
                            *pTemp = nChar;
                            if ( nCode == 8 && *pTemp == '#')
                            {
                                nCode = 0;
                                for (++pTemp; *pTemp >= '0' && *pTemp <= '7'; ++pTemp)
                                {
                                    nCode = nCode * 8 + (*pTemp - '0');
                                }
                            }
                            if ( nCode < 256 )
                            {
                                for ( pCur = pTemp; *pCur == ' ' || *pCur == '\t'; ++pCur ) ;
                                if ( *pCur == '/')
                                {
                                    ++pCur;
                                    for ( pTemp = pCur; *pTemp && *pTemp != ' ' && *pTemp != '\t'; ++pTemp ) ;
                                    *pTemp = '\0';
                                    m_arrEncoding[ nCode ] = CopyString( pCur );
                                }
                            }
                        }
                    }
                    else
                    {
                        if ( strtok( sBuffer, " \t") && ( pCur = strtok(NULL, " \t\n\r")) && !strcmp( pCur, "def"))
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                if ( !sEexec )
                    sEexec = (unsigned char*)strstr( sLine, "currentfile eexec" );

                sLine = GetNextLine(sLine);
            }
        }

        if ( NULL != sEexec )
        {
            unsigned char* sTemp = sEexec;
            while ( sTemp != (unsigned char*)strstr( (char*)sTemp, "cleartomark" ) && sTemp < m_sFile + m_nLen )
                sTemp++;

            int nBufferLen = sTemp - ( sEexec + 17 );
            unsigned char *sEexecBuffer = (unsigned char*)MemUtilsMalloc( nBufferLen );
            if ( !sEexecBuffer )
                return;

            memcpy( sEexecBuffer, sEexec + 17, nBufferLen );
            DecryptEexec( &sEexecBuffer, nBufferLen );

            sEexec = sEexecBuffer + 4; // Первые четыре байта были случайными
            int nEexecLen = nBufferLen - 4;

            // Теперь прочитаем содержимое Private Dict
            bool bGlyphsSection = false, bSubrsSection = false;
            //unsigned short ushChar = '';
            std::wstring sToken, sGlyph;
            int nLength = 0;

            // Выставляем значения по умолчанию элементов Private Dict
            m_oPrivateDict.nBlueValues       = 0;
            m_oPrivateDict.nOtherBlues       = 0;
            m_oPrivateDict.nFamilyBlues      = 0;
            m_oPrivateDict.nFamilyOtherBlues = 0;
            m_oPrivateDict.dBlueScale        = 0.039625;
            m_oPrivateDict.nBlueShift        = 7;
            m_oPrivateDict.nBlueFuzz         = 1;
            m_oPrivateDict.bHasStdHW         = false;
            m_oPrivateDict.bHasStdVW         = false;
            m_oPrivateDict.nStemSnapH        = 0;
            m_oPrivateDict.nStemSnapV        = 0;
            m_oPrivateDict.bHasForceBold     = false;
            m_oPrivateDict.nLanguageGroup    = 0;
            m_oPrivateDict.nLenIV            = 4;
            m_oPrivateDict.dExpansionFactor  = 0.06;

            for ( int nIndex = 0; nIndex < nEexecLen; nIndex++ )
            {
                unsigned char nChar = sEexec[nIndex];

                if ( ( bGlyphsSection || bSubrsSection ) && 'R' == nChar && nLength > 0 )
                {
                    unsigned char *sData = new unsigned char[nLength];
                    if ( sData )
                    {
                        memcpy( sData, sEexec + nIndex + 3, nLength );
                        unsigned short unKey = 4330U;
                        unsigned char *sCur = sData;
                        EexecDecode( &sCur, sCur + nLength, sCur, nLength, &unKey );

                        if ( m_oPrivateDict.nLenIV > 0 && m_oPrivateDict.nLenIV < nLength )
                        {
                            Type1Charstring oCharstring = DecodeCharString( sData + m_oPrivateDict.nLenIV, nLength - m_oPrivateDict.nLenIV );
                            if ( bGlyphsSection )
                            {
                                int nUnicode = Type1NameToUnicodeW( sGlyph.c_str() );

                                if ( 0 != nUnicode )
                                    m_arrCharstrings.push_back( Type1Glyph( sGlyph, nUnicode, oCharstring )  );
                            }
                            else // if ( bSubrsSection )
                            {
                                m_arrSubrs.push_back( oCharstring );
                            }
                        }

                        delete []sData;
                    }

                    nIndex += nLength + 3;
                }
                else if ( IS_PS_SPACE( nChar ) )
                {
                    nLength = Utils::GetInteger( sToken );
                    sToken.clear();
                }
                else
                {
                    sToken.push_back( (wchar_t)nChar );
                    if ( !bGlyphsSection && '/' == sToken[0] )
                    {
                        int nTempChar = sToken[1];
                        switch (nTempChar)
                        {
                        case 'B':
                            {
                                if ( L"/BlueValues" == sToken )
                                    m_oPrivateDict.nBlueValues = ReadIntArray<type1MaxBlueValues>( sEexec + nIndex + 2, nEexecLen - nIndex - 2, m_oPrivateDict.arrnBlueValues );
                                else if ( L"/BlueScale" == sToken )
                                    m_oPrivateDict.dBlueScale  = ReadDouble( sEexec + nIndex + 1, nEexecLen - nIndex - 1 );
                                else if ( L"/BlueShift" == sToken )
                                    m_oPrivateDict.nBlueShift  = ReadInt( sEexec + nIndex + 1, nEexecLen - nIndex - 1 );
                                else if ( L"/BlueFuzz" == sToken )
                                    m_oPrivateDict.nBlueFuzz   = ReadInt( sEexec + nIndex + 1, nEexecLen - nIndex - 1 );

                                break;
                            }
                        case 'C':
                            {
                                if ( L"/CharString" == sToken )
                                    bGlyphsSection = true;

                                break;
                            }
                        case 'E':
                            {
                                if ( L"/ExpansionFactor" == sToken )
                                    m_oPrivateDict.dExpansionFactor = ReadDouble( sEexec + nIndex + 1, nEexecLen - nIndex - 1 );

                                break;
                            }
                        case 'F':
                            {
                                if ( L"/FamilyBlues" == sToken )
                                    m_oPrivateDict.nFamilyBlues = ReadIntArray<type1MaxBlueValues>( sEexec + nIndex + 2, nEexecLen - nIndex - 2, m_oPrivateDict.arrnFamilyBlues );
                                else if ( L"/FamilyOtherBlues" == sToken )
                                    m_oPrivateDict.nFamilyOtherBlues = ReadIntArray<type1MaxOtherBlues>( sEexec + nIndex + 2, nEexecLen - nIndex - 2, m_oPrivateDict.arrnFamilyOtherBlues );
                                else if ( L"/ForceBold" == sToken )
                                {
                                    m_oPrivateDict.bHasForceBold = true;
                                    m_oPrivateDict.bForceBold = ReadBool( sEexec + nIndex + 1, nEexecLen - nIndex - 1 );
                                }

                                break;
                            }
                        case 'L':
                            {
                                if ( L"/LanguageGroup" == sToken )
                                    m_oPrivateDict.nLanguageGroup = ReadInt( sEexec + nIndex + 1, nEexecLen - nIndex - 1 );
                                else if ( L"/lenIV" == sToken )
                                    m_oPrivateDict.nLenIV = ReadInt( sEexec + nIndex + 1, nEexecLen - nIndex - 1 );

                                break;
                            }
                        case 'S':
                            {
                                if ( L"/Subrs" == sToken )
                                    bSubrsSection = true;
                                else if ( L"/StemSnapH" == sToken )
                                    m_oPrivateDict.nStemSnapH = ReadDoubleArray<type1MaxStemSnap>( sEexec + nIndex + 2, nEexecLen - nIndex - 2, m_oPrivateDict.arrdStemSnapH );
                                else if ( L"/StemSnapV" == sToken )
                                    m_oPrivateDict.nStemSnapV = ReadDoubleArray<type1MaxStemSnap>( sEexec + nIndex + 2, nEexecLen - nIndex - 2, m_oPrivateDict.arrdStemSnapV );
                                else if ( L"/StdHW" == sToken )
                                {
                                    // Здесь содержится массив с одним значением
                                    double dTemp[1];

                                    if ( ReadDoubleArray<1>( sEexec + nIndex + 2, nEexecLen - nIndex - 2, dTemp ) > 0 )
                                    {
                                        m_oPrivateDict.bHasStdHW = true;
                                        m_oPrivateDict.dStdHW = dTemp[0];
                                    }
                                }
                                else if ( L"/StdVW" == sToken )
                                {
                                    // Здесь содержится массив с одним значением
                                    double dTemp[1];
                                    if ( ReadDoubleArray<1>( sEexec + nIndex + 2, nEexecLen - nIndex - 2, dTemp ) > 0 )
                                    {
                                        m_oPrivateDict.bHasStdHW = true;
                                        m_oPrivateDict.dStdVW = dTemp[0];
                                    }
                                }
                            }
                        }
                    }
                    else if ( '/' == nChar  )
                    {
                        sToken.clear();
                        sGlyph.clear();

                        while ( nIndex < nEexecLen && ( nChar = sEexec[++nIndex] ) != ' ' )
                            sGlyph.push_back( (wchar_t)nChar );
                    }
                }
            }
            MemUtilsFree( sEexecBuffer );

            // Проведем сортировку элементов m_arrCharstrings по юникодному значению
            qsort( m_arrCharstrings.data(), m_arrCharstrings.size(), sizeof(Type1Glyph), CompareType1Glyph );
        }

        m_bParsed = true;
    }

    void CFontFileType1::DecryptEexec(unsigned char** ppEexecBuffer, int nLen)
    {
        // Согласно спецификации Type1, первый байт не должен быть ASCII пробелом
        // (пробел, таб, перенос каретки или перенос строки).
        unsigned char *sCur = (unsigned char*)(*ppEexecBuffer);
        while( sCur < (unsigned char*)(*ppEexecBuffer) + nLen && ( ' ' == *sCur || '\t' == *sCur || '\r' == *sCur || '\n' == *sCur ) )
        {
            ++sCur;
            --nLen;
        }

        // Теперь нам надо определить в каком формате у нас данные: ASKII или бинарные данные.
        // Если первые четыре байта являются шестнадцатиричными символами, значит, кодировка ASCII.
        bool bASCII = false;

        if ( nLen > 3 && isxdigit( sCur[0] ) && isxdigit( sCur[1] ) && isxdigit( sCur[2] ) && isxdigit( sCur[3] ) )
            bASCII = true;

        if ( bASCII )
            ASCIIHexDecode( &sCur, sCur + nLen, sCur, nLen );

        unsigned short ushKey = 55665U;
        EexecDecode( &sCur, *ppEexecBuffer + nLen, sCur, nLen, &ushKey );
    }
    bool CFontFileType1::RemovePfbMarkers()
    {
        bool bSuccess = true;

        int nBlockType = 0;
        int nBlockLen  = 0;
        int nChar = 0;

        unsigned char *sBuffer = NULL;
        unsigned int nBufLen = 0;

        while ( nBlockType != PFB_DONE )
        {
            while ( 0 == nBlockLen )
            {
                nChar = ReadU8( &bSuccess );
                if ( !bSuccess )
                    return false;

                nBlockType = ReadU8( &bSuccess );
                if ( !bSuccess || PFB_MARKER != nChar || ( PFB_ASCII != nBlockType && PFB_BINARY != nBlockType && PFB_DONE != nBlockType ) )
                    return false;

                if ( PFB_DONE == nBlockType )
                    break;

                nBlockLen = ReadU32LE( &bSuccess );
                if ( !bSuccess )
                    return false;
            }

            // Читаем сам блок данных
            if ( nBlockLen > 0 )
            {
                if ( !sBuffer )
                {
                    sBuffer = (unsigned char*)MemUtilsMalloc( nBlockLen );
                    if ( !sBuffer )
                        return false;
                }
                else
                    sBuffer = (unsigned char*)MemUtilsRealloc( sBuffer, nBufLen + nBlockLen );

                Read( sBuffer + nBufLen, nBlockLen );
                nBufLen += nBlockLen;
            }
            nBlockLen = 0;
        }

        if ( m_bFreeFileData )
            MemUtilsFree( m_sFile );

        m_bFreeFileData = true;
        m_sFile         = (unsigned char*)sBuffer;
        m_sFileData     = m_sFile;
        m_nLen          = nBufLen;
        m_nPos          = 0;

        return true;
    }

    Type1Charstring CFontFileType1::DecodeCharString(unsigned char *sString, int nLen)
    {
        std::vector<Type1CharstringItem> sCharString;

        int nLSB = 0, nWidth = 0;

        for ( int nIndex = 0; nIndex < nLen; nIndex++ )
        {
            int nValue = sString[nIndex];

            if ( nValue < 32 ) // команда
            {
                int nCommand = 0;

                if ( 12 == nValue )
                {
                    int nNextValue = sString[++nIndex];

                    if ( 16 == nNextValue )
                    {
                        if ( sCharString.size() <= 0 )
                            continue;

                        int nInd = sCharString[sCharString.size() - 1].nValue;
                        sCharString.pop_back() ;

                        while ( sCharString.size() > 0 && false == sCharString[sCharString.size() - 1].bCommand )
                            sCharString.pop_back( );

                        // If the flex mechanishm is not used in a font program, Adobe
                        // state that that entries 0, 1 and 2 can simply be replace by
                        // {}, which means that we can simply ignore them.
                        if ( nInd < 3 )
                            continue;

                        // This is the same things about hint replacement, if it is not used
                        // entry 3 can be replaced by {3}
                        if ( 3 == nInd )
                        {
                            sCharString.push_back( Type1CharstringItem( 3, true ) );
                            nIndex++;
                            continue;
                        }
                    }

                    nCommand = 12 + ( nNextValue << 8 );
                }
                else
                {
                    if ( 13 == nValue )
                    {
                        if ( 2 == sCharString.size() )
                            nWidth = sCharString[1].nValue;
                        else if ( 4 == sCharString.size() && 0x0C0C == sCharString[3].nValue && sCharString[3].bCommand )
                            nWidth = sCharString[1].nValue / sCharString[2].nValue;
                        else
                        {
                            // TO DO: обработать ошибку
                            nWidth = 0;
                        }

                        if ( sCharString.size() > 0 )
                        {
                            nLSB = sCharString[0].nValue;
                            sCharString.push_back( Type1CharstringItem( nLSB, false ) );
                            sCharString.push_back( Type1CharstringItem( c_nType1hmoveto, true ) );
                            sCharString.erase( sCharString.begin() );
                        }
                        else
                        {
                            nLSB = 0;
                            sCharString.push_back( Type1CharstringItem( nLSB, false ) );
                            sCharString.push_back( Type1CharstringItem( c_nType1hmoveto, true ) );
                        }

                        continue;
                    }

                    nCommand = nValue;
                }

                // Some charstring commands are meaningless in Type2 and will return
                // a null, let's just ignored them
                if ( !nCommand && nIndex < nLen )
                    continue;
                else if ( !nCommand )
                    break;
                else if ( c_nType1seac == nCommand || c_nType1sbw == nCommand )
                {
                    // TO DO: обработать ошибку
                }

                sCharString.push_back( Type1CharstringItem( nCommand, true ) );
            }
            else
            {
                if ( nValue <= 246 )
                    nValue = nValue - 139;
                else if ( nValue <= 250 )
                    nValue =  ( ( nValue - 247 ) * 256 ) + (int)( sString[++nIndex] ) + 108;
                else if ( nValue <= 254 )
                    nValue = -( ( nValue - 251 ) * 256 ) - (int)( sString[++nIndex] ) - 108;
                else
                    nValue = ( sString[++nIndex] & 0xff ) << 24 | ( sString[++nIndex] & 0xff ) << 16 | ( sString[++nIndex] & 0xff ) << 8 | ( sString[++nIndex] & 0xff ) << 0;

                sCharString.push_back( Type1CharstringItem( nValue, false ) );
            }
        }

        return Type1Charstring( sCharString, nWidth, nLSB );
    }
    Type1Charstring CFontFileType1::FlattenCharstring(Type1Charstring& oCharstring, int nBias)
    {
        Type1Charstring oNew;
        oNew.nLSB   = oCharstring.nLSB;
        oNew.nWidth = oCharstring.nWidth;

        for ( size_t nIndex = 0; nIndex < oCharstring.arrCharstring.size(); nIndex++ )
        {
            Type1CharstringItem oItem = oCharstring.arrCharstring[nIndex];
            int nValue = oItem.nValue;
            if ( oItem.bCommand )
            {
                if ( nValue == c_nType1sub )
                {
                    oNew.arrCharstring.push_back( Type1CharstringItem( 12, true ) );
                    oNew.arrCharstring.push_back( Type1CharstringItem( 11, true ) );
                }
                else if ( nValue == c_nType1div )
                {
                    oNew.arrCharstring.push_back( Type1CharstringItem( 12, true ) );
                    oNew.arrCharstring.push_back( Type1CharstringItem( 12, true ) );
                }
                else if ( nValue == c_nType1pop )
                {
                    //oNew.arrCharstring.Add( Type1CharstringItem( 1, true ) );
                    oNew.arrCharstring.push_back( Type1CharstringItem( 12, true ) );
                    oNew.arrCharstring.push_back( Type1CharstringItem( 18, true ) );
                }
                else if ( nValue == c_nType1callsubr  )
                {
                    //int nTempLen = oNew.arrCharstring.GetSize();
                    //int nT1 = oNew.arrCharstring[nTempLen - 2].nValue;
                    //int nT2 = oNew.arrCharstring[nTempLen - 1].nValue;
                    //int nInd = nT1 << 8 | nT2;

                    //if ( 107 == nBias )
                    //{
                    //	if ( nInd <= 215 )
                    //	{
                    //		oNew.arrCharstring.RemoveAt( nTempLen - 1 );
                    //		oNew.arrCharstring.RemoveAt( nTempLen - 2 );

                    //		oNew.arrCharstring[nTempLen - 3].nValue = nInd - nBias + 139;
                    //		oNew.arrCharstring.Add( Type1CharstringItem( oItem.nValue, true ) );
                    //	}
                    //	else
                    //	{
                    //		oNew.arrCharstring.RemoveAt( nTempLen - 1 );

                    //		oNew.arrCharstring[nTempLen - 3].nValue = ((nInd - nBias) - 108) / 256 + 247;
                    //		oNew.arrCharstring[nTempLen - 2].nValue = ((nInd - nBias) - 108) - (oNew.arrCharstring[nTempLen - 3].nValue - 247) * 256;
                    //		oNew.arrCharstring.Add( Type1CharstringItem( oItem.nValue, true ) );
                    //	}
                    //}
                    //else
                    //{

                    //	nInd -= nBias;
                    //	oNew.arrCharstring[nTempLen - 2].nValue = nInd >> 8;
                    //	oNew.arrCharstring[nTempLen - 1].nValue = nInd & 0xFF;
                    //	oNew.arrCharstring.Add( Type1CharstringItem( oItem.nValue, true ) );
                    //}
                    oNew.arrCharstring.push_back( Type1CharstringItem( oItem.nValue, true ) );
                }
                else
                    oNew.arrCharstring.push_back( Type1CharstringItem( oItem.nValue, true ) );
            }
            else
            {
                // Type1 charstrings используют деления для чисел больших 32000
                if ( oItem.nValue > 32000 )
                {
                    int nDivisor = oCharstring.arrCharstring[nIndex + 1].nValue;
                    if ( 0 != nDivisor )
                        nValue /= nDivisor;
                }
                oNew.arrCharstring.push_back( Type1CharstringItem( 28, true ) );
                oNew.arrCharstring.push_back( Type1CharstringItem( nValue >> 8, false ) );
                oNew.arrCharstring.push_back( Type1CharstringItem( nValue & 0xFF, false ) );
            }
        }

        return oNew;
    }
    void CFontFileType1::CFFCreateIndexHeader(FontFileOutputFunc pOutputFunc, void *pOutputStream, std::vector<std::wstring> aObjects)
    {
        char nChar;
        size_t nCount = aObjects.size();
        if ( 0 == nCount )
        {
            pOutputFunc( pOutputStream, "\x00\x00\x00", 3 );
            return;
        }

        // Первые два байта - количество элементов
        WriteChar( nCount >> 8 );
        WriteChar( nCount & 0xFF );
        // Размер сдвигов, ставим по максимуму
        WriteChar( 0x04 );

        int nRelativeOffset = 1;
        for ( size_t nIndex = 0; nIndex < nCount + 1; nIndex++ )
        {
            WriteChar( (nRelativeOffset >> 24) & 0xFF );
            WriteChar( (nRelativeOffset >> 16) & 0xFF );
            WriteChar( (nRelativeOffset >>  8) & 0xFF );
            WriteChar( (nRelativeOffset)       & 0xFF );

            if ( nIndex < nCount )
                nRelativeOffset += aObjects[nIndex].length();
        }

        for ( size_t nIndex = 0; nIndex < nCount; nIndex++ )
        {
            std::string sCur = U_TO_UTF8((aObjects[nIndex]));
            pOutputFunc( pOutputStream, sCur.c_str(), sCur.length() );
        }
    }
    void CFontFileType1::CFFCreateIndexHeader(FontFileOutputFunc pOutputFunc, void *pOutputStream, std::vector<Type1Charstring> aObjects)
    {
        char nChar;
        size_t nCount = aObjects.size();
        if ( 0 == nCount )
        {
            pOutputFunc( pOutputStream, "\x00\x00\x00", 3 );
            return;
        }

        // Первые два байта - количество элементов
        WriteChar( nCount >> 8 );
        WriteChar( nCount & 0xFF );
        // Размер сдвигов, ставим по максимуму
        WriteChar( 0x04 );

        int nRelativeOffset = 1;
        for ( size_t nIndex = 0; nIndex < nCount + 1; nIndex++ )
        {
            WriteChar( (nRelativeOffset >> 24) & 0xFF );
            WriteChar( (nRelativeOffset >> 16) & 0xFF );
            WriteChar( (nRelativeOffset >>  8) & 0xFF );
            WriteChar( (nRelativeOffset)       & 0xFF );

            if ( nIndex < nCount )
                nRelativeOffset += aObjects[nIndex].arrCharstring.size();
        }

        for ( size_t nI = 0; nI < nCount; nI++ )
        {
            for ( size_t nJ = 0; nJ < aObjects[nI].arrCharstring.size(); nJ++ )
            {
                WriteChar( aObjects[nI].arrCharstring[nJ].nValue & 0xFF );
            }
        }
    }
    void CFontFileType1::CFFEncodeNumber(FontFileOutputFunc pOutputFunc, void *pOutputStream, int nValue, bool bForceLong)
    {
        char nChar;
        if ( !bForceLong && nValue >= -32768 && nValue <= 32767 )
        {
            WriteChar( 0x1c );
            WriteChar( ( nValue >> 8 ) & 0xFF );
            WriteChar( nValue & 0xFF );
        }
        else // if ( nValue >= (-2147483648) && nValue <= 2147483647 )
        {
            WriteChar( 0x1d );
            WriteChar( ( nValue >> 24 ) & 0xFF );
            WriteChar( ( nValue >> 16 ) & 0xFF );
            WriteChar( ( nValue >>  8 ) & 0xFF );
            WriteChar( nValue & 0xFF );
        }
    }
    void CFontFileType1::CFFEncodeNumber(FontFileOutputFunc pOutputFunc, void *pOutputStream, double dValue)
    {
        char nChar = 0;

        WriteChar( 0x1e ); // начало десятичного числа

        std::wstring sValue = std::to_wstring(dValue);
        bool bFirstNibble = true;
        for ( size_t nIndex = 0; nIndex < sValue.length(); nIndex++ )
        {
            int nCurChar = sValue.c_str()[ nIndex ];
            if ( '0' <= nCurChar && nCurChar <= '9' )
                nCurChar -= (int)('0');
            else if ( '.' == nCurChar )
                nCurChar = 0x0a;
            else if ( '-' == nCurChar )
                nCurChar = 0x0e;
            else
                continue;

            if ( bFirstNibble )
                nChar = nCurChar << 4;
            else
            {
                nChar += nCurChar;
                WriteChar( nChar );
            }

            bFirstNibble = !bFirstNibble;
        }

        // Записываем окончаниедесятичного числа
        if ( bFirstNibble )
            nChar = (char)0xff;
        else
            nChar += 0x0f;

        WriteChar( nChar );
    }
    void CFontFileType1::ToCFF(FontFileOutputFunc pOutputFunc, void *pOutputStream)
    {
        std::wstring sFontName = NSFile::CUtf8Converter::GetUnicodeFromCharPtr( m_sName, (LONG)strlen(m_sName) );
        std::vector<std::wstring> aString;

        int nBias = 0;
        size_t nSubrsLen = m_arrSubrs.size();
        if ( nSubrsLen < 1240 )
            nBias = 107;
        else if ( nSubrsLen < 33900 )
            nBias = 1131;
        else
            nBias = 32768;

        std::vector<Type1Charstring> arrType2Charstrings;
        Type1Charstring oFirstCharstring;
        oFirstCharstring.arrCharstring.push_back( Type1CharstringItem( 0x8B, false ) );
        oFirstCharstring.arrCharstring.push_back( Type1CharstringItem( 0x0E, false ) );
        arrType2Charstrings.push_back( oFirstCharstring );
        for ( size_t nIndex = 0; nIndex < m_arrCharstrings.size(); nIndex++ )
        {
            std::wstring sG = m_arrCharstrings[nIndex].sGlyph;
            if ( L"afii10090" == sG )
                int k = 10;
            arrType2Charstrings.push_back( FlattenCharstring( m_arrCharstrings[nIndex].oData, nBias ) );
        }

        std::vector<Type1Charstring> arrType2Subrs;

        //Type1Charstring oBias;
        ////oBias.arrCharstring.Add( Type1CharstringItem( 0x0B, false ) );
        //oBias.arrCharstring.Add( Type1CharstringItem( 0x0E, false ) );
        //for ( int nIndex = 0; nIndex < nBias; nIndex++ )
        //	arrType2Subrs.Add( oBias );

        for ( size_t nIndex = 0; nIndex < nSubrsLen; nIndex++ )
        {
            //if ( nIndex == 256 )
            //{
            //	Type1Charstring oBias;
            //	oBias.arrCharstring.Add( Type1CharstringItem( 0x0B, false ) );
            //	for ( int nIndex = 0; nIndex < nBias; nIndex++ )
            //		arrType2Subrs.Add( oBias );
            //}
            arrType2Subrs.push_back( FlattenCharstring( m_arrSubrs[nIndex], 0 ) );
        }

        // Header
        TCharBuffer oHeader;
        oHeader.Write( "\x01\x00\x04\x04", 4 );

        // Name
        TCharBuffer oName;
        aString.clear();
        aString.push_back( sFontName );
        CFFCreateIndexHeader( CharBufferWrite, &oName, aString );

        // Strings
        TCharBuffer oStrings;
        aString.clear();
        int nNewSID = CFF_STANDARD_STRINGS_COUNT;
        aString.push_back( L"Version 0.11" );        nNewSID++; // Version
        aString.push_back( L"See original notice" ); nNewSID++; // Notice
        aString.push_back( sFontName );                 nNewSID++; // FullName
        aString.push_back( sFontName );                 nNewSID++; // FamilyName
        aString.push_back( L"Medium" );              nNewSID++; // Weight

        for ( size_t nIndex = 0; nIndex < m_arrCharstrings.size(); nIndex++ )
        {
            int nSID = GetCFFStringIndex( m_arrCharstrings[nIndex].sGlyph.c_str() );
            if ( nSID < 0 )
            {
                aString.push_back( m_arrCharstrings[nIndex].sGlyph );
                nSID = nNewSID;
                nNewSID++;
            }

            m_arrCharstrings[nIndex].nReserved = nSID;
        }

        CFFCreateIndexHeader( CharBufferWrite, &oStrings, aString );

        // GlobalSubrs
        TCharBuffer oGlobalSubrs;
        aString.clear(); // Записываем пустой массив
        CFFCreateIndexHeader( CharBufferWrite, &oGlobalSubrs, aString );

        // Charset
        TCharBuffer oCharset;
        oCharset.Write( (char)0x00 ); // Encoding

        size_t nGlyphsCount = m_arrCharstrings.size();
        for ( size_t nIndex = 0; nIndex < nGlyphsCount; nIndex++ )
        {
            int nSID = m_arrCharstrings[nIndex].nReserved;
            oCharset.Write( (char)(nSID >> 8) );
            oCharset.Write( (char)(nSID & 0xFF) );
        }

        // Charstrings
        TCharBuffer oCharstrings;
        CFFCreateIndexHeader( NSFontConverter::CharBufferWrite, &oCharstrings, arrType2Charstrings );

        // Private
        TCharBuffer oPrivate;
        oPrivate.Write( "\x8b\x14", 2 ); // defaultWidth
        oPrivate.Write( "\x8b\x15", 2 ); // nominalWidth

        // Private: BlueValues
        if ( m_oPrivateDict.nBlueValues > 0 )
        {
            CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.arrnBlueValues[0] );
            for ( int nIndex = 1; nIndex < m_oPrivateDict.nBlueValues; nIndex++ )
                CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.arrnBlueValues[nIndex] - m_oPrivateDict.arrnBlueValues[nIndex - 1] );

            oPrivate.Write( (char)0x06 );
        }

        // Private: OtherBlues
        if ( m_oPrivateDict.nOtherBlues > 0 )
        {
            CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.arrnOtherBlues[0] );
            for ( int nIndex = 1; nIndex < m_oPrivateDict.nOtherBlues; nIndex++ )
                CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.arrnOtherBlues[nIndex] - m_oPrivateDict.arrnOtherBlues[nIndex - 1] );

            oPrivate.Write( (char)0x07 );
        }

        // Private: FamilyBlues
        if ( m_oPrivateDict.nFamilyBlues > 0 )
        {
            CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.arrnFamilyBlues[0] );
            for ( int nIndex = 1; nIndex < m_oPrivateDict.nFamilyBlues; nIndex++ )
                CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.arrnFamilyBlues[nIndex] - m_oPrivateDict.arrnFamilyBlues[nIndex - 1] );

            oPrivate.Write( (char)0x08 );
        }

        // Private: FamilyOtherBlues
        if ( m_oPrivateDict.nFamilyOtherBlues > 0 )
        {
            CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.arrnFamilyOtherBlues[0] );
            for ( int nIndex = 1; nIndex < m_oPrivateDict.nFamilyOtherBlues; nIndex++ )
                CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.arrnFamilyOtherBlues[nIndex] - m_oPrivateDict.arrnFamilyOtherBlues[nIndex - 1] );

            oPrivate.Write( (char)0x09 );
        }

        // Private: StemSnapH
        if ( m_oPrivateDict.nStemSnapH > 0 )
        {
            CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.arrdStemSnapH[0] );
            for ( int nIndex = 1; nIndex < m_oPrivateDict.nStemSnapH; nIndex++ )
                CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.arrdStemSnapH[nIndex] - m_oPrivateDict.arrdStemSnapH[nIndex - 1] );

            oPrivate.Write( "\x0c\x0c" , 2);
        }

        // Private: StemSnapV
        if ( m_oPrivateDict.nStemSnapV > 0 )
        {
            CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.arrdStemSnapV[0] );
            for ( int nIndex = 1; nIndex < m_oPrivateDict.nStemSnapV; nIndex++ )
                CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.arrdStemSnapV[nIndex] - m_oPrivateDict.arrdStemSnapV[nIndex - 1] );

            oPrivate.Write( "\x0c\x0d" , 2);
        }

        // Private: BlueShift
        CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.nBlueShift );
        oPrivate.Write( "\x0c\x0a", 2 );

        // Private: BlueFuzz
        CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.nBlueFuzz );
        oPrivate.Write( "\x0c\x0b", 2 );

        // Private: BlueScale
        CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.dBlueScale );
        oPrivate.Write( "\x0c\x09", 2 );

        // Private: LanguageGroup
        CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.nLanguageGroup );
        oPrivate.Write( "\x0c\x11", 2 );

        // Private: ExpansionFactor
        CFFEncodeNumber( CharBufferWrite, &oPrivate, m_oPrivateDict.dExpansionFactor );
        oPrivate.Write( "\x0c\x18", 2 );

        // Private: Subrs
        int nPrivateLen = oPrivate.nLen + (5 + 1);
        CFFEncodeNumber( CharBufferWrite, &oPrivate, nPrivateLen, true );
        oPrivate.Write( "\x13", 1 );

        // LocalSubrs
        TCharBuffer oLocalSubrs;
        CFFCreateIndexHeader( CharBufferWrite, &oLocalSubrs, arrType2Subrs );

        // Top Dict
        TCharBuffer oTopDict;
        oTopDict.Write( "\x00\x01\x04\x00\x00\x00\x01\x00\x00\0x00\x00", 11 );
        oTopDict.Write( "\xf8\x1b\x00", 3 ); // Version
        oTopDict.Write( "\xf8\x1c\x01", 3 ); // Notice
        oTopDict.Write( "\xf8\x1d\x02", 3 ); // FullName
        oTopDict.Write( "\xf8\x1e\x03", 3 ); // FamilyName
        oTopDict.Write( "\xf8\x1f\x04", 3 ); // Weight
        oTopDict.Write( "\x1c\x00\x00\x10", 4 ); // Encoding

        // BBox
        CFFEncodeNumber( CharBufferWrite, &oTopDict, m_oTopDict.arrdFontBBox[0] );
        CFFEncodeNumber( CharBufferWrite, &oTopDict, m_oTopDict.arrdFontBBox[1] );
        CFFEncodeNumber( CharBufferWrite, &oTopDict, m_oTopDict.arrdFontBBox[2] );
        CFFEncodeNumber( CharBufferWrite, &oTopDict, m_oTopDict.arrdFontBBox[3] );
        oTopDict.Write( "\x05", 1 );

        // Теперь оценим размер TopDict: Возьмем текущую длину, добавим к ней (4 * 5 + 3)
        // ( 4 числа, которые пишем по 5 байт + 3 байта на 3 команды)
        int nTopDictLen = oTopDict.nLen + ( 4 * 5 + 3);

        int nOffset = oHeader.nLen + oName.nLen + nTopDictLen + oStrings.nLen + oGlobalSubrs.nLen;
        CFFEncodeNumber( CharBufferWrite, &oTopDict, nOffset, true );
        oTopDict.Write( "\x0f", 1 ); // Charset

        nOffset += oCharset.nLen;//( arrType2Charstrings.GetSize() * 2 ) + 1;
        CFFEncodeNumber( CharBufferWrite, &oTopDict, nOffset, true );
        oTopDict.Write( "\x11", 1 ); // Charstrings

        CFFEncodeNumber( CharBufferWrite, &oTopDict, oPrivate.nLen, true );
        nOffset += oCharstrings.nLen;
        CFFEncodeNumber( CharBufferWrite, &oTopDict, nOffset, true );
        oTopDict.Write( "\x12", 1 ); // Private

        // Теперь запишем реальный размер TopDict
        int nTopDictDataLen = nTopDictLen - 10;
        oTopDict.sBuffer[7]  = ( nTopDictDataLen >> 24 ) & 0xFF;
        oTopDict.sBuffer[8]  = ( nTopDictDataLen >> 16 ) & 0xFF;
        oTopDict.sBuffer[9]  = ( nTopDictDataLen >> 8  ) & 0xFF;
        oTopDict.sBuffer[10] = nTopDictDataLen & 0xFF;

        // Записываем все в файл
        pOutputFunc( pOutputStream, oHeader.sBuffer,      oHeader.nLen      );
        pOutputFunc( pOutputStream, oName.sBuffer,        oName.nLen        );
        pOutputFunc( pOutputStream, oTopDict.sBuffer,     oTopDict.nLen     );
        pOutputFunc( pOutputStream, oStrings.sBuffer,     oStrings.nLen     );
        pOutputFunc( pOutputStream, oGlobalSubrs.sBuffer, oGlobalSubrs.nLen );
        pOutputFunc( pOutputStream, oCharset.sBuffer,     oCharset.nLen     );
        pOutputFunc( pOutputStream, oCharstrings.sBuffer, oCharstrings.nLen );
        pOutputFunc( pOutputStream, oPrivate.sBuffer,     oPrivate.nLen     );
        pOutputFunc( pOutputStream, oLocalSubrs.sBuffer,  oLocalSubrs.nLen  );
    }
}
