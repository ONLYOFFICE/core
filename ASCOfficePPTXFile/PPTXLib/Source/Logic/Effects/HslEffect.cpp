#include "HslEffect.h"

namespace PPTX
{
	namespace Logic
	{

		HslEffect::HslEffect()
		{
		}

		HslEffect::~HslEffect()
		{
		}

		HslEffect::HslEffect(const XML::XNode& node)
		{
			fromXML(node);
		}

		const HslEffect& HslEffect::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void HslEffect::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			hue = element.attribute("hue").value();
			sat = element.attribute("sat").value();
			lum = element.attribute("lum").value();
		}

		const XML::XNode HslEffect::toXML() const
		{
			return XML::XElement(ns.a + "hsl",
					XML::XAttribute("hue", hue) +			
					XML::XAttribute("sat", sat) +
					XML::XAttribute("lum", lum)
			);
		}
	} // namespace Logic
} // namespace PPTX