#include "FontStyleManager.h"

#include <utility>

namespace NSDocxRenderer
{
	CFontStyleManager::CFontStyleManager()
	{
	}

	CFontStyleManager::~CFontStyleManager()
	{
		Clear();
	}

	void CFontStyleManager::Clear()
	{
		m_arFontStyles.clear();
	}

	void CFontStyleManager::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		for(auto& val : m_arFontStyles)
			val->ToXml(oWriter);
	}

	std::shared_ptr<CFontStyle> CFontStyleManager::GetOrAddFontStyle(const CFontStyle& oFontStyle)
	{
		return GetOrAddFontStyle(
		            oFontStyle.oBrush,
		            oFontStyle.wsFontName,
		            oFontStyle.dFontSize,
		            oFontStyle.bItalic,
		            oFontStyle.bBold);
	}
	std::shared_ptr<CFontStyle> CFontStyleManager::GetOrAddFontStyle(
	        const NSStructures::CBrush& oBrush,
	        const std::wstring& wsFontName,
	        double dFontSize,
	        bool bItalic,
	        bool bBold)
	{
		for(auto it = m_arFontStyles.begin(); it != m_arFontStyles.end(); ++it)
		{
			if (oBrush.Type == (*it)->oBrush.Type &&
			        oBrush.Color1 == (*it)->oBrush.Color1 &&
			        oBrush.Color2 == (*it)->oBrush.Color2 &&
			        oBrush.Alpha1 == (*it)->oBrush.Alpha1 &&
			        oBrush.Alpha2 == (*it)->oBrush.Alpha2 &&
			        oBrush.LinearAngle == (*it)->oBrush.LinearAngle &&
			        dFontSize == (*it)->dFontSize &&
			        wsFontName == (*it)->wsFontName &&
			        (bItalic == (*it)->bItalic) && (bBold == (*it)->bBold))
			{
				auto val = *it;

				// в начало списка
				if (it != m_arFontStyles.begin())
				{
					m_arFontStyles.erase(it);
					m_arFontStyles.push_front(val);

				}
				return val;
			}
		}
		auto pFontStyle = std::make_shared<CFontStyle>();
		pFontStyle->oBrush = oBrush;
		pFontStyle->wsFontName = wsFontName;
		pFontStyle->dFontSize = dFontSize;
		pFontStyle->bItalic = bItalic;
		pFontStyle->bBold = bBold;

		m_arFontStyles.push_front(pFontStyle);
		return pFontStyle;
	}
}
