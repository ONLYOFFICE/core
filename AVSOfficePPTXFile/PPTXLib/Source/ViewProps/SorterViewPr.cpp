#include "SorterViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		SorterViewPr::SorterViewPr()
		{
		}

		SorterViewPr::~SorterViewPr()
		{
		}

		SorterViewPr::SorterViewPr(const XML::XNode& node)
		{
			fromXML(node);
		}

		const SorterViewPr& SorterViewPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void SorterViewPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			attrShowFormatting = element.attribute("showFormatting").value();
			CViewPr = element.element("cViewPr");

			FillParentPointersForChilds();
		}

		const XML::XNode SorterViewPr::toXML() const
		{
			return	XML::XElement(ns.p + "sorterViewPr",
				XML::XAttribute("showFormatting", attrShowFormatting) +
				XML::Write(CViewPr)
			);
		}

		void SorterViewPr::FillParentPointersForChilds()
		{
			CViewPr->SetParentPointer(*this);
		}
	} // namespace nsViewProps
} // namespace PPTX