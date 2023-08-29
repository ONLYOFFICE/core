#include "DropCap.h"
#include "src/resources/Constants.h"

namespace NSDocxRenderer
{
	void CDropCap::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		oWriter.WriteString(L"<w:p>");
		oWriter.WriteString(L"<w:pPr>");
		oWriter.WriteString(L"<w:spacing w:after=\"0\"");
		oWriter.WriteString(L" w:line=\"");
		oWriter.AddInt(static_cast<LONG>((m_dBaselinePos - m_dTop) * c_dMMToDx));
		oWriter.WriteString(L"\" ");
		oWriter.WriteString(L"w:lineRule=\"exact\" />");
//		oWriter.WriteString(L"<w:framePr w:dropCap=\"drop\" w:lines=\"");
//		oWriter.AddInt(nLines);
//		oWriter.WriteString(L"\" w:wrap=\"around\" w:vAnchor=\"page\" w:hAnchor=\"page\" />");
//		oWriter.WriteString(L" w:x=\"");
//		oWriter.AddInt(static_cast<LONG>(m_dLeft * c_dMMToDx));
//		oWriter.WriteString(L"\" w:y=\"");
//		oWriter.AddInt(static_cast<LONG>(m_dTop * c_dMMToDx));
//		oWriter.WriteString(L"\" />");
		oWriter.WriteString(L"</w:pPr>");
		oWriter.WriteString(L"<w:r>");
		oWriter.WriteString(L"<w:rPr>");
		oWriter.WriteString(L"<w:rFonts w:ascii=\"" + wsFont +
							L"\" w:hAnsi=\"" + wsFont +
							L"\" w:eastAsia=\"" + wsFont +
							L"\" w:cs=\"" + wsFont + L"\" />");

//		oWriter.WriteString(L"<w:position w:val=\"");
//		oWriter.AddInt(nOffset);
//		oWriter.WriteString(L"\" />");
		oWriter.WriteString(L"<w:sz w:val=\"");
		oWriter.AddInt(nFontSize);
		oWriter.WriteString(L"\" />");
		oWriter.WriteString(L"</w:rPr>");
		oWriter.WriteString(L"<w:t xml:space=\"preserve\">" + wsText + L"</w:t>");
		oWriter.WriteString(L"</w:r>");
		oWriter.WriteString(L"</w:p>");
	}
}
