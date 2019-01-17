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
#ifndef _ASC_FONT_CONVERTER_UTILS_H_
#define _ASC_FONT_CONVERTER_UTILS_H_

// FreeType
#include <stdint.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_TRUETYPE_TABLES_H
#include "MemoryUtils.h"

#include <string>

namespace NSFontConverter
{
    struct TCharBuffer
    {
        char *sBuffer;  // Буфер
        int   nSize;    // Реальный размер буфера
        int   nLen;     // Фактический размер буфера (т.е. размер используемой части буфера)

        TCharBuffer(int _initsize = 32)
        {
            nSize   = _initsize;
            nLen    = 0;

            sBuffer = (char*)MemUtilsMalloc( nSize );
            memset( sBuffer, 0x00, nSize );
        }
        ~TCharBuffer()
        {
            if ( sBuffer )
                MemUtilsFree( sBuffer );
        }

        void Write(char nChar)
        {
            if ( nLen + 1 >= nSize )
            {
                nSize *= 2;
                sBuffer = (char*)MemUtilsRealloc( (void*)sBuffer, nSize );
                memset( sBuffer + nLen, 0x00, nSize - nLen );
            }

            sBuffer[nLen++] = nChar;
        }

        void Write(const char *sString, int nStringLen = -1)
        {
            if ( nStringLen < 0 )
                nStringLen = strlen( sString );

            while ( nStringLen + nLen >= nSize )
            {
                nSize *= 2;
                sBuffer = (char*)MemUtilsRealloc( (void*)sBuffer, nSize );
                memset( sBuffer + nLen, 0x00, nSize - nLen );
            }

            memcpy( sBuffer + nLen, sString, nStringLen );
            nLen += nStringLen;
        }
        void Write(unsigned char unChar)
        {
            Write( (char)unChar );
        }

        void Write16(int nValue)
        {
            Write( (unsigned char)( (nValue >> 8) & 0xFF ) );
            Write( (unsigned char)( nValue & 0xFF ) );
        }
        void Write32(int nValue)
        {
            Write( (unsigned char)( (nValue >> 24) & 0xFF ) );
            Write( (unsigned char)( (nValue >> 16) & 0xFF ) );
            Write( (unsigned char)( (nValue >> 8) & 0xFF ) );
            Write( (unsigned char)( nValue & 0xFF ) );
        }
    };

    static void CharBufferWrite(void *pBuffer, const char *sData, int nLen)
    {
        TCharBuffer *pCharBuffer = (TCharBuffer*)pBuffer;
        pCharBuffer->Write( sData, nLen );
    }

    static void FileWrite(void* pFile, const char *sData, int nLen)
    {
        ::fwrite( sData, 1, nLen, (FILE*)pFile );
        ::fflush( (FILE*)pFile );
    }

    namespace Utils
    {
        inline static int     GetDigit   (wchar_t c)
        {
            if (c >= '0' && c <= '9')
                return (int)(c - '0');
            if (c >= 'a' && c <= 'f')
                return 10 + (int)(c - 'a');
            if (c >= 'A' && c <= 'F')
                return 10 + (int)(c - 'A');

            return 0;
        }
        inline static bool     IsDigit   (wchar_t c)
        {
            if (c >= '0' && c <= '9')
                return true;
            return false;
        }
        inline static int64_t GetHex     (const std::wstring& string)
        {
            int64_t nResult = 0;
            int nLen = (int)string.length();
            const wchar_t* buf = string.c_str();
            for ( int nIndex = 0; nIndex < nLen; ++nIndex )
            {
                nResult += GetDigit( buf[nIndex] ) << ( 4 * ( nLen - 1 - nIndex ) );
            }

            return nResult;
        }

        inline static int     GetInteger (const std::wstring& string)
        {
            return std::stoi(string);
        }
        inline static int     GetInteger (const char* string)
        {
            return atoi(string);
        }
        inline static double  GetDouble  (const std::wstring& string)
        {
            return std::stod(string);
        }
        inline static double  GetDouble  (const char* string)
        {
            double d = 0;
            sscanf(string, "%lf", &d);
            return d;
        }
    }

    // Функция взята из Graphics.dll
    static int SetCMapForCharCode(FT_Face pFace, long lUnicode, int *pnCMapIndex)
    {
        *pnCMapIndex = -1;

        if ( 0 == pFace->num_charmaps )
            return lUnicode;

        int nCharIndex = 0;

        if ( !pFace )
            return nCharIndex;

        for ( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
        {
            FT_CharMap pCharMap = pFace->charmaps[nIndex];

            if ( FT_Set_Charmap( pFace, pCharMap ) )
                continue;

            FT_Encoding pEncoding = pCharMap->encoding;

            if ( FT_ENCODING_UNICODE == pEncoding )
            {
                if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
                {
                    *pnCMapIndex = nIndex;
                    return nCharIndex;
                }
            }
            else if ( FT_ENCODING_NONE == pEncoding || FT_ENCODING_MS_SYMBOL == pEncoding || FT_ENCODING_APPLE_ROMAN == pEncoding )
            {
                FT_ULong  charcode;
                FT_UInt   gindex;


                charcode = FT_Get_First_Char( pFace, &gindex );
                while ( gindex != 0 )
                {
                    charcode = FT_Get_Next_Char( pFace, charcode, &gindex );
                    if ( charcode == lUnicode )
                    {
                        nCharIndex = gindex;
                        *pnCMapIndex = nIndex;
                        break;
                    }
                }

                if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
                {
                    *pnCMapIndex = nIndex;
                }
            }

            //else if ( FT_ENCODING_ADOBE_STANDARD == pEncoding )
            //{
            //	if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
            //		return nCharIndex;

            //}
            //else if ( FT_ENCODING_ADOBE_CUSTOM == pEncoding )
            //{
            //	if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
            //		return nCharIndex;

            //}
            //else if ( FT_ENCODING_ADOBE_EXPERT == pEncoding )
            //{
            //	if ( nCharIndex = FT_Get_Char_Index( pFace, lUnicode ) )
            //		return nCharIndex;

            //}
        }

        return nCharIndex;
    }

    static int GetSymbolicCmapIndex(FT_Face pFace)
    {
        TT_OS2 *pOs2 = (TT_OS2 *)FT_Get_Sfnt_Table( pFace, ft_sfnt_os2 );
        if ( NULL == pOs2 || 0xFFFF == pOs2->version )
            return -1;

        // Проверяем установлен ли 31 бит
        if ( !( pOs2->ulCodePageRange1 & 0x80000000 ) && !( pOs2->ulCodePageRange1 == 0 && pOs2->ulCodePageRange2 == 0 ) )
            return -1;


        for( int nIndex = 0; nIndex < pFace->num_charmaps; nIndex++ )
        {
            // Symbol
            if ( 0 == pFace->charmaps[nIndex]->encoding_id && 3 == pFace->charmaps[nIndex]->platform_id )
                return nIndex;
        }

        return -1;
    }
}

#endif // _ASC_FONT_CONVERTER_UTILS_H_
