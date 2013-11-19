#include "Br.h"

namespace PPTX
{
	namespace Logic
	{

		Br::Br()
		{
		}

		Br::~Br()
		{
		}

		Br::Br(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Br& Br::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Br::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			rPr = element.element("rPr");
			FillParentPointersForChilds();
		}

		const XML::XNode Br::toXML() const
		{
			return XML::XElement(ns.a + "br",
					XML::Write(rPr)
				);
		}

		void Br::FillParentPointersForChilds()
		{
			if(rPr.is_init())
				rPr->SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX