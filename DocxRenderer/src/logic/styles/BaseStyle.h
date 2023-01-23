#pragma once
#include "../DesktopEditor/common/StringBuilder.h"

namespace NSDocxRenderer
{
    class CBaseStyle
    {
        protected:
        enum class eStyleType
        {
            stUnknown,
            stParagraph,
            stCharacter,
            stTable,
            stNumbering
        };

        public:
            CBaseStyle(const eStyleType& eType): m_eType(eType) {}
            virtual ~CBaseStyle() {}

            CBaseStyle& operator=(const CBaseStyle& oSrc)
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

        private:
            eStyleType m_eType {eStyleType::stUnknown};

        public:
            bool m_bIsNotNecessaryToUse {false};
    };

}

