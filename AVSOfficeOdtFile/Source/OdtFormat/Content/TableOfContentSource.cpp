
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TableOfContentSource.h"


namespace Odt
{
	namespace Content
	{

		TableOfContentSource::TableOfContentSource()
		{
		}


		TableOfContentSource::~TableOfContentSource()
		{
		}


		TableOfContentSource::TableOfContentSource(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableOfContentSource& TableOfContentSource::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableOfContentSource::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			OutlineLevel			 = element.attribute("outline-level").value();
			IndexTitleTemplate = element.element("index-title-template");
			XML::Fill(TableOfContentEntryTemplates, element, "table-of-content-entry-template");
		}


		const XML::XNode TableOfContentSource::toXML() const
		{
			return 
				XML::XElement(ns.text + "table-of-content-source",
					XML::XAttribute(ns.text + "outline-level", OutlineLevel) +
					XML::Write(IndexTitleTemplate) +
					XML::Write(TableOfContentEntryTemplates)
				);
		}	

	} // namespace Content
} // namespace Odt