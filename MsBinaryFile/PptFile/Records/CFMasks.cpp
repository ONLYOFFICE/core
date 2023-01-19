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
