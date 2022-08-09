#pragma once
#include <map>
#include "../DesktopEditor/common/StringBuilder.h"
#include "../styles/FontStyle.h"

namespace NSDocxRenderer
{
    class CStyleManager
    {
        public:
            std::map<std::wstring, CFontStyle*>	m_mapStyles;

            CFontStyle* m_pCurrentStyle;

        public:
            CStyleManager();
            virtual ~CStyleManager();

            void Clear();

            void NewDocument();

            CFontStyle* GetStyle();
    };
}

