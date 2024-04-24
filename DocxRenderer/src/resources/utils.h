/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#include "../DesktopEditor/common/Types.h"
#include "../DesktopEditor/common/StringUTF32.h"

inline LONG ConvertColorBGRToRGB(LONG lBGR)
{
    return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
}

inline bool IsSpaceUtf32(const uint32_t& c)
{
    return (0x20 == c ||        //пробел
            0xA0 == c ||        //неразрывный пробел
            0x2003 == c         //Em пробел
            ) ? true : false;
}
inline bool IsSpaceUtf32(const NSStringUtils::CStringUTF32& oText)
{
    if (1 != oText.length())
        return false;
    return IsSpaceUtf32(oText.ToStdWString()[0]);
}

inline bool IsUnicodeSymbol(const int& symbol )
{
    if ( ( 0x0009 == symbol ) || ( 0x000A == symbol ) || ( 0x000D == symbol ) ||
       ( ( 0x0020 <= symbol ) && ( 0xD7FF >= symbol ) ) || ( ( 0xE000 <= symbol ) && ( symbol <= 0xFFFD ) ) ||
       ( ( 0x10000 <= symbol ) && symbol ) )
    {
        return true;
    }
    return false;
}

// 2-byte number
inline short little_endian_2_big_endian( short s )
{
    return ( ( s >> 8) & 0xff ) + ( ( s << 8 ) & 0xff00 );
}

/*========================================================================================================*/

// 4-byte number
inline int little_endian_2_big_endian( int i )
{
    return ( ( i & 0xff ) << 24 ) + ( ( i & 0xff00 ) << 8 ) + ( ( i & 0xff0000 ) >> 8 ) + ( ( i >> 24 ) & 0xff );
}
