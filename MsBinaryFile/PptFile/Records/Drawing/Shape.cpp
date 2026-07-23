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
