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

#pragma once
#include <type_traits>
#include <limits>
#include <algorithm>
#include <functional>

#include "../../../DesktopEditor/common/Types.h"

inline LONG ConvertColorBGRToRGB(LONG lBGR)
{
	return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
}

template<typename It>
It MoveNullptr(It start, It end)
{
	if (end <= start) return start;
	It left = start, right = end - 1;
	for (;;)
	{
		while (!*right && left < right) right--;
		while (*left && left < right) left++;
		if (left >= right) break;
		std::swap(*left, *right);
	}
	if (*right)
		++right;

	return right;
}

template <class T, class Cmp = std::less<T>>
bool CmpOrEqual(const T& val1,
                const T& val2,
                const T& eps = std::numeric_limits<T>::epsilon(),
                const Cmp& cmp = Cmp())
{
	return std::abs(val1 - val2) < eps || cmp(val1, val2);
}

template <typename T>
std::function<bool(const T&, const T&)> makeEqualComp(T eps)
{
	return [eps](const T& a, const T& b) -> bool {
		return std::abs(a - b) < eps;
	};
}
