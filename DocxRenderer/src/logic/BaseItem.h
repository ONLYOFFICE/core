#pragma once
#include "../DesktopEditor/common/StringBuilder.h"

namespace NSDocxRenderer
{
    class CBaseItem
    {
    public:
        enum ElemType
        {
            etParagraph	= 0,
            etImage		= 1,
            etShape		= 2,
            etOldShape  = 3,
        };

        ElemType m_eType;

        bool m_bIsNotNecessaryToUse;

    public:
        CBaseItem(const ElemType& eType):
            m_eType(eType), m_bIsNotNecessaryToUse(false) {}
        virtual ~CBaseItem() {}

        CBaseItem& operator=(const CBaseItem& oSrc)
        {
            if (this == &oSrc)
            {
                return *this;
            }

            m_eType                 = oSrc.m_eType;
            m_bIsNotNecessaryToUse	= oSrc.m_bIsNotNecessaryToUse;

            return *this;
        }

        virtual void ToXml(NSStringUtils::CStringBuilder& oWriter) = 0;
        virtual void Clear() = 0;
    };
}
