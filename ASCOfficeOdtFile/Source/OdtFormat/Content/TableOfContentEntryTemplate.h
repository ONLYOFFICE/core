#pragma once
#ifndef ODT_CONTENT_TABLE_OF_CONTENT_ENTRY_TEMPLATE_INCLUDE_H_
#define ODT_CONTENT_TABLE_OF_CONTENT_ENTRY_TEMPLATE_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "IndexEntryTabStop.h"


namespace Odt
{
	namespace Content
	{
		class TableOfContentEntryTemplate : public WritingElement
		{
		public:
			TableOfContentEntryTemplate();
			virtual ~TableOfContentEntryTemplate();
			explicit TableOfContentEntryTemplate(const XML::XNode& node);
			const TableOfContentEntryTemplate& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<bool>							IndexEntryChapter;
			property<bool>							IndexEntryText;
			property<bool>							IndexEntryPageNumber;
			property<IndexEntryTabStop> IndexEntryTabStop;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TABLE_OF_CONTENT_ENTRY_TEMPLATE_INCLUDE_H_