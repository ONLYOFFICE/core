#include "BaseItem.h"

#include <math.h>

#include "../../resources/Constants.h"

namespace NSDocxRenderer
{
	CBaseItem& CBaseItem::operator=(const CBaseItem& oSrc)
	{
		if (this == &oSrc)
			return *this;

		m_dLeft = oSrc.m_dLeft;
		m_dTop = oSrc.m_dTop;
		m_dWidth = oSrc.m_dWidth;
		m_dHeight = oSrc.m_dHeight;
		m_dBaselinePos = oSrc.m_dBaselinePos;
		m_dRight = oSrc.m_dRight;

		return *this;
	}

	eVerticalCrossingType CBaseItem::GetVerticalCrossingType(const CBaseItem* oSrc) const
	{
		if (m_dTop > oSrc->m_dTop && m_dBaselinePos < oSrc->m_dBaselinePos)
		{
			return eVerticalCrossingType::vctCurrentInsideNext;
		}
		else if (m_dTop < oSrc->m_dTop && m_dBaselinePos > oSrc->m_dBaselinePos)
		{
			return  eVerticalCrossingType::vctCurrentOutsideNext;
		}
		else if (m_dTop < oSrc->m_dTop && m_dBaselinePos < oSrc->m_dBaselinePos &&
				 (m_dBaselinePos >= oSrc->m_dTop || fabs(m_dBaselinePos - oSrc->m_dTop) < c_dTHE_SAME_STRING_Y_PRECISION_MM))
		{
			return  eVerticalCrossingType::vctCurrentAboveNext;
		}
		else if (m_dTop > oSrc->m_dTop && m_dBaselinePos > oSrc->m_dBaselinePos &&
				 (m_dTop <= oSrc->m_dBaselinePos || fabs(m_dTop - oSrc->m_dBaselinePos) < c_dTHE_SAME_STRING_Y_PRECISION_MM))
		{
			return  eVerticalCrossingType::vctCurrentBelowNext;
		}
		else if (m_dTop == oSrc->m_dTop && m_dBaselinePos == oSrc->m_dBaselinePos &&
				 m_dLeft == oSrc->m_dLeft && m_dRight == oSrc->m_dRight)
		{
			return  eVerticalCrossingType::vctDublicate;
		}
		else if (fabs(m_dTop - oSrc->m_dTop) < c_dTHE_SAME_STRING_Y_PRECISION_MM &&
				 fabs(m_dBaselinePos - oSrc->m_dBaselinePos) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
		{
			return  eVerticalCrossingType::vctTopAndBottomBordersMatch;
		}
		else if (fabs(m_dTop - oSrc->m_dTop) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
		{
			return  eVerticalCrossingType::vctTopBorderMatch;
		}
		else if (fabs(m_dBaselinePos - oSrc->m_dBaselinePos) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
		{
			return  eVerticalCrossingType::vctBottomBorderMatch;
		}
		else if (m_dBaselinePos < oSrc->m_dTop)
		{
			return  eVerticalCrossingType::vctNoCrossingCurrentAboveNext;
		}
		else if (m_dTop > oSrc->m_dBaselinePos)
		{
			return  eVerticalCrossingType::vctNoCrossingCurrentBelowNext;
		}
		else
		{
			return  eVerticalCrossingType::vctUnknown;
		}
	}
	eHorizontalCrossingType CBaseItem::GetHorizontalCrossingType(const CBaseItem* oSrc) const
	{
		if (m_dLeft > oSrc->m_dLeft && m_dRight < oSrc->m_dRight)
		{
			return eHorizontalCrossingType::hctCurrentInsideNext;
		}
		else if (m_dLeft < oSrc->m_dLeft && m_dRight > oSrc->m_dRight)
		{
			return  eHorizontalCrossingType::hctCurrentOutsideNext;
		}
		else if (m_dLeft < oSrc->m_dLeft && m_dRight < oSrc->m_dRight &&
				 (m_dRight >= oSrc->m_dLeft || fabs(m_dRight - oSrc->m_dLeft) < c_dTHE_SAME_STRING_Y_PRECISION_MM))
		{
			return  eHorizontalCrossingType::hctCurrentLeftOfNext;
		}
		else if (m_dLeft > oSrc->m_dLeft && m_dRight > oSrc->m_dRight &&
				 (m_dLeft <= oSrc->m_dRight || fabs(m_dLeft - oSrc->m_dRight) < c_dTHE_SAME_STRING_Y_PRECISION_MM))
		{
			return  eHorizontalCrossingType::hctCurrentRightOfNext;
		}
		else if (m_dLeft == oSrc->m_dLeft && m_dRight == oSrc->m_dRight &&
				 m_dTop == oSrc->m_dTop && m_dBaselinePos == oSrc->m_dBaselinePos)
		{
			return  eHorizontalCrossingType::hctDublicate;
		}
		else if (fabs(m_dLeft - oSrc->m_dLeft) < c_dTHE_SAME_STRING_Y_PRECISION_MM &&
				 fabs(m_dRight - oSrc->m_dRight) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
		{
			return  eHorizontalCrossingType::hctLeftAndRightBordersMatch;
		}
		else if (fabs(m_dLeft - oSrc->m_dLeft) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
		{
			return  eHorizontalCrossingType::hctLeftBorderMatch;
		}
		else if (fabs(m_dRight - oSrc->m_dRight) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
		{
			return  eHorizontalCrossingType::hctRightBorderMatch;
		}
		else if (m_dRight < oSrc->m_dLeft)
		{
			return  eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext;
		}
		else if (m_dLeft > oSrc->m_dRight)
		{
			return  eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext;
		}
		else
		{
			return  eHorizontalCrossingType::hctUnknown;
		}
	}

	bool CBaseItem::AreObjectsNoCrossingByVertically(const CBaseItem* pObj) const noexcept
	{
		eVerticalCrossingType eVType = GetVerticalCrossingType(pObj);

		return (eVType == eVerticalCrossingType::vctNoCrossingCurrentAboveNext ||
				eVType == eVerticalCrossingType::vctNoCrossingCurrentBelowNext);
	}
	bool CBaseItem::AreObjectsNoCrossingByHorizontally(const CBaseItem* pObj) const noexcept
	{
		eHorizontalCrossingType eHType = GetHorizontalCrossingType(pObj);

		return (eHType == eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext ||
				eHType == eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext);
	}

	void CBaseItem::RecalcWithNewItem(const CBaseItem* pItem)
	{
		m_dBaselinePos = std::max(m_dBaselinePos, pItem->m_dBaselinePos);

		if ((pItem->m_dLeft < m_dLeft) || (pItem->m_dLeft > 0 && m_dLeft == 0.0))
			m_dLeft = pItem->m_dLeft;

		if ((pItem->m_dRight > m_dRight) || (pItem->m_dRight > 0 && m_dRight == 0.0))
			m_dRight = pItem->m_dRight;

		if (m_dTop > pItem->m_dTop || m_dTop == 0.0)
			m_dTop = pItem->m_dTop;

		m_dWidth = m_dRight - m_dLeft;
		m_dHeight = m_dBaselinePos - m_dTop;
	}
}
