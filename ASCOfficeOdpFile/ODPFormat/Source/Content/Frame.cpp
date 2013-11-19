#include "Frame.h"

namespace Odp
{
	namespace Content
	{
		Frame::Frame()
		{
		}

		Frame::~Frame()
		{
		}

		Frame::Frame(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Frame& Frame::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Frame::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			draw_style_name = element.attribute(ns.draw + "style-name").value();
			presentation_style_name = element.attribute(ns.presentation + "style-name").value();
			text_style_name = element.attribute("text-style-name").value();
			layer = element.attribute("layer").value();
			height = element.attribute("height").value().ToString();
			width = element.attribute("width").value().ToString();
			if(element.attribute("x").exist())
				x = element.attribute("x").value().ToString();
			if(element.attribute("y").exist())
				y = element.attribute("y").value().ToString();
			Class = element.attribute("class").value();
			placeholder = element.attribute("placeholder").value();
			id = element.attribute("id").value();

			if(element.element("object").exist())
				object = element.element("object");
			else
				object = element.element("object-ole");

			image = element.element("image");
			text_box = element.element("text-box");
			Table = element.element("table");
		}

		const XML::XNode Frame::toXML() const
		{
			return 
				XML::XElement(ns.draw + "frame", 
					XML::XAttribute(ns.draw + "style-name", draw_style_name) +
					XML::XAttribute(ns.presentation + "style-name", presentation_style_name) +
					XML::XAttribute(ns.draw + "text-style-name", text_style_name) +
					XML::XAttribute(ns.draw + "layer", layer) + 
					XML::XAttribute(ns.svg + "width", width) +
					XML::XAttribute(ns.svg + "height", height) +
					XML::XAttribute(ns.svg + "x", x) +
					XML::XAttribute(ns.svg + "y", y) +
					XML::XAttribute(ns.presentation + "class", Class) + 
					XML::XAttribute(ns.presentation + "placeholder", placeholder) +
					XML::XAttribute(ns.draw + "id", id) +
					XML::Write(object) +
					XML::Write(Table) +
					XML::Write(image) + 
					XML::Write(text_box)
				);
		}

		void Frame::GetMmRect(RECT& rect)const
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