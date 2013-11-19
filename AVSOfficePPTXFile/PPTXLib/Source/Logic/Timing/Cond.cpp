#include "Cond.h"

namespace PPTX
{
	namespace Logic
	{

		Cond::Cond()
		{
		}

		Cond::~Cond()
		{
		}

		Cond::Cond(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Cond& Cond::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Cond::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			name = element.XName->Name;

			delay = element.attribute("delay").value();
			evt = element.attribute("evt").value();

			if(element.element("tn").exist())
				tn = element.element("tn").attribute("val").value();
			else if(element.element("tgtEl").exist())
				tgtEl = element.element("tgtEl");
			else if(element.element("rtn").exist())
				rtn = element.element("rtn").attribute("val").value();
			FillParentPointersForChilds();
		}

		const XML::XNode Cond::toXML() const
		{
			return XML::XElement(ns.p + name.get(),
				XML::XAttribute("evt", evt) +
				XML::XAttribute("delay", delay) +
				XML::WriteIf(XML::XElement(ns.p + "tn", XML::XAttribute("val", tn)), tn.is_init()) +
				XML::Write(tgtEl) +
				XML::WriteIf(XML::XElement(ns.p + "rtn", XML::XAttribute("val", rtn)), rtn.is_init())
				);
		}

		void Cond::FillParentPointersForChilds()
		{
			if(tgtEl.is_init())
				tgtEl->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX