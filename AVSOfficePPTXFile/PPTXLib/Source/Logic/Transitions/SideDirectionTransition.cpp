#include "SideDirectionTransition.h"

namespace PPTX
{
	namespace Logic
	{

		SideDirectionTransition::SideDirectionTransition()
		{
		}

		SideDirectionTransition::~SideDirectionTransition()
		{
		}

		SideDirectionTransition::SideDirectionTransition(const XML::XNode& node)
		{
			fromXML(node);
		}

		const SideDirectionTransition& SideDirectionTransition::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void SideDirectionTransition::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			name = element.XName->Name;
			dir = element.attribute("dir").value();
		}

		const XML::XNode SideDirectionTransition::toXML() const
		{
			return XML::XElement(ns.p + name.get(),
					XML::XAttribute("dir", dir)
				);
		}

	} // namespace Logic
} // namespace PPTX