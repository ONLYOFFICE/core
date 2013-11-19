#include "ColorModifier.h"

namespace PPTX
{
	namespace Logic
	{

		ColorModifier::ColorModifier()
		{
		}


		ColorModifier::~ColorModifier()
		{
		}
	

		ColorModifier::ColorModifier(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ColorModifier& ColorModifier::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ColorModifier::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			name = element.XName.get().Name;
			val = element.attribute("val").value();
		}


		const XML::XNode ColorModifier::toXML() const
		{
			return XML::XElement(ns.a + name.get(),
					XML::XAttribute("val", val)
				);
		}

	} // namespace Logic
} // namespace PPTX