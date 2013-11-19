#include "AlphaModFix.h"

namespace PPTX
{
	namespace Logic
	{

		AlphaModFix::AlphaModFix()
		{
		}

		AlphaModFix::~AlphaModFix()
		{
		}

		AlphaModFix::AlphaModFix(const XML::XNode& node)
		{
			fromXML(node);
		}

		const AlphaModFix& AlphaModFix::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void AlphaModFix::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			amt = element.attribute("amt").value();
		}

		const XML::XNode AlphaModFix::toXML() const
		{
			return XML::XElement(ns.a + "alphaModFix",
				XML::XAttribute("amt", amt)
				);
		}
	} // namespace Logic
} // namespace PPTX