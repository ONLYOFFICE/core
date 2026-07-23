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
#include "PersistDirectoryAtom.h"

using namespace PPT;

_UINT32 CPersistDirectoryEntry::FromStream(POLE::Stream *pStream)
{
    _UINT32 nFlag = StreamUtils::ReadDWORD(pStream);
    m_nPersistID = (nFlag & 0x000FFFFF);			// 20 bit
    m_nPersistCount = (nFlag & 0xFFF00000) >> 20;	// 12 bit

    m_arPersistOffsets.clear();

    for (_UINT32 index = 0; index < m_nPersistCount; ++index)
    {
        _UINT32 Mem = StreamUtils::ReadDWORD(pStream);
        m_arPersistOffsets.push_back(Mem);
    }

    return 4 * (m_nPersistCount + 1);
}

void CRecordPersistDirectoryAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    _UINT32 nCountRead = 0;
    _UINT32 nCountEnries = 0;
    while (nCountRead < m_oHeader.RecLen)
    {
        CPersistDirectoryEntry elm;
        m_arEntries.push_back(elm);

        nCountRead += m_arEntries[nCountEnries].FromStream(pStream);
        ++nCountEnries;
    }
}

void CRecordPersistDirectoryAtom::ToMap(std::map<_UINT32, _UINT32> *pMap)
{
    pMap->clear();

    for (size_t nEntry = 0; nEntry < m_arEntries.size(); ++nEntry)
    {
        _UINT32 nPID = m_arEntries[nEntry].m_nPersistID;

        for (size_t nIndex = 0; nIndex < m_arEntries[nEntry].m_nPersistCount; ++nIndex)
        {
            _UINT32 nOffset = m_arEntries[nEntry].m_arPersistOffsets[nIndex];
            pMap->insert(std::pair<_UINT32, _UINT32>(nPID, nOffset));
            ++nPID;
        }
    }
}
