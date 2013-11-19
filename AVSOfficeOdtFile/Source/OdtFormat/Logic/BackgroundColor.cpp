
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "BackgroundColor.h"
#include <boost/format.hpp>
#include "Parse.h"


namespace Odt
{
	namespace Logic
	{

		BackgroundColor::BackgroundColor()
		{
			m_transparent = false;
		}


		BackgroundColor::BackgroundColor(const Common::Color& color)
		{
			fromBase(color);
		}


		BackgroundColor::BackgroundColor(const std::string& value)
		{
			fromString(value);
		}


		const BackgroundColor& BackgroundColor::operator= (const Common::Color& color)
		{
			fromBase(color);
			return *this;
		}


		const BackgroundColor& BackgroundColor::operator= (const std::string& value)
		{
			fromString(value);
			return *this;
		}


		const std::string BackgroundColor::ToString() const
		{
			if (Red == 0xFF && Green == 0xFF && Blue == 0xFF)
				return "transparent";
			return (boost::format("#%02x%02x%02x") % Red % Green % Blue).str();	
		}


		void BackgroundColor::fromString(const std::string& value)
		{
			if (value == "transparent")
			{
				Red		= 0xFF;
				Green = 0xFF;
				Blue	= 0xFF;
				m_transparent = true;
			}
			else
			{
                if (value.size() == 6)
                {
                    Red		=	HexString2Int(value.substr(0, 2));
                    Green =	HexString2Int(value.substr(2, 2));
                    Blue	=	HexString2Int(value.substr(4, 2));
                    m_transparent = false;                
                }
                else if (value.size() == 7 &&
                    '#' == value[0])
                {
				    Red		=	HexString2Int(value.substr(1, 2));
				    Green =	HexString2Int(value.substr(3, 2));
				    Blue	=	HexString2Int(value.substr(5, 2));
				    m_transparent = false;
                }
                else
                {
                    Red = Green = Blue = 0;
                }
			}
		}


		const bool BackgroundColor::isTransparent() const
		{
			return m_transparent;
		}

	} // namespace Logic
} // namespace Odt