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

	void CFontStyleManager::NewDocument()
	{
		Clear();
	}
	void CFontStyleManager::ToXml(NSStringUtils::CStringBuilder& oWriter)
	{
		for(auto& val : m_arFontStyles)
			val->ToXml(oWriter);
	}

	std::shared_ptr<const CFontStyle> CFontStyleManager::GetOrAddFontStyle(const CFontStyle& oFontStyle)
	{
		return GetOrAddFontStyle(oFontStyle.oBrush,
						  oFontStyle.wsFontName,
						  oFontStyle.dFontSize,
						  oFontStyle.bItalic,
						  oFontStyle.bBold);
	}
	std::shared_ptr<const CFontStyle> CFontStyleManager::GetOrAddFontStyle(const NSStructures::CBrush& oBrush,
								   const std::wstring& wsFontName,
								   double dFontSize,
								   bool bItalic,
								   bool bBold)
	{
		for(auto& val : m_arFontStyles)
		{
			if(oBrush.Type == val->oBrush.Type &&
					oBrush.Color1 == val->oBrush.Color1 &&
					oBrush.Color2 == val->oBrush.Color2 &&
					oBrush.Alpha1 == val->oBrush.Alpha1 &&
					oBrush.Alpha2 == val->oBrush.Alpha2 &&
					oBrush.LinearAngle == val->oBrush.LinearAngle &&
					dFontSize == val->dFontSize &&
					wsFontName == val->wsFontName &&
					(bItalic == val->bItalic) && (bBold == val->bBold))

				return val;
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
