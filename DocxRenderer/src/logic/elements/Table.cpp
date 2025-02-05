#include "Table.h"

#include "../../resources/SingletonTemplate.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
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
		oWriter.WriteString(L"\"w:type=\"dxa\"/>");

		oWriter.WriteString(L"<w:tcBorders>");

		auto write_border = [&oWriter] (const CBorder& border, const std::wstring& prefix) {
			oWriter.WriteString(L"<w:");
			oWriter.WriteString(prefix);
			oWriter.WriteString(L" w:val=");
			oWriter.WriteString(SingletonInstance<LinesTable>().ConvertLineToString(border.lineType));
			oWriter.WriteString(L" w:sz=\"");
			oWriter.AddUInt(static_cast<unsigned int>(border.dWidth * c_dMMToPt / 8));
			oWriter.WriteString(L"\" w:space=\"");
			oWriter.AddUInt(static_cast<unsigned int>(border.dSpacing * c_dMMToPt));
			oWriter.WriteString(L"\" w:color=\"");
			oWriter.WriteHexInt3(ConvertColorBGRToRGB(border.lColor));
			oWriter.WriteString(L"\" />");
		};

		write_border(m_oBorderTop, L"top");
		write_border(m_oBorderBot, L"bot");
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
		oWriter.WriteString(L"<w:tblW w:w=\"");
		oWriter.AddUInt(static_cast<unsigned int>(m_dWidth * c_dMMToDx));
		oWriter.WriteString(L"\"w:type=\"dxa\"/>");
		oWriter.WriteString(L"</w:tblPr>");

		oWriter.WriteString(L"<w:tblGrid>");
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
	bool CTable::IsEmpty() const
	{
		return m_arRows.empty();
	}

} // namespace NSDocxRenderer
