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
#include "ByteBuilder.h"
#include "File.h"

namespace NSMemoryUtils
{
    CByteBuilder::CByteBuilder()
    {
        m_pData = NULL;
        m_lSize = 0;

        m_pDataCur = m_pData;
        m_lSizeCur = m_lSize;
    }
    CByteBuilder::~CByteBuilder()
    {
        if (NULL != m_pData)
            free(m_pData);
        m_pData = NULL;
    }
    void CByteBuilder::AddSize(size_t nSize)
    {
        if (NULL == m_pData)
        {
            m_lSize = (std::max)((int)nSize, 1000);
            m_pData = (BYTE*)malloc(m_lSize * sizeof(BYTE));

            m_lSizeCur = 0;
            m_pDataCur = m_pData;
            return;
        }

        if ((m_lSizeCur + nSize) > m_lSize)
        {
            while ((m_lSizeCur + nSize) > m_lSize)
            {
                m_lSize *= 2;
            }

            BYTE* pRealloc = (BYTE*)realloc(m_pData, m_lSize * sizeof(BYTE));
            if (NULL != pRealloc)
            {
                // realloc succeeded
                m_pData = pRealloc;
                m_pDataCur = m_pData + m_lSizeCur;
            }
            else
            {
                BYTE* pMalloc = (BYTE*)malloc(m_lSize * sizeof(BYTE));
                memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(BYTE));

                free(m_pData);
                m_pData = pMalloc;
                m_pDataCur = m_pData + m_lSizeCur;
            }
        }
    }

    void CByteBuilder::WriteString(const std::string& sText)
    {
        int nSizeof = sizeof(char);
        size_t nSize = (sText.length() + 1) * nSizeof;
        AddSize(nSize);
        memcpy(m_pDataCur, sText.c_str(), nSize - nSizeof);

        m_pDataCur += (nSize - nSizeof);
        (*((char*)m_pDataCur)) = 0;
        m_lSizeCur += nSize;
        m_pDataCur += nSizeof;
    }
    void CByteBuilder::WriteString(const std::wstring& sText)
    {
        int nSizeof = sizeof(wchar_t);
        size_t nSize = (sText.length() + 1) * nSizeof;
        AddSize(nSize);
        memcpy(m_pDataCur, sText.c_str(), nSize - nSizeof);

        m_pDataCur += (nSize - nSizeof);
        (*((char*)m_pDataCur)) = 0;
        m_lSizeCur += nSize;
        m_pDataCur += nSizeof;
    }
    void CByteBuilder::WriteStringUTF8(const std::wstring& sText)
    {
        WriteString(U_TO_UTF8(sText));
    }

    void CByteBuilder::WriteInt(const int& value)
    {
        int nSizeof = sizeof(int);
        AddSize(nSizeof);
        memcpy(m_pDataCur, &value, nSizeof);

        m_lSizeCur += nSizeof;
        m_pDataCur += nSizeof;
    }

    size_t CByteBuilder::GetCurSize()
    {
        return m_lSizeCur;
    }
    void CByteBuilder::SetCurSize(size_t lCurSize)
    {
        m_lSizeCur = lCurSize;
        m_pDataCur = m_pData + m_lSizeCur;
    }
    size_t CByteBuilder::GetSize()
    {
        return m_lSize;
    }

    void CByteBuilder::Clear()
    {
        if (NULL != m_pData)
            free(m_pData);
        m_pData = NULL;

        m_pData = NULL;
        m_lSize = 0;

        m_pDataCur = m_pData;
        m_lSizeCur = 0;
    }
    void CByteBuilder::ClearNoAttack()
    {
        m_pDataCur = m_pData;
        m_lSizeCur = 0;
    }

    BYTE* CByteBuilder::GetData()
    {
        return m_pData;
    }

    // Reader
    CByteReader::CByteReader(BYTE* pData)
    {
        m_pData = pData;
        m_pDataCur = pData;
    }

    CByteReader::~CByteReader()
    {
    }

    std::string CByteReader::GetString()
    {
        BYTE* pEnd = m_pDataCur;
        while (*pEnd != 0)
            pEnd++;
        std::string sRet((char*)m_pDataCur, pEnd - m_pDataCur);
        m_pDataCur = pEnd + 1;
        return sRet;
    }
    std::wstring CByteReader::GetStringW()
    {
        wchar_t* pStart = (wchar_t*)m_pDataCur;
        wchar_t* pEnd = pStart;
        while (*pEnd != 0)
            pEnd++;
        std::wstring sRet((wchar_t*)m_pDataCur, pEnd - pStart);
        m_pDataCur += (((pEnd - pStart) + 1) * sizeof(wchar_t));
        return sRet;
    }
    std::wstring CByteReader::GetStringUTF8()
    {
        BYTE* pEnd = m_pDataCur;
        while (*pEnd != 0)
            pEnd++;
        std::string sRet((char*)m_pDataCur, pEnd - m_pDataCur);
        m_pDataCur = pEnd + 1;
        return UTF8_TO_U(sRet);
    }

    int CByteReader::GetInt()
    {
        int nRet = 0;
        memcpy(&nRet, m_pDataCur, sizeof(int));
        m_pDataCur += sizeof(int);
        return nRet;
    }
}
