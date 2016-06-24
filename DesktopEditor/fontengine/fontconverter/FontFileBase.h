/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
            m_nLen = nLen;
            m_bFreeFileData = bFreeFileData;
            m_nPos = 0;
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
        int          GetS8    (int nPos, bool *pbSuccess)
        {
            *pbSuccess = true;

            if ( nPos < 0 || nPos >= m_nLen )
            {
                *pbSuccess = false;
                return 0;
            }
            int nRes = m_sFile[ nPos ];
            if ( nRes & 0x80 )
                nRes |= ~0xff;
            return nRes;
        }

        int          GetU8    (int nPos, bool *pbSuccess)
        {
            *pbSuccess = true;
            if ( nPos < 0 || nPos >= m_nLen )
            {
                *pbSuccess = false;
                return 0;
            }
            return m_sFile[ nPos ];
        }

        int          GetS16BE (int nPos, bool *pbSuccess)
        {
            *pbSuccess = true;

            if ( nPos < 0 || nPos + 1 >= m_nLen )
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

        int          GetU16BE (int nPos, bool *pbSuccess)
        {
            *pbSuccess = true;

            if ( nPos < 0 || nPos + 1 >= m_nLen)
            {
                *pbSuccess = false;
                return 0;
            }
            int nRes = m_sFile[ nPos ];
            nRes = (nRes << 8) + m_sFile[ nPos + 1 ];
            return nRes;
        }

        int          GetS32BE (int nPos, bool *pbSuccess)
        {
            *pbSuccess = true;

            if ( nPos < 0 || nPos + 3 >= m_nLen )
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

        unsigned int GetU32BE (int nPos, bool *pbSuccess)
        {
            *pbSuccess = true;

            if ( nPos < 0 || nPos + 3 >= m_nLen )
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
        unsigned int GetU32LE (int nPos, bool *pbSuccess)
        {
            *pbSuccess = true;

            if ( nPos < 0 || nPos + 3 >= m_nLen )
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
        unsigned int GetUVarBE(int nPos, int nSize, bool *pbSuccess)
        {
            *pbSuccess = true;

            if ( nPos < 0 || nPos + nSize > m_nLen )
            {
                *pbSuccess = false;
                return 0;
            }
            unsigned int nRes = 0;
            for ( int nIndex = 0; nIndex < nSize; ++nIndex )
                nRes = (nRes << 8) + m_sFile[nPos + nIndex];

            return nRes;
        }

        bool         CheckRegion(int nPos, int nSize)
        {
            return (nPos >= 0 && nPos + nSize >= nPos && nPos + nSize <= m_nLen);
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
        int          Read(void* pDestBuffer, int nSize)
        {
            if ( m_nPos + nSize >= m_nLen )
                nSize = m_nLen - m_nPos - 1;

            memcpy( pDestBuffer, (m_sFile + m_nPos), nSize );
            m_nPos += nSize;

            return nSize;
        }

    protected:

        unsigned char *m_sFileData;
        unsigned char *m_sFile;
        int            m_nLen;
        bool           m_bFreeFileData;
        int            m_nPos;

    };
}

#endif /* _ASC_FONTCONVERTER_FONT_FILE_BASE_H */
