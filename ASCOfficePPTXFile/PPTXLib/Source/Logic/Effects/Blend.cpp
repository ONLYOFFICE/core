#include "Blend.h"

namespace PPTX
{
	namespace Logic
	{
		Blend::Blend()
		{
		}

		Blend::~Blend()
		{
		}

		Blend::Blend(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Blend& Blend::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Blend::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cont = element.element("cont");
			blend = element.attribute("blend").value();
			FillParentPointersForChilds();
		}

		const XML::XNode Blend::toXML() const
		{
			return XML::XElement(ns.a + "blend",
					XML::Write(cont) +
					XML::XAttribute("blend", blend)
				);
		}

		void Blend::FillParentPointersForChilds()
		{
			cont->SetParentPointer(*this);
		}
	} // namespace Logic
} // namespace PPTX