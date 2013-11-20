#pragma once
#ifndef COMMON_COLOR_INCLUDE_H_
#define COMMON_COLOR_INCLUDE_H_

#include "property.h"


namespace Common
{
	class Color
	{
	public:
		Color();
		Color(const int red, const int green, const int blue);

	public:
		property<int, setter::between<int, 0, 255> > Red;
		property<int, setter::between<int, 0, 255> > Green;
		property<int, setter::between<int, 0, 255> > Blue;

	protected:
		void fromBase(const Color& color);
	};
} // namespace Common

#endif // COMMON_COLOR_INCLUDE_H_