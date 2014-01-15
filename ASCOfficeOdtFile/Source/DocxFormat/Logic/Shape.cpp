
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Shape.h"

namespace OOX
{
	namespace Logic
	{			
		Shape::Shape()
		{

		}

		Shape::~Shape()
		{

		}

		Shape::Shape(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Shape& Shape::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Shape::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Id					=	element.attribute("id").value();
			Type				=	element.attribute("type").value();	
			style				=	element.attribute("style").value();			
			adj					=	element.attribute("adj").value();
			AllowInCell			=	element.attribute("allowincell").value();
			Ole					=	element.attribute("ole").value();
			coordsize			=	element.attribute("coordsize").value();
			path				=	element.attribute("path").value();
			imageData			=	element.element("imagedata");
			TextPath			=	element.element("textpath");
			Wrap				=	element.element("wrap");
			Extrusion			=	element.element("extrusion");
			anchorlock			=	element.element("anchorlock");

			fillstyle			=	element;
			linestyle			=	element;
			shadow				=	element.element("shadow");

			textbox				=	element.element("textbox");

			if (element.element("fill").exist())
				fillstyle->fromXML(element.element("fill"));

			if (element.element("stroke").exist())
				linestyle->fromXML(element.element("stroke"));		
		}

		const XML::XNode Shape::toXML() const
		{			
			return
				XML::XElement(ns.v + "shape", 
				XML::XAttribute("id", Id) +
				XML::XAttribute("type", Type) +
				XML::XAttribute("style", style.ToString()) +	
				XML::XAttribute("adj", adj) +
				XML::XAttribute("allowincell", AllowInCell) +
				XML::XAttribute(ns.o + "ole", Ole) +
				XML::XAttribute("coordsize", coordsize) +
				XML::XAttribute("path", path) +
				XML::Write(shadow) +
				XML::Write(textbox) +
				XML::Write(imageData) +
				XML::Write(TextPath) +
				XML::Write(Wrap) +
				XML::Write(Extrusion));
		}

	} // namespace Logic
} // namespace OOX
