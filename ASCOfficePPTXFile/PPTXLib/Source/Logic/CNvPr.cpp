#include "CNvPr.h"

namespace PPTX
{
	namespace Logic
	{

		CNvPr::CNvPr()
		{
		}


		CNvPr::~CNvPr()
		{
		}
	

		CNvPr::CNvPr(const XML::XNode& node)
		{
			fromXML(node);
		}


		const CNvPr& CNvPr::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void CNvPr::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			id = element.attribute("id").value();
			name = element.attribute("name").value();
			descr = element.attribute("descr").value();
			hidden = element.attribute("hidden").value();
			title = element.attribute("title").value();

			hlinkClick = element.element("hlinkClick");
			hlinkHover = element.element("hlinkHover");
		}


		const XML::XNode CNvPr::toXML() const
		{
			return XML::XElement(ns.p + "cNvPr", 
				XML::XAttribute("id", id) +
				XML::XAttribute("name", name) +
				XML::XAttribute("descr", descr) +
				XML::XAttribute("hidden", hidden) +
				XML::XAttribute("title", title) +
				XML::Write(hlinkClick) +
				XML::Write(hlinkHover)
			);
		}


		void CNvPr::FillParentPointersForChilds()
		{
			if(hlinkClick.is_init())
				hlinkClick->SetParentPointer(*this);
			if(hlinkHover.is_init())
				hlinkHover->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX