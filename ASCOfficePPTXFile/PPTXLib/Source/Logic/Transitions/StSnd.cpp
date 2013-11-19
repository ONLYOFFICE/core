#include "StSnd.h"

namespace PPTX
{
	namespace Logic
	{

		StSnd::StSnd()
		{
		}

		StSnd::~StSnd()
		{
		}

		StSnd::StSnd(const XML::XNode& node)
		{
			fromXML(node);
		}

		const StSnd& StSnd::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void StSnd::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			loop = element.attribute("loop").value();
			embed = element.element("snd").attribute("embed").value().ToString();
			name = element.element("snd").attribute("name").value();
			FillParentPointersForChilds();
		}

		const XML::XNode StSnd::toXML() const
		{
			return XML::XElement(ns.p + "stSnd", 
				XML::XAttribute("loop", loop) +
				XML::XElement(ns.p + "snd",
					XML::XAttribute(ns.r + "embed", embed) +
					XML::XAttribute("name", name)
				)
			);
		}

	} // namespace Logic
} // namespace PPTX