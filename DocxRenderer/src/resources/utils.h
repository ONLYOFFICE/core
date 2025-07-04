#pragma once
#include <type_traits>
#include <limits>
#include <algorithm>

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
