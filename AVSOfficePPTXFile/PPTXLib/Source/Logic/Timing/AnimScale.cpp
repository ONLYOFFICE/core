#include "AnimScale.h"

namespace PPTX
{
	namespace Logic
	{

		AnimScale::AnimScale()
		{
		}

		AnimScale::~AnimScale()
		{
		}

		AnimScale::AnimScale(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AnimScale& AnimScale::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AnimScale::fromXML(const XML::XNode& node)
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

			zoomContents = element.attribute("zoomContents").value();

			FillParentPointersForChilds();
		}

		const XML::XNode AnimScale::toXML() const
		{
			return XML::XElement(ns.p + "animScale",
				XML::XAttribute("zoomContents", zoomContents) +
				XML::Write(cBhvr) +
				XML::WriteIf(XML::XElement(ns.p + "by", XML::XAttribute("x", byX) + XML::XAttribute("y", byY)), byX.is_init() || byY.is_init()) +
				XML::WriteIf(XML::XElement(ns.p + "from", XML::XAttribute("x", fromX) + XML::XAttribute("y", fromY)), fromX.is_init() || fromY.is_init()) +
				XML::WriteIf(XML::XElement(ns.p + "to", XML::XAttribute("x", toX) + XML::XAttribute("y", toY)), toX.is_init() || toY.is_init())
				);
		}

		void AnimScale::FillParentPointersForChilds()
		{
			cBhvr->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX