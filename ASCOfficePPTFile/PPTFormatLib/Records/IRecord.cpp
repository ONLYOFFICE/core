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
#pragma once


#include "IRecord.h"

#include "../Reader/Records.h"
#include "../../../ASCOfficeXlsFile2/source/XlsFormat/Binary/CFStream.h"


using namespace XLS;


std::wstring CUnknownRecord::ReadStringW(const CFStreamPtr &pStream, int lLen)
{
    if (!pStream) return (L"");

    unsigned char* pData = new unsigned char[2 * (lLen + 1)];
    memset (pData, 0, 2 * (lLen + 1));

    pStream->read(pData, 2 * lLen);

    if (sizeof(wchar_t) == 4)
    {
        ConversionResult eUnicodeConversionResult;
        UTF32 *pStrUtf32 = new UTF32 [lLen + 1];
        pStrUtf32[lLen] = 0 ;

        const UTF16 *pStrUtf16_Conv = (const UTF16 *) pData;
        UTF32		*pStrUtf32_Conv =                 pStrUtf32;

        eUnicodeConversionResult = ConvertUTF16toUTF32 ( &pStrUtf16_Conv
                                                         , &pStrUtf16_Conv[lLen]
                                                         , &pStrUtf32_Conv
                                                         , &pStrUtf32 [lLen]
                                                         , strictConversion);

        if (conversionOK != eUnicodeConversionResult)
        {
            delete [] pStrUtf32;
            return (L"");
        }
        std::wstring res((wchar_t*)pStrUtf32, lLen);
        if (pStrUtf32) delete [] pStrUtf32;
        return res;
    }
    else
    {
        std::wstring str((wchar_t*)pData);
        delete[] pData;
        return str;
    }

}


std::string CUnknownRecord::ReadStringA(const CFStreamPtr &pStream, int lLen)
{
    if (!pStream) return ("");

    char* pData = new char[lLen + 1];

    pStream->read((unsigned char*)pData, lLen);

    pData[lLen] = 0;

    std::string str(pData, lLen);

    delete[] pData;
    return str;
}


void CRecordsContainer::ReadFromStream(SRecordHeader & oHeader, const CFStreamPtr &pStream)
{
    m_oHeader = oHeader;
    m_arRecords.clear();

    LONG lPosition = pStream->getStreamPointer();

    m_oHeader = oHeader;

    UINT lCurLen = 0;
    ULONG lReadLen = 0;
    SRecordHeader oRec;

    while (lCurLen < m_oHeader.RecLen)
    {
        if (oRec.ReadFromStream(pStream) == FALSE )
        {
            break;
        }

        IRecord* pRecord = CreateByType(oRec);
        pRecord->ReadFromStream(oRec, pStream);

        m_arRecords.push_back(pRecord);
        lCurLen += (8 + oRec.RecLen);
    }
    if (lCurLen != m_oHeader.RecLen)
    {
        // нужно разобраться, что тут такое!!!
        LONG lPosition = 0;
        pStream->seekFromBegin(lPosition + m_oHeader.RecLen);
    }
}


void CRecordsContainer::ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
{
    m_oHeader = oHeader;
    m_arRecords.clear();

    LONG lPosition = 0;
    StreamUtils::StreamPosition(lPosition, pStream);

    m_oHeader = oHeader;

    UINT lCurLen = 0;
    ULONG lReadLen = 0;
    SRecordHeader oRec;
    while (lCurLen < m_oHeader.RecLen)
    {
        if (oRec.ReadFromStream(pStream) == FALSE )
        {
            break;
        }

        IRecord* pRecord = CreateByType(oRec);
        pRecord->ReadFromStream(oRec, pStream);

        m_arRecords.push_back(pRecord);
        lCurLen += (8 + oRec.RecLen);
    }
    if (lCurLen != m_oHeader.RecLen)
    {
        // нужно разобраться, что тут такое!!!
        LONG lPosition = 0;
        StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
    }
}
