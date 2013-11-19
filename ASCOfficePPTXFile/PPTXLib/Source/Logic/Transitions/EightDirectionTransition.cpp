#include "EightDirectionTransition.h"

namespace PPTX
{
	namespace Logic
	{

		EightDirectionTransition::EightDirectionTransition()
		{
		}

		EightDirectionTransition::~EightDirectionTransition()
		{
		}

		EightDirectionTransition::EightDirectionTransition(const XML::XNode& node)
		{
			fromXML(node);
		}

		const EightDirectionTransition& EightDirectionTransition::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void EightDirectionTransition::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			name = element.XName->Name;
			dir = element.attribute("dir").value();
		}

		const XML::XNode EightDirectionTransition::toXML() const
		{
			return XML::XElement(ns.p + name.get(),
					XML::XAttribute("dir", dir)
				);
		}

	} // namespace Logic
} // namespace PPTX