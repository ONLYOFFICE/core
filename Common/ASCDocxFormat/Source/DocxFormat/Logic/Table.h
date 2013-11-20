#pragma once
#ifndef OOX_LOGIC_TABLE_INCLUDE_H_
#define OOX_LOGIC_TABLE_INCLUDE_H_

#include "TextItemBase.h"
#include "TableProperty.h"
#include "TableGrid.h"
#include "TableRow.h"

namespace OOX
{
	namespace Logic
	{
		class Table : public TextItemBase
		{
		public:
			Table();
			virtual ~Table();
			explicit Table(const XML::XNode& node);
			const Table& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		private:
			void SetCountsVMerged();

		public:

			property<TableProperty>				tblPr;
			property<TableGrid>					tblGrid;
			
			property<std::vector<TableRow> >	Rows;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_INCLUDE_H_