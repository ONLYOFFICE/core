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
#pragma once

#include "PPTFileDefines.h"
#include "../Enums/RecordType.h"
#include "../Reader/ReadStructures.h"
#include "../../Common/Vml/PPTShape/Enums.h"
#include "../../XlsFile/Format/Binary/CFStream.h"
#include "../../../Common/3dParty/pole/pole.h"
#include "../../../OfficeCryptReader/source/CryptTransform.h"
#include "../../../OOXML/Base/Nullable.h"
#include "ReadStructures.h"

#include <boost/smart_ptr/shared_array.hpp>

//#include <iostream>
//#include <iomanip>
//#include <fstream>

std::string GetRecordName(PPT::RecordType dwType);

namespace PPT
{
    struct _commonInfo
    {
        std::wstring tempPath;
    };
class SRecordHeader
{
public:
    unsigned char   RecVersion;
    unsigned short  RecInstance;
    RecordType	    RecType;
    _UINT32         RecLen;

    bool bBadHeader;

    void Clear();
    SRecordHeader();
    bool ReadFromStream(const XLS::CFStreamPtr &pStream);

    bool ReadFromStream(POLE::Stream * pStream);

    bool IsContainer();

    SRecordHeader& operator =(const SRecordHeader& oSrc);

};

class IRecord
{
public:
    SRecordHeader m_oHeader;
    _commonInfo* m_pCommonInfo = NULL;

    virtual ~IRecord();
    virtual void ReadFromStream(SRecordHeader & oHeader, const XLS::CFStreamPtr &pStream) = 0;
    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream) = 0;
};

class CUnknownRecord : public IRecord
{
public:
    CUnknownRecord();

    virtual ~CUnknownRecord();
    virtual void ReadFromStream(SRecordHeader & oHeader, const XLS::CFStreamPtr &pStream);
    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream);

    std::wstring ReadStringW(const XLS::CFStreamPtr &pStream, int size);
    std::string	 ReadStringA(const XLS::CFStreamPtr &pStream, int size);
};

IRecord* CreateByType(SRecordHeader oHeader, _commonInfo* commonInfo);

class CRecordsContainer : public CUnknownRecord
{
public:
    std::vector<IRecord*> m_arRecords;

    CRecordsContainer();

    virtual ~CRecordsContainer();

    void Clear();

    virtual void ReadFromStream(SRecordHeader & oHeader, const XLS::CFStreamPtr &pStream);
    virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream);

    template <typename T>
    void GetRecordsByType(std::vector<T>* pArray, bool bIsChild, bool bOnlyFirst = false) const
    {
        if (NULL == pArray)
            return;

        // returning pointers, don't delete them above!!!!
        for (size_t nIndex = 0; nIndex < m_arRecords.size(); ++nIndex)
        {
            T pRec = dynamic_cast<T>(m_arRecords[nIndex]);
            if (NULL != pRec)
            {
                pArray->push_back(pRec);
                if (bOnlyFirst)
                {
                    return;
                }
            }
            else if ((bIsChild) && (m_arRecords[nIndex]->m_oHeader.IsContainer()))
            {
                CRecordsContainer* pContainer = dynamic_cast<CRecordsContainer*>(m_arRecords[nIndex]);
                if (NULL != pContainer)
                {
                    pContainer->GetRecordsByType(pArray, bIsChild, bOnlyFirst);
                }
            }
        }
    }
};
}

