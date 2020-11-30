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
#ifndef MEMORYSTREAM
#define MEMORYSTREAM

#include <string>
#include "../common/Types.h"
#include "../common/File.h"

namespace NSMemoryStream
{
class CMemoryStream
{
private:
    // сам метафайл
    BYTE* m_pBuffer;
    BYTE* m_pBufferMem;

    size_t m_lSize;

public:
    CMemoryStream()
    {
        m_pBuffer		= NULL;
        m_pBufferMem	= NULL;

        m_lSize			= 0;
    }
    ~CMemoryStream()
    {
        RELEASEARRAYOBJECTS(m_pBuffer);
    }

    inline BYTE* GetData()
    {
        return m_pBuffer;
    }
    inline int GetSize()
    {
        return (int)(m_pBufferMem - m_pBuffer);
    }

    inline void Clear()
    {
        m_lSize		= 0;

        m_pBuffer		= NULL;
        m_pBufferMem	= NULL;
    }

    inline void ClearNoAttack()
    {
        m_pBufferMem = m_pBuffer;
    }

    inline void Copy(const CMemoryStream* pData, const size_t& nPos, const size_t& nLen)
    {
        CheckBufferSize(nLen);
        memcpy(m_pBufferMem, pData->m_pBuffer + nPos, nLen);
        m_pBufferMem += nLen;
    }

    inline void CheckBufferSize(size_t lPlus)
    {
        if (NULL != m_pBuffer)
        {
            size_t nNewSize = (m_pBufferMem - m_pBuffer) + lPlus;
            if (nNewSize >= m_lSize)
            {
                while (nNewSize >= m_lSize)
                {
                    m_lSize *= 2;
                }

                BYTE* pNew = new BYTE[m_lSize];
                memcpy(pNew, m_pBuffer, m_pBufferMem - m_pBuffer);

                m_pBufferMem	= pNew + (m_pBufferMem - m_pBuffer);

                RELEASEARRAYOBJECTS(m_pBuffer);
                m_pBuffer		= pNew;
            }
        }
        else
        {
            m_lSize			= 1000;
            m_pBuffer		= new BYTE[m_lSize];
            m_pBufferMem	= m_pBuffer;

            CheckBufferSize(lPlus);
        }
    }

    inline void WriteBYTE(const BYTE& lValue)
    {
        CheckBufferSize(sizeof(BYTE));
        *m_pBufferMem = lValue;
        m_pBufferMem += sizeof(BYTE);
    }
    inline void WriteLONG(const LONG& lValue)
    {
        CheckBufferSize(sizeof(LONG));
        *((LONG*)(m_pBufferMem)) = lValue;
        m_pBufferMem += sizeof(LONG);
    }
    inline void WriteDouble(const double& dValue)
    {
        CheckBufferSize(sizeof(double));
        *((double*)(m_pBufferMem)) = dValue;
        m_pBufferMem += sizeof(double);
    }
    inline void WriteStringA(const char* pData, int nLen)
    {
        CheckBufferSize(nLen + sizeof(USHORT));
        *((USHORT*)(m_pBufferMem)) = (USHORT)nLen;
        m_pBufferMem += sizeof(USHORT);

        memcpy(m_pBufferMem, pData, nLen);
        m_pBufferMem += nLen;
    }
    inline void WriteStringA2(const char* pData, int nLen)
    {
        CheckBufferSize(nLen + sizeof(LONG));
        *((LONG*)(m_pBufferMem)) = (LONG)nLen;
        m_pBufferMem += sizeof(LONG);

        memcpy(m_pBufferMem, pData, nLen);
        m_pBufferMem += nLen;
    }

    inline void WriteString(const wchar_t* pData, int nLen)
    {
        CheckBufferSize(nLen + sizeof(USHORT));
        *((USHORT*)(m_pBufferMem)) = (USHORT)nLen;
        m_pBufferMem += sizeof(USHORT);

        int nLen2 = nLen << 1;
        if (sizeof(wchar_t) == 2)
        {
            memcpy(m_pBufferMem, pData, nLen2);
        }
        else
        {
            int len = nLen >> 1;
            USHORT* mass = new USHORT[len];
            for (int i = 0; i < len; ++i)
                mass[i] = (USHORT)pData[i];
            memcpy(m_pBufferMem, mass, nLen2);
            RELEASEARRAYOBJECTS(mass);
        }
        m_pBufferMem += nLen2;
    }
    inline void WriteString2(const wchar_t* pData, int nLen)
    {
        int nLen2 = nLen << 1;

        CheckBufferSize(nLen2 + sizeof(LONG));
        *((LONG*)(m_pBufferMem)) = (LONG)nLen2;
        m_pBufferMem += sizeof(LONG);

        if (sizeof(wchar_t) == 2)
        {
            memcpy(m_pBufferMem, pData, nLen2);
        }
        else
        {
            USHORT* mass = new USHORT[nLen];
            for (int i = 0; i < nLen; ++i)
                mass[i] = (USHORT)pData[i];
            memcpy(m_pBufferMem, mass, nLen2);
            RELEASEARRAYOBJECTS(mass);
        }
        m_pBufferMem += nLen2;
    }
};
}

#endif // MEMORYSTREAM

