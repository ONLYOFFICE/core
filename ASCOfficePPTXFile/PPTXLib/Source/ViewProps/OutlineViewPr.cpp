#include "OutlineViewPr.h"

namespace PPTX
{
	namespace nsViewProps
	{
		OutlineViewPr::OutlineViewPr()
		{
		}

		OutlineViewPr::~OutlineViewPr()
		{
		}

		OutlineViewPr::OutlineViewPr(const XML::XNode& node)
		{
			fromXML(node);
		}

		const OutlineViewPr& OutlineViewPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void OutlineViewPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			SldLst->clear();

			CViewPr = element.element("cViewPr");
			if(element.element("sldLst").exist())
				XML::Fill(SldLst, element.element("sldLst"), "sld");
		}

		const XML::XNode OutlineViewPr::toXML() const
		{
			return	XML::XElement(ns.p + "outlineViewPr",
				XML::Write(CViewPr) +
				XML::WriteIf(XML::XElement(ns.p + "sldLst", XML::Write(SldLst)), !SldLst->empty())
			);
		}

		void OutlineViewPr::FillParentPointersForChilds()
		{
			CViewPr->SetParentPointer(*this);
			for(std::list<Sld>::iterator i = SldLst->begin(); i != SldLst->end(); i++)
				i->SetParentPointer(*this);
		}
	} // namespace nsViewProps
} // namespace PPTX