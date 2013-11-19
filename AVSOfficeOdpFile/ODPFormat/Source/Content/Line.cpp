#include "Line.h"

namespace Odp
{
	namespace Content
	{
		Line::Line()
		{
		}

		Line::~Line()
		{
		}

		Line::Line(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Line& Line::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Line::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			StyleName = element.attribute("style-name").value();
			TextStyleName = element.attribute("text-style-name").value();
			layer = element.attribute("layer").value();
			
			x1 = element.attribute("x1").value().ToString();
			y1 = element.attribute("y1").value().ToString();
			x2 = element.attribute("x2").value().ToString();
			y2 = element.attribute("y2").value().ToString();

			XML::Fill(Text, element, "p", "list");
		}

		const XML::XNode Line::toXML() const
		{
			return 
				XML::XElement(ns.draw + "line", 
					XML::XAttribute(ns.draw + "style-name", StyleName) +
					XML::XAttribute(ns.draw + "text-style-name", TextStyleName) +
					XML::XAttribute(ns.draw + "layer", layer) + 
					XML::XAttribute(ns.svg + "x1", x1) +
					XML::XAttribute(ns.svg + "y1", y1) +
					XML::XAttribute(ns.svg + "x2", x2) +
					XML::XAttribute(ns.svg + "y2", y2) +
					XML::Write(Text)
				);
		}

		void Line::GetMmRect(RECT& rect)const
		{
			rect.top = 0;
			rect.left = 0;
			rect.right = 0;
			rect.bottom = 0;

			rect.left = (long)(x1->value(UniversalUnit::Mm));
			rect.top = (long)(y1->value(UniversalUnit::Mm));

			rect.right = (long)(x2->value(UniversalUnit::Mm));
			rect.bottom = (long)(y2->value(UniversalUnit::Mm));
		}

	} // namespace Content
} // namespace Odp