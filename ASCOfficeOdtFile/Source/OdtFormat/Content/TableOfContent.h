#pragma once
#ifndef ODT_CONTENT_TABLE_OF_CONTENT_INCLUDE_H_
#define ODT_CONTENT_TABLE_OF_CONTENT_INCLUDE_H_

#include "TextItem.h"
#include "property.h"
#include "TableOfContentSource.h"
#include "IndexBody.h"


namespace Odt
{
	namespace Content
	{
		class TableOfContent : public TextItem
		{
		public:
			TableOfContent();
			virtual ~TableOfContent();
			explicit TableOfContent(const XML::XNode& node);
			const TableOfContent& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			virtual const bool hasList() const;
			virtual const bool hasNote() const;		

		public:
			property<std::string>						StyleName;
			property<bool>									Protected;
			property<std::string>						Name;
			property<TableOfContentSource>  TableOfContentSource;
			property<IndexBody>							IndexBody;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TABLE_OF_CONTENT_INCLUDE_H_