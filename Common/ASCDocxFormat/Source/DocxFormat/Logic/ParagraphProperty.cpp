
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "ParagraphProperty.h"

namespace OOX
{
	namespace Logic
	{
		ParagraphProperty::ParagraphProperty()
		{
			pageBreakBefore		=	false;
		}

		ParagraphProperty::~ParagraphProperty()
		{

		}	

		ParagraphProperty::ParagraphProperty(const XML::XNode& node)
		{
			fromXML(node);
		}

		const ParagraphProperty& ParagraphProperty::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void ParagraphProperty::fromXML(const XML::XNode& node)
		{
			pageBreakBefore			=	false;

			const XML::XElement element(node);

			PStyle					=	element.element("pStyle").attribute("val").value();
			RunProperty				=	element.element("rPr");
			Align					=	element.element("jc").attribute("val").value();
			//Background			=	element.element("shd").attribute("fill").value();
			Shading					=	element.element("shd");
			TextFrameProperties		=	element.element("framePr");
			OutlineLvl				=	element.element("outlineLvl").attribute("val").value();
			TextAlignment			=	element.element("textAlignment ").attribute("val").value();
			Ind						=	element.element("ind");
			Spacing					=	element.element("spacing");
			NumPr					=	element.element("numPr");
			ParagraphBorder			=	element.element("pBdr");
			SectorProperty			=	element.element("sectPr");
			Tabs					=	element.element("tabs");
			KeepNext				=	element.element("keepNext").exist();
			KeepLines				=	element.element("keepLines").exist();

			if (element.element("pageBreakBefore").exist())
			{
				if (element.element("pageBreakBefore").attribute("val").exist())
				{
					pageBreakBefore	=	element.element("pageBreakBefore").attribute("val").value();
				}
			}

			ContextualSpacing		=	element.element("contextualSpacing").exist();
			SuppressLineNumbers		=	element.element("suppressLineNumbers").exist();
			WidowControl			=	element.element("widowControl").attribute("val").value();
			PropertyChange			=	element.element("pPrChange");
		}

		const XML::XNode ParagraphProperty::toXML() const
		{
			return	XML::XElement(ns.w + "pPr", 
				XML::Write(RunProperty) +
				XML::Write(SectorProperty) +
				XML::Write(ns.w + "pStyle", ns.w + "val", PStyle) +
				XML::WriteIf(XML::XElement(ns.w + "keepNext"), KeepNext) +
				XML::WriteIf(XML::XElement(ns.w + "keepLines"), KeepLines) +
				XML::WriteIf(XML::XElement(ns.w + "pageBreakBefore"), pageBreakBefore) +
				XML::Write(TextFrameProperties) +
				XML::Write(ns.w + "widowControl", ns.w + "val", WidowControl) +
				XML::Write(NumPr) +
				XML::Write(Spacing) +
				XML::WriteIf(XML::XElement(ns.w + "suppressLineNumbers"), SuppressLineNumbers) +
				XML::Write(ParagraphBorder) +
				//XML::Write(ns.w + "shd", ns.w + "fill", Background) +
				XML::Write(Shading) +
				XML::Write(Tabs) +
				XML::Write(Ind) +
				XML::WriteIf(XML::XElement(ns.w + "contextualSpacing"), ContextualSpacing) +
				XML::Write(ns.w + "jc", ns.w + "val", Align) +
				XML::Write(ns.w + "textAlignment", ns.w + "val", TextAlignment) +
				XML::Write(ns.w + "outlineLvl", ns.w + "val", OutlineLvl) +
				XML::Write(PropertyChange));
		}

		const bool ParagraphProperty::isSimple() const
		{
			if (PStyle.is_init())
				return false;
			if (Align.is_init())
				return false;
			// if (Background.is_init())	return false;
			if (Shading.is_init())
				return false;
			if (TextFrameProperties.is_init())
				return false;
			if (NumPr.is_init())
				return false;
			if (Ind.is_init())
				return false;
			if (Spacing.is_init())
				return false;
			if(*KeepLines)
				return false;
			if(*KeepNext)
				return false;
			if(*SuppressLineNumbers)
				return false;
			if (!RunProperty.is_init())
				return true;
			if (PropertyChange.is_init())
				return false;
			return RunProperty->isSimple();
		}
	} // namespace Logic
} // namespace OOX