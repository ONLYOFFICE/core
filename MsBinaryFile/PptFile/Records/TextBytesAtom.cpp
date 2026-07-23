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
#include "TextBytesAtom.h"

using namespace PPT;

CRecordTextBytesAtom::CRecordTextBytesAtom()
{
}

CRecordTextBytesAtom::~CRecordTextBytesAtom()
{
}

void CRecordTextBytesAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    //UTF-16 Unicode  character whose high byte is 0x00.
    unsigned short	*pUTF16		= new unsigned short[m_oHeader.RecLen];
    unsigned char	*pUTF16_low = new unsigned char	[m_oHeader.RecLen];

    if (pUTF16 && pUTF16_low)
    {
        pStream->read(pUTF16_low, m_oHeader.RecLen);

        for (UINT i = 0 ; i < m_oHeader.RecLen; i++)
        {
            pUTF16[i] = pUTF16_low[i];
        }

        m_strText = NSFile::CUtf8Converter::GetWStringFromUTF16(pUTF16, m_oHeader.RecLen);

    }

    RELEASEARRAYOBJECTS(pUTF16_low);
    RELEASEARRAYOBJECTS(pUTF16);

    //std::string tmpStrTextA = StreamUtils::ReadCStringA(pStream, m_oHeader.RecLen);

    //std::wstring tmpStrTextW (tmpStrTextA.begin(), tmpStrTextA.end());

    //m_strText = std_string2string(tmpStrTextW);
}
