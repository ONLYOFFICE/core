#include "Fld.h"

namespace PPTX
{
	namespace Logic
	{

		Fld::Fld()
		{
		}

		Fld::~Fld()
		{
		}

		Fld::Fld(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Fld& Fld::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Fld::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			id = element.attribute("id").value();
			type = element.attribute("type").value();

			rPr = element.element("rPr");
			pPr = element.element("pPr");
			text = element.element("t").text();//.ToString();
			FillParentPointersForChilds();
		}

		const XML::XNode Fld::toXML() const
		{
			return XML::XElement(ns.a + "fld",
					XML::XAttribute("id", id) +
					XML::XAttribute("type", type) +
					XML::Write(rPr) +
					XML::Write(pPr) +
					XML::XElement(ns.a + "t", XML::XText(text))
				);
		}

		void Fld::FillParentPointersForChilds()
		{
			if(rPr.is_init())
				rPr->SetParentPointer(*this);
			if(pPr.is_init())
				pPr->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX