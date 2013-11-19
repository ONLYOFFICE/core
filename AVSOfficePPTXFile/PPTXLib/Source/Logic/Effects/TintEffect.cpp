#include "TintEffect.h"

namespace PPTX
{
	namespace Logic
	{

		TintEffect::TintEffect()
		{
		}

		TintEffect::~TintEffect()
		{
		}

		TintEffect::TintEffect(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TintEffect& TintEffect::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TintEffect::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			amt = element.attribute("amt").value();
			hue = element.attribute("hue").value();
		}

		const XML::XNode TintEffect::toXML() const
		{
			return XML::XElement(ns.a + "tint",
					XML::XAttribute("hue", hue) +
					XML::XAttribute("amt", amt)
			);
		}
	} // namespace Logic
} // namespace PPTX