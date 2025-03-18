#include "BaseItem.h"

#include <math.h>

#include "../../resources/Constants.h"

namespace NSDocxRenderer
{
	CBaseItem::CBaseItem()
	{
	}
	CBaseItem::CBaseItem(const CBaseItem& other) :
	    m_dTop(other.m_dTop),
	    m_dBot(other.m_dBot),
	    m_dLeft(other.m_dLeft),
	    m_dRight(other.m_dRight),
	    m_dHeight(other.m_dHeight),
	    m_dWidth(other.m_dWidth)
	{
	}

	CBaseItem::CBaseItem(CBaseItem&& other) :
	    m_dTop(other.m_dTop),
	    m_dBot(other.m_dBot),
	    m_dLeft(other.m_dLeft),
	    m_dRight(other.m_dRight),
	    m_dHeight(other.m_dHeight),
	    m_dWidth(other.m_dWidth)
	{
	}
	CBaseItem::~CBaseItem()
	{
	}

	CBaseItem& CBaseItem::operator=(const CBaseItem& other)
	{
		if (this == &other)
			return *this;

		m_dTop = other.m_dTop;
		m_dBot = other.m_dBot;
		m_dLeft = other.m_dLeft;
		m_dRight = other.m_dRight;
		m_dHeight = other.m_dHeight;
		m_dWidth = other.m_dWidth;
		return *this;
	}
	CBaseItem& CBaseItem::operator=(CBaseItem&& other)
	{
		if (this == &other)
			return *this;

		m_dTop = other.m_dTop;
		m_dBot = other.m_dBot;
		m_dLeft = other.m_dLeft;
		m_dRight = other.m_dRight;
		m_dHeight = other.m_dHeight;
		m_dWidth = other.m_dWidth;
		return *this;
	}

	eVerticalCrossingType CBaseItem::GetVerticalCrossingType(const CBaseItem* pBaseItem) const
	{
		if (m_dTop > pBaseItem->m_dTop && m_dBot < pBaseItem->m_dBot)
		{
			return eVerticalCrossingType::vctCurrentInsideNext;
		}
		else if (m_dTop < pBaseItem->m_dTop && m_dBot > pBaseItem->m_dBot)
		{
			return  eVerticalCrossingType::vctCurrentOutsideNext;
		}
		else if (m_dTop < pBaseItem->m_dTop && m_dBot < pBaseItem->m_dBot &&
		         (m_dBot >= pBaseItem->m_dTop || fabs(m_dBot - pBaseItem->m_dTop) < c_dTHE_SAME_STRING_Y_PRECISION_MM))
		{
			return  eVerticalCrossingType::vctCurrentAboveNext;
		}
		else if (m_dTop > pBaseItem->m_dTop && m_dBot > pBaseItem->m_dBot &&
		         (m_dTop <= pBaseItem->m_dBot || fabs(m_dTop - pBaseItem->m_dBot) < c_dTHE_SAME_STRING_Y_PRECISION_MM))
		{
			return  eVerticalCrossingType::vctCurrentBelowNext;
		}
		else if (m_dTop == pBaseItem->m_dTop && m_dBot == pBaseItem->m_dBot &&
		         m_dLeft == pBaseItem->m_dLeft && m_dRight == pBaseItem->m_dRight)
		{
			return  eVerticalCrossingType::vctDublicate;
		}
		else if (fabs(m_dTop - pBaseItem->m_dTop) < c_dTHE_SAME_STRING_Y_PRECISION_MM &&
		         fabs(m_dBot - pBaseItem->m_dBot) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
		{
			return  eVerticalCrossingType::vctTopAndBottomBordersMatch;
		}
		else if (fabs(m_dTop - pBaseItem->m_dTop) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
		{
			return  eVerticalCrossingType::vctTopBorderMatch;
		}
		else if (fabs(m_dBot - pBaseItem->m_dBot) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
		{
			return  eVerticalCrossingType::vctBottomBorderMatch;
		}
		else if (m_dBot < pBaseItem->m_dTop)
		{
			return  eVerticalCrossingType::vctNoCrossingCurrentAboveNext;
		}
		else if (m_dTop > pBaseItem->m_dBot)
		{
			return  eVerticalCrossingType::vctNoCrossingCurrentBelowNext;
		}
		else
		{
			return  eVerticalCrossingType::vctUnknown;
		}
	}
	eHorizontalCrossingType CBaseItem::GetHorizontalCrossingType(const CBaseItem* pBaseItem) const
	{
		if (m_dLeft > pBaseItem->m_dLeft && m_dRight < pBaseItem->m_dRight)
		{
			return eHorizontalCrossingType::hctCurrentInsideNext;
		}
		else if (m_dLeft < pBaseItem->m_dLeft && m_dRight > pBaseItem->m_dRight)
		{
			return  eHorizontalCrossingType::hctCurrentOutsideNext;
		}
		else if (m_dLeft < pBaseItem->m_dLeft && m_dRight < pBaseItem->m_dRight &&
		         (m_dRight >= pBaseItem->m_dLeft || fabs(m_dRight - pBaseItem->m_dLeft) < c_dTHE_SAME_STRING_X_PRECISION_MM))
		{
			return  eHorizontalCrossingType::hctCurrentLeftOfNext;
		}
		else if (m_dLeft > pBaseItem->m_dLeft && m_dRight > pBaseItem->m_dRight &&
		         (m_dLeft <= pBaseItem->m_dRight || fabs(m_dLeft - pBaseItem->m_dRight) < c_dTHE_SAME_STRING_X_PRECISION_MM))
		{
			return  eHorizontalCrossingType::hctCurrentRightOfNext;
		}
		else if (m_dLeft == pBaseItem->m_dLeft && m_dRight == pBaseItem->m_dRight &&
		         m_dTop == pBaseItem->m_dTop && m_dBot == pBaseItem->m_dBot)
		{
			return  eHorizontalCrossingType::hctDublicate;
		}
		else if (fabs(m_dLeft - pBaseItem->m_dLeft) < c_dTHE_SAME_STRING_X_PRECISION_MM &&
		         fabs(m_dRight - pBaseItem->m_dRight) < c_dTHE_SAME_STRING_X_PRECISION_MM)
		{
			return  eHorizontalCrossingType::hctLeftAndRightBordersMatch;
		}
		else if (fabs(m_dLeft - pBaseItem->m_dLeft) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
		{
			return  eHorizontalCrossingType::hctLeftBorderMatch;
		}
		else if (fabs(m_dRight - pBaseItem->m_dRight) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
		{
			return  eHorizontalCrossingType::hctRightBorderMatch;
		}
		else if (m_dRight < pBaseItem->m_dLeft)
		{
			return  eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext;
		}
		else if (m_dLeft > pBaseItem->m_dRight)
		{
			return  eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext;
		}
		else
		{
			return  eHorizontalCrossingType::hctUnknown;
		}
	}

	bool CBaseItem::AreObjectsNoCrossingByVertically(const CBaseItem* pBaseItem) const noexcept
	{
		eVerticalCrossingType eVType = GetVerticalCrossingType(pBaseItem);

		return (eVType == eVerticalCrossingType::vctNoCrossingCurrentAboveNext ||
		        eVType == eVerticalCrossingType::vctNoCrossingCurrentBelowNext);
	}
	bool CBaseItem::AreObjectsNoCrossingByHorizontally(const CBaseItem* pBaseItem) const noexcept
	{
		eHorizontalCrossingType eHType = GetHorizontalCrossingType(pBaseItem);

		return (eHType == eHorizontalCrossingType::hctNoCrossingCurrentLeftOfNext ||
		        eHType == eHorizontalCrossingType::hctNoCrossingCurrentRightOfNext);
	}
	bool CBaseItem::IsEqual(double dTop, double dBot, double dLeft, double dRight) const noexcept
	{
		return m_dLeft == dLeft && m_dTop == dTop && m_dBot == dBot && m_dRight == dRight;
	}
	bool CBaseItem::operator==(const CBaseItem& other)
	{
		return IsEqual(other.m_dTop, other.m_dBot, other.m_dLeft, other.m_dRight);
	}

	void CBaseItem::RecalcWithNewItem(const CBaseItem* pItem)
	{
		m_dBot = std::max(m_dBot, pItem->m_dBot);

		if ((pItem->m_dLeft < m_dLeft) || (pItem->m_dLeft > 0 && m_dLeft == 0.0))
			m_dLeft = pItem->m_dLeft;

		if ((pItem->m_dRight > m_dRight) || (pItem->m_dRight > 0 && m_dRight == 0.0))
			m_dRight = pItem->m_dRight;

		if (m_dTop > pItem->m_dTop || m_dTop == 0.0)
			m_dTop = pItem->m_dTop;

		m_dWidth = m_dRight - m_dLeft;
		m_dHeight = m_dBot - m_dTop;
	}
}
