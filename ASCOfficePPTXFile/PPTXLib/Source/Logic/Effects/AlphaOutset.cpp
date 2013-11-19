#include "AlphaOutset.h"

namespace PPTX
{
	namespace Logic
	{

		AlphaOutset::AlphaOutset()
		{
		}

		AlphaOutset::~AlphaOutset()
		{
		}

		AlphaOutset::AlphaOutset(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AlphaOutset& AlphaOutset::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AlphaOutset::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			rad = element.attribute("rad").value();
		}

		const XML::XNode AlphaOutset::toXML() const
		{
			return XML::XElement(ns.a + "alphaOutset",
				XML::XAttribute("rad", rad)
				);
		}
	} // namespace Logic
} // namespace PPTX