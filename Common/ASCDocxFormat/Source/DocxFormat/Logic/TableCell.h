#pragma once
#ifndef OOX_LOGIC_TABLE_CELL_INCLUDE_H_
#define OOX_LOGIC_TABLE_CELL_INCLUDE_H_

#include "./../WritingElement.h"
#include "TableCellProperties.h"
#include "../Logic/Paragraph.h"
#include "property.h"
#include <vector>
#include "TextItem.h"


namespace OOX
{
	namespace Logic
	{
		class TableCell : public WritingElement
		{
		public:
			TableCell();
			virtual ~TableCell();
			explicit TableCell(const XML::XNode& node);
			const TableCell& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool isFromHMerged() const;
			const bool isFromVMergedButNotFirst() const;			
			const bool isFirstFromVMerged() const;

		public:
			property<int>		NumCol;

		public:
			property<TableCellProperties>		Properties;
			property<std::vector<TextItem> >	Items;

		private:
			const bool hasParagraph() const;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_CELL_INCLUDE_H_