#include "SldSz.h"

namespace PPTX
{
	namespace nsPresentation
	{
		SldSz::SldSz()
		{
		}

		SldSz::~SldSz()
		{
		}

		SldSz::SldSz(const XML::XNode& node)
		{
			fromXML(node);
		}

		const SldSz& SldSz::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void SldSz::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			cx = element.attribute("cx").value();
			cy = element.attribute("cy").value();
			type = element.attribute("type").value();
		}

		const XML::XNode SldSz::toXML() const
		{
			return	XML::XElement(ns.p + "sldSz",
				XML::XAttribute("cx", cx) +
				XML::XAttribute("cy", cy) +
				XML::XAttribute("type", type)
			);
		}
	} // namespace nsPresentation
} // namespace PPTX