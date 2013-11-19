#include "Group.h"

namespace Odp
{
	namespace Content
	{
		Group::Group()
		{
		}

		Group::~Group()
		{
		}

		Group::Group(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Group& Group::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Group::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(Elements, element, "frame", "custom-shape", "g", "line", "rect");
		}

		const XML::XNode Group::toXML() const
		{
			return 
				XML::XElement(ns.draw + "g", 
					XML::Write(Elements)
				);
		}
	} // namespace Content
} // namespace Odp