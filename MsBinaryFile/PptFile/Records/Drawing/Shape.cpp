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
#include "Shape.h"

using namespace PPT;

CRecordShape::CRecordShape()
{

}

CRecordShape::~CRecordShape()
{

}

void CRecordShape::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_nID = (UINT)StreamUtils::ReadDWORD(pStream);

    UINT nFlag = (UINT)StreamUtils::ReadDWORD(pStream);

    m_bGroup = ((nFlag & 0x01) == 0x01);
    m_bChild = ((nFlag & 0x02) == 0x02);
    m_bPatriarch = ((nFlag & 0x04) == 0x04);
    m_bDeleted = ((nFlag & 0x08) == 0x08);
    m_bOleShape = ((nFlag & 0x10) == 0x10);
    m_bHaveMaster = ((nFlag & 0x20) == 0x20);
    m_bFlipH = ((nFlag & 0x40) == 0x40);
    m_bFlipV = ((nFlag & 0x80) == 0x80);
    m_bConnector = ((nFlag & 0x0100) == 0x0100);
    m_bHaveAnchor = ((nFlag & 0x0200) == 0x0200);
    m_bBackground = ((nFlag & 0x0400) == 0x0400);
    m_bHaveSpt = ((nFlag & 0x0800) == 0x0800);

    LONG lSize = m_oHeader.RecLen - 8;
    if (lSize > 0)
    {
        StreamUtils::StreamSkip(lSize, pStream);
    }

    m_nShapeID = m_oHeader.RecInstance;
}
