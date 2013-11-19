#include "TgtEl.h"

namespace PPTX
{
	namespace Logic
	{

		TgtEl::TgtEl()
		{
		}

		TgtEl::~TgtEl()
		{
		}

		TgtEl::TgtEl(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TgtEl& TgtEl::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TgtEl::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			if(element.element("inkTgt").exist())
				inkTgt = element.element("inkTgt").attribute("spid").value();
			else if(element.element("sndTgt").exist())
			{
				embed = element.element("sndTgt").attribute("embed").value().ToString();
				name = element.element("sndTgt").attribute("name").value();
				builtIn = element.element("sndTgt").attribute("builtIn").value();
			}
			else if(element.element("spTgt").exist())
				spTgt = element.element("spTgt");

			FillParentPointersForChilds();
		}

		const XML::XNode TgtEl::toXML() const
		{
			if(inkTgt.is_init())
				return XML::XElement(ns.p + "tgtEl", XML::XElement(ns.p + "inkTgt", XML::XAttribute("spid", inkTgt)));
			if(embed.is_init())
				return XML::XElement(ns.p + "tgtEl", XML::XElement(ns.p + "sndTgt", XML::XAttribute(ns.r + "embed", embed)
				+ XML::XAttribute("name", name) + XML::XAttribute("builtIn", builtIn)));
			if(spTgt.is_init())
				return XML::XElement(ns.p + "tgtEl", XML::Write(spTgt));
			return XML::XElement(ns.p + "tgtEl", XML::XElement(ns.p + "sldTgt"));
		}

		void TgtEl::FillParentPointersForChilds()
		{
			if(spTgt.is_init())
				spTgt->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX