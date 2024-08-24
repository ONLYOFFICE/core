#include "DropCap.h"

#include "../../resources/Constants.h"

namespace NSDocxRenderer
{
	void CDropCap::BuildXml(NSStringUtils::CStringBuilder& oWriter, const std::wstring& wsTag) const
	{
		oWriter.WriteString(L"<" + wsTag + L":p>");
		oWriter.WriteString(L"<" + wsTag + L":pPr>");
		oWriter.WriteString(L"<" + wsTag + L":spacing " + wsTag + L":after=\"0\"");
		oWriter.WriteString(L" " + wsTag + L":line=\"");
		oWriter.AddInt(static_cast<LONG>((m_dBaselinePos - m_dTop) * c_dMMToDx));
		oWriter.WriteString(L"\" ");
		oWriter.WriteString(wsTag + L":lineRule=\"exact\" />");
		oWriter.WriteString(L"</" + wsTag + L":pPr>");
		oWriter.WriteString(L"<" + wsTag + L":r>");
		oWriter.WriteString(L"<" + wsTag + L":rPr>");
		oWriter.WriteString(L"<" + wsTag + L":rFonts " + wsTag + L":ascii=\"" + wsFont +
							L"\" " + wsTag + L":hAnsi=\"" + wsFont +
							L"\" " + wsTag + L":eastAsia=\"" + wsFont +
							L"\" " + wsTag + L":cs=\"" + wsFont + L"\" />");

		oWriter.WriteString(L"<" + wsTag + L":sz " + wsTag + L":val=\"");
		oWriter.AddInt(nFontSize);
		oWriter.WriteString(L"\" />");
		oWriter.WriteString(L"</" + wsTag + L":rPr>");
		oWriter.WriteString(L"<" + wsTag + L":t xml:space=\"preserve\">" + wsText + L"</" + wsTag + L":t>");
		oWriter.WriteString(L"</" + wsTag + L":r>");
		oWriter.WriteString(L"</" + wsTag + L":p>");
	}
	void CDropCap::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{
		BuildXml(oWriter, L"w");
	}
	void CDropCap::ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const
	{
		BuildXml(oWriter, L"a");
	}
}
