#include "TextLine.h"
#include "../../resources/Constants.h"
#include "../../resources/utils.h"
#include "src/logic/elements/Shape.h"

namespace NSDocxRenderer
{
	CTextLine::CTextLine() : CBaseItem(ElemType::etTextLine)
	{
	}

	void CTextLine::Clear()
	{
		for(auto& val : m_arConts)
				delete val;
		m_arConts.clear();
	}

	CTextLine::~CTextLine()
	{
		Clear();
	}

	void CTextLine::AddContent(CBaseItem *pObj)
	{
		CBaseItem::AddContent(pObj);
		m_dTrueHeight = std::max(m_dTrueHeight, dynamic_cast<CContText*>(pObj)->m_dTrueHeight);

		if (dynamic_cast<CContText*>(pObj)->m_pCont && m_eVertAlignType == eVertAlignType::vatUnknown)
		{
			m_eVertAlignType = dynamic_cast<CContText*>(pObj)->m_eVertAlignType;
		}

		m_arConts.push_back(dynamic_cast<CContText*>(pObj));
	}

	void CTextLine::CheckLineToNecessaryToUse()
	{
		for (size_t i = 0; i < m_arConts.size(); ++i)
			if (!m_arConts[i]->m_bIsNotNecessaryToUse)
				return;

		m_bIsNotNecessaryToUse = true;
	}

	void CTextLine::MergeConts()
	{
		if (m_arConts.empty())
			return;

		auto pFirst = m_arConts.front();

		for (size_t i = 1; i < m_arConts.size(); ++i)
		{
			auto pCurrent = m_arConts[i];

			if (pCurrent->m_bIsNotNecessaryToUse)
			{
				continue;
			}

			double dSpaceDefaultSize = pCurrent->CalculateThinSpace();
			double dSpaceWideSize = pCurrent->CalculateWideSpace();

			double dDifference = fabs(pCurrent->m_dLeft - pFirst->m_dRight);

			//todo возможно стоит доработать логику
			bool bIsEqual = pFirst->IsEqual(pCurrent);
			bool bIsBigDelta = dDifference > dSpaceDefaultSize;
			bool bIsVeryBigDelta = dDifference > dSpaceWideSize;

			if (bIsVeryBigDelta)
			{
				pFirst->m_bSpaceIsNotNeeded = false;
				pFirst = pCurrent;

			}
			else if (bIsEqual)
			{
				if (fabs(pFirst->m_dRight - pCurrent->m_dLeft) < dSpaceDefaultSize)
				{
					pFirst->m_oText += pCurrent->m_oText;
				}
				else if (bIsBigDelta)
				{
					pFirst->m_oText += uint32_t(' ');
					pFirst->m_oText += pCurrent->m_oText;
				}

				pFirst->m_dWidth = pCurrent->m_dRight - pFirst->m_dLeft;
				pFirst->m_dRight = pCurrent->m_dRight;

				if (!pFirst->m_pCont)
				{
					pFirst->m_pCont = pCurrent->m_pCont;
					pFirst->m_eVertAlignType = pCurrent->m_eVertAlignType;
				}

				pFirst->m_bSpaceIsNotNeeded = true;
				pCurrent->m_bIsNotNecessaryToUse = true;
			}
			else
			{
				if (bIsBigDelta)
				{
					if (!IsSpaceUtf32(pFirst->m_oText[pFirst->m_oText.length()-1]) &&
							!IsSpaceUtf32(pCurrent->m_oText[0]))
					{
						if (pFirst->GetNumberOfFeatures() <= pCurrent->GetNumberOfFeatures())
						{
							pFirst->m_oText += L" ";
							pFirst->m_dWidth += (pCurrent->m_dLeft - pFirst->m_dRight);
						}
						else
						{
							NSStringUtils::CStringUTF32 oNewText = L" ";
							oNewText += pCurrent->m_oText;
							pCurrent->m_oText = oNewText;
							pCurrent->m_dWidth += (pCurrent->m_dLeft - pFirst->m_dRight);
						}
					}

					pFirst->m_bSpaceIsNotNeeded = true;
				}
				else
				{
					pFirst->m_bSpaceIsNotNeeded = false;
				}
				pFirst = pCurrent;
			}
		}
	}

	void CTextLine::RecalcSizes()
	{
		m_dLeft = 0.0;
		m_dTop = 0.0;
		m_dWidth = 0.0;
		m_dHeight = 0.0;
		m_dBaselinePos = 0.0;
		m_dRight = 0.0;
		m_dTrueHeight = 0.0;

		for(auto&& cont : m_arConts)
			if(!cont->m_bIsNotNecessaryToUse)
			{
				m_dTrueHeight = std::max(m_dTrueHeight, dynamic_cast<CContText*>(cont)->m_dTrueHeight);
				CBaseItem::AddContent(cont);
			}
	}

	void CTextLine::SetVertAlignType(const eVertAlignType& oType)
	{
		m_eVertAlignType = oType;
		for (size_t i = 0; i < m_arConts.size(); ++i)
		{
			m_arConts[i]->m_eVertAlignType = oType;
		}
	}

	bool CTextLine::IsShadingPresent(const CTextLine *pLine)
	{
		return (m_pDominantShape && pLine->m_pDominantShape &&
				m_pDominantShape->m_oBrush.Color1 == pLine->m_pDominantShape->m_oBrush.Color1 &&
				fabs(m_pDominantShape->m_dLeft - pLine->m_pDominantShape->m_dLeft) < c_dGRAPHICS_ERROR_IN_LINES_MM &&
				fabs(m_pDominantShape->m_dWidth - pLine->m_pDominantShape->m_dWidth) < c_dGRAPHICS_ERROR_IN_LINES_MM);
	}

	void CTextLine::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		if (m_bIsNotNecessaryToUse)
		{
			return;
		}

		size_t nCountConts = m_arConts.size();

		if (0 == nCountConts)
			return;

		auto pPrev = m_arConts[0];
		double dDelta = 0;

		for (size_t i = 1; i < nCountConts; ++i)
		{
			auto pCurrent = m_arConts[i];

			if (pCurrent->m_bIsNotNecessaryToUse)
			{
				continue;
			}

			dDelta = pCurrent->m_dLeft - pPrev->m_dRight;
			pPrev->CalcSelectedWidth();
			pPrev->ToXml(oWriter);

			if (!(dDelta < pPrev->CalculateWideSpace() || pPrev->m_bSpaceIsNotNeeded))
				pPrev->AddWideSpaceToXml(dDelta, oWriter, pPrev->IsEqual(pCurrent));

			pPrev = pCurrent;
		}

		pPrev->CalcSelectedWidth();
		pPrev->ToXml(oWriter);

	}
	[[nodiscard]] eVerticalCrossingType CTextLine::GetVerticalCrossingType(const CBaseItem* oSrc) noexcept
	{
		if(oSrc->m_eType != ElemType::etContText)
			return CBaseItem::GetVerticalCrossingType(oSrc);

		auto m_dTop_copy = m_dTop;
		auto m_dTop_copy_src = oSrc->m_dTop;

		m_dTop = m_dBaselinePos - m_dTrueHeight;
		const_cast<CBaseItem*>(oSrc)->m_dTop = oSrc->m_dBaselinePos - static_cast<const CContText*>(oSrc)->m_dTrueHeight;

		auto vert_cross = CBaseItem::GetVerticalCrossingType(oSrc);

		m_dTop = m_dTop_copy;
		const_cast<CBaseItem*>(oSrc)->m_dTop = m_dTop_copy_src;

		return vert_cross;
	}
}
