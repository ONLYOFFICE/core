#pragma once
#include <type_traits>

#include "../../../DesktopEditor/common/Types.h"
#include "../../../DesktopEditor/common/StringUTF32.h"

inline LONG ConvertColorBGRToRGB(LONG lBGR)
{
	return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
}

// перемещает nullptr в конец и возвращает итератор, после которого начинаются nullptr
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
