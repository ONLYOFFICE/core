#include "Rect.h"

namespace Odp
{
	namespace Content
	{
		Rect::Rect()
		{
		}

		Rect::~Rect()
		{
		}

		Rect::Rect(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Rect& Rect::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Rect::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			PresentationStyleName = element.attribute(ns.presentation + "style-name").value();
			DrawStyleName = element.attribute(ns.draw + "style-name").value();
			TextStyleName = element.attribute("text-style-name").value();
			Id = element.attribute("id").value();
			layer = element.attribute("layer").value();
			height = element.attribute("height").value().ToString();
			width = element.attribute("width").value().ToString();
			if(element.attribute("x").exist())
				x = element.attribute("x").value().ToString();
			else
				x = "0";
			if(element.attribute("y").exist())
				y = element.attribute("y").value().ToString();
			else
				y = "0";
			XML::Fill(Text, element, "p", "list");
		}

		const XML::XNode Rect::toXML() const
		{
			return 
				XML::XElement(ns.draw + "rect", 
					XML::XAttribute(ns.presentation + "style-name", PresentationStyleName) +
					XML::XAttribute(ns.draw + "style-name", DrawStyleName) +
					XML::XAttribute(ns.draw + "text-style-name", TextStyleName) +
					XML::XAttribute(ns.draw + "id", Id) +
					XML::XAttribute(ns.draw + "layer", layer) + 
					XML::XAttribute(ns.svg + "width", width) +
					XML::XAttribute(ns.svg + "height", height) +
					XML::XAttribute(ns.svg + "x", x) +
					XML::XAttribute(ns.svg + "y", y) +
					XML::Write(Text)
				);
		}

		void Rect::GetMmRect(RECT& rect)const
		{
			rect.top = 0;
			rect.left = 0;
			rect.right = 0;
			rect.bottom = 0;

			rect.top = (long)(y->value(UniversalUnit::Mm));
			rect.left = (long)(x->value(UniversalUnit::Mm));

			rect.right = (long)(width->value(UniversalUnit::Mm));
			rect.right = rect.right + rect.left;
			rect.bottom = (long)(height->value(UniversalUnit::Mm));
			rect.bottom = rect.bottom + rect.top;
		}
	} // namespace Content
} // namespace Odp