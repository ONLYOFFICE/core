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
		for (size_t i = 0; i < m_arStyles.size(); ++i)
		{
			if (m_arStyles[i]->IsEqual(m_pCurrentStyle))
			{
				return m_arStyles[i];
			}
		}

		m_arStyles.push_back(m_pCurrentStyle);

		auto pStyle = m_pCurrentStyle;

		m_pCurrentStyle = std::make_shared<CFontStyle>();

		return pStyle;
	}
}
