#include "SlideViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		SlideViewPr::SlideViewPr()
		{
		}

		SlideViewPr::~SlideViewPr()
		{
		}

		SlideViewPr::SlideViewPr(const XML::XNode& node)
		{
			fromXML(node);
		}

		const SlideViewPr& SlideViewPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void SlideViewPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			CSldViewPr = element.element("cSldViewPr");

			FillParentPointersForChilds();
		}

		const XML::XNode SlideViewPr::toXML() const
		{
			return	XML::XElement(ns.p + "slideViewPr",
				XML::Write(CSldViewPr)
			);
		}

		void SlideViewPr::FillParentPointersForChilds()
		{
			CSldViewPr->SetParentPointer(*this);
		}
	} // namespace nsViewProps
} // namespace PPTX