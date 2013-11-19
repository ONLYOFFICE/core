
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "NumFormat.h"


namespace Odt
{
	namespace Logic
	{

		NumFormat::NumFormat()
		{
		}


		NumFormat::NumFormat(const Common::NumFormat& numFormat)
		{
			fromBase(numFormat);
		}


		NumFormat::NumFormat(const std::string& value)
		{
			fromString(value);
		}


		const NumFormat& NumFormat::operator= (const Common::NumFormat& numFormat)
		{
			fromBase(numFormat);
			return *this;
		}


		const NumFormat& NumFormat::operator= (const std::string& value)
		{
			fromString(value);
			return *this;
		}


		const std::string NumFormat::ToString() const
		{
			switch (type())
			{
			case upperLetter:
				return "A";
			case lowerLetter:
				return "a";
			case upperRoman:
				return "I";
			case lowerRoman:
				return "i";
			case decimal:
				return "1";
			default:
				return "1";
			}
		}


		void NumFormat::fromString(const std::string& value)
		{
			if (value == "A")
				setUpperLetter();
			else if (value == "a")
				setLowerLetter();
			else if (value == "I")
				setUpperRoman();
			else if (value == "i")
				setLowerRoman();			
			else
				setDecimal();
		}

	} // namespace Logic
} // namespace Odt