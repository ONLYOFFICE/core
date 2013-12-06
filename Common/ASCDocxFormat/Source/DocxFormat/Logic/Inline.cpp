
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Inline.h"

namespace OOX
{
	namespace Logic
	{
		Inline::Inline()
		{

		}

		Inline::~Inline()
		{

		}

		Inline::Inline(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Inline& Inline::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Inline::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			distanceLeftTop.init();

			distanceLeftTop->X			=	element.attribute("distL").value().ToString();
			distanceLeftTop->Y			=	element.attribute("distT").value().ToString();
			
			distanceRightBottom.init();

			distanceRightBottom->X		=	element.attribute("distR").value().ToString();
			distanceRightBottom->Y		=	element.attribute("distB").value().ToString();

			Name						=	element.XName->Name;

			if (element.attribute("distT").exist())
				DistT					=	element.attribute("distT").value();
			if (element.attribute("distB").exist())
				DistB					=	element.attribute("distB").value();
			if (element.attribute("distL").exist())
				DistL					=	element.attribute("distL").value();
			if (element.attribute("distR").exist())
				DistR					=	element.attribute("distR").value();

			Extent						=	element.element("extent");
			DocPr						=	element.element("docPr");
			NoChangeAspect				=	element.element("cNvGraphicFramePr").element("graphicFrameLocks").attribute("noChangeAspect").value();
			Graphic						=	element.element("graphic");
			
			anchorXY.init();
			anchorXY->X					=	0;
			anchorXY->Y					=	0;

			if (element.element("positionH").exist())
			{
				anchorXY->X				=	element.element("positionH").element("posOffset").text().ToString();
				
				positionH				=	element.element("positionH").element("posOffset").text();
				positionHAlign			=	element.element("positionH").element("align").text();
				positionHRelativeFrom	=	element.element("positionH").attribute("relativeFrom").value();
			}

			if (element.element("positionV").exist())
			{
				anchorXY->Y				=	element.element("positionV").element("posOffset").text().ToString();
				
				positionV				=	element.element("positionV").element("posOffset").text();
				positionVAlign			=	element.element("positionV").element("align").text();
				positionVRelativeFrom	=	element.element("positionV").attribute("relativeFrom").value();
			}

			Wrap						=	element;

			SimplePos					=	element.attribute("simplePos").value();
			SimplePosPoint				=	element.element("simplePos");
			RelativeHeight				=	element.attribute("relativeHeight").value();
			BehindDoc					=	element.attribute("behindDoc").value();
			Locked						=	element.attribute("locked").value();
			LayoutInCell				=	element.attribute("layoutInCell").value();
			AllowOverlap				=	element.attribute("allowOverlap").value();
		}

		const XML::XNode Inline::toXML() const
		{
			return XML::XElement();
				//XML::XElement(ns.wp + Name.ToString(), 
				//	XML::XAttribute("distT", DistT) +
				//	XML::XAttribute("distB", DistB) +
				//	XML::XAttribute("distL", DistL) +
				//	XML::XAttribute("distR", DistR) +

				//	XML::XAttribute("simplePos", SimplePos) +
				//	XML::XAttribute("relativeHeight", RelativeHeight) +
				//	XML::XAttribute("behindDoc", BehindDoc) +
				//	XML::XAttribute("locked", Locked) +
				//	XML::XAttribute("layoutInCell", LayoutInCell) +
				//	XML::XAttribute("allowOverlap", AllowOverlap) +

				//	XML::Write(SimplePosPoint) +
				//	WriteIf(
				//		XML::XElement(ns.wp + "positionH",
				//			XML::XAttribute("relativeFrom", positionHRelativeFrom.get_value_or("column")) +
				//			WriteIf( XML::XElement(ns.wp + "posOffset",
				//				XML::XText(positionH)
				//			), positionH.is_init()) +
				//			WriteIf( XML::XElement(ns.wp + "align",
				//				XML::XText(positionHAlign)
				//			), positionHAlign.is_init())
				//		), positionH.is_init() || positionHAlign.is_init()) +
				//	WriteIf(
				//		XML::XElement(ns.wp + "positionV",
				//			XML::XAttribute("relativeFrom", positionVRelativeFrom.get_value_or("paragraph")) +
				//			WriteIf( XML::XElement(ns.wp + "posOffset",
				//				XML::XText(positionV)
				//			), positionV.is_init()) +
				//			WriteIf( XML::XElement(ns.wp + "align",
				//				XML::XText(positionVAlign)
				//			), positionVAlign.is_init())
				//		), positionV.is_init() || positionVAlign.is_init()) +
				//	XML::Write(Extent) +
				//	XML::Write(EffectExtent) +
				//	XML::Write(Wrap) +
				//	XML::Write(DocPr) +
				//		XML::XElement(ns.wp + "cNvGraphicFramePr",
				//			XML::XElement(ns.a + "graphicFrameLocks",
				//				XML::XAttribute("noChangeAspect", NoChangeAspect)
				//			)
				//		) +
				//	XML::Write(Graphic)
				//);
		}

	} // namespace Logic
} // namespace OOX