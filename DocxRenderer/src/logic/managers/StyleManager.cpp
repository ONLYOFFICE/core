#include "StyleManager.h"
#include <utility>

namespace NSDocxRenderer
{
    CStyleManager::CStyleManager()
    {
        m_pCurrentStyle = std::make_shared<CFontStyle>();
    }

    CStyleManager::~CStyleManager()
    {
        Clear();
    }

    void CStyleManager::Clear()
    {
        m_arStyles.clear();
    }

    void CStyleManager::NewDocument()
    {
        Clear();
    }

    std::shared_ptr<CFontStyle> CStyleManager::GetStyle()
    {
        for (const auto &pStyle : m_arStyles)
        {
            if (pStyle->IsEqual(m_pCurrentStyle))
            {
                return pStyle;
            }
        }

        m_arStyles.push_back(m_pCurrentStyle);

        auto pStyle = m_pCurrentStyle;

        m_pCurrentStyle = std::make_shared<CFontStyle>();

        return pStyle;
    }
}
