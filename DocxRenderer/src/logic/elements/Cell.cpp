//#include "Cell.h"
//#include "Shape.h"

//namespace NSDocxRenderer
//{
//	CCell::~CCell()
//	{
//		Clear();
//	}

//	void CCell::Clear()
//	{
//		m_arTextLines.clear();
//		for(size_t i = 0; i < m_arOutputObjects.size(); ++i)

//			m_arOutputObjects[i]->Clear();
//		m_arOutputObjects.clear();
//	}

//	void CCell::AddContent(CBaseItem* pItem)
//	{
//		CBaseItem::AddContent(pItem);
//		m_arTextLines.push_back(dynamic_cast<CTextLine*>(pItem));
//	}

//	void CCell::ToXml(NSStringUtils::CStringBuilder &oWriter) const
//	{
//		oWriter.WriteString(L"<w:tc>");
//		oWriter.WriteString(L"<w:tcPr>");
//		oWriter.WriteString(L"<w:tcW w:w=\"");
//		oWriter.AddInt(static_cast<int>(m_dWidth * c_dMMToDx));
//		oWriter.WriteString(L"\" w:type=\"dxa\"/>");

//		if (m_bIsvMergeStart)
//		{
//			oWriter.WriteString(L"<w:vMerge w:val=\"restart\"/>");
//		}
//		else if (m_bIsvMerge)
//		{
//			oWriter.WriteString(L"<w:vMerge/>");
//		}

//		if (m_uGridSpan > 1)
//		{
//			oWriter.WriteString(L"<w:gridSpan w:val=\"");
//			oWriter.AddUInt(m_uGridSpan);
//			oWriter.WriteString(L"\"/>");
//		}
//		if (!m_bIsTopBorder || !m_bIsLeftBorder || !m_bIsBottomBorder ||
//				!m_bIsRightBorder || m_bIsDiagonalDownBorder || m_bIsDiagonalUpBorder)
//		{
//			oWriter.WriteString(L"<w:tcBorders>");
//			if (!m_bIsTopBorder)
//			{
//				oWriter.WriteString(L"<w:top w:val=\"nil\"/>");
//			}
//			if (!m_bIsLeftBorder)
//			{
//				oWriter.WriteString(L"<w:left w:val=\"nil\"/>");
//			}
//			if (!m_bIsBottomBorder)
//			{
//				oWriter.WriteString(L"<w:bottom w:val=\"nil\"/>");
//			}
//			if (!m_bIsRightBorder)
//			{
//				oWriter.WriteString(L"<w:right w:val=\"nil\"/>");
//			}
//			if (m_bIsDiagonalDownBorder)
//			{
//				oWriter.WriteString(L"<w:tl2br w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>");
//			}
//			if (m_bIsDiagonalUpBorder)
//			{
//				oWriter.WriteString(L"<w:tr2bl w:val=\"single\" w:sz=\"4\" w:space=\"0\" w:color=\"auto\"/>");
//			}

//			oWriter.WriteString(L"</w:tcBorders>");
//		}

//		oWriter.WriteString(L"</w:tcPr>");

//		if (m_arOutputObjects.empty())
//		{
//			oWriter.WriteString(L"<w:p/>");
//		}
//		else
//		{
//			for (size_t i = 0; i < m_arOutputObjects.size(); ++i)
//			{
//				auto pObj = m_arOutputObjects[i];

//				CParagraph* pParagraph = nullptr;
//				if((pParagraph = dynamic_cast<CParagraph*>(pObj)) != nullptr)
//					pParagraph->ToXml(oWriter);
//			}
//		}

//		oWriter.WriteString(L"</w:tc>");
//	}

//	void CCell::SetParameters(CPeak* pPeak, eCorners eCorner)
//	{
//		m_pPeaks[eCorner] = pPeak;

//		switch (eCorner)
//		{
//		case cI:
//			if (pPeak->m_pLines[CPeak::dI])
//			{
//				m_bIsLeftBorder = true;

//				m_dLeft = pPeak->m_pLines[CPeak::dI]->m_dLeft + pPeak->m_pPeak->m_dWidth/2;
//				m_dBaselinePos = pPeak->m_pLines[CPeak::dI]->m_dTop + pPeak->m_pPeak->m_dHeight/2;
//			}
//			if (pPeak->m_pLines[CPeak::dII])
//			{
//				m_bIsBottomBorder = true;

//				m_dLeft = pPeak->m_pLines[CPeak::dII]->m_dLeft + pPeak->m_pPeak->m_dWidth/2;
//				m_dBaselinePos = pPeak->m_pLines[CPeak::dII]->m_dTop + pPeak->m_pPeak->m_dHeight/2;
//			}
//			break;
//		case cII:
//			if (pPeak->m_pLines[CPeak::dII])
//			{
//				m_bIsRightBorder = true;

//				m_dRight = pPeak->m_pLines[CPeak::dII]->m_dLeft + pPeak->m_pPeak->m_dWidth/2;
//				m_dBaselinePos = pPeak->m_pLines[CPeak::dII]->m_dTop + pPeak->m_pPeak->m_dHeight/2;
//			}
//			if (pPeak->m_pLines[CPeak::dIII])
//			{
//				m_bIsBottomBorder = true;

//				m_dRight = pPeak->m_pLines[CPeak::dIII]->m_dRight - pPeak->m_pPeak->m_dWidth/2;
//				m_dBaselinePos = pPeak->m_pLines[CPeak::dIII]->m_dTop + pPeak->m_pPeak->m_dHeight/2;
//			}
//			break;
//		case cIII:
//			if (pPeak->m_pLines[CPeak::dIV])
//			{
//				m_bIsRightBorder = true;

//				m_dRight = pPeak->m_pLines[CPeak::dIV]->m_dLeft + pPeak->m_pPeak->m_dWidth/2;
//				m_dTop = pPeak->m_pLines[CPeak::dIV]->m_dTop + pPeak->m_pPeak->m_dHeight/2;
//			}
//			if (pPeak->m_pLines[CPeak::dIII])
//			{
//				m_bIsTopBorder = true;

//				m_dRight = pPeak->m_pLines[CPeak::dIII]->m_dRight - pPeak->m_pPeak->m_dWidth/2;
//				m_dTop = pPeak->m_pLines[CPeak::dIII]->m_dTop + pPeak->m_pPeak->m_dHeight/2;
//			}
//			break;
//		case cIV:
//			if (pPeak->m_pLines[CPeak::dIV])
//			{
//				m_bIsLeftBorder = true;

//				m_dLeft = pPeak->m_pLines[CPeak::dIV]->m_dLeft + pPeak->m_pPeak->m_dWidth/2;
//				m_dTop = pPeak->m_pLines[CPeak::dIV]->m_dTop + pPeak->m_pPeak->m_dHeight/2;
//			}
//			if (pPeak->m_pLines[CPeak::dI])
//			{
//				m_bIsTopBorder = true;

//				m_dLeft = pPeak->m_pLines[CPeak::dI]->m_dLeft + pPeak->m_pPeak->m_dWidth/2;
//				m_dTop = pPeak->m_pLines[CPeak::dI]->m_dTop + pPeak->m_pPeak->m_dHeight/2;
//			}
//			break;
//		default:
//			break;
//		}

//		if (m_dLeft != 0 && m_dRight != 0)
//		{
//			m_dWidth = m_dRight - m_dLeft;
//		}

//		if (m_dTop != 0 && m_dBaselinePos != 0)
//		{
//			m_dHeight = m_dBaselinePos - m_dTop;
//		}
//	}
//}
