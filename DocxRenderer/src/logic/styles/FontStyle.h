#pragma once
#include <memory>
#include "BaseStyle.h"
#include "../managers/FontManager.h"


namespace NSDocxRenderer
{
    class CFontStyle : public CBaseStyle
    {
        public:
            NSStructures::CFont		m_oFont;
            NSStructures::CBrush	m_oBrush;

            std::wstring m_strPickFontName {L""};
            LONG	m_lPickFontStyle {0};

        private:
            std::wstring m_strStyleId {L""};

        public:
            CFontStyle();
            ~CFontStyle(){}

            CFontStyle& operator=(const CFontStyle& oSrc);
            void CopyFormat(const CFontStyle& oSrc);

            void ToXml(NSStringUtils::CStringBuilder& oWriter) override final;

            bool IsEqual(std::shared_ptr<CFontStyle> oSrc);

            std::wstring GetStyleId() {return m_strStyleId;}
    };
}


