#include "TextLine.h"

#include "../../logic/elements/Shape.h"
#include "../../resources/Constants.h"
#include "../../resources/utils.h"

namespace NSDocxRenderer
{

	CTextLine::~CTextLine()
	{
		Clear();
	}

	void CTextLine::Clear()
	{
		m_arConts.clear();
		m_pLine = nullptr;
	}
	void CTextLine::AddCont(std::shared_ptr<CContText> oCont)
	{
		RecalcWithNewItem(oCont.get());
		m_arConts.push_back(oCont);
	}
	void CTextLine::AddConts(const std::vector<std::shared_ptr<CContText>>& arConts)
	{
		for (auto& cont : arConts)
			AddCont(cont);
	}

	bool CTextLine::IsCanBeDeleted() const
	{
		for (size_t i = 0; i < m_arConts.size(); ++i)
			if (m_arConts[i])
				return false;

		return true;
	}

	void CTextLine::MergeConts()
	{
		if (m_arConts.empty())
			return;

		using cont_ptr_t = std::shared_ptr<CContText>;
		std::sort(m_arConts.begin(), m_arConts.end(), [] (const cont_ptr_t& a, const cont_ptr_t& b) {
			if (!a) return false;
			if (!b) return true;
			return a->m_dLeft < b->m_dLeft;
		});

		std::shared_ptr<CContText> pFirst;
		size_t j = 0;

		for(; j < m_arConts.size() && !pFirst; ++j)
			pFirst = m_arConts[j];

		for (size_t i = j; i < m_arConts.size(); ++i)
		{
			auto& pCurrent = m_arConts[i];
			if (!pCurrent)
				continue;

			double avg_space_width = pCurrent->m_pFontStyle->GetAvgSpaceWidth();
			double space_width = avg_space_width != 0.0 ?
									 avg_space_width * c_dAVERAGE_SPACE_WIDTH_COEF :
									 pCurrent->CalculateSpace() * c_dSPACE_WIDTH_COEF;

			double dDifference = pCurrent->m_dLeft - pFirst->m_dRight;

			bool bIsEqual = pFirst->IsEqual(pCurrent.get());
			bool bIsSpaceDelta = dDifference > space_width;
			bool bIsWideSpaceDelta = dDifference > space_width * 3;

			if (bIsWideSpaceDelta || (pCurrent->m_bPossibleSplit && bIsSpaceDelta))
			{
				auto wide_space = std::make_shared<CContText>(pFirst->m_pManager);

				// sets all members for wide_space except highlight things
				auto set_base = [&pFirst, &pCurrent, &wide_space] () {
					wide_space->m_dLeft = pFirst->m_dRight;
					wide_space->m_dRight = pCurrent->m_dLeft;
					wide_space->m_dWidth = wide_space->m_dRight - wide_space->m_dLeft;

					wide_space->m_dBaselinePos = pCurrent->m_dBaselinePos;
					wide_space->m_dTop = pCurrent->m_dTop;

					wide_space->m_dTopWithAscent = pCurrent->m_dTopWithAscent;
					wide_space->m_dBotWithDescent = pCurrent->m_dBotWithDescent;

					wide_space->m_dHeight = pCurrent->m_dHeight;

					wide_space->SetSym(c_SPACE_SYM, wide_space->m_dRight - wide_space->m_dLeft);
					wide_space->m_pFontStyle = pFirst->m_pFontStyle;
					wide_space->m_pShape = nullptr;
					wide_space->m_iNumDuplicates = 0;

					// cache that value? (calls rarely)
					wide_space->CalcSelected();
				};

				if (bIsEqual)
				{
					// assign all
					*wide_space = *pFirst;

					// then set all for wide space
					set_base();
				}
				else
					set_base();

				m_arConts.insert(m_arConts.begin() + i, wide_space);

				i++;
				while (!m_arConts[i] && i < m_arConts.size()) i++;
				if (i == m_arConts.size()) break;
				pFirst = m_arConts[i];
			}
			else if (bIsEqual)
			{
				if (!bIsSpaceDelta)
				{
					pFirst->AddTextBack(pCurrent->GetText(), pCurrent->GetSymWidths());
				}
				else
				{
					pFirst->AddSymBack(c_SPACE_SYM, pCurrent->m_dLeft - pFirst->m_dRight);
					pFirst->AddTextBack(pCurrent->GetText(), pCurrent->GetSymWidths());
				}

				if (pFirst->m_pCont.expired())
				{
					pFirst->m_pCont = pCurrent->m_pCont;
					pFirst->m_eVertAlignType = pCurrent->m_eVertAlignType;
				}
				pCurrent = nullptr;
			}
			else
			{
				if (bIsSpaceDelta)
				{
					if (pFirst->GetNumberOfFeatures() <= pCurrent->GetNumberOfFeatures())
						pFirst->AddSymBack(c_SPACE_SYM, pCurrent->m_dLeft - pFirst->m_dRight);
					else
						pCurrent->AddSymFront(c_SPACE_SYM, pCurrent->m_dLeft - pFirst->m_dRight);
				}
				pFirst = pCurrent;
			}
		}

		auto right = MoveNullptr(m_arConts.begin(), m_arConts.end());
		m_arConts.erase(right, m_arConts.end());

		using cont_ptr_t = std::shared_ptr<CContText>;
		std::sort(m_arConts.begin(), m_arConts.end(), [] (const cont_ptr_t& a, const cont_ptr_t& b) {
			return a->m_dLeft < b->m_dLeft;
		});
	}

	void CTextLine::CalcFirstWordWidth()
	{
		bool is_done = false;
		double width = 0;
		for (auto& cont : m_arConts)
		{
			if (!cont)
				continue;

			const auto& text = cont->GetText();
			auto ar_widths = cont->GetSymWidths();
			for (size_t i = 0; i < text.length(); ++i)
			{
				if (text.at(i) == c_SPACE_SYM)
				{
					m_dFirstWordWidth = width;
					is_done = true;
					break;
				}
				width += ar_widths[i];
			}
			if (is_done)
				break;
		}
		if (!is_done)
			m_dFirstWordWidth = m_dWidth;
	}

	void CTextLine::RecalcSizes()
	{
		m_dLeft = 0.0;
		m_dTop = 0.0;
		m_dWidth = 0.0;
		m_dHeight = 0.0;
		m_dBaselinePos = 0.0;
		m_dRight = 0.0;
		m_dHeight = 0.0;

		for (const auto& cont : m_arConts)
			if(cont)
				RecalcWithNewItem(cont.get());
	}

	eVerticalCrossingType CTextLine::GetVerticalCrossingType(const CTextLine* pLine) const noexcept
	{
		const double& this_top = m_dTopWithMaxAscent;
		const double& this_bot = m_dBotWithMaxDescent;

		const double& other_top = pLine->m_dTopWithMaxAscent;
		const double& other_bot = pLine->m_dBotWithMaxDescent;

		if (this_top > other_top && this_bot < other_bot)
			return eVerticalCrossingType::vctCurrentInsideNext;

		else if (this_top < other_top && this_bot > other_bot)
			return  eVerticalCrossingType::vctCurrentOutsideNext;

		else if (this_top < other_top && this_bot < other_bot &&
				 (this_bot >= other_top || fabs(this_bot - other_top) < c_dTHE_SAME_STRING_Y_PRECISION_MM))
			return  eVerticalCrossingType::vctCurrentAboveNext;

		else if (this_top > other_top && this_bot > other_bot &&
				 (this_top <= other_bot || fabs(this_top - other_bot) < c_dTHE_SAME_STRING_Y_PRECISION_MM))
			return  eVerticalCrossingType::vctCurrentBelowNext;

		else if (this_top == other_top && this_bot == other_bot &&
				 m_dLeft == pLine->m_dLeft && m_dRight == pLine->m_dRight)
			return  eVerticalCrossingType::vctDublicate;

		else if (fabs(this_top - other_top) < c_dTHE_SAME_STRING_Y_PRECISION_MM &&
				 fabs(this_bot - other_bot) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
			return  eVerticalCrossingType::vctTopAndBottomBordersMatch;

		else if (fabs(this_top - other_top) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
			return  eVerticalCrossingType::vctTopBorderMatch;

		else if (fabs(this_bot - other_bot) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
			return  eVerticalCrossingType::vctBottomBorderMatch;

		else if (this_bot < other_top)
			return  eVerticalCrossingType::vctNoCrossingCurrentAboveNext;

		else if (this_top > other_bot)
			return  eVerticalCrossingType::vctNoCrossingCurrentBelowNext;

		else
			return  eVerticalCrossingType::vctUnknown;
	}

	void CTextLine::RecalcWithNewItem(const CContText* pCont)
	{
		CBaseItem::RecalcWithNewItem(pCont);
		if (m_dTopWithMaxAscent == 0.0) m_dTopWithMaxAscent = pCont->m_dTopWithAscent;
		else m_dTopWithMaxAscent = std::min(m_dTopWithMaxAscent, pCont->m_dTopWithAscent);

		m_dBotWithMaxDescent = std::max(m_dBotWithMaxDescent, pCont->m_dBotWithDescent);
	}

	void CTextLine::SetVertAlignType(const eVertAlignType& oType)
	{
		m_eVertAlignType = oType;
		for (size_t i = 0; i < m_arConts.size(); ++i)
		{
			if(m_arConts[i])
				m_arConts[i]->m_eVertAlignType = oType;
		}
	}

	bool CTextLine::IsShadingPresent(const CTextLine *pLine) const noexcept
	{
		return (m_pDominantShape && pLine->m_pDominantShape &&
				m_pDominantShape->m_oBrush.Color1 == pLine->m_pDominantShape->m_oBrush.Color1 &&
				fabs(m_pDominantShape->m_dLeft - pLine->m_pDominantShape->m_dLeft) < c_dGRAPHICS_ERROR_IN_LINES_MM &&
				fabs(m_pDominantShape->m_dWidth - pLine->m_pDominantShape->m_dWidth) < c_dGRAPHICS_ERROR_IN_LINES_MM);
	}

	void CTextLine::ToXml(NSStringUtils::CStringBuilder& oWriter) const
	{
		for (const auto& cont : m_arConts)
			if(cont)
				cont->ToXml(oWriter);
	}

	void CTextLine::ToXmlPptx(NSStringUtils::CStringBuilder& oWriter) const
	{
		for (const auto& cont : m_arConts)
			if(cont)
				cont->ToXmlPptx(oWriter);
	}

	size_t CTextLine::GetLength() const
	{
		size_t len = 0;
		for (const auto& cont : m_arConts)
			if (cont)
				len += cont->GetLength();

		return len;
	}
}
