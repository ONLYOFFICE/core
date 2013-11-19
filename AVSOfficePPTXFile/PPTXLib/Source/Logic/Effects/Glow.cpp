#include "Glow.h"

namespace PPTX
{
	namespace Logic
	{

		Glow::Glow()
		{
		}


		Glow::~Glow()
		{
		}


		Glow::Glow(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Glow& Glow::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Glow::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Color.GetColorFrom(element);
			rad = element.attribute("rad").value();
			FillParentPointersForChilds();
		}


		const XML::XNode Glow::toXML() const
		{
			return XML::XElement(ns.a + "glow",
					XML::XAttribute("rad", rad) +
					XML::Write(Color)
				);
		}

		void Glow::FillParentPointersForChilds()
		{
			Color.SetParentPointer(*this);
		}

	} // namespace Logic
} // namespace PPTX