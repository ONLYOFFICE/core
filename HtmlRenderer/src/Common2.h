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
#ifndef _ASC_HTMLRENDERER_COMMON2_H_
#define _ASC_HTMLRENDERER_COMMON2_H_

#include "Common.h"
#include "Meta.h"
#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/xml/include/xmlutils.h"

namespace NSHtmlRenderer
{
    static RECT GetImageBounds(CBgraFrame* pFrame)
    {
        BYTE* pBuffer = pFrame->get_Data();
        LONG lWidth = (LONG)pFrame->get_Width();
        LONG lHeight = (LONG)pFrame->get_Height();

        RECT rect;
        rect.left	= 0;
        rect.top	= 0;
        rect.right	= lWidth - 1;
        rect.bottom = lHeight - 1;

        // top
        unsigned int* pData = (unsigned int*)pBuffer;
        for (; rect.top < lHeight; rect.top++)
        {
            bool bIsBreak = false;
            for (LONG i = 0; i < lWidth; ++i, ++pData)
            {
                if (*pData != 0x00)
                {
                    bIsBreak = true;
                    break;
                }
            }
            if (bIsBreak)
                break;
        }
        if (rect.top >= lHeight)
            rect.top = (lHeight - 1);

        // bottom
        for (; rect.bottom >= rect.top; rect.bottom--)
        {
            pData = (unsigned int*)pBuffer;
            pData += (lWidth * rect.bottom);

            bool bIsBreak = false;
            for (LONG i = 0; i < lWidth; ++i, ++pData)
            {
                if (*pData != 0x00)
                {
                    bIsBreak = true;
                    break;
                }
            }
            if (bIsBreak)
                break;
        }
        if (rect.bottom < rect.top)
            rect.bottom = rect.top;

        LONG lDelta = rect.bottom - rect.top + 1;

        // left
        for (; rect.left < lWidth; rect.left++)
        {
            pData = (unsigned int*)(pBuffer + 4 * lWidth * rect.top);
            pData += rect.left;

            bool bIsBreak = false;
            for (LONG i = 0; i < lDelta; ++i, pData += lWidth)
            {
                if (*pData != 0x00)
                {
                    bIsBreak = true;
                    break;
                }
            }
            if (bIsBreak)
                break;
        }
        if (rect.left >= lWidth)
            rect.left = lWidth - 1;

        // right
        for (; rect.right >= rect.left; rect.right--)
        {
            pData = (unsigned int*)(pBuffer + 4 * lWidth * rect.top);
            pData += rect.right;

            bool bIsBreak = false;
            for (LONG i = 0; i < lDelta; ++i, pData += lWidth)
            {
                if (*pData != 0x00)
                {
                    bIsBreak = true;
                    break;
                }
            }
            if (bIsBreak)
                break;
        }
        if (rect.right < rect.left)
            rect.right = rect.left;

        return rect;
    }

    static RECT GetImageBounds2(CBgraFrame* pFrame, BYTE* pCache)
    {
        BYTE* pBuffer = pFrame->get_Data();
        LONG lWidth = (LONG)pFrame->get_Width();
        LONG lHeight = (LONG)pFrame->get_Height();

        RECT rect;
        rect.left	= 0;
        rect.top	= 0;
        rect.right	= lWidth - 1;
        rect.bottom = lHeight - 1;

        // top
        unsigned int** pData = (unsigned int**)pBuffer;
        unsigned int** pDataSrc = (unsigned int**)pCache;
        for (; rect.top < lHeight; rect.top++)
        {
            if (0 != memcmp((void*)pData, (void*)pDataSrc, 4 * lWidth))
                break;

            pData += lWidth;
            pDataSrc += lWidth;
        }
        if (rect.top >= lHeight)
            rect.top = (lHeight - 1);

        // bottom
        for (; rect.bottom >= rect.top; rect.bottom--)
        {
            pData = (unsigned int**)pBuffer;
            pData += (lWidth * rect.bottom);
            pDataSrc = (unsigned int**)pCache;
            pDataSrc += (lWidth * rect.bottom);

            if (0 != memcmp((void*)pData, (void*)pDataSrc, 4 * lWidth))
                break;
        }
        if (rect.bottom < rect.top)
            rect.bottom = rect.top;

        LONG lDelta = rect.bottom - rect.top + 1;

        // left
        for (; rect.left < lWidth; rect.left++)
        {
            pData = (unsigned int**)(pBuffer + 4 * lWidth * rect.top);
            pData += rect.left;
            pDataSrc = (unsigned int**)(pCache + 4 * lWidth * rect.top);
            pDataSrc += rect.left;

            bool bIsBreak = false;
            for (LONG i = 0; i < lDelta; ++i, pData += lWidth, pDataSrc += lWidth)
            {
                if (*pData != *pDataSrc)
                {
                    bIsBreak = true;
                    break;
                }
            }
            if (bIsBreak)
                break;
        }
        if (rect.left >= lWidth)
            rect.left = lWidth - 1;

        // right
        for (; rect.right >= rect.left; rect.right--)
        {
            pData = (unsigned int**)(pBuffer + 4 * lWidth * rect.top);
            pData += rect.right;

            pDataSrc = (unsigned int**)(pCache + 4 * lWidth * rect.top);
            pDataSrc += rect.right;

            bool bIsBreak = false;
            for (LONG i = 0; i < lDelta; ++i, pData += lWidth)
            {
                if (*pData != *pDataSrc)
                {
                    bIsBreak = true;
                    break;
                }
            }
            if (bIsBreak)
                break;
        }
        if (rect.right < rect.left)
            rect.right = rect.left;

        return rect;
    }
}

#endif // _ASC_HTMLRENDERER_COMMON2_H_
