/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "base.h"

#include <vector>
#include <cstring>

class CData
{
protected:
    unsigned char* m_pData;
    size_t m_lSize;

    unsigned char* m_pDataCur;
    size_t m_lSizeCur;

public:
    CData()
    {
        m_pData = NULL;
        m_lSize = 0;

        m_pDataCur = m_pData;
        m_lSizeCur = m_lSize;
    }
    CData(size_t nLen)
    {
        m_lSize = nLen;
        m_pData = (unsigned char*)malloc(m_lSize * sizeof(unsigned char));

        m_lSizeCur = 0;
        m_pDataCur = m_pData;
    }
    CData(unsigned char* value, unsigned int len)
    {
        m_lSize = len;
        m_pData = value;

        m_lSizeCur = m_lSize;
        m_pDataCur = m_pData + m_lSizeCur;
    }
    virtual ~CData()
    {
        Clear();
    }

    inline void AddSize(size_t nSize)
    {
        if (NULL == m_pData)
        {
            m_lSize = 1000;
            if (nSize > m_lSize)
                m_lSize = nSize;

            m_pData = (unsigned char*)malloc(m_lSize * sizeof(unsigned char));

            m_lSizeCur = 0;
            m_pDataCur = m_pData;
            return;
        }

        if ((m_lSizeCur + nSize) > m_lSize)
        {
            while ((m_lSizeCur + nSize) > m_lSize)
                m_lSize *= 2;

            unsigned char* pRealloc = (unsigned char*)realloc(m_pData, m_lSize * sizeof(unsigned char));
            if (NULL != pRealloc)
            {
                m_pData    = pRealloc;
                m_pDataCur = m_pData + m_lSizeCur;
            }
            else
            {
                unsigned char* pMalloc = (unsigned char*)malloc(m_lSize * sizeof(unsigned char));
                memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(unsigned char));

                free(m_pData);
                m_pData    = pMalloc;
                m_pDataCur = m_pData + m_lSizeCur;
            }
        }
    }

public:
    void AddInt(unsigned int value)
    {
        AddSize(4);
        memcpy(m_pDataCur, &value, sizeof(unsigned int));
        m_pDataCur += 4;
        m_lSizeCur += 4;
    }
    void WriteString(unsigned char* value, unsigned int len)
    {
        AddSize(len + 4);
        memcpy(m_pDataCur, &len, sizeof(unsigned int));
        m_pDataCur += 4;
        m_lSizeCur += 4;
        memcpy(m_pDataCur, value, len);
        m_pDataCur += len;
        m_lSizeCur += len;
    }
    unsigned char* GetBuffer()
    {
        return m_pData;
    }

    void Clear()
    {
        if (m_pData) free(m_pData);

        m_pData = NULL;
        m_lSize = 0;

        m_pDataCur = m_pData;
        m_lSizeCur = 0;
    }
    void ClearNoAttack()
    {
        m_pDataCur = m_pData;
        m_lSizeCur = 0;
    }
    unsigned int GetSize()
    {
        return (unsigned int)m_lSizeCur;
    }

    void SkipLen()
    {
        AddInt(0);
    }
    void WriteLen()
    {
        unsigned int len = (unsigned int)m_lSizeCur;
        memcpy(m_pData, &len, sizeof(unsigned int));
    }
};

void* Zlib_Malloc(unsigned int size)
{
    return ::malloc(size);
}
void  Zlib_Free(void* p)
{
    if (p) ::free(p);
}

CZipBuffer* Zlib_Create()
{
    return new CZipBuffer();
}
CZipBuffer* Zlib_Open(unsigned char* buffer, unsigned long size)
{
    return new CZipBuffer(buffer, size);
}
void   Zlib_Close(CZipBuffer* p)
{
    if (p) delete p;
}

void Zlib_AddFile   (CZipBuffer* p, const char* path, unsigned char* data, unsigned long length)
{
    if (!p)
        return;
    p->addFile(path, data, length);
}
void Zlib_RemoveFile(CZipBuffer* p, const char* path)
{
    if (!p)
        return;
    p->removeFile(path);
}

unsigned char* Zlib_GetPaths(CZipBuffer* p)
{
    if (!p)
        return NULL;
    std::vector<std::string> oRes = p->getPaths();
    if (oRes.empty())
        return NULL;

    CData* oPaths = new CData();
    oPaths->SkipLen();
    for (std::string& sPath : oRes)
        oPaths->WriteString((unsigned char*)sPath.c_str(), sPath.length());
    oPaths->WriteLen();
    return oPaths->GetBuffer();
}
unsigned char* Zlib_GetFile (CZipBuffer* p, const char* path)
{
    if (!p)
        return NULL;
    unsigned char* data;
    unsigned long length;
    p->getFile(path, data, length);
    if (length == 0)
        return NULL;

    CData* oData = new CData();
    oData->WriteString(data, length);
    return oData->GetBuffer();
}
unsigned char* Zlib_Save    (CZipBuffer* p)
{
    if (!p)
        return NULL;
    unsigned char* data;
    unsigned long length;
    p->save(data, length);
    if (length == 0)
        return NULL;

    CData* oData = new CData();
    oData->WriteString(data, length);
    return oData->GetBuffer();
}
