
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Position.h"
#include "nullable_property.h"
#include "./../Limit/RunThrough.h"


namespace Odt
{
	namespace Logic
	{

		Position::Position()
		{
		}


		Position::Position(const Common::Position& position)
		{
			fromBase(position);
		}


		Position::Position(const std::string& value)
		{
			fromString(value);
		}


		Position::Position(const int type)
		{
			if(type >= 0)
				Foreground();
			else 
				Background();
		}


		Position::Position(const XML::XElement& element)
		{
			nullable_property<std::string, Limit::RunThrough>	RunThrough = element.attribute("run-through").value();
			if(RunThrough.is_init())
			{
				if(RunThrough == "background")
					setBackground();
				else 
					setForeground();
			}
			else
				setForeground();
		}


		const XML::XNode Position::toXML(const Namespaces& ns) const
		{						
			return 
				XML::XContainer(
					XML::XAttribute(ns.style + "run-through", ToString()));
		}


		const Position& Position::operator = (const Common::Position& position)
		{
			fromBase(position);
			return *this;
		}


		const Position& Position::operator = (const std::string& value)
		{
			fromString(value);
			return *this;
		}


		/*const Position& operator = (const Common::ZIndex& zIndex)
		{
			if(zIndex.Index >= 0)
				setForeground();
			else 
				setBackground();		
		}*/


		const std::string Position::ToString() const
		{
			switch (type())
			{
			case background:
				return "background";
			default:
				return "foreground";
			}
		}


		void Position::fromString(const std::string& value)
		{
			if (value == "background")
				setBackground();
			else
				setForeground();			
		}
	} // namespace Logic
} // namespace Odt