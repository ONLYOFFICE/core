#include "OrientationTransition.h"

namespace PPTX
{
	namespace Logic
	{

		OrientationTransition::OrientationTransition()
		{
		}

		OrientationTransition::~OrientationTransition()
		{
		}

		OrientationTransition::OrientationTransition(const XML::XNode& node)
		{
			fromXML(node);
		}

		const OrientationTransition& OrientationTransition::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void OrientationTransition::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			name = element.XName->Name;
			dir = element.attribute("dir").value();
		}

		const XML::XNode OrientationTransition::toXML() const
		{
			return XML::XElement(ns.p + name.get(),
					XML::XAttribute("dir", dir)
				);
		}

	} // namespace Logic
} // namespace PPTX