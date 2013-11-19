#include "Blur.h"

namespace PPTX
{
	namespace Logic
	{

		Blur::Blur()
		{
		}

		Blur::~Blur()
		{
		}

		Blur::Blur(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Blur& Blur::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Blur::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			rad = element.attribute("rad").value();
			grow = element.attribute("grow").value();
		}

		const XML::XNode Blur::toXML() const
		{
			return XML::XElement(ns.a + "blur",
					XML::XAttribute("rad", rad) +
					XML::XAttribute("grow", grow)					
				);
		}
	} // namespace Logic
} // namespace PPTX