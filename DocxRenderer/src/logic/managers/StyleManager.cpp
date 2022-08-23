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
        m_mapStyles.clear();
    }

    void CStyleManager::NewDocument()
    {
        Clear();
    }

    std::shared_ptr<CFontStyle> CStyleManager::GetStyle()
    {
        for (const auto &pStyle : m_mapStyles)
        {
            if (pStyle.second->IsEqual(m_pCurrentStyle))
            {
                return pStyle.second;
            }
        }

        auto pairStyle = std::make_pair(m_pCurrentStyle->GetStyleId(), m_pCurrentStyle);

        m_mapStyles.insert(pairStyle);

        m_pCurrentStyle = std::make_shared<CFontStyle>();

        return pairStyle.second;
    }
}
