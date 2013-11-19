#pragma once
#ifndef DOCX_2_ODT_PRIVATE_TABLE_INCLUDE_H_
#define DOCX_2_ODT_PRIVATE_TABLE_INCLUDE_H_

#include "Base.h"
#include "ConverterProperties.h"
#include "nullable.h"
#include "nullable_property.h"


namespace Docx2Odt
{
	namespace Private
	{
		class Table : public Base
		{
		public:
			Table(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer);

		public:
			const Odt::Content::Table table2table(const OOX::Logic::Table& ooxTable);

		private:
			void Table::convert(std::vector<Odt::Content::TableColumn>& odt, const std::vector<OOX::Logic::GridColumn>& oox);
			void Table::convert(std::vector<Odt::Content::TableRow>& odt, const std::vector<OOX::Logic::TableRow>& oox);

		private:
			const Odt::Content::TableColumn column2column(const OOX::Logic::GridColumn& ooxColumn);
			const Odt::Content::TableRow row2row(const OOX::Logic::TableRow& ooxRow);
			const Odt::Content::TableCellItem cell2cell(const OOX::Logic::TableCell& ooxCell);
			const Odt::Content::TableCell cell2tableCell(const OOX::Logic::TableCell& ooxCell);
			const Odt::Content::CoveredTableCell cell2coveredTableCell(const OOX::Logic::TableCell& ooxCell);			

		private:
			void fullTableBorders();
			const OOX::Logic::TableCellBorders pickBorders(const OOX::Logic::TableCellProperties& ooxPr);
			
		public:
			ConverterProperties converterProperties;
					
		private:
			int m_currentColumn;
			int m_currentRow;
			int m_countColumns;
			int m_countRows;
			OOX::Logic::TableBorders m_tableBorders;
			nullable<OOX::Styles::Style> m_tableStyle;
			int m_Look;
		};
	} // namespace Private
} // namespace Docx2Odt

#endif // DOCX_2_ODT_PRIVATE_TABLE_INCLUDE_H_