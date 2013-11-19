
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TabStop.h"


namespace Odt
{
	namespace Logic
	{

		TabStop::TabStop()
		{
		}


		TabStop::~TabStop()
		{
		}


		TabStop::TabStop(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TabStop& TabStop::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TabStop::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			if (element.attribute("position").exist())
				Position = element.attribute("position").value().ToString();			
			Type				= element.attribute("type").value();
			LeaderStyle = element.attribute("leader-style").value();
			LeaderText  = element.attribute("leader-text").value();
		}


		const XML::XNode TabStop::toXML() const
		{
			return
				XML::XElement(ns.style + "tab-stop",
					XML::XAttribute(ns.style + "position", Position) +
					XML::XAttribute(ns.style + "type", Type) + 
					XML::XAttribute(ns.style + "leader-style", LeaderStyle) + 
					XML::XAttribute(ns.style + "leader-text", LeaderText)
				);
		}

	} // namespace Logic
} // namespace Odt