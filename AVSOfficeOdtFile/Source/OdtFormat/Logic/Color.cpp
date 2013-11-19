
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Color.h"
#include <boost/format.hpp>
#include "Parse.h"


namespace Odt
{
	namespace Logic
	{

		Color::Color()
		{
		}


		Color::Color(const Common::Color& color)
		{
			fromBase(color);
		}


		Color::Color(const std::string& value)
		{
			fromString(value);
		}


		const Color& Color::operator= (const Common::Color& color)
		{
			fromBase(color);
			return *this;
		}


		const Color& Color::operator= (const std::string& value)
		{
			fromString(value);
			return *this;
		}


		const std::string Color::ToString() const
		{
			return (boost::format("#%02x%02x%02x") % Red % Green % Blue).str();	
		}


		void Color::fromString(const std::string& value)
		{
			Red		=	HexString2Int(value.substr(1, 2));
			Green =	HexString2Int(value.substr(3, 2));
			Blue	=	HexString2Int(value.substr(5, 2));
		}

	} // namespace Logic
} // namespace Odt