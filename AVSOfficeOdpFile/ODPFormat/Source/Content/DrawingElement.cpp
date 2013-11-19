#include "DrawingElement.h"
#include "Frame.h"
#include "CustomShape.h"
#include "Group.h"
#include "Line.h"
#include "Rect.h"

namespace Odp
{
	namespace Content
	{
		DrawingElement::DrawingElement()
		{
		}

		DrawingElement::~DrawingElement()
		{
		}

		DrawingElement::DrawingElement(const XML::XNode& node)
		{
			fromXML(node);
		}

		const DrawingElement& DrawingElement::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void DrawingElement::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			if (element->XName == "frame")
				m_item.reset(new Frame(node));
			else if (element->XName == "custom-shape")
				m_item.reset(new CustomShape(node));
			else if(element->XName == "g")
				m_item.reset(new Group(node));
			else if(element->XName == "line")
				m_item.reset(new Line(node));
			else if(element->XName == "rect")
				m_item.reset(new Rect(node));
			else m_item.reset();
		}

		const XML::XNode DrawingElement::toXML() const
		{
			return XML::Write(m_item);
		}
	} // namespace Content
} // namespace Odp