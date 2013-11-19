#include "AnimMotion.h"

namespace PPTX
{
	namespace Logic
	{

		AnimMotion::AnimMotion()
		{
		}

		AnimMotion::~AnimMotion()
		{
		}

		AnimMotion::AnimMotion(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AnimMotion& AnimMotion::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AnimMotion::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cBhvr = element.element("cBhvr");
			if(element.element("by").exist())
			{
				byX = element.element("by").attribute("x").value();
				byY = element.element("by").attribute("y").value();
			}
			if(element.element("from").exist())
			{
				fromX = element.element("from").attribute("x").value();
				fromY = element.element("from").attribute("y").value();
			}
			if(element.element("to").exist())
			{
				toX = element.element("to").attribute("x").value();
				toY = element.element("to").attribute("y").value();
			}
			if(element.element("rCtr").exist())
			{
				rCtrX = element.element("rCtr").attribute("x").value();
				rCtrY = element.element("rCtr").attribute("y").value();
			}

			path = element.attribute("path").value();
			ptsTypes = element.attribute("ptsTypes").value();
			rAng = element.attribute("rAng").value();
			origin = element.attribute("origin").value();
			pathEditMode = element.attribute("pathEditMode").value();

			FillParentPointersForChilds();
		}

		const XML::XNode AnimMotion::toXML() const
		{
			return XML::XElement(ns.p + "animMotion",
				XML::XAttribute("origin", origin) +
				XML::XAttribute("path", path) +
				XML::XAttribute("pathEditMode", pathEditMode) +
				XML::XAttribute("rAng", rAng) +
				XML::XAttribute("ptsTypes", ptsTypes) +
				XML::Write(cBhvr) +
				XML::WriteIf(XML::XElement(ns.p + "by", XML::XAttribute("x", byX) + XML::XAttribute("y", byY)), byX.is_init() || byY.is_init()) +
				XML::WriteIf(XML::XElement(ns.p + "from", XML::XAttribute("x", fromX) + XML::XAttribute("y", fromY)), fromX.is_init() || fromY.is_init()) +
				XML::WriteIf(XML::XElement(ns.p + "to", XML::XAttribute("x", toX) + XML::XAttribute("y", toY)), toX.is_init() || toY.is_init()) +
				XML::WriteIf(XML::XElement(ns.p + "rCtr", XML::XAttribute("x", rCtrX) + XML::XAttribute("y", rCtrY)), rCtrX.is_init() || rCtrY.is_init())
				);
		}

		void AnimMotion::FillParentPointersForChilds()
		{
			cBhvr->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX