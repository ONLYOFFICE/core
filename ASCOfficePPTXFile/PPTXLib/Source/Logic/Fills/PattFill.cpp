#include "PattFill.h"

namespace PPTX
{
	namespace Logic
	{
		PattFill::PattFill()
		{
		}

		PattFill::~PattFill()
		{
		}

		PattFill::PattFill(const XML::XNode& node)
		{
			fromXML(node);
		}

		const PattFill& PattFill::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void PattFill::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			m_namespace = element.XName.get().Ns;

			prst = element.attribute("prst").value();
			if(element.element("fgClr").exist())
				fgClr.GetColorFrom(element.element("fgClr"));
			if(element.element("bgClr").exist())
				bgClr.GetColorFrom(element.element("bgClr"));
			FillParentPointersForChilds();
		}

		const XML::XNode PattFill::toXML() const
		{
			XML::XElement element(m_namespace + "pattFill",
					XML::XAttribute("prst", prst)
				);
			if(fgClr.is_init())
				element.Add(XML::XElement(ns.a + "fgClr", XML::Write(fgClr)));
			if(bgClr.is_init())
				element.Add(XML::XElement(ns.a + "bgClr", XML::Write(bgClr)));
			return element;
		}

		void PattFill::FillParentPointersForChilds()
		{
			fgClr.SetParentPointer(*this);
			bgClr.SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX