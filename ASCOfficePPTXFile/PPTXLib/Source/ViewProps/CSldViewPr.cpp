#include "CSldViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		CSldViewPr::CSldViewPr()
		{
		}

		CSldViewPr::~CSldViewPr()
		{
		}

		CSldViewPr::CSldViewPr(const XML::XNode& node)
		{
			fromXML(node);
		}

		const CSldViewPr& CSldViewPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void CSldViewPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			GuideLst->clear();

			CViewPr = element.element("cViewPr");
			XML::Fill(GuideLst, element.element("guideLst"), "guide");
			
			attrShowGuides = element.attribute("showGuides").value();
			attrShowGuides = element.attribute("snapToGrid").value();
			attrShowGuides = element.attribute("snapToObjects").value();

			FillParentPointersForChilds();
		}

		const XML::XNode CSldViewPr::toXML() const
		{
			return	XML::XElement(ns.p + "cSldViewPr",
				XML::XAttribute("showGuides", attrShowGuides) +
				XML::XAttribute("snapToGrid", attrSnapToGrid) +
				XML::XAttribute("snapToObjects", attrSnapToObjects) +
				XML::Write(CViewPr) +
				XML::XElement(ns.p + "guideLst",
					XML::Write(GuideLst)
				)
			);
		}

		void CSldViewPr::FillParentPointersForChilds()
		{
			CViewPr->SetParentPointer(*this);
			for(std::list<Guide>::iterator i = GuideLst->begin(); i != GuideLst->end(); i++)
				i->SetParentPointer(*this);
		}
	} // namespace nsViewProps
} // namespace PPTX