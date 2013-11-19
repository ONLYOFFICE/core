
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Line.h"

namespace OOX
{
	namespace Logic
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

		const Line& Line::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Line::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Id					=	element.attribute("id").value();
			style				=	element.attribute("style").value();			
			Wrap				=	element.element("wrap");
			anchorlock			=	element.element("anchorlock");

			fillstyle			=	element;
			linestyle			=	element;
			shadow				=	element.element("shadow");

			if (element.attribute("from").exist())
				from			=	element.attribute("from").value();
			if (element.attribute("to").exist())
				to				=	element.attribute("to").value();
		}

		const XML::XNode Line::toXML() const
		{			
			return XML::XNode();
		}

	} // namespace Logic
} // namespace OOX
