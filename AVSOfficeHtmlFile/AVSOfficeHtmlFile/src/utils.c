#include "stdafx.h"
#include <string.h>

#ifdef __cplusplus
extern "C"
#endif
void str2wstr( wchar_t * dest, const char * src )
{
    size_t i = 0;
    size_t l = strlen( src );

    for ( i = 0; i < l; ++i )
    {
        dest[ i ] = ( wchar_t ) src[ i ];
    }
    dest[ l ] = L'\0';
}

#ifdef __cplusplus
extern "C"
#endif
void wstr2str( char * dest, const wchar_t * src )
{
    size_t i = 0;
    size_t l = wcslen( src );

    for ( i = 0; i < l; ++i )
    {
        dest[ i ] = ( char ) src[ i ];
    }
    dest[ l ] = '\0';
}