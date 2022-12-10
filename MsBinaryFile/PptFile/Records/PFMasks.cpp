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
#include "PFMasks.h"

using namespace PPT;

static inline bool getBit(_UINT32 data, _UINT32 mask)
{
    return mask == (mask & data);
}


void PFMasks::ReadFromStream(POLE::Stream *pStream)
{
    _UINT32 data = StreamUtils::ReadDWORD(pStream);

    m_hasBullet         = getBit(data, 0x0001);
    m_bulletHasFont     = getBit(data, 0x0002);
    m_bulletHasColor    = getBit(data, 0x0004);
    m_bulletHasSize     = getBit(data, 0x0008);
    m_bulletFont        = getBit(data, 0x0010);
    m_bulletColor       = getBit(data, 0x0020);
    m_bulletSize        = getBit(data, 0x0040);
    m_bulletChar        = getBit(data, 0x0080);
    m_leftMargin        = getBit(data, 0x0100);
    // unused
    m_indent            = getBit(data, 0x000400);
    m_align             = getBit(data, 0x000800);
    m_lineSpacing       = getBit(data, 0x001000);
    m_spaceBefore       = getBit(data, 0x002000);
    m_spaceAfter        = getBit(data, 0x004000);
    m_defaultTabSize    = getBit(data, 0x008000);
    m_fontAlign         = getBit(data, 0x010000);
    m_charWrap          = getBit(data, 0x020000);
    m_wordWrap          = getBit(data, 0x040000);
    m_overflow          = getBit(data, 0x080000);
    m_tabStops          = getBit(data, 0x100000);
    m_textDirection     = getBit(data, 0x200000);
    // reserved1
    m_bulletBlip        = getBit(data, 0x0800000);
    m_bulletScheme      = getBit(data, 0x1000000);
    m_bulletHasScheme   = getBit(data, 0x2000000);
}
