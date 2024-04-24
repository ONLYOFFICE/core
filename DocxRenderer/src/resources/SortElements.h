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
#include <vector>

// у класса T должен быть метод IsBigger, IsBiggerOrEqual
template<typename T>
void SortElements(std::vector<T*>& oArray)
{
    int nSize = (int)oArray.size();

    // handle 0, 1 and 2 elements
    if (nSize <= 1)
        return;
    if (nSize == 2)
    {
        if (oArray[0]->IsBigger(oArray[1]))
        {
            std::swap(oArray[0], oArray[1]);
        }
        return;
    }

    T* tTemp;

    // arrange elements as tree with greater elements appearing first
    int nIndex = (nSize >> 1) - 1, nCurr = 0, nNext = 0;
    int nLast = nSize - 1;
    int nHalf = nSize >> 1;
    do
    {
        // save element at start of chain
        tTemp = oArray[nIndex];

        nCurr = nIndex;
        while (nCurr < nHalf)
        {
            nNext = (nCurr << 1) + 1;
            if (nNext < nLast && (oArray[nNext + 1]->IsBigger(oArray[nNext])))
                nNext++;
            if (tTemp->IsBiggerOrEqual(oArray[nNext]))
                break;

            // promote element in chain
            oArray[nCurr] = oArray[nNext];
            nCurr = nNext;
        }

        // restore element at end of chain
        oArray[nCurr] = tTemp;
    }
    while (nIndex--);

    // sequentially reduce tree size by removing maximum element and rebalancing
    nIndex = nSize;
    while (--nIndex)
    {
        // save element at start of chain
        tTemp = oArray[nIndex];
        oArray[nIndex] = oArray[0];

        nCurr = 0;
        nLast = nIndex - 1;
        nHalf = nIndex >> 1;
        while (nCurr < nHalf)
        {
            nNext = (nCurr << 1) + 1;
            if (nNext < nLast && (oArray[nNext + 1]->IsBigger(oArray[nNext])))
                nNext++;
            if (tTemp->IsBiggerOrEqual(oArray[nNext]))
                break;

            // promote element in chain
            oArray[nCurr] = oArray[nNext];
            nCurr = nNext;
        }

        // restore element at end of chain
        oArray[nCurr] = tTemp;
    }
}
