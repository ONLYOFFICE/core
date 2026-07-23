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
#include "StyleTextPropAtom.h"

using namespace PPT;

CRecordStyleTextPropAtom::CRecordStyleTextPropAtom()
{
    m_lCount = 0;
    m_lOffsetInStream = 0;
}

CRecordStyleTextPropAtom::~CRecordStyleTextPropAtom()
{
    m_lCount = 0;
}

void CRecordStyleTextPropAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    StreamUtils::StreamPosition(m_lOffsetInStream, pStream);

    _UINT32 lMemCount = 0;
    _UINT32 lCountItems = 0;

    if (0 == m_lCount)
    {
        StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
        return;
    }

    while (lMemCount < m_lCount + 1)
    {
        CTextPFRunRecord elm;
        m_arrPFs.push_back(elm);
        m_arrPFs[lCountItems].LoadFromStream(pStream);
        lMemCount += m_arrPFs[lCountItems].m_lCount;

        ++lCountItems;
    }

    lMemCount = 0;
    lCountItems = 0;
    while (lMemCount < m_lCount + 1)
    {
        CTextCFRunRecord elm;
        m_arrCFs.push_back(elm);

        m_arrCFs[lCountItems].LoadFromStream(pStream);
        lMemCount += m_arrCFs[lCountItems].m_lCount;

        ++lCountItems;
    }

    // just in case...
    // here when the text is initially different (e.g. placeholder in master) -
    // then m_lCount is set incorrectly...
    StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
}
