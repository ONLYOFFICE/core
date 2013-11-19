#include "SpTgt.h"

namespace PPTX
{
	namespace Logic
	{

		SpTgt::SpTgt()
		{
		}

		SpTgt::~SpTgt()
		{
		}

		SpTgt::SpTgt(const XML::XNode& node)
		{
			fromXML(node);
		}

		const SpTgt& SpTgt::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void SpTgt::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			spid = element.attribute("spid").value();

			bg = element.element("bg").exist();
			if(element.element("subSp").exist())
				subSpid = element.element("subSp").attribute("spid").value();
			else if(element.element("oleChartEl").exist())
			{
				type = element.element("oleChartEl").attribute("type").value();
				lvl = element.element("oleChartEl").attribute("lvl").value();
			}
			else if(element.element("txEl").exist())
				txEl = element.element("txEl");
			else if(element.element("graphicEl").exist())
				graphicEl = element.element("graphicEl");

			FillParentPointersForChilds();
		}

		const XML::XNode SpTgt::toXML() const
		{
			if(bg.get_value_or(false))
				return XML::XElement(ns.p + "spTgt", XML::XAttribute("spid", spid) + XML::XElement(ns.p + "bg"));
			if(subSpid.is_init())
				return XML::XElement(ns.p + "spTgt", XML::XAttribute("spid", spid) + XML::XElement(ns.p + "subSp", XML::XAttribute("spid", subSpid)));
			if(type.is_init())
				return XML::XElement(ns.p + "spTgt", XML::XAttribute("spid", spid) + XML::XElement(ns.p + "oleChartEl",
				XML::XAttribute("type", type) + XML::XAttribute("lvl", lvl)));
			if(txEl.is_init())
				return XML::XElement(ns.p + "spTgt", XML::XAttribute("spid", spid) + XML::Write(txEl));
			if(graphicEl.is_init())
				return XML::XElement(ns.p + "spTgt", XML::XAttribute("spid", spid) + XML::Write(graphicEl));
			return XML::XElement(ns.p + "spTgt", XML::XAttribute("spid", spid));
		}

		void SpTgt::FillParentPointersForChilds()
		{
			if(txEl.is_init())
				txEl->SetParentPointer(*this);
			if(graphicEl.is_init())
				graphicEl->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX