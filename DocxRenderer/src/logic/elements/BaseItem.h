#pragma once
#include "../DesktopEditor/common/StringBuilder.h"

namespace NSDocxRenderer
{
    enum class eVerticalCrossingType
    {
        vctUnknown,
        vctCurrentInsideNext,
        vctCurrentOutsideNext,
        vctCurrentAboveNext,
        vctCurrentBelowNext,
        vctDublicate,
        vctTopBordersMatch,
        vctBottomBordersMatch,
        vctNoCrossingCurrentAboveNext,
        vctNoCrossingCurrentBelowNext
    };

    enum class eHorizontalCrossingType
    {
        hctUnknown,
        hctCurrentInsideNext,
        hctCurrentOutsideNext,
        hctCurrentLeftOfNext,
        hctCurrentRightOfNext,
        hctDublicate,
        hctLeftBordersMatch,
        hctRightBordersMatch,
        hctNoCrossingCurrentLeftOfNext,
        hctNoCrossingCurrentRightOfNext
    };

    class CBaseItem
    {
    public:
        enum class ElemType
        {
            etContText	= 0,
            etTextLine  = 1,
            etParagraph	= 2,
            etImage		= 3,
            etShape		= 4,
            etOldShape  = 5,
        };

        ElemType m_eType;

        bool m_bIsNotNecessaryToUse {false};

        //General
        double m_dLeft {0.0};
        double m_dTop {0.0};
        double m_dWidth {0.0};
        double m_dHeight {0.0};

        //Secondary
        double m_dBaselinePos {0.0};
        double m_dRight {0.0};

    public:
        CBaseItem(const ElemType& eType): m_eType(eType) {}
        virtual ~CBaseItem() {}

        CBaseItem& operator=(const CBaseItem& oSrc);

        friend bool operator == (const CBaseItem& lh, const CBaseItem& rh)
        {
            return (lh.m_dLeft == rh.m_dLeft) ? true : false;
        }

        friend bool operator < (const CBaseItem& lh, const CBaseItem& rh)
        {
            return (lh.m_dLeft < rh.m_dLeft) ? true : false;
        }

        friend bool operator > (const CBaseItem& lh, const CBaseItem& rh)
        {
            return (lh.m_dLeft > rh.m_dLeft) ? true : false;
        }

        virtual bool IsBigger(const CBaseItem* oSrc);
        virtual bool IsBiggerOrEqual(const CBaseItem* oSrc);

        virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) = 0;
        virtual void Clear() = 0;

        eVerticalCrossingType GetVerticalCrossingType(const CBaseItem* oSrc);
        eHorizontalCrossingType GetHorizontalCrossingType(const CBaseItem* oSrc);
    };
}
