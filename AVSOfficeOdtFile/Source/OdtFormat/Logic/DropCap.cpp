
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "DropCap.h"


namespace Odt
{
	namespace Logic
	{

		DropCap::DropCap()
		{
		}


		DropCap::~DropCap()
		{
		}


		DropCap::DropCap(const XML::XNode& node)
		{
			fromXML(node);
		}


		const DropCap& DropCap::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void DropCap::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Lines		= element.attribute("lines").value();
			Lenght		= element.attribute("lenght").value();
			if (element.attribute("distance").exist())
				Distance = element.attribute("distance").value().ToString();
		}


		const XML::XNode DropCap::toXML() const
		{
			return
				XML::XElement(ns.style + "drop-cap",
					XML::XAttribute(ns.style + "lines", Lines) +
					XML::XAttribute(ns.style + "lenght", Lenght) +
					XML::XAttribute(ns.style + "distance", Distance)
				);
		}

	} // namespace Logic
} // namespace Odt