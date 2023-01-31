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
#pragma once
#include "../Reader/Records.h"

namespace PPT
{
struct SCFMasks
{
    bool m_bold;
    bool m_italic;
    bool m_underline;
    // unused
    bool m_shadow;
    bool m_fehint;
    // unused2
    bool m_kumi;
    // unused3
    bool    m_emboss;
    _UINT16 m_fHasStyle;    // 4 bits
    // unused 2 bits

    bool m_typeface;
    bool m_size;
    bool m_color;
    bool m_position;
    bool m_pp10ext;
    bool m_oldEATypeface;
    bool m_ansiTypeface;
    bool m_symbolTypeface;
    bool m_newEATypeface;
    bool m_csTypeface;
    bool m_pp11ext;
    // reserved 5 bits


    void ReadFromStream(POLE::Stream* pStream);
};
}
