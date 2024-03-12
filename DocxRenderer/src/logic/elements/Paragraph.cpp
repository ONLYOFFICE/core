#include "Paragraph.h"
#include "src/resources/ColorTable.h"
#include "src/resources/utils.h"

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

	void CParagraph::BuildXml(NSStringUtils::CStringBuilder& oWriter, const std::wstring& wsTag) const
	{
		oWriter.WriteString(L"<" + wsTag + L":p>");
		oWriter.WriteString(L"<" + wsTag + L":pPr>");

		// styles
		if(!m_wsStyleId.empty()) oWriter.WriteString(L"<" + wsTag + L":pStyle " + wsTag + L":val=\"" + m_wsStyleId + L"\"/>");

		oWriter.WriteString(L"<" + wsTag + L":spacing");
		if (m_dSpaceBefore > 0)
		{
			oWriter.WriteString(L" " + wsTag + L":before=\"");
			oWriter.AddInt(static_cast<int>(m_dSpaceBefore * c_dMMToDx));
			oWriter.WriteString(L"\"");
		}

		if (m_dSpaceAfter > 0)
		{
			oWriter.WriteString(L" " + wsTag + L":after=\"");
			oWriter.AddInt(static_cast<int>(m_dSpaceAfter * c_dMMToDx));
			oWriter.WriteString(L"\"");
		}

		if (m_dLineHeight > 0)
		{
			oWriter.WriteString(L" " + wsTag + L":line=\"");
			oWriter.AddInt(static_cast<int>(m_dLineHeight * c_dMMToDx));
			oWriter.WriteString(L"\" " + wsTag + L":lineRule=\"exact\""); // exact - точный размер строки
		}

		oWriter.WriteString(L"/>"); //конец w:spacing

		oWriter.WriteString(L"<" + wsTag + L":ind");
		if (m_dLeftBorder > 0)
		{
			oWriter.WriteString(L" " + wsTag + L":left=\"");
			oWriter.AddInt(static_cast<int>(m_dLeftBorder * c_dMMToDx));
			oWriter.WriteString(L"\"");
		}
		if (m_dRightBorder > 0)
		{
			oWriter.WriteString(L" " + wsTag + L":right=\"");
			oWriter.AddInt(static_cast<int>(m_dRightBorder * c_dMMToDx)); //здесь m_dRight - расстояние от правого края
			oWriter.WriteString(L"\"");
		}
		if (m_bIsNeedFirstLineIndent)
		{
			oWriter.WriteString(L" " + wsTag + L":firstLine=\"");
			oWriter.AddInt(static_cast<int>(m_dFirstLine * c_dMMToDx));
			oWriter.WriteString(L"\"");
		}
		oWriter.WriteString(L"/>"); //конец w:ind

		switch (m_eTextAlignmentType)
		{
		case tatByCenter:
			oWriter.WriteString(L"<" + wsTag + L":jc " + wsTag + L":val=\"center\"/>");
			break;
		case tatByRight:
			oWriter.WriteString(L"<" + wsTag + L":jc " + wsTag + L":val=\"end\"/>");
			break;
		case tatByWidth:
			oWriter.WriteString(L"<" + wsTag + L":jc " + wsTag + L":val=\"both\"/>");
			break;
		case tatByLeft:
			oWriter.WriteString(L"<" + wsTag + L":jc " + wsTag + L":val=\"begin\"/>");
			break;
		case tatUnknown:
		default: //по умолчанию выравнивание по левому краю - можно ничего не добавлять
			break;
		}

		if (m_bIsShadingPresent)
		{
			oWriter.WriteString(L"<" + wsTag + L":shd " + wsTag + L":val=\"clear\" " + wsTag + L":color=\"auto\" " + wsTag + L":fill=\"");
			oWriter.WriteHexInt3(ConvertColorBGRToRGB(m_lColorOfShadingFill));
			oWriter.WriteString(L"\"/>");
		}
	}

	void CParagraph::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{
		BuildXml(oWriter, L"w");

		oWriter.WriteString(L"</w:pPr>");
		for(const auto& line : m_arLines)
			if(line)
				line->ToXml(oWriter);
		oWriter.WriteString(L"</w:p>");
	}

	void CParagraph::ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const
	{
		BuildXml(oWriter, L"a");

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
