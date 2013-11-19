#include "CustomShape.h"

namespace Odp
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
			name = element.attribute("name").value();
			style_name = element.attribute("style-name").value();
			text_style_name = element.attribute("text-style-name").value();
			layer = element.attribute("layer").value();
			height = element.attribute("height").value().ToString();
			width = element.attribute("width").value().ToString();
			if(element.attribute("x").exist())
				x = element.attribute("x").value().ToString();
			if(element.attribute("y").exist())
				y = element.attribute("y").value().ToString();
			transform = element.attribute("transform").value();
			id = element.attribute("id").value();

			XML::Fill(text, element, "p", "list");
			geometry = element.element("enhanced-geometry");
		}

		const XML::XNode CustomShape::toXML() const
		{
			return 
				XML::XElement(ns.draw + "custom-shape", 
					XML::XAttribute(ns.draw + "name", name) +
					XML::XAttribute(ns.draw + "style-name", style_name) +
					XML::XAttribute(ns.draw + "text-style-name", text_style_name) +
					XML::XAttribute(ns.draw + "layer", layer) + 
					XML::XAttribute(ns.svg + "width", width) +
					XML::XAttribute(ns.svg + "height", height) +
					XML::XAttribute(ns.svg + "x", x) +
					XML::XAttribute(ns.svg + "y", y) +
					XML::XAttribute(ns.draw + "transform", transform) +
					XML::XAttribute(ns.draw + "id", id) +
					XML::Write(text) +
					XML::Write(geometry)
				);
		}

		void CustomShape::GetMmRect(RECT& rect)const
		{
			rect.top = 0;
			rect.left = 0;
			rect.right = 0;
			rect.bottom = 0;

			if(y.is_init())
				rect.top = (long)(y->value(UniversalUnit::Mm));

			if(x.is_init())
				rect.left = (long)(x->value(UniversalUnit::Mm));

			rect.right = (long)(width->value(UniversalUnit::Mm));
			rect.right = rect.right + rect.left;
			rect.bottom = (long)(height->value(UniversalUnit::Mm));
			rect.bottom = rect.bottom + rect.top;
		}
	} // namespace Content
} // namespace Odp