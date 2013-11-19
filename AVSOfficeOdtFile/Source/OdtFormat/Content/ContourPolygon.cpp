
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ContourPolygon.h"


namespace Odt
{
	namespace Content
	{

		ContourPolygon::ContourPolygon()
		{
		}


		ContourPolygon::~ContourPolygon()
		{
		}


		ContourPolygon::ContourPolygon(const XML::XNode& node)
		{
			fromXML(node);
		}


		const ContourPolygon& ContourPolygon::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void ContourPolygon::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Width = element.attribute("width").value().ToString();
			Height = element.attribute("height").value().ToString();
			ViewBox =	element.attribute("viewBox").value();
			Points =	element.attribute("points").value();		
			RecreateOnEdit = element.attribute("recreate-on-edit").value();
		}


		const XML::XNode ContourPolygon::toXML() const
		{
			return
				XML::XElement(ns.draw + "contour-polygon",
					XML::XAttribute(ns.svg + "width", Width) + 
					XML::XAttribute(ns.svg + "height", Height) +
					XML::XAttribute(ns.svg + "viewBox", ViewBox) +
					XML::XAttribute(ns.draw + "points", Points) +
					XML::XAttribute(ns.draw + "recreate-on-edit", RecreateOnEdit)
				);
		}
		
	} // namespace Content
} // namespace Odt