#include "OptionalBlackTransition.h"

namespace PPTX
{
	namespace Logic
	{

		OptionalBlackTransition::OptionalBlackTransition()
		{
		}

		OptionalBlackTransition::~OptionalBlackTransition()
		{
		}

		OptionalBlackTransition::OptionalBlackTransition(const XML::XNode& node)
		{
			fromXML(node);
		}

		const OptionalBlackTransition& OptionalBlackTransition::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void OptionalBlackTransition::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			name = element.XName->Name;
			thruBlk = element.attribute("thruBlk").value();
		}

		const XML::XNode OptionalBlackTransition::toXML() const
		{
			return XML::XElement(ns.p + name.get(),
					XML::XAttribute("thruBlk", thruBlk)
				);
		}

	} // namespace Logic
} // namespace PPTX