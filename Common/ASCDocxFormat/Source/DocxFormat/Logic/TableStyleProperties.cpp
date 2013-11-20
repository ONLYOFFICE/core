
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableStyleProperties.h"

namespace OOX
{
	namespace Logic
	{
		TableStyleProperties::TableStyleProperties()
		{

		}

		TableStyleProperties::~TableStyleProperties()
		{

		}	

		TableStyleProperties::TableStyleProperties(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableStyleProperties& TableStyleProperties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableStyleProperties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			
			Type				=	element.attribute("type").value();
			ParagraphProperty	=	element.element("pPr");
			RunProperty			=	element.element("rPr");
			TableProperty		=	element.element("tblPr");
			TableRowProperties	=	element.element("trPr");
			tcPr				=	element.element("tcPr");
		}

		const XML::XNode TableStyleProperties::toXML() const
		{
			return XML::XElement(ns.w + "tblStylePr",
				XML::XAttribute(ns.w + "type", Type) +
				XML::Write(ParagraphProperty) +
				XML::Write(RunProperty) +
				XML::Write(TableProperty) +
				XML::Write(TableRowProperties) +
				XML::Write(tcPr));
		}

	} // namespace Logic
} // namespace OOX