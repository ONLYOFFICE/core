#pragma once
#ifndef ODT_CONTENT_TABLE_INCLUDE_H_
#define ODT_CONTENT_TABLE_INCLUDE_H_

#include "TextItem.h"
#include <string>
#include <vector>
#include "property.h"
#include "nullable_property.h"
#include "TableColumn.h"
#include "TableRow.h"
#include "TableHeader.h"


namespace Odt
{
	namespace Content
	{
		class Table : public TextItem
		{
		public:
			Table();
			virtual ~Table();
			explicit Table(const XML::XNode& node);
			const Table& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			virtual const bool hasNote() const;
			virtual const bool hasList() const;

		public:
			nullable_property<std::string>			Name;
			nullable_property<std::string>			Style;
			nullable_property<std::string>			TemplateName;
			nullable_property<bool>							UseBandingColumnsStyles;
			nullable_property<bool>							UseBandingRowsStyles;
			nullable_property<bool>							UseFirstColumnStyles;
			nullable_property<bool>							UseFirstRowStyles;
			nullable_property<bool>							UseLastColumnStyles;
			nullable_property<bool>							UseLastRowStyles;
			nullable_property<bool>							IsSubTable;
			property<std::vector<TableColumn> >	Columns;
			property<std::vector<TableRow> >	Rows;
			nullable_property<TableHeader>		Header;

		private:
			void SetCountsHMerged();

		private:
			bool m_hasNote;
			bool m_hasList;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TABLE_INCLUDE_H_