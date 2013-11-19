#include "Iterate.h"

namespace PPTX
{
	namespace Logic
	{

		Iterate::Iterate()
		{
		}

		Iterate::~Iterate()
		{
		}

		Iterate::Iterate(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Iterate& Iterate::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Iterate::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			type = element.attribute("type").value();
			backwards = element.attribute("backwards").value();
			if(element.element("tmAbs").exist())
				tmAbs = element.element("tmAbs").attribute("val").value();
			else if(element.element("tmPct").exist())
				tmPct = element.element("tmPct").attribute("val").value();

			FillParentPointersForChilds();
		}

		const XML::XNode Iterate::toXML() const
		{
			if(tmAbs.is_init())
				return XML::XElement(ns.p + "iterate",
				XML::XAttribute("type", type) + XML::XAttribute("backwards", backwards) +
				XML::XElement(ns.p + "tmAbs", XML::XAttribute("val", tmAbs)));
			return XML::XElement(ns.p + "iterate",
			XML::XAttribute("type", type) + XML::XAttribute("backwards", backwards) +
			XML::XElement(ns.p + "tmPct", XML::XAttribute("val", tmPct)));
		}
	} // namespace Logic
} // namespace PPTX