#include "StyleManager.h"
#include <utility>

namespace NSDocxRenderer
{
    CStyleManager::CStyleManager()
    {
        m_pCurrentStyle = new CFontStyle();
    }

    CStyleManager::~CStyleManager()
    {
        Clear();
        delete m_pCurrentStyle;
    }

    void CStyleManager::Clear()
    {
        for (auto pStyle : m_mapStyles)
        {
            RELEASEOBJECT(pStyle.second);
        }
        m_mapStyles.clear();
    }

    void CStyleManager::NewDocument()
    {
        Clear();
    }

    CFontStyle* CStyleManager::GetStyle()
    {
        for (auto pStyle : m_mapStyles)
        {
            if (pStyle.second->IsEqual(m_pCurrentStyle))
            {
                return pStyle.second;
            }
        }

        auto pairStyle = std::make_pair(m_pCurrentStyle->GetStyleId(), m_pCurrentStyle);

        m_mapStyles.insert(pairStyle);

        m_pCurrentStyle = new CFontStyle();

        return pairStyle.second;
    }
}
