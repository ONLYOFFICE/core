#include "Paragraph.h"

#include "../../resources/utils.h"

namespace NSDocxRenderer
{
	CParagraph::~CParagraph()
	{
		Clear();
	}

	void CParagraph::Clear()
	{
		m_arTextLines.clear();
	}

	void CParagraph::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{
		oWriter.WriteString(L"<w:p>");
		oWriter.WriteString(L"<w:pPr>");

		// styles
		if (!m_wsStyleId.empty()) oWriter.WriteString(L"<w:pStyle w:val=\"" + m_wsStyleId + L"\"/>");

		oWriter.WriteString(L"<w:spacing");

		oWriter.WriteString(L" w:before=\"");
		if (m_dSpaceBefore > 0)
			oWriter.AddInt(static_cast<int>(m_dSpaceBefore * c_dMMToDx));
		else
			oWriter.AddInt(static_cast<int>(0));
		oWriter.WriteString(L"\"");


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
			if (m_dFirstLine > 0)
			{
				oWriter.WriteString(L" w:firstLine=\"");
				oWriter.AddInt(static_cast<int>(m_dFirstLine * c_dMMToDx));
				oWriter.WriteString(L"\"");
			}
			else
			{
				oWriter.WriteString(L" w:hanging=\"");
				oWriter.AddInt(static_cast<int>(-m_dFirstLine * c_dMMToDx));
				oWriter.WriteString(L"\"");
			}
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
		for(const auto& line : m_arTextLines)
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

		oWriter.WriteString(L"\" ");

		if (m_bIsNeedFirstLineIndent)
		{
			oWriter.WriteString(L" indent=\" ");
			oWriter.AddInt(static_cast<int>(m_dFirstLine * c_dMMToEMU));
			oWriter.WriteString(L"\"");
		}

		if (m_dLeftBorder > 0)
		{
			oWriter.WriteString(L" marL=\" ");
			oWriter.AddInt(static_cast<int>(m_dLeftBorder * c_dMMToEMU));
			oWriter.WriteString(L"\"");
		}
		if (m_dRightBorder > 0)
		{
			oWriter.WriteString(L" marR=\"");
			oWriter.AddInt(static_cast<int>(m_dRightBorder * c_dMMToEMU));
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
		oWriter.AddInt(static_cast<int>(m_dSpaceAfter * c_dMMToPt * 100));
		oWriter.WriteString(L"\"/>");
		oWriter.WriteString(L"</a:spcAft>");

		oWriter.WriteString(L"<a:lnSpc>");
		oWriter.WriteString(L"<a:spcPts val=\"");
		oWriter.AddInt(static_cast<int>(m_dLineHeight * c_dMMToPt * 100));
		oWriter.WriteString(L"\"/>");
		oWriter.WriteString(L"</a:lnSpc>");

		oWriter.WriteString(L"</a:pPr>");
		for(const auto& line : m_arTextLines)
			if(line)
				line->ToXmlPptx(oWriter);
		oWriter.WriteString(L"</a:p>");
	}
	void CParagraph::ToBin(NSWasm::CData& oWriter) const
	{
		// WriteRecord WriteTextParagraphPr
		[this, &oWriter] () {
			oWriter.StartRecord(0);
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			oWriter.WriteBYTE(0);
			switch (m_eTextAlignmentType)
			{
			case tatByCenter:
				oWriter.WriteBYTE(0);
				break;
			case tatByRight:
				oWriter.WriteBYTE(5);
				break;
			case tatByWidth:
				oWriter.WriteBYTE(2);
				break;
			case tatByLeft:  // fallthrough
			case tatUnknown: // fallthrough
			default:
				oWriter.WriteBYTE(4);
				break;
			}

			// marL
			if (m_dLeftBorder > 0)
			{
				oWriter.WriteBYTE(8);
				oWriter.AddSInt(static_cast<int>(m_dLeftBorder * c_dMMToEMU));
			}

			// marR
			if (m_dRightBorder > 0)
			{
				oWriter.WriteBYTE(9);
				oWriter.AddSInt(static_cast<int>(m_dRightBorder * c_dMMToEMU));
			}

			// indent
			if (m_bIsNeedFirstLineIndent)
			{
				oWriter.WriteBYTE(5);
				oWriter.AddSInt(static_cast<int>(m_dFirstLine * c_dMMToEMU));
			}
			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);

			const int max_value = 158400;

			// line spacing
			oWriter.StartRecord(0);
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			int height_value = static_cast<int>(m_dLineHeight * c_dMMToPt * 100);
			if (fabs(height_value) > max_value)
				height_value > 0 ? height_value = max_value : height_value = -max_value;
			oWriter.WriteBYTE(1); oWriter.AddSInt(static_cast<int>(height_value));
			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
			oWriter.EndRecord();

			// space after
			oWriter.StartRecord(1);
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			int after_value = static_cast<int>(m_dSpaceAfter * c_dMMToPt * 100);
			if (fabs(after_value) > max_value)
				after_value > 0 ? after_value = max_value : after_value = -max_value;
			oWriter.WriteBYTE(1); oWriter.AddSInt(static_cast<int>(after_value));
			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
			oWriter.EndRecord();

			// space before
			oWriter.StartRecord(2);
			oWriter.WriteBYTE(kBin_g_nodeAttributeStart);
			int before_value = static_cast<int>(m_dSpaceBefore * c_dMMToPt * 100);
			if (fabs(before_value) > max_value)
				before_value > 0 ? before_value = max_value : before_value = -max_value;
			oWriter.WriteBYTE(1); oWriter.AddSInt(static_cast<int>(before_value));
			oWriter.WriteBYTE(kBin_g_nodeAttributeEnd);
			oWriter.EndRecord();

			oWriter.EndRecord();
		}();

		oWriter.StartRecord(2);
		unsigned int count = 0;
		for (const auto& text_line : m_arTextLines)
			count += text_line->m_arConts.size();

		oWriter.AddInt(count);
		for (const auto& text_line : m_arTextLines)
			text_line->ToBin(oWriter);

		oWriter.EndRecord();
	}

	void CParagraph::RemoveHighlightColor()
	{
		if (!m_bIsShadingPresent)
			return;

		for(size_t i = 0; i < m_arTextLines.size(); ++i)
		{
			auto& pLine = m_arTextLines[i];
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
		for(size_t i = 0; i < m_arTextLines.size() - 1; ++i)
		{
			auto pLine = m_arTextLines[i];
			auto pLastCont = pLine->m_arConts.back();
			size_t iNumConts = pLine->m_arConts.size() - 1;

			while(!pLastCont)
				pLastCont = pLine->m_arConts[--iNumConts];

			auto text = pLastCont->GetText();
			auto last_sym = text[text.length() - 1];

			if (last_sym != c_SPACE_SYM && m_arTextLines.size() != 1)
				pLastCont->AddSymBack(c_SPACE_SYM, 0, 0, pLastCont->m_dRight);
		}
	}
}
