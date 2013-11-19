#include "NormalViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		NormalViewPr::NormalViewPr()
		{
		}

		NormalViewPr::~NormalViewPr()
		{
		}

		NormalViewPr::NormalViewPr(const XML::XNode& node)
		{
			fromXML(node);
		}

		const NormalViewPr& NormalViewPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void NormalViewPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			attrHorzBarState = element.attribute("horzBarState").value();
			attrVertBarState = element.attribute("vertBarState").value();
			attrPreferSingleView = element.attribute("preferSingleView").value();
			attrShowOutlineIcons = element.attribute("showOutlineIcons").value();
			attrSnapVertSplitter = element.attribute("snapVertSplitter").value();

			restoredLeft = element.element("restoredLeft");
			restoredTop = element.element("restoredTop");

			FillParentPointersForChilds();
		}

		const XML::XNode NormalViewPr::toXML() const
		{
			return	XML::XElement(ns.p + "normalViewPr",
				XML::XAttribute("horzBarState", attrHorzBarState) +
				XML::XAttribute("vertBarState", attrVertBarState) +
				XML::XAttribute("preferSingleView", attrPreferSingleView) +
				XML::XAttribute("showOutlineIcons", attrShowOutlineIcons) +
				XML::XAttribute("snapVertSplitter", attrSnapVertSplitter) +
				XML::Write(restoredLeft) +
				XML::Write(restoredTop)
			);
		}

		void NormalViewPr::FillParentPointersForChilds()
		{
			restoredLeft->SetParentPointer(*this);
			restoredTop->SetParentPointer(*this);
		}
	} // namespace nsViewProps
} // namespace PPTX