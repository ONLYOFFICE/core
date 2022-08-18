#pragma once
#include <map>
#include "../styles/FontStyle.h"

namespace NSDocxRenderer
{
    class CStyleManager
    {
        public:
            std::map<std::wstring, std::shared_ptr<CFontStyle>>	m_mapStyles;

            std::shared_ptr<CFontStyle> m_pCurrentStyle;

        public:
            CStyleManager();
            virtual ~CStyleManager();

            void Clear();

            void NewDocument();

            std::shared_ptr<CFontStyle> GetStyle();
    };
}

