#pragma once
#ifndef ODT_CONTENT_TABLE_OF_CONTENT_SOURCE_INCLUDE_H_
#define ODT_CONTENT_TABLE_OF_CONTENT_SOURCE_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include <vector>
#include "TableOfContentEntryTemplate.h"
#include "IndexTitleTemplate.h"


namespace Odt
{
	namespace Content
	{
		class TableOfContentSource : public WritingElement
		{
		public:
			TableOfContentSource();
			virtual ~TableOfContentSource();
			explicit TableOfContentSource(const XML::XNode& node);
			const TableOfContentSource& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<int>	OutlineLevel;
			property<IndexTitleTemplate> IndexTitleTemplate;
			property<std::vector<TableOfContentEntryTemplate> > TableOfContentEntryTemplates;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TABLE_OF_CONTENT_SOURCE_INCLUDE_H_