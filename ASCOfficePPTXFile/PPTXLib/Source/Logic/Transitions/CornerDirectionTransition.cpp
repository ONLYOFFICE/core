#include "CornerDirectionTransition.h"

namespace PPTX
{
	namespace Logic
	{

		CornerDirectionTransition::CornerDirectionTransition()
		{
		}

		CornerDirectionTransition::~CornerDirectionTransition()
		{
		}

		CornerDirectionTransition::CornerDirectionTransition(const XML::XNode& node)
		{
			fromXML(node);
		}

		const CornerDirectionTransition& CornerDirectionTransition::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void CornerDirectionTransition::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			name = element.XName->Name;
			dir = element.attribute("dir").value();
		}

		const XML::XNode CornerDirectionTransition::toXML() const
		{
			return XML::XElement(ns.p + name.get(),
					XML::XAttribute("dir", dir)
				);
		}

	} // namespace Logic
} // namespace PPTX