
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Wrap.h"
#include "nullable_property.h"


namespace Odt
{
	namespace Logic
	{

		Wrap::Wrap()
		{
		}


		Wrap::Wrap(const Common::Wrap& wrap)
		{
			fromBase(wrap);
		}


		Wrap::Wrap(const std::string& value)
		{
			fromString(value);
		}


		Wrap::Wrap(const XML::XElement& element)
		{
			// TODO «ачем тут используетс€ ограничитель, какой в нем смысл?
/*
			nullable_property<std::string, Limit::Wrap>	Wrap = element.attribute("wrap").value();
			if(Wrap.is_init())
			{
				if(Wrap == "none")
					setTopAndBottom();
				else if(Wrap == "parallel" ||Wrap == "dynamic")
					setSquare();
				else if(Wrap == "run-through")
					setNone();
				else
					setTopAndBottom();
			}
			else
				setTopAndBottom();
*/
		}


		const XML::XNode Wrap::toXML(const Namespaces& ns) const
		{
			std::string wrap;

			if (isSquare() || isTight())
				wrap = "parallel";
			else if (isTopAndBottom())
				wrap = "none";
			else 
				wrap = "run-through";			
			return 
				XML::XContainer(
					XML::XAttribute(ns.style + "wrap", wrap)); // TODO это нерабоча€ конструкци€
		}


		const Wrap& Wrap::operator = (const Common::Wrap& wrap)
		{
			fromBase(wrap);
			return *this;
		}


		const Wrap& Wrap::operator = (const std::string& value)
		{
			fromString(value);
			return *this;
		}


		const std::string Wrap::ToString() const
		{
			switch (type())
			{
			case square:
				return "square";
			case tight:
				return "tight";
			case topAndBottom:
				return "topAndBottom";
			case none:
				return "none";
			default:
				return "square";
			}
		}


		void Wrap::fromString(const std::string& value)
		{
			if (value == "square")
				setSquare();
			else if (value == "tight")
				setTight();
			else if (value == "topAndBottom")
				setTopAndBottom();
			else if (value == "none")
				setNone();
			else 
				setSquare();
		}
	} // namespace Logic
} // namespace Odt