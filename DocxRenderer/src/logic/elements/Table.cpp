#include "Table.h"

#include "../../resources/SingletonTemplate.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
	CTable::CCell::CCell(const CCell& other)
	{
		*this = other;
	}
	void CTable::CCell::Clear()
	{
		m_arParagraphs.clear();
	}
	void CTable::CCell::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{
		oWriter.WriteString(L"<w:tc>");
		oWriter.WriteString(L"<w:tcPr>");
		oWriter.WriteString(L"<w:tcW w:w=\"");
		oWriter.AddUInt(static_cast<unsigned int>(m_dWidth * c_dMMToDx));
		oWriter.WriteString(L"\" w:type=\"dxa\"/>");

		oWriter.WriteString(L"<w:gridSpan w:val=\"");
		oWriter.AddUInt(m_nGridSpan);
		oWriter.WriteString(L"\"/>");

		oWriter.WriteString(L"<w:vMerge w:val=\"");
		switch (m_eVMerge) {
		case eVMerge::vmContinue:
			oWriter.WriteString(L"continue");
			break;
		case eVMerge::vmRestart:
			oWriter.WriteString(L"restart");
			break;
		default:
			break;
		}
		oWriter.WriteString(L"\"/>");

		oWriter.WriteString(L"<w:tcBorders>");
		auto write_border = [&oWriter] (const CBorder& border, const std::wstring& prefix) {
			oWriter.WriteString(L"<w:");
			oWriter.WriteString(prefix);
			oWriter.WriteString(L" w:val=");
			oWriter.WriteString(SingletonInstance<LinesTable>().ConvertLineToString(border.lineType));
			oWriter.WriteString(L" w:sz=\"");
			oWriter.AddUInt(static_cast<unsigned int>(border.dWidth * c_dMMToPt * 8));
			oWriter.WriteString(L"\" w:space=\"");
			oWriter.AddUInt(static_cast<unsigned int>(border.dSpacing * c_dMMToPt));
			oWriter.WriteString(L"\" w:color=\"");
			oWriter.WriteHexInt3(ConvertColorBGRToRGB(border.lColor));
			oWriter.WriteString(L"\" />");
		};

		write_border(m_oBorderTop, L"top");
		write_border(m_oBorderBot, L"bottom");
		write_border(m_oBorderLeft, L"left");
		write_border(m_oBorderRight, L"right");

		oWriter.WriteString(L"</w:tcBorders>");
		oWriter.WriteString(L"</w:tcPr>");

		for (const auto& p : m_arParagraphs)
			p->ToXml(oWriter);

		oWriter.WriteString(L"</w:tc>");
	}
	void CTable::CCell::ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const
	{

	}
	CTable::CCell& CTable::CCell::operator=(const CCell& other)
	{
		CBaseItem::operator=(other);

		m_oBorderBot = other.m_oBorderBot;
		m_oBorderTop = other.m_oBorderTop;
		m_oBorderLeft = other.m_oBorderLeft;
		m_oBorderRight = other.m_oBorderRight;

		m_nGridSpan = other.m_nGridSpan;
		m_eVMerge = other.m_eVMerge;

		m_arParagraphs.clear();
		for (const auto& p : other.m_arParagraphs)
			m_arParagraphs.push_back(p);

		return *this;
	}
	void CTable::CCell::AddParagraph(const paragraph_ptr_t& pParagraph)
	{
		m_arParagraphs.push_back(pParagraph);
	}

	void CTable::CRow::Clear()
	{
		m_arCells.clear();
	}
	void CTable::CRow::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{
		oWriter.WriteString(L"<w:tr>");

		oWriter.WriteString(L"<w:trPr>");
		oWriter.WriteString(L"<w:trHeight w:val=\"");
		oWriter.AddUInt(static_cast<unsigned int>(m_dHeight * c_dMMToDx));
		oWriter.WriteString(L"\" w:hRule=\"exact\"/>");
		oWriter.WriteString(L"</w:trPr>");

		for (const auto& c : m_arCells)
			c->ToXml(oWriter);

		oWriter.WriteString(L"</w:tr>");
	}
	void CTable::CRow::ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const
	{

	}
	void CTable::CRow::AddCell(const cell_ptr_t& pCell)
	{
		CBaseItem::RecalcWithNewItem(pCell.get());
		m_arCells.push_back(pCell);
	}
	bool CTable::CRow::IsEmpty() const
	{
		return m_arCells.empty();
	}

	void CTable::Clear()
	{
		m_arRows.clear();
	}
	void CTable::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{
		oWriter.WriteString(L"<w:tbl>");

		oWriter.WriteString(L"<w:tblPr>");
		oWriter.WriteString(L"<w:tblpPr ");

		oWriter.WriteString(L"w:horzAnchor=\"page\" w:vertAnchor=\"page\" w:tblpX=\"");
		oWriter.AddInt64(static_cast<long long>(m_dLeft * c_dMMToDx));
		oWriter.WriteString(L"\" w:tblpY=\"");
		oWriter.AddInt64(static_cast<long long>(m_dTop * c_dMMToDx));
		oWriter.WriteString(L"\" />");

		oWriter.WriteString(L"<w:tblW w:w=\"");
		oWriter.AddUInt(static_cast<unsigned int>(m_dWidth * c_dMMToDx));
		oWriter.WriteString(L"\" w:type=\"dxa\"/>");
		oWriter.WriteString(L"</w:tblPr>");

		oWriter.WriteString(L"<w:tblGrid>");
		for (const auto& gc : m_arGridCols)
		{
			oWriter.WriteString(L"<w:gridCol w:w=\"");
			oWriter.AddUInt(static_cast<unsigned int>(gc * c_dMMToDx));
			oWriter.WriteString(L"\" />");
		}
		oWriter.WriteString(L"</w:tblGrid>");

		for (const auto& r : m_arRows)
			r->ToXml(oWriter);

		oWriter.WriteString(L"</w:tbl>");
	}
	void CTable::ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const
	{

	}
	void CTable::AddRow(const row_ptr_t& pRow)
	{
		CBaseItem::RecalcWithNewItem(pRow.get());
		m_arRows.push_back(pRow);
	}
	void CTable::CalcGridCols()
	{
		std::vector<double> cells_left;
		auto add_if_no_exists = [&cells_left] (double val) {
			bool exists = false;
			for (const auto& curr : cells_left)
			{
				if (fabs(curr - val) < c_dMAX_TABLE_LINE_WIDTH)
				{
					exists = true;
					break;
				}
			}
			if (!exists)
				cells_left.push_back(val);
		};

		double right = 0;
		for (const auto& row : m_arRows)
		{
			for (const auto& cell : row->m_arCells)
			{
				right = std::max(right, cell->m_dRight);
				add_if_no_exists(cell->m_dLeft);
			}
		}
		std::sort(cells_left.begin(), cells_left.end(), std::less<double>{});

		for (size_t i = 0; i < cells_left.size() - 1; ++i)
			m_arGridCols.push_back(cells_left[i + 1] - cells_left[i]);

		m_arGridCols.push_back(right - cells_left.back());
	}
	bool CTable::IsEmpty() const
	{
		return m_arRows.empty();
	}

	void CTextCell::AddTextLine(const std::shared_ptr<CTextLine>& pTextLine)
	{
		CBaseItem::RecalcWithNewItem(pTextLine.get());
		m_arTextLines.push_back(pTextLine);
	}

} // namespace NSDocxRenderer
