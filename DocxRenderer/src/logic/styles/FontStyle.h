#pragma once
#include <memory>

#include "../../../../DesktopEditor/graphics/structures.h"
#include "../../../../DesktopEditor/common/StringBuilder.h"

namespace NSDocxRenderer
{
	class CFontStyle
	{
	public:
		CFontStyle();
		CFontStyle(const CFontStyle& oFontStyle);
		~CFontStyle();

		CFontStyle& operator=(const CFontStyle& oSrc);
		bool operator==(const CFontStyle& oSrc);

		void ToXml(NSStringUtils::CStringBuilder& oWriter);
		void UpdateAvgSpaceWidth(double dWidth);
		double GetAvgSpaceWidth() const;

		std::wstring wsFontStyleId {L""};
		NSStructures::CBrush oBrush;
		std::wstring wsFontName {L""};
		double dFontSize {0};
		bool bItalic {false};
		bool bBold {false};


	private:
		const std::wstring m_wsIdStart = L"fontstyle";
		double dAvgSpaceWidth {0};
		size_t m_nN {0};
	};
}


