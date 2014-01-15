
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "DrawingWrap.h"


namespace OOX
{
	namespace Logic
	{
		DrawingWrap::DrawingWrap()
		{

		}

		DrawingWrap::~DrawingWrap()
		{

		}

		DrawingWrap::DrawingWrap(const XML::XNode& node)
		{
			fromXML(node);
		}

		const DrawingWrap& DrawingWrap::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void DrawingWrap::fromXML(const XML::XNode& node)
		{
			const XML::XElement element (node);
			if (element.element("wrapSquare").exist())
			{
				Type		=	"wrapSquare";
				TextWrap	=	element.element("wrapSquare").attribute("wrapText").value();
			}
			else if(element.element("wrapTight").exist())
			{
				Type		=	"wrapTight";
				TextWrap	=	element.element("wrapTight").attribute("wrapText").value();
			}
			else if(element.element("wrapThrough").exist())
			{
				Type		=	"wrapThrough";
				TextWrap	=	element.element("wrapThrough").attribute("wrapText").value();
			}
			else if(element.element("wrapTopAndBottom").exist())
			{
				Type		=	"wrapTopAndBottom";
				TextWrap	=	"bothSides";
			}
			else
			{
				Type		=	"wrapNone";
				TextWrap	=	"bothSides";
			}

			DistT			=	element.attribute("distT").value();
			DistB			=	element.attribute("distB").value();
			DistL			=	element.attribute("distL").value();
			DistR			=	element.attribute("distR").value();
			EffectExtent	=	element.element("effectExtent");
			WrapPath		=	element.element("wrapPolygon");
		}

		const XML::XNode DrawingWrap::toXML() const
		{
			if (*Type == "wrapSquare")
			{
				return XML::XElement(ns.wp + "wrapSquare",
					XML::XAttribute("wrapText", TextWrap) +
					XML::XAttribute("distT", DistT) +
					XML::XAttribute("distB", DistB) +
					XML::XAttribute("distL", DistL) +
					XML::XAttribute("distR", DistR) +
					XML::Write(EffectExtent)
					);
			}
			else if (*Type == "wrapTopAndBottom")
			{
				return XML::XElement(ns.wp + "wrapTopAndBottom",
					XML::XAttribute("distT", DistT) +
					XML::XAttribute("distB", DistB) +
					XML::Write(EffectExtent)
					);
			}
			else if (*Type == "wrapTight")
			{
				return XML::XElement(ns.wp + "wrapTight",
					XML::XAttribute("wrapText", TextWrap) +
					XML::XAttribute("distL", DistL) +
					XML::XAttribute("distR", DistR) +
					XML::Write(WrapPath)
					);
			}
			else if (*Type == "wrapThrough")
			{
				return XML::XElement(ns.wp + "wrapThrough",
					XML::XAttribute("wrapText", TextWrap) +
					XML::XAttribute("distL", DistL) +
					XML::XAttribute("distR", DistR) +
					XML::Write(WrapPath)
					);
			}

			return XML::XElement(ns.wp + "wrapNone");
		}
	} // namespace Logic
} // namespace OOX