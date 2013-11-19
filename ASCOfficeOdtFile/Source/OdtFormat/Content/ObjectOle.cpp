
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ObjectOle.h"


namespace Odt
{
	namespace Content
	{

		ObjectOle::ObjectOle()
		{
		}


		ObjectOle::~ObjectOle()
		{
		}


		ObjectOle::ObjectOle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ObjectOle& ObjectOle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ObjectOle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Href = element.attribute("href").value();
			Type = element.attribute("type").value();
			Show =	element.attribute("show").value();
			Actuate =	element.attribute("actuate").value();				
		}


		const XML::XNode ObjectOle::toXML() const
		{
			return
				XML::XElement(ns.draw + "object-ole",
					XML::XAttribute(ns.xlink + "href", Href) + 
					XML::XAttribute(ns.xlink + "type", Type) +
					XML::XAttribute(ns.xlink + "show", Show) +
					XML::XAttribute(ns.xlink + "actuate", Actuate)
				);
		}
		
	} // namespace Content
} // namespace Odt