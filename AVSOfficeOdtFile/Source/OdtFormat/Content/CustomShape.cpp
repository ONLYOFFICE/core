
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "CustomShape.h"


namespace Odt
{
	namespace Content
	{

		CustomShape::CustomShape()
		{
		}


		CustomShape::~CustomShape()
		{
		}


		CustomShape::CustomShape(const XML::XNode& node)
		{
			fromXML(node);
		}


		const CustomShape& CustomShape::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void CustomShape::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			AnchorType		= element.attribute("anchor-type").value();
			StyleName			= element.attribute("style-name").value();
			TextStyleName	= element.attribute("text-style-name").value();
			ZIndex		= element.attribute("z-index").value();
			Size			= XML::XElement2Size<UnitCm>(element, "height", "width");
			Point			= XML::XElement2Point<UnitCm>(element, "x", "y");
			Transform	= element.attribute("transform").value();
			Title			= element.element("title").text();
			EnhancedGeometry	= element.element("enhanced-geometry");			
			XML::Fill(Items, element, "p");
		}


		const XML::XNode CustomShape::toXML() const
		{
			return
				XML::XElement(ns.draw + "custom-shape",
					XML::XAttribute(ns.text + "anchor-type", AnchorType) +
					XML::XAttribute(ns.draw + "z-index", ZIndex) +
					XML::XAttribute(ns.draw + "style-name", StyleName) +
					XML::XAttribute(ns.draw + "text-style-name", TextStyleName) +
					XML::XAttribute(ns.svg + "width", Size->Width) +
					XML::XAttribute(ns.svg + "height", Size->Height) +
					XML::XAttribute(ns.svg + "x", Point->X) +
					XML::XAttribute(ns.svg + "y", Point->Y) +
					XML::XAttribute(ns.draw + "transform", Transform) +
					XML::WriteIf(XML::XElement(ns.svg + "title", XML::XText(Title)), Title.is_init()) +
					XML::Write(Items) +
					XML::Write(EnhancedGeometry)
				);
		}


		const std::string CustomShape::toTxt() const
		{
			return "[custom-shape]";
		}

	} // namespace Content
} // namespace Odt