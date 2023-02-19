#include "BaseItem.h"
#include "../../resources/Constants.h"
#include <math.h>

namespace NSDocxRenderer
{
	CBaseItem& CBaseItem::operator=(const CBaseItem& oSrc)
	{
		if (this == &oSrc)
		{
			return *this;
		}

		m_eType                 = oSrc.m_eType;
		m_bIsNotNecessaryToUse	= oSrc.m_bIsNotNecessaryToUse;

		m_dLeft = oSrc.m_dLeft;
		m_dTop = oSrc.m_dTop;
		m_dWidth = oSrc.m_dWidth;
		m_dHeight = oSrc.m_dHeight;
		m_dBaselinePos = oSrc.m_dBaselinePos;
		m_dRight = oSrc.m_dRight;

		return *this;
	}

	void CBaseItem::AddContent(CBaseItem* pObj)
	{
		m_dBaselinePos = std::max(m_dBaselinePos, pObj->m_dBaselinePos);

		if ((pObj->m_dLeft < m_dLeft) || (pObj->m_dLeft > 0 && m_dLeft == 0.0))
		{
			m_dLeft = pObj->m_dLeft;
		}

		if ((pObj->m_dRight > m_dRight) || (pObj->m_dRight > 0 && m_dRight == 0.0))
		{
			m_dRight = pObj->m_dRight;
		}

		if (m_dTop > pObj->m_dTop || m_dTop == 0.0)
		{
			m_dTop = pObj->m_dTop;
		}

		m_dWidth = m_dRight - m_dLeft;
		m_dHeight = m_dBaselinePos - m_dTop;
	}

	eVerticalCrossingType CBaseItem::GetVerticalCrossingType(const CBaseItem* oSrc)
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

	eHorizontalCrossingType CBaseItem::GetHorizontalCrossingType(const CBaseItem* oSrc)
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

	bool CBaseItem::AreObjectsNoCrossingByVertically(const CBaseItem* pObj)
	{
		eVerticalCrossingType eVType = GetVerticalCrossingType(pObj);

		if (eVType == eVerticalCrossingType::vctNoCrossingCurrentAboveNext ||
				eVType == eVerticalCrossingType::vctNoCrossingCurrentBelowNext)
		{
			return true;
		}
		return false;
	}

	bool CBaseItem::AreObjectsNoCrossingByHorizontally(const CBaseItem* pObj)
	{
		eHorizontalCrossingType eHType = GetHorizontalCrossingType(pObj);

		if (eHType == eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext ||
				eHType == eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext)
		{
			return true;
		}
		return false;
	}

	double CBaseItem::CalculateBeforeSpacing(double dPreviousBaseline)
	{
		return m_dTop - dPreviousBaseline;
	}

	bool CBaseItem::IsCurrentLeftOfNext(const CBaseItem* oSrc)
	{
		return m_dLeft < oSrc->m_dLeft;
	}

	bool CBaseItem::IsCurrentAboveOfNext(const CBaseItem* oSrc)
	{
		return m_dBaselinePos < oSrc->m_dBaselinePos;
	}
}
