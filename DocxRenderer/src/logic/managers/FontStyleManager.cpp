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
		return GetOrAddFontStyle(oFontStyle.GetBrush(),
						  oFontStyle.GetFontName(),
						  oFontStyle.GetFontSize(),
						  oFontStyle.IsItalic(),
						  oFontStyle.IsBold());
	}
	std::shared_ptr<const CFontStyle> CFontStyleManager::GetOrAddFontStyle(const NSStructures::CBrush& oBrush,
								   const std::wstring& wsFontName,
								   double dFontSize,
								   bool bItalic,
								   bool bBold)
	{
		for(auto& val : m_arFontStyles)
		{
			if(oBrush.Type == val->GetBrush().Type &&
					oBrush.Color1 == val->GetBrush().Color1 &&
					oBrush.Color2 == val->GetBrush().Color2 &&
					oBrush.Alpha1 == val->GetBrush().Alpha1 &&
					oBrush.Alpha2 == val->GetBrush().Alpha2 &&
					oBrush.LinearAngle == val->GetBrush().LinearAngle &&
					dFontSize == val->GetFontSize() &&
					wsFontName == val->GetFontName() &&
					(bItalic == val->IsItalic()) && (bBold == val->IsBold()))

				return val;
		}
		auto pFontStyle = std::make_shared<CFontStyle>();
		pFontStyle->SetBrush(oBrush);
		pFontStyle->SetFontName(wsFontName);
		pFontStyle->SetFontSize(dFontSize);
		pFontStyle->SetItalic(bItalic);
		pFontStyle->SetBold(bBold);

		m_arFontStyles.push_front(pFontStyle);
		return pFontStyle;
	}
}
