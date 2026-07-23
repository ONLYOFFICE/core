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
            // TODO: Throw memory allocation error
        }
    }

	//------------------------------------------------------------------------
	// Same as malloc, but with error handling.
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

    // Same as free, but checks and ignores NULL pointers.
    static void MemUtilsFree(void* pData)
    {
        if (pData)
            free(pData);
    }

    // Same as realloc, but with error handling.
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
