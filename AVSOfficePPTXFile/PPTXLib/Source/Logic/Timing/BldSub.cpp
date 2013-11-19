#include "BldSub.h"

namespace PPTX
{
	namespace Logic
	{

		BldSub::BldSub()
		{
		}

		BldSub::~BldSub()
		{
		}

		BldSub::BldSub(const XML::XNode& node)
		{
			fromXML(node);
		}

		const BldSub& BldSub::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void BldSub::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if(element.element("bldChart").exist())
			{
				chart = true;
				bldChart = element.element("bldChart").attribute("bld").value();
				animBg = element.element("bldChart").attribute("animBg").value();
			}
			else if(element.element("bldDgm").exist())
			{
				chart = false;
				bldDgm = element.element("bldDgm").attribute("bld").value();
				rev = element.element("bldChart").attribute("rev").value();
			}
			else
				chart.reset();

			FillParentPointersForChilds();
		}

		const XML::XNode BldSub::toXML() const
		{
			if(chart.is_init())
			{
				if(chart.get())
					return XML::XElement(ns.p + "bldSub", XML::XElement(ns.a + "bldChart", XML::XAttribute("bld", bldChart) + XML::XAttribute("animBg", animBg)));
				return XML::XElement(ns.p + "bldSub", XML::XElement(ns.a + "bldDgm", XML::XAttribute("bld", bldDgm) + XML::XAttribute("rev", rev)));
			}
			return XML::XElement(ns.p + "bldSub");
		}
	} // namespace Logic
} // namespace PPTX