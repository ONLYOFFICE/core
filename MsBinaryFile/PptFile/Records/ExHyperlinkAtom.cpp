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
