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

    // на всякий случай...
    // здесь когда текст сначала другой (т.е. например - placeholder в мастере) -
    // то у нас неправильно выставился m_lCount...
    StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
}
