#include "Hyperlink.h"

namespace PPTX
{
	namespace Logic
	{

		Hyperlink::Hyperlink()
		{
		}


		Hyperlink::~Hyperlink()
		{
		}


		Hyperlink::Hyperlink(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Hyperlink& Hyperlink::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Hyperlink::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			m_name = element.XName.get().Name;

			snd = element.element("snd");
			//if(element.attribute("id").exist())
				id = element.attribute("id").value();//.ToString();
			invalidUrl = element.attribute("invalidUrl").value();
			action = element.attribute("action").value();
			tgtFrame = element.attribute("tgtFrame").value();
			tooltip = element.attribute("tooltip").value();
			history = element.attribute("history").value();
			highlightClick = element.attribute("highlightClick").value();
			endSnd = element.attribute("endSnd").value();
		}


		const XML::XNode Hyperlink::toXML() const
		{
			return XML::XElement(ns.a + m_name.get(),
				XML::Write(snd) +
				XML::XAttribute(ns.r + "id", id) +
				XML::XAttribute("invalidUrl", invalidUrl) +
				XML::XAttribute("action", action) +
				XML::XAttribute("tgtFrame", tgtFrame) +
				XML::XAttribute("tooltip", tooltip) +
				XML::XAttribute("history", history) +
				XML::XAttribute("highlightClick", highlightClick) +
				XML::XAttribute("endSnd", endSnd)
			);
		}

		void Hyperlink::FillParentPointersForChilds()
		{
			if(snd.is_init())
				snd->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX