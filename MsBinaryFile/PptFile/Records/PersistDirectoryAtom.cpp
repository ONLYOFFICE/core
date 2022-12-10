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
