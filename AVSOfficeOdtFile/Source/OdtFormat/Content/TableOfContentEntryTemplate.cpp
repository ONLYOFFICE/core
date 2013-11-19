
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TableOfContentEntryTemplate.h"


namespace Odt
{
	namespace Content
	{

		TableOfContentEntryTemplate::TableOfContentEntryTemplate()
		{
		}


		TableOfContentEntryTemplate::~TableOfContentEntryTemplate()
		{
		}


		TableOfContentEntryTemplate::TableOfContentEntryTemplate(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableOfContentEntryTemplate& TableOfContentEntryTemplate::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableOfContentEntryTemplate::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			IndexEntryChapter			= element.element("index-entry-chapter").exist();
			IndexEntryText				= element.element("index-entry-text").exist();
			IndexEntryPageNumber  = element.element("index-entry-page-number").exist();
			IndexEntryTabStop			= element.element("index-entry-tab-stop");
		}


		const XML::XNode TableOfContentEntryTemplate::toXML() const
		{
			return 
				XML::XElement(ns.text + "table-of-content-entry-template",
					XML::WriteIf(XML::XElement(ns.text + "index-entry-chapter"), IndexEntryChapter) +
					XML::WriteIf(XML::XElement(ns.text + "index-entry-text"), IndexEntryText) +
					XML::WriteIf(XML::XElement(ns.text + "index-entry-page-number"), IndexEntryPageNumber) +
					XML::Write(IndexEntryTabStop)
				);
		}	

	} // namespace Content
} // namespace Odt