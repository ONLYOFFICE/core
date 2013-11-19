#include "AnimClr.h"

namespace PPTX
{
	namespace Logic
	{

		AnimClr::AnimClr()
		{
		}

		AnimClr::~AnimClr()
		{
		}

		AnimClr::AnimClr(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AnimClr& AnimClr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AnimClr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			clrSpc = element.attribute("clrSpc").value();
			dir = element.attribute("dir").value();

			cBhvr = element.element("cBhvr");
			if(element.element("from").exist())
				from.GetColorFrom(element.element("from"));
			if(element.element("to").exist())
				to.GetColorFrom(element.element("to"));
			if(element.element("by").exist())
			{
				if(element.element("by").element("rgb").exist())
				{
					byR = element.element("by").element("rgb").attribute("r").value();
					byG = element.element("by").element("rgb").attribute("g").value();
					byB = element.element("by").element("rgb").attribute("b").value();
				}
				else if(element.element("by").element("hsl").exist())
				{
					byH = element.element("by").element("hsl").attribute("h").value();
					byS = element.element("by").element("hsl").attribute("s").value();
					byL = element.element("by").element("hsl").attribute("l").value();
				}
			}

			FillParentPointersForChilds();
		}

		const XML::XNode AnimClr::toXML() const
		{
			return XML::XElement(ns.p + "animClr",
				XML::XAttribute("clrSpc", clrSpc) +
				XML::XAttribute("dir", dir) +
				XML::Write(cBhvr) +

				XML::WriteIf(XML::XElement(ns.p + "by",
					XML::WriteIf(XML::XElement(ns.p + "rgb", XML::XAttribute("r", byR) + XML::XAttribute("g", byG) + XML::XAttribute("b", byB)),
					byR.is_init() && byG.is_init() && byB.is_init()) +
					XML::WriteIf(XML::XElement(ns.p + "hsl", XML::XAttribute("h", byH) + XML::XAttribute("s", byS) + XML::XAttribute("l", byL)),
					byH.is_init() && byS.is_init() && byL.is_init())
				), byR.is_init() || byG.is_init() || byB.is_init() || byH.is_init() || byS.is_init() || byL.is_init()) +

				XML::WriteIf(XML::XElement(ns.p + "from", XML::Write(from)), from.is_init()) +
				XML::WriteIf(XML::XElement(ns.p + "to", XML::Write(to)), to.is_init())
				);
		}

		void AnimClr::FillParentPointersForChilds()
		{
			cBhvr->SetParentPointer(*this);
			from.SetParentPointer(*this);
			to.SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX