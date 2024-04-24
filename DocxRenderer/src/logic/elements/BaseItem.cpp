/*
 * (c) Copyright Ascensio System SIA 2010-2024
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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
