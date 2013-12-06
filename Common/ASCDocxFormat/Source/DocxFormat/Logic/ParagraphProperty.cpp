
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
		return XML::XElement();
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