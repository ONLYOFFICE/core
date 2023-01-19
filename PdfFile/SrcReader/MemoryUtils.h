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
#ifndef _PDF_READER_MEMORY_UTILS_H
#define _PDF_READER_MEMORY_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef INT_MAX
#define INT_MIN     (-2147483647 - 1) /* minimum (signed) int value */
#define INT_MAX       2147483647    /* maximum (signed) int value */
#endif


namespace PdfReader
{
    static inline void CheckMemory(void* memory)
    {
        if (!memory)
        {
            // TODO: Выдать ошибку выделения памяти
        }
    }

	//------------------------------------------------------------------------
	// Аналогично malloc, но с обработкой ошибок.
    static void* MemUtilsMalloc(unsigned int nSize)
	{
        void* pResult;

        if (nSize == 0)
			return NULL;

        pResult = malloc(nSize);
        CheckMemory(pResult);
        return pResult;
    }
    static void* MemUtilsMallocArray(unsigned int nObjectsCount, unsigned int nObjectSize)
    {
        if (0 == nObjectsCount || 0 == nObjectSize)
            return NULL;

        if (nObjectsCount >= (UINT_MAX / nObjectSize))
        {
            CheckMemory(NULL);
            return NULL;
        }

        return MemUtilsMalloc(nObjectsCount * nObjectSize);
    }

    // Тоже что и free, но проверяет и игнорирует NULL-указатели.
    static void MemUtilsFree(void* pData)
    {
        if (pData)
            free(pData);
    }

    // Тоже что и realloc, но с обработкой ошибок.
    static void* MemUtilsRealloc(void* pData, unsigned int nSize, unsigned int nOldSize)
	{
        void* pResult;

        if (!pData || nSize == 0)
            return NULL;

        pResult = realloc(pData, nSize);
        if (!pResult)
        {
            pResult = malloc(nSize);
            if (pResult)
            {
                if (0 != nOldSize)
                    memcpy(pResult, pData, nOldSize);
                free(pData);
            }
        }

        CheckMemory(pResult);
		return pResult;
	}
    static void* MemUtilsReallocArray(void* pData, unsigned int nObjectsCount, unsigned int nObjectSize, unsigned int nOldCount)
    {
        if (0 == nObjectsCount || 0 == nObjectSize)
            return NULL;

        if (nObjectsCount >= (UINT_MAX / nObjectSize))
        {
            CheckMemory(NULL);
            return NULL;
        }

        return MemUtilsRealloc(pData, nObjectsCount * nObjectSize, nOldCount * nObjectSize);
    }
}

#endif // _PDF_READER_MEMORY_UTILS_H
