#include "SplitTransition.h"

namespace PPTX
{
	namespace Logic
	{

		SplitTransition::SplitTransition()
		{
		}

		SplitTransition::~SplitTransition()
		{
		}

		SplitTransition::SplitTransition(const XML::XNode& node)
		{
			fromXML(node);
		}

		const SplitTransition& SplitTransition::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void SplitTransition::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			dir = element.attribute("dir").value();
			orient = element.attribute("orient").value();
		}

		const XML::XNode SplitTransition::toXML() const
		{
			return XML::XElement(ns.p + "split",
					XML::XAttribute("dir", dir) +
					XML::XAttribute("orient", orient)
				);
		}

	} // namespace Logic
} // namespace PPTX