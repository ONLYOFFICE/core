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
