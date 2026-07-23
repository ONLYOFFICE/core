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
#include "ExHyperlinkAtom.h"

using namespace PPT;

void CRecordExHyperlinkAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nHyperlinkID = StreamUtils::ReadDWORD(pStream);
}

bool CRecordExHyperlinkContainer::hasCString() const
{
    return m_friendlyNameAtom.IsInit() || m_targetAtom.IsInit() || m_locationAtom.IsInit();
}

void CRecordExHyperlinkContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    SRecordHeader header;

    header.ReadFromStream(pStream);
    m_exHyperlinkAtom.ReadFromStream(header, pStream);

    unsigned currentLen = 12; // 12(atom)
    while (currentLen < m_oHeader.RecLen)
    {
        header.ReadFromStream(pStream);
        auto* pCString = new CRecordCString;
        pCString->ReadFromStream(header, pStream);
        switch (header.RecInstance)
        {
        case 0: m_friendlyNameAtom  = pCString; break;
        case 1: m_targetAtom        = pCString; break;
        case 3: m_locationAtom      = pCString; break;
        default: delete pCString;
        }
        currentLen += 8 + header.RecLen; // headerLen + CStringLen
    }
//        if (m_friendlyNameAtom.IsInit() && (int)m_friendlyNameAtom->m_strText.find(L"NEXT") == -1)
//            std::wcout << m_exHyperlinkAtom.m_nHyperlinkID << L" "
//                       << (m_friendlyNameAtom.IsInit() ? m_friendlyNameAtom->m_strText : L"-") << L" "
//                       << (m_targetAtom.IsInit() ? m_targetAtom->m_strText : L"-") << L" "
//                       << (m_locationAtom.IsInit() ? m_locationAtom->m_strText : L"-")
//                       << std::endl;
}
