#include "DropCap.h"
#include "../../resources/Constants.h"

namespace NSDocxRenderer
{
	void CDropCap::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{
		oWriter.WriteString(L"<w:p>");
		oWriter.WriteString(L"<w:pPr>");
		oWriter.WriteString(L"<w:spacing w:after=\"0\"");
		oWriter.WriteString(L" w:line=\"");
		oWriter.AddInt(static_cast<LONG>((m_dBaselinePos - m_dTop) * c_dMMToDx));
		oWriter.WriteString(L"\" ");
		oWriter.WriteString(L"w:lineRule=\"exact\" />");
		oWriter.WriteString(L"</w:pPr>");
		oWriter.WriteString(L"<w:r>");
		oWriter.WriteString(L"<w:rPr>");
		oWriter.WriteString(L"<w:rFonts w:ascii=\"" + wsFont +
							L"\" w:hAnsi=\"" + wsFont +
							L"\" w:eastAsia=\"" + wsFont +
							L"\" w:cs=\"" + wsFont + L"\" />");

		oWriter.WriteString(L"<w:sz w:val=\"");
		oWriter.AddInt(nFontSize);
		oWriter.WriteString(L"\" />");
		oWriter.WriteString(L"</w:rPr>");
		oWriter.WriteString(L"<w:t xml:space=\"preserve\">" + wsText + L"</w:t>");
		oWriter.WriteString(L"</w:r>");
		oWriter.WriteString(L"</w:p>");
	}
}
