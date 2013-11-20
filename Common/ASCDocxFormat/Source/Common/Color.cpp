
// auto inserted precompiled begin
#include "precompiled_common.h"
// auto inserted precompiled end

#include "Color.h"


namespace Common
{

	Color::Color()
		: Red(0x00),
			Green(0x00),
			Blue(0x00)
	{
	}


	Color::Color(const int red, const int green, const int blue)
		: Red(red),
			Green(green),
			Blue(blue)
	{
	}


	void Color::fromBase(const Color& color)
	{
		Red		= color.Red;
		Green	= color.Green;
		Blue	= color.Blue;
	}

} // namespace Common