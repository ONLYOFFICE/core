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

        bool CBaseItem::IsBigger(const CBaseItem* oSrc)
        {
            return (m_dLeft > oSrc->m_dLeft) ? true : false;
        }

        bool CBaseItem::IsBiggerOrEqual(const CBaseItem* oSrc)
        {
            return (m_dLeft >= oSrc->m_dLeft) ? true : false;
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
            else if (m_dTop < oSrc->m_dTop && m_dBaselinePos < oSrc->m_dBaselinePos && m_dBaselinePos > oSrc->m_dTop)
            {
                return  eVerticalCrossingType::vctCurrentAboveNext;
            }
            else if (m_dTop > oSrc->m_dTop && m_dBaselinePos > oSrc->m_dBaselinePos && m_dTop < oSrc->m_dBaselinePos)
            {
                return  eVerticalCrossingType::vctCurrentBelowNext;
            }
            else if (m_dTop == oSrc->m_dTop && m_dBaselinePos == oSrc->m_dBaselinePos &&
                     m_dLeft == oSrc->m_dLeft && m_dRight == oSrc->m_dRight)
            {
                return  eVerticalCrossingType::vctDublicate;
            }
            else if (fabs(m_dTop - oSrc->m_dTop) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
            {
                return  eVerticalCrossingType::vctTopBordersMatch;
            }
            else if (fabs(m_dBaselinePos - oSrc->m_dBaselinePos) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
            {
                return  eVerticalCrossingType::vctBottomBordersMatch;
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
            else if (m_dLeft < oSrc->m_dLeft && m_dRight < oSrc->m_dRight && m_dRight > oSrc->m_dLeft)
            {
                return  eHorizontalCrossingType::hctCurrentLeftOfNext;
            }
            else if (m_dLeft > oSrc->m_dLeft && m_dRight > oSrc->m_dRight && m_dLeft < oSrc->m_dRight)
            {
                return  eHorizontalCrossingType::hctCurrentRightOfNext;
            }
            else if (m_dLeft == oSrc->m_dLeft && m_dRight == oSrc->m_dRight &&
                     m_dTop == oSrc->m_dTop && m_dBaselinePos == oSrc->m_dBaselinePos)
            {
                return  eHorizontalCrossingType::hctDublicate;
            }
            else if (fabs(m_dLeft - oSrc->m_dLeft) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
            {
                return  eHorizontalCrossingType::hctLeftBordersMatch;
            }
            else if (fabs(m_dRight - oSrc->m_dRight) < c_dTHE_SAME_STRING_Y_PRECISION_MM)
            {
                return  eHorizontalCrossingType::hctRightBordersMatch;
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
}
