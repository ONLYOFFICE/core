#include "Paragraph.h"
#include "../../resources/ColorTable.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{
	CParagraph::~CParagraph()
	{
		Clear();
	}

	void CParagraph::Clear()
	{
		m_arLines.clear();
	}

	void CParagraph::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{
		oWriter.WriteString(L"<w:p>");
		oWriter.WriteString(L"<w:pPr>");

		// styles
		if(!m_wsStyleId.empty()) oWriter.WriteString(L"<w:pStyle w:val=\"" + m_wsStyleId + L"\"/>");

		oWriter.WriteString(L"<w:spacing");
		if (m_dSpaceBefore > 0)
		{
			oWriter.WriteString(L" w:before=\"");
			oWriter.AddInt(static_cast<int>(m_dSpaceBefore * c_dMMToDx));
			oWriter.WriteString(L"\"");
		}

		if (m_dSpaceAfter > 0)
		{
			oWriter.WriteString(L" w:after=\"");
			oWriter.AddInt(static_cast<int>(m_dSpaceAfter * c_dMMToDx));
			oWriter.WriteString(L"\"");
		}

		if (m_dLineHeight > 0)
		{
			oWriter.WriteString(L" w:line=\"");
			oWriter.AddInt(static_cast<int>(m_dLineHeight * c_dMMToDx));
			oWriter.WriteString(L"\" w:lineRule=\"exact\""); // exact - точный размер строки
		}

		oWriter.WriteString(L"/>"); //конец w:spacing

		oWriter.WriteString(L"<w:ind");
		if (m_dLeftBorder > 0)
		{
			oWriter.WriteString(L" w:left=\"");
			oWriter.AddInt(static_cast<int>(m_dLeftBorder * c_dMMToDx));
			oWriter.WriteString(L"\"");
		}
		if (m_dRightBorder > 0)
		{
			oWriter.WriteString(L" w:right=\"");
			oWriter.AddInt(static_cast<int>(m_dRightBorder * c_dMMToDx)); //здесь m_dRight - расстояние от правого края
			oWriter.WriteString(L"\"");
		}
		if (m_bIsNeedFirstLineIndent)
		{
			oWriter.WriteString(L" w:firstLine=\"");
			oWriter.AddInt(static_cast<int>(m_dFirstLine * c_dMMToDx));
			oWriter.WriteString(L"\"");
		}
		oWriter.WriteString(L"/>"); //конец w:ind

		switch (m_eTextAlignmentType)
		{
		case tatByCenter:
			oWriter.WriteString(L"<w:jc w:val=\"center\"/>");
			break;
		case tatByRight:
			oWriter.WriteString(L"<w:jc w:val=\"end\"/>");
			break;
		case tatByWidth:
			oWriter.WriteString(L"<w:jc w:val=\"both\"/>");
			break;
		case tatByLeft:
			oWriter.WriteString(L"<w:jc w:val=\"begin\"/>");
			break;
		case tatUnknown:
		default: //по умолчанию выравнивание по левому краю - можно ничего не добавлять
			break;
		}

		if (m_bIsShadingPresent)
		{
			oWriter.WriteString(L"<w:shd w:val=\"clear\" w:color=\"auto\" w:fill=\"");
			oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lColorOfShadingFill));
			oWriter.WriteString(L"\"/>");
		}

		oWriter.WriteString(L"</w:pPr>");
		for(const auto& line : m_arLines)
			if(line)
				line->ToXml(oWriter);
		oWriter.WriteString(L"</w:p>");
	}

	void CParagraph::ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const
	{
		oWriter.WriteString(L"<a:p>");
		oWriter.WriteString(L"<a:pPr algn=\"");
		switch (m_eTextAlignmentType)
		{
		case tatByCenter:
			oWriter.WriteString(L"ctr");
			break;
		case tatByRight:
			oWriter.WriteString(L"r");
			break;
		case tatByWidth:
			oWriter.WriteString(L"just");
			break;
		case tatByLeft:
			oWriter.WriteString(L"l");
			break;
		case tatUnknown:
		default:
			oWriter.WriteString(L"l");
			break;
		}

		oWriter.WriteString(L"\"");

		if (m_bIsNeedFirstLineIndent)
		{
			oWriter.WriteString(L" indent=\"");
			oWriter.AddInt(static_cast<int>(m_dFirstLine * c_dMMToEMU));
			oWriter.WriteString(L"\"");
		}
		oWriter.WriteString(L">");

		oWriter.WriteString(L"<a:spcBef>");
		oWriter.WriteString(L"<a:spcPts val=\"");
		oWriter.AddInt(static_cast<int>(m_dSpaceBefore * c_dMMToPt * 100));
		oWriter.WriteString(L"\"/>");
		oWriter.WriteString(L"</a:spcBef>");


		oWriter.WriteString(L"<a:spcAft>");
		oWriter.WriteString(L"<a:spcPts val=\"");
		oWriter.AddInt(static_cast<int>(m_dSpaceBefore * c_dMMToPt * 100));
		oWriter.WriteString(L"\"/>");
		oWriter.WriteString(L"</a:spcAft>");

		oWriter.WriteString(L"<a:lnSpc>");
		oWriter.WriteString(L"<a:spcPts val=\"");
		oWriter.AddInt(static_cast<int>(m_dLineHeight * c_dMMToPt * 100));
		oWriter.WriteString(L"\"/>");
		oWriter.WriteString(L"</a:lnSpc>");

		oWriter.WriteString(L"</a:pPr>");
		for(const auto& line : m_arLines)
			if(line)
				line->ToXmlPptx(oWriter);
		oWriter.WriteString(L"</a:p>");
	}

	void CParagraph::RemoveHighlightColor()
	{
		if (!m_bIsShadingPresent)
			return;

		for(size_t i = 0; i < m_arLines.size(); ++i)
		{
			auto& pLine = m_arLines[i];
			if (pLine || pLine->m_pDominantShape)
			{
				for (size_t j = 0; j < pLine->m_arConts.size(); ++j)
				{
					auto& pCont = pLine->m_arConts[j];
					if(pCont)
						if (m_lColorOfShadingFill == pCont->m_lHighlightColor)
							pCont->m_bIsHighlightPresent = false;
				}
			}
		}
	}

	void CParagraph::MergeLines()
	{
		for(size_t i = 0; i < m_arLines.size() - 1; ++i)
		{
			auto pLine = m_arLines[i];
			auto pLastCont = pLine->m_arConts.back();
			size_t iNumConts = pLine->m_arConts.size() - 1;

			while(!pLastCont)
				pLastCont = pLine->m_arConts[--iNumConts];

			pLastCont->m_oText += L" ";
		}
	}
}
