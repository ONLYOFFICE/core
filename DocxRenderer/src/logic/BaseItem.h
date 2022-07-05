#pragma once
#include "../DesktopEditor/common/StringBuilder.h"

namespace NSDocxRenderer
{
    class CBaseItem
    {
    public:
        enum ElemType
        {
            etContText	= 0,
            etTextLine  = 1,
            etParagraph	= 2,
            etImage		= 3,
            etShape		= 4,
            etOldShape  = 5,
        };

        ElemType m_eType;

        bool m_bIsNotNecessaryToUse;

        double m_dLeft;
        double m_dTop;
        double m_dWidth;
        double m_dHeight;

    public:
        CBaseItem(const ElemType& eType):
            m_eType(eType), m_bIsNotNecessaryToUse(false),
            m_dLeft(0), m_dTop(0), m_dWidth(0), m_dHeight(0) {}
        virtual ~CBaseItem() {}

        CBaseItem& operator=(const CBaseItem& oSrc)
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

            return *this;
        }

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

        virtual bool IsBigger(const CBaseItem* oSrc)
        {
            return (m_dLeft > oSrc->m_dLeft) ? true : false;
        }

        virtual bool IsBiggerOrEqual(const CBaseItem* oSrc)
        {
            return (m_dLeft >= oSrc->m_dLeft) ? true : false;
        }


        virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) = 0;
        virtual void Clear() = 0;
    };
}
