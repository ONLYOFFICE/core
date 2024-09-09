#pragma once
#include <list>

#include "../styles/FontStyle.h"

namespace NSDocxRenderer
{
	class CFontStyleManager
	{
	public:
		CFontStyleManager();
		~CFontStyleManager();

		void Clear();
		void ToXml(NSStringUtils::CStringBuilder& oWriter);

		std::shared_ptr<CFontStyle> GetOrAddFontStyle(const CFontStyle& oFontStyle);
		std::shared_ptr<CFontStyle> GetOrAddFontStyle(const NSStructures::CBrush& oBrush,
													  const std::wstring& wsFontName,
													  double dFontSize,
													  bool bItalic,
													  bool bBold);

	private:
		std::list<std::shared_ptr<CFontStyle>> m_arFontStyles;
	};
}

