#include "OleObject.h"

namespace Odp
{
	namespace Content
	{

		OleObject::OleObject()
		{
		}

		OleObject::~OleObject()
		{
		}

		OleObject::OleObject(const XML::XNode& node)
		{
			fromXML(node);
		}

		const OleObject& OleObject::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void OleObject::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			href = element.attribute("href").value();
			type = element.attribute("type").value();
			show = element.attribute("show").value();
			actuate = element.attribute("actuate").value();
		}

		const XML::XNode OleObject::toXML() const
		{
			return 
				XML::XElement(ns.draw + "object", 
					XML::XAttribute(ns.xlink + "href", href) +
					XML::XAttribute(ns.xlink + "type", type) +
					XML::XAttribute(ns.xlink + "show", show) + 
					XML::XAttribute(ns.xlink + "actuate", actuate)
				);
		}
	} // namespace Content
} // namespace Odp