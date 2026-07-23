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
#include "CFMasks.h"

using namespace PPT;

void SCFMasks::ReadFromStream(POLE::Stream *pStream)
{
    BYTE data1  = StreamUtils::ReadBYTE(pStream);
    BYTE data2  = StreamUtils::ReadBYTE(pStream);
    BYTE data3  = StreamUtils::ReadBYTE(pStream);
    BYTE data4  = StreamUtils::ReadBYTE(pStream);

    m_bold              = 0x01 == (0x01 & data1);
    m_italic            = 0x02 == (0x02 & data1);
    m_underline         = 0x04 == (0x04 & data1);
    // unused
    m_shadow            = 0x10 == (0x10 & data1);
    m_fehint            = 0x20 == (0x20 & data1);
    // unused2
    m_kumi              = 0x80 == (0x80 & data1);
    // unused3

    m_emboss            = 0x02 == (0x02 & data2);
    m_fHasStyle         = 0x3C & data2; // 4 bits
    // unused 2 bits

    m_typeface          = 0x01 == (0x01 & data3);
    m_size              = 0x02 == (0x02 & data3);
    m_color             = 0x04 == (0x04 & data3);
    m_position          = 0x08 == (0x08 & data3);
    m_pp10ext           = 0x10 == (0x10 & data3);
    m_oldEATypeface     = 0x20 == (0x20 & data3);
    m_ansiTypeface      = 0x40 == (0x40 & data3);
    m_symbolTypeface    = 0x80 == (0x80 & data3);

    m_newEATypeface     = 0x01 == (0x01 & data4);
    m_csTypeface        = 0x02 == (0x02 & data4);
    m_pp11ext           = 0x04 == (0x04 & data4);
    // reserved 5 bits
}
