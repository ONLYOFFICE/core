#pragma once
#ifndef ODT_CONTENT_TABLE_HEADER_INCLUDE_H_
#define ODT_CONTENT_TABLE_HEADER_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "TableRow.h"
#include <vector>


namespace Odt
{
	namespace Content
	{
		class TableHeader : public WritingElement
		{
		public:
			TableHeader();
			virtual ~TableHeader();
			explicit TableHeader(const XML::XNode& node);
			const TableHeader& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool hasNote() const;
			const bool hasList() const;

		public:
			property<std::vector<TableRow> >		Rows;

		private:
			bool m_hasNote;
			bool m_hasList;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TABLE_HEADER_INCLUDE_H_