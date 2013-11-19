
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Handle.h"


namespace Odt
{
	namespace Content
	{

		Handle::Handle()
		{
		}


		Handle::~Handle()
		{
		}


		Handle::Handle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Handle& Handle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Handle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Position	= element.attribute("handle-position").value();
			XMin	= element.attribute("handle-range-x-minimum").value();
			YMin	= element.attribute("handle-range-y-minimum").value();
			XMax	= element.attribute("handle-range-x-maximum").value();
			YMax	= element.attribute("handle-range-y-maximum").value();

			Polar	= element.attribute("handle-polar").value();
			RMin	= element.attribute("handle-radius-range-minimum").value();
			RMax	= element.attribute("handle-radius-range-maximum").value();
		}


		const XML::XNode Handle::toXML() const
		{
			return 
				XML::XElement(ns.draw + "handle",
					XML::XAttribute(ns.draw + "handle-position", Position) +
					XML::XAttribute(ns.draw + "handle-range-x-minimum", XMin) +
					XML::XAttribute(ns.draw + "handle-range-x-maximum", XMax) +
					XML::XAttribute(ns.draw + "handle-range-y-minimum", YMin) +
					XML::XAttribute(ns.draw + "handle-range-y-maximum", YMax) +
					XML::XAttribute(ns.draw + "handle-polar", Polar) +
					XML::XAttribute(ns.draw + "handle-radius-range-minimum", RMin) +
					XML::XAttribute(ns.draw + "handle-radius-range-maximum", RMax)
				);
		}

	} // namespace Content
} // namespace Odt