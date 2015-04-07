#pragma once
#ifndef COMMON_COLOR_INCLUDE_H_
#define COMMON_COLOR_INCLUDE_H_

namespace Common
{
	class Color
	{
	public:
		Color();
		Color(const int red, const int green, const int blue);

	public:
		unsigned char Red;
		unsigned char Green;
		unsigned char Blue;

	protected:
		void fromBase(const Color& color);
	};
} // namespace Common

#endif // COMMON_COLOR_INCLUDE_H_