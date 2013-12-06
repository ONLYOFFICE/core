
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../Styles.h"

namespace OOX
{
	Styles::Style::Style()
	{

	}

	Styles::Style::~Style()
	{

	}	

	Styles::Style::Style(const XML::XNode& node)
	{
		fromXML(node);
	}

	const Styles::Style& Styles::Style::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}

	void Styles::Style::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);

		Default				=	element.attribute("default").value();
		CustomStyle			=	element.attribute("customStyle").value();				
		tblPr				=	element.element("tblPr");
		tcPr				=	element.element("tcPr");
		tableStyles			=	element;
		Link				=	element.element("link").attribute("val").value();
		UiPriority			=	element.element("uiPriority").attribute("val").value();		
		Type				=	element.attribute("type").value();
		StyleId				=	element.attribute("styleId").value();
		BasedOn				=	element.element("basedOn").attribute("val").value();
		Next				=	element.element("next").attribute("val").value();
		ParagraphProperty	=	element.element("pPr");
		RunProperty			=	element.element("rPr");
		name				=	element.element("name").attribute("val").value();
		QFormat				=	element.element("qFormat").exist();
		AutoRedefine		=	element.element("autoRedefine").exist();
		SemiHidden			=	element.element("semiHidden").exist();
		UnhideWhenUsed		=	element.element("unhideWhenUsed").exist();		
	}

	const XML::XNode Styles::Style::toXML() const
	{
		return XML::XElement();
	}

} // namespace OOX