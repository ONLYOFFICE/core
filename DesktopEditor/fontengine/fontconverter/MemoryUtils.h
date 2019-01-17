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
#ifndef _ASC_FONT_CONVERTER_MEMORY_UTILS_H_
#define _ASC_FONT_CONVERTER_MEMORY_UTILS_H_

#include <stdio.h>
#include <limits>
#include <string>
#include <stdlib.h>

#ifdef __linux__
    #include <string.h>
#endif

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
        if ( nObjectSize <= 0 || nObjectsCount < 0 || nObjectsCount >= 0x7fffffff / nObjectSize )
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

        if ( nObjectSize <= 0 || nObjectsCount < 0 || nObjectsCount >= 0x7fffffff / nObjectSize )
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
