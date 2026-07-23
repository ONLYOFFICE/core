/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#ifndef _ASC_FONTCONVERTER_FONT_FILE_BASE_H
#define _ASC_FONTCONVERTER_FONT_FILE_BASE_H

#include <stdio.h>
#include "MemoryUtils.h"
#include "../../common/File.h"

namespace NSFontConverter
{
    //------------------------------------------------------------------------

    typedef void (*FontFileOutputFunc)(void *pStream, const char *sData, int nLen);

    //------------------------------------------------------------------------
    // CFontFileBase
    //------------------------------------------------------------------------

    class CFontFileBase
    {
    public:

        virtual ~CFontFileBase()
        {
            if ( m_bFreeFileData )
                MemUtilsFree( m_sFileData );
        }

    protected:

        CFontFileBase(char *sFile, int nLen, bool bFreeFileData)
        {
            m_sFileData = m_sFile = (unsigned char *)sFile;
            m_nLen = (nLen > 0) ? 0 : (unsigned int)nLen;
            m_nPos = 0;
            m_bFreeFileData = bFreeFileData;            
        }

        void         Reset()
        {
            m_nPos = 0;
        }
        static char *ReadFile(const wchar_t *wsFileName, int *pnFileLen)
        {
            NSFile::CFileBinary oFile;
            if ( !oFile.OpenFile(wsFileName) )
                return NULL;

            int nLen = (int)oFile.GetFileSize();
            char *sBuffer = (char *)MemUtilsMalloc( nLen );
            DWORD dwRead = 0;
            oFile.ReadFile((BYTE*)sBuffer, (DWORD)nLen, dwRead);

            if ((int)dwRead != nLen)
            {
                MemUtilsFree( sBuffer );
                return NULL;
            }

            *pnFileLen = nLen;
            return sBuffer;
        }

        // S = signed / U = unsigned
        // 8/16/32/Var = word length, in bytes
        // BE = big endian
        int          GetS8    (const unsigned int& nPos, bool *pbSuccess)
        {
            //*pbSuccess = true;

            if ( nPos >= m_nLen )
            {
                *pbSuccess = false;
                return 0;
            }
            int nRes = m_sFile[ nPos ];
            if ( nRes & 0x80 )
                nRes |= ~0xff;
            return nRes;
        }

        int          GetU8    (const unsigned int& nPos, bool *pbSuccess)
        {
            //*pbSuccess = true;
            if ( nPos >= m_nLen )
            {
                *pbSuccess = false;
                return 0;
            }
            return m_sFile[ nPos ];
        }

        int          GetS16BE (const unsigned int& nPos, bool *pbSuccess)
        {
            //*pbSuccess = true;

            if ( m_nLen < 2 || nPos > (m_nLen - 2) )
            {
                *pbSuccess = false;
                return 0;
            }
            int nRes = m_sFile[nPos];
            nRes = (nRes << 8) + m_sFile[ nPos + 1 ];
            if ( nRes & 0x8000 )
                nRes |= ~0xffff;
            return nRes;
        }

        int          GetU16BE (const unsigned int& nPos, bool *pbSuccess)
        {
            //*pbSuccess = true;

            if ( m_nLen < 2 || nPos > (m_nLen - 2) )
            {
                *pbSuccess = false;
                return 0;
            }
            int nRes = m_sFile[ nPos ];
            nRes = (nRes << 8) + m_sFile[ nPos + 1 ];
            return nRes;
        }

        int          GetS32BE (const unsigned int& nPos, bool *pbSuccess)
        {
            //*pbSuccess = true;

            if ( m_nLen < 4 || nPos > (m_nLen - 4) )
            {
                *pbSuccess = false;
                return 0;
            }
            int nRes = m_sFile[ nPos ];
            nRes = (nRes << 8) + m_sFile[nPos + 1];
            nRes = (nRes << 8) + m_sFile[nPos + 2];
            nRes = (nRes << 8) + m_sFile[nPos + 3];
            if ( nRes & 0x80000000 )
                nRes |= ~0xffffffff;

            return nRes;
        }

        unsigned int GetU32BE (const unsigned int& nPos, bool *pbSuccess)
        {
            //*pbSuccess = true;

            if ( m_nLen < 4 || nPos > (m_nLen - 4) )
            {
                *pbSuccess = false;
                return 0;
            }
            unsigned int nRes = m_sFile[nPos];
            nRes = (nRes << 8) + m_sFile[nPos + 1];
            nRes = (nRes << 8) + m_sFile[nPos + 2];
            nRes = (nRes << 8) + m_sFile[nPos + 3];
            return nRes;
        }
        unsigned int GetU32LE (const unsigned int& nPos, bool *pbSuccess)
        {
            //*pbSuccess = true;

            if ( m_nLen < 4 || nPos > (m_nLen - 4) )
            {
                *pbSuccess = false;
                return 0;
            }
            unsigned int nRes = m_sFile[nPos + 3];
            nRes = (nRes << 8) + m_sFile[nPos + 2];
            nRes = (nRes << 8) + m_sFile[nPos + 1];
            nRes = (nRes << 8) + m_sFile[nPos + 0];
            return nRes;
        }
        unsigned int GetUVarBE(const unsigned int& nPos, const unsigned int& nSize, bool *pbSuccess)
        {
            //*pbSuccess = true;

            if ( m_nLen < nSize || nPos > (m_nLen - nSize) )
            {
                *pbSuccess = false;
                return 0;
            }
            unsigned int nRes = 0;
            for ( int nIndex = 0; nIndex < nSize; ++nIndex )
                nRes = (nRes << 8) + m_sFile[nPos + nIndex];

            return nRes;
        }

        bool         CheckRegion(const unsigned int& nPos, const unsigned int& nSize)
        {
            return (m_nLen >= nSize && nPos <= (m_nLen - nSize));
        }
        int          ReadS8   (bool *pbSuccess)
        {
            return GetS8( m_nPos++, pbSuccess );
        }
        int          ReadU8   (bool *pbSuccess)
        {
            return GetU8( m_nPos++, pbSuccess );
        }
        unsigned int ReadU32BE(bool *pbSuccess)
        {
            unsigned int unResult = GetU32BE( m_nPos, pbSuccess );
            m_nPos += 4;
            return unResult;
        }
        unsigned int ReadU32LE(bool *pbSuccess)
        {
            unsigned int unResult = GetU32LE( m_nPos, pbSuccess );
            m_nPos += 4;
            return unResult;
        }
        int          Read(void* pDestBuffer, unsigned int nSize)
        {
            if (m_nPos >= m_nLen)
                nSize = 0;
            else if (nSize > (m_nLen - m_nPos))
                nSize = m_nLen - m_nPos;

            if (0 == nSize)
                return nSize;

            memcpy( pDestBuffer, (m_sFile + m_nPos), nSize );
            m_nPos += nSize;

            return nSize;
        }

    protected:

        unsigned char *m_sFileData;
        unsigned char *m_sFile;
        unsigned int   m_nLen;
        unsigned int   m_nPos;
        bool           m_bFreeFileData;
    };
}

#endif /* _ASC_FONTCONVERTER_FONT_FILE_BASE_H */
