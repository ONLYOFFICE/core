
// auto inserted precompiled begin
#include "precompiled_docx2odt.h"
// auto inserted precompiled end

#include "Table.h"
#include <vector>
#include "TextItem.h"
#include "NamedStyle.h"


namespace Docx2Odt
{
	namespace Private
	{

		Table::Table(Odt::Folder& odtFolder, const OOX::IFileContainer& docxFileContainer)
			: Base(odtFolder, docxFileContainer)
		{
		}


		const Odt::Content::Table Table::table2table(const OOX::Logic::Table& ooxTable)
		{
			if(ooxTable.tblPr->Style.is_init())
			{
				boost::shared_ptr<OOX::Styles> pStyle = boost::shared_dynamic_cast<OOX::Styles, OOX::File>(m_docxFileContainer.get(OOX::FileTypes::Style));
				if (pStyle)
				{
					m_tableStyle = pStyle->GetStyleById(*ooxTable.tblPr->Style);
				}
			}
			m_Look = HexString2Int(ooxTable.tblPr->Look.get_value_or("0000"));
			Odt::Content::Table odtTable;
			
			NamedStyle styleConverter(m_odtFolder, m_docxFileContainer);
			Odt::Logic::NamedStyle odtStyle = styleConverter.table2style(ooxTable);
			if (converterProperties.getDestStyleIsContent())
				styleConverter.addStyle2Content(odtStyle);
			else 
				styleConverter.addStyle2Styles(odtStyle);
			odtTable.Style = odtStyle.Name;
			odtTable.Name	 = odtStyle.Name;

			if((m_tableStyle.is_init()) && (m_tableStyle->tblPr.is_init()))
				m_tableBorders = m_tableStyle->tblPr->tblBorders.get_value_or(OOX::Logic::TableBorders());
			else
				m_tableBorders = ooxTable.tblPr->tblBorders.get_value_or(OOX::Logic::TableBorders());

			if (ooxTable.tblPr->Style.is_init())
				fullTableBorders();
			m_countColumns = ooxTable.tblGrid->Columns->size();
			m_countRows		 = ooxTable.Rows->size();

			convert(*odtTable.Columns, ooxTable.tblGrid->Columns);
			convert(*odtTable.Rows, ooxTable.Rows);

			return odtTable;
		}


		void Table::convert(std::vector<Odt::Content::TableColumn>& odt, const std::vector<OOX::Logic::GridColumn>& oox)
		{
			BOOST_FOREACH(const OOX::Logic::GridColumn& column, oox)
			{
				odt.push_back(column2column(column));
			}
		}


		void Table::convert(std::vector<Odt::Content::TableRow>& odt, const std::vector<OOX::Logic::TableRow>& oox)
		{		
			m_currentRow = 0;
			BOOST_FOREACH(const OOX::Logic::TableRow& row, oox)
			{
				++m_currentRow;
				odt.push_back(row2row(row));
			}
		}


		const Odt::Content::TableColumn Table::column2column(const OOX::Logic::GridColumn& ooxColumn)
		{			
			Odt::Content::TableColumn odtColumn;

			NamedStyle styleConverter(m_odtFolder, m_docxFileContainer);
			Odt::Logic::NamedStyle odtStyle = styleConverter.tableColumnProperty2style(ooxColumn);
			if (converterProperties.getDestStyleIsContent())
				styleConverter.addStyle2Content(odtStyle);
			else 
				styleConverter.addStyle2Styles(odtStyle);
			odtColumn.Style = odtStyle.Name;		

			return odtColumn;
		}


		const Odt::Content::TableRow Table::row2row(const OOX::Logic::TableRow& ooxRow)
		{
			m_currentColumn = 0;
			Odt::Content::TableRow odtRow;

			if (ooxRow.Properties.is_init())
			{
				NamedStyle styleConverter(m_odtFolder, m_docxFileContainer);
				Odt::Logic::NamedStyle odtStyle = styleConverter.tableRowProperty2style(ooxRow.Properties);
				if (converterProperties.getDestStyleIsContent())
					styleConverter.addStyle2Content(odtStyle);
				else 
					styleConverter.addStyle2Styles(odtStyle);
				odtRow.StyleName = odtStyle.Name;
			}

			BOOST_FOREACH(const OOX::Logic::TableCell& cell, *ooxRow.Cells)
			{
				++m_currentColumn;
				odtRow.Cells->push_back(cell2cell(cell));
			}
			return odtRow;
		}


		const Odt::Content::TableCellItem Table::cell2cell(const OOX::Logic::TableCell& ooxCell)
		{
			if(!ooxCell.isFromVMergedButNotFirst())					
				return cell2tableCell(ooxCell);
			else
				return cell2coveredTableCell(ooxCell);
		}


		const Odt::Content::TableCell Table::cell2tableCell(const OOX::Logic::TableCell& ooxCell)
		{
			Odt::Content::TableCell odtCell;
			//OOX::Theme::File theme;
			//if (m_docxFileContainer.exist<OOX::Theme::File>())
			//	theme = m_docxFileContainer.find<OOX::Theme::File>();


			OOX::Logic::TableCellProperties tableCellPr;// = ooxCell.Properties;
			OOX::Logic::RunProperty runProperty;

			int shiftVert = 0;
			int shiftHorz = 0;
			int horzStart = 1;
			int vertStart = 1;
			int horzEnd = m_countColumns;
			int vertEnd = m_countRows;
			if(m_tableStyle.is_init())
			{
				if(m_tableStyle->tcPr.is_init())
					tableCellPr = *m_tableStyle->tcPr;
				runProperty = m_tableStyle->RunProperty.get_value_or_default();

				if((m_Look & 0x0080) && (m_tableStyle->tableStyles->exist("firstCol")))
				{
					shiftHorz = 1;
					horzStart++;
				}
				if((m_Look & 0x0020) && (m_tableStyle->tableStyles->exist("firstRow")))
				{
					shiftVert = 1;
					vertStart++;
				}
				if((m_Look & 0x0100) && (m_tableStyle->tableStyles->exist("lastCol")))
					horzEnd--;
				if((m_Look & 0x0040) && (m_tableStyle->tableStyles->exist("lastRow")))
					vertEnd--;

				if(((~m_Look) & 0x0200) && (m_tableStyle->tableStyles->exist("band1Horz")) && ((m_currentRow + shiftVert) % 2 == 1)
					&& (m_currentRow >= vertStart) && (m_currentRow <= vertEnd))
				{
					tableCellPr = m_tableStyle->tableStyles->find("band1Horz").tcPr.get_value_or(tableCellPr);
					runProperty = m_tableStyle->tableStyles->find("band1Horz").RunProperty.get_value_or(runProperty);
				}
				if(((~m_Look) & 0x0400) && (m_tableStyle->tableStyles->exist("band1Vert")) && ((m_currentColumn + shiftHorz) % 2 == 1)
					&& (m_currentColumn >= horzStart) && (m_currentColumn <= horzEnd))
				{
					tableCellPr = m_tableStyle->tableStyles->find("band1Vert").tcPr.get_value_or(tableCellPr);
					runProperty = m_tableStyle->tableStyles->find("band1Vert").RunProperty.get_value_or(runProperty);
				}

				if((m_Look & 0x0080) && (m_tableStyle->tableStyles->exist("firstCol")) && (m_currentColumn == 1))
				{
					tableCellPr = m_tableStyle->tableStyles->find("firstCol").tcPr.get_value_or(tableCellPr);
					runProperty = m_tableStyle->tableStyles->find("firstCol").RunProperty.get_value_or(runProperty);
				}
				else if((m_Look & 0x0100) && (m_tableStyle->tableStyles->exist("lastCol")) && (m_currentColumn == m_countColumns))
				{
					tableCellPr = m_tableStyle->tableStyles->find("lastCol").tcPr.get_value_or(tableCellPr);
					runProperty = m_tableStyle->tableStyles->find("lastCol").RunProperty.get_value_or(runProperty);
				}

				if((m_Look & 0x0020) && (m_tableStyle->tableStyles->exist("firstRow")) && (m_currentRow == 1))
				{
					tableCellPr = m_tableStyle->tableStyles->find("firstRow").tcPr.get_value_or(tableCellPr);
					runProperty = m_tableStyle->tableStyles->find("firstRow").RunProperty.get_value_or(runProperty);
				}
				else if((m_Look & 0x0040) && (m_tableStyle->tableStyles->exist("lastRow")) && (m_currentRow == m_countRows))
				{
					tableCellPr = m_tableStyle->tableStyles->find("lastRow").tcPr.get_value_or(tableCellPr);
					runProperty = m_tableStyle->tableStyles->find("lastRow").RunProperty.get_value_or(runProperty);
				}
			}
			if(ooxCell.Properties->Shading.is_init())
				tableCellPr = ooxCell.Properties;

			tableCellPr.tblBorders = pickBorders(tableCellPr);//собираем информацию о borders со всех уровней
					
			NamedStyle styleConverter(m_odtFolder, m_docxFileContainer);
			Odt::Logic::NamedStyle odtStyle = styleConverter.tableCellProperty2style(tableCellPr);
			odtStyle.Properties->Text = styleConverter.rpr2tpr(runProperty/*, theme*/);
			if (converterProperties.getDestStyleIsContent())
				styleConverter.addStyle2Content(odtStyle);
			else 
				styleConverter.addStyle2Styles(odtStyle);
			odtCell.Style = odtStyle.Name;		

			TextItem converter(m_odtFolder, m_docxFileContainer);
			converter.converterProperties = converterProperties;
			converter.convert(ooxCell.Items, *odtCell.Texts);
			
			if (ooxCell.isFromHMerged())
				odtCell.ColumnsSpanned = ooxCell.Properties->GridSpan;

			if (ooxCell.isFirstFromVMerged())
				odtCell.RowsSpanned = ooxCell.Properties->CountVMerge;
			
			return odtCell;
		}


		const Odt::Content::CoveredTableCell Table::cell2coveredTableCell(const OOX::Logic::TableCell& ooxCell)
		{
			Odt::Content::CoveredTableCell odtCoveredCell;	
			if (ooxCell.isFromHMerged())
				odtCoveredCell.CountHMerge = ooxCell.Properties->GridSpan;
			return odtCoveredCell;
		}		


		void Table::fullTableBorders()
		{
			OOX::Logic::BorderItem border;
			border.Value = "single";
			border.Color = "000000";
			border.Sz  = 4;
			border.Space = 0;			
			if (!m_tableBorders.top.is_init())
				m_tableBorders.top = OOX::Logic::Border("top", border);
			if (!m_tableBorders.bottom.is_init())
				m_tableBorders.bottom = OOX::Logic::Border("bottom", border);
			if (!m_tableBorders.left.is_init())
				m_tableBorders.left = OOX::Logic::Border("left", border);
			if (!m_tableBorders.right.is_init())
				m_tableBorders.right = OOX::Logic::Border("right", border);
			if (!m_tableBorders.insideH.is_init())
				m_tableBorders.insideH = OOX::Logic::Border("insideH", border);
			if (!m_tableBorders.insideV.is_init())
				m_tableBorders.insideV = OOX::Logic::Border("insideV", border);
		}


		const OOX::Logic::TableCellBorders Table::pickBorders(const OOX::Logic::TableCellProperties& ooxPr)
		{
			OOX::Logic::TableCellBorders borders = ooxPr.tblBorders.get_value_or(OOX::Logic::TableCellBorders());

			if (!borders.top.is_init())
            {
                if (m_currentRow == 1 && m_tableBorders.top.is_init())
				    borders.top = OOX::Logic::Border("top", m_tableBorders.top->Bdr);
                else if (m_currentRow != 1 && m_tableBorders.insideH.is_init())
                    borders.top = OOX::Logic::Border("top", m_tableBorders.insideH->Bdr);
            }

			if (!borders.bottom.is_init())
            {
                if (m_currentRow == m_countRows && m_tableBorders.bottom.is_init())
				    borders.bottom = OOX::Logic::Border("bottom", m_tableBorders.bottom->Bdr);
                else if (m_currentRow != m_countRows && m_tableBorders.insideH.is_init())
                    borders.bottom = OOX::Logic::Border("bottom", m_tableBorders.insideH->Bdr);
            }

			if (!borders.right.is_init())
            {
                if (m_currentColumn == m_countColumns && m_tableBorders.right.is_init())
				    borders.right = OOX::Logic::Border("right", m_tableBorders.right->Bdr);
                else if (m_currentColumn != m_countColumns && m_tableBorders.insideV.is_init() )
                    borders.right = OOX::Logic::Border("right", m_tableBorders.insideV->Bdr);
            }
			
            if (!borders.left.is_init())
            {
                if (m_currentColumn == 1 && m_tableBorders.left.is_init())
				    borders.left = OOX::Logic::Border("left", m_tableBorders.left->Bdr);
                else if (m_currentColumn != 1 && m_tableBorders.insideV.is_init())
                    borders.left = OOX::Logic::Border("left", m_tableBorders.insideV->Bdr);
            }
			return borders;
		}

	} // namespace Private
} // namespace Docx2Odt