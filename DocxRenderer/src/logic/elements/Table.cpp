#include "Table.h"

namespace NSDocxRenderer
{
    void CTable::CCell::Clear()
	{

	}
	void CTable::CCell::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{

	}
	void CTable::CCell::ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const
	{

	}

	void CTable::CRow::Clear()
	{

	}
	void CTable::CRow::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{

	}
	void CTable::CRow::ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const
	{

	}

	void CTable::Clear()
	{

	}
	void CTable::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{
		oWriter.WriteString(L"<w:tbl>");

		oWriter.WriteString(L"<w:tblPr>");
		oWriter.WriteString(L"</w:tblPr>");

		oWriter.WriteString(L"<w:tblGrid>");
		oWriter.WriteString(L"</w:tblGrid>");

		oWriter.WriteString(L"<w:tr>");
		oWriter.WriteString(L"</w:tr>");

		oWriter.WriteString(L"</w:tbl>");
	}
	void CTable::ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const
	{

	}

} // namespace NSDocxRenderer
