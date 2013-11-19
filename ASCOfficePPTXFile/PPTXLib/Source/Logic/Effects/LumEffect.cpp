#include "LumEffect.h"

namespace PPTX
{
	namespace Logic
	{

		LumEffect::LumEffect()
		{
		}

		LumEffect::~LumEffect()
		{
		}

		LumEffect::LumEffect(const XML::XNode& node)
		{
			fromXML(node);
		}

		const LumEffect& LumEffect::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void LumEffect::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			bright = element.attribute("bright").value();
			contrast = element.attribute("contrast").value();
		}

		const XML::XNode LumEffect::toXML() const
		{
			return XML::XElement(ns.a + "lum",
					XML::XAttribute("bright", bright) +			
					XML::XAttribute("contrast", contrast)
			);
		}
	} // namespace Logic
} // namespace PPTX