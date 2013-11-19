#include "EffectElement.h"

namespace PPTX
{
	namespace Logic
	{

		EffectElement::EffectElement()
		{
		}

		EffectElement::~EffectElement()
		{
		}

		EffectElement::EffectElement(const XML::XNode& node)
		{
			fromXML(node);
		}

		const EffectElement& EffectElement::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void EffectElement::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			ref = element.attribute("ref").value();
		}

		const XML::XNode EffectElement::toXML() const
		{
			return XML::XElement(ns.a + "effect",
				XML::XAttribute("ref", ref)
				);
		}
	} // namespace Logic
} // namespace PPTX