#pragma once
#include <vector>
#include "../styles/FontStyle.h"

namespace NSDocxRenderer
{
    class CStyleManager
    {
        public:
            std::vector<std::shared_ptr<CFontStyle>> m_arStyles;

            std::shared_ptr<CFontStyle> m_pCurrentStyle;

        public:
            CStyleManager();
            virtual ~CStyleManager();

            void Clear();

            void NewDocument();

            std::shared_ptr<CFontStyle> GetStyle();
    };
}

