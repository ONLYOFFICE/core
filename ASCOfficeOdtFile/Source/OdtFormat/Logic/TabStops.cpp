
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TabStops.h"


namespace Odt
{
	namespace Logic
	{

		TabStops::TabStops()
		{
		}


		TabStops::~TabStops()
		{
		}


		TabStops::TabStops(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TabStops& TabStops::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TabStops::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(TabStop, element, "tab-stop");
		}


		const XML::XNode TabStops::toXML() const
		{
			return
				XML::XElement(ns.style + "tab-stops",
					XML::Write(TabStop)
				);
		}

	} // namespace Logic
} // namespace Odt