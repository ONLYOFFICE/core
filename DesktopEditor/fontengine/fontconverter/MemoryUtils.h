#ifndef _ASC_FONT_CONVERTER_MEMORY_UTILS_H_
#define _ASC_FONT_CONVERTER_MEMORY_UTILS_H_

#include <stdio.h>

namespace NSFontConverter
{
    //------------------------------------------------------------------------
    // Аналогично malloc, но с обработкой ошибок.
    static void *MemUtilsMalloc(int nSize)
    {
        void *pResult;

        if ( nSize <= 0 )
            return NULL;

        if ( !( pResult = malloc( nSize ) ) )
        {
            // TO DO: Выдать ошибку выделения памяти
        }
        return pResult;
    }


    // Тоже что и realloc, но с обработкой ошибок.
    // Если <pData> NULL, вызывается функция malloc вместо realloc.
    static void *MemUtilsRealloc(void *pData, int nSize)
    {
        void *pResult;

        if ( nSize <= 0 )
        {
            if ( pData )
                free( pData );
            return NULL;
        }
        if ( pData )
            pResult = realloc( pData, nSize);
        else
            pResult = malloc( nSize );
        if ( !pResult )
        {
            // TO DO: Выдать ошибку выделения памяти
        }
        return pResult;
    }


    // Тоже самое, что и MemUtilsMalloc and MemUtilsRelloc, толькоt
    // учитывает количество элементов и размер элемента. В результате
    // выделяется память размером nObjectsCount * nObjectSize байт.
    // Кроме того присутствует обработка ошибок и проверка того, чтобы
    // суммарный размер не превышал предел для int.
    static void *MemUtilsMallocArray(int nObjectsCount, int nObjectSize)
    {
        if ( 0 == nObjectsCount )
            return NULL;

        int nSize = nObjectsCount * nObjectSize;
        if ( nObjectSize <= 0 || nObjectsCount < 0 || nObjectsCount >= INT_MAX / nObjectSize )
        {
            // TO DO: Выдать ошибку выделения памяти
        }
        return MemUtilsMalloc( nSize );
    }

    // Тоже что и free, но проверяет и игнорирует NULL-указатели.
    static void MemUtilsFree(void *pData)
    {
        if ( pData )
            free( pData );
    }

    static void *MemUtilsReallocArray(void *pData, int nObjectsCount, int nObjectSize)
    {
        if ( 0 == nObjectsCount )
        {
            if ( pData )
                MemUtilsFree( pData );
            return NULL;
        }
        int nSize = nObjectsCount * nObjectSize;

        if ( nObjectSize <= 0 || nObjectsCount < 0 || nObjectsCount >= INT_MAX / nObjectSize )
        {
            // TO DO: Выдать ошибку выделения памяти
        }
        return MemUtilsRealloc( pData, nSize );
    }

    // Выделяем память и копируем туда строку.
    static char *CopyString(char *sString)
    {
        char *sResult = (char *)MemUtilsMalloc( strlen( sString ) + 1 );
        strcpy( sResult, sString );
        return sResult;
    }
}

#endif /* _ASC_FONT_CONVERTER_MEMORY_UTILS_H_ */
