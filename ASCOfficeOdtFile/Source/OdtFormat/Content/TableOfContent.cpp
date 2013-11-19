
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TableOfContent.h"


namespace Odt
{
	namespace Content
	{

		TableOfContent::TableOfContent()
		{
		}
		

		TableOfContent::~TableOfContent()
		{
		}


		TableOfContent::TableOfContent(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableOfContent& TableOfContent::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableOfContent::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			StyleName						 = element.attribute("style-name").value();
			Protected						 = element.attribute("protected").value();
			Name								 = element.attribute("name").value();
			TableOfContentSource = element.element("table-of-content-source");
			IndexBody						 = element.element("index-body");
		}


		const XML::XNode TableOfContent::toXML() const
		{
			return 
				 XML::XElement(ns.text + "table-of-content",
					XML::XAttribute(ns.text + "style-name", StyleName) +
					XML::XAttribute(ns.text + "protected", Protected) + 
					XML::XAttribute(ns.text + "name", Name) +
					XML::Write(TableOfContentSource) +
					XML::Write(IndexBody)
				);
		}


		const bool TableOfContent::hasList() const
		{
			return false;
		}


		const bool TableOfContent::hasNote() const
		{
			return false;
		}

	} // namespace Content
} // namespace Odt