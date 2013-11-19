
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Align.h"


namespace Odt
{
	namespace Logic
	{

		Align::Align()
		{
		}


		Align::Align(const Common::Align& align)
		{
			fromBase(align);
		}


		Align::Align(const std::string& value)
		{
			fromString(value);
		}


		const Align& Align::operator= (const Common::Align& align)
		{
			fromBase(align);
			return *this;
		}


		const Align& Align::operator= (const std::string& value)
		{
			fromString(value);
			return *this;
		}


		const std::string Align::ToString() const
		{
			switch (type())
			{
			case left:
				return "start";
			case center:
				return "center";
			case right:
				return "end";
			case both:
				return "justify";
			default:
				return "left";
			}
		}


		void Align::fromString(const std::string& value)
		{
			if (value == "center")
				setCenter();
			else if (value == "end")
				setRight();
			else if (value == "justify")
				setBoth();
			else
				setLeft();
		}

	} // namespace Logic
} // namespace Odt