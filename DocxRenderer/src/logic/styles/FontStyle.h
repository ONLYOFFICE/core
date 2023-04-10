#pragma once
#include <memory>

#include "../DesktopEditor/graphics/structures.h"
#include "../DesktopEditor/common/StringBuilder.h"


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

		const std::wstring& GetFontStyleId() const noexcept;
		const std::wstring& GetFontName() const noexcept;
		const NSStructures::CBrush& GetBrush() const noexcept;

		double GetFontSize() const noexcept;
		bool IsBold() const noexcept;
		bool IsItalic() const noexcept;

		void SetFontName(const std::wstring& wsFontName);
		void SetBrush(const NSStructures::CBrush& oBrush);

		void SetFontSize(double dFontSize);
		void SetBold(bool bBold);
		void SetItalic(double bItalic);

		void CopyNoId(const CFontStyle& oSrc);
		void ToXml(NSStringUtils::CStringBuilder& oWriter);

	private:
		std::wstring m_wsFontStyleId {L""};
		NSStructures::CBrush m_oBrush;
		std::wstring m_wsFontName {L""};
		double m_dFontSize {0};
		bool m_bItalic {false};
		bool m_bBold {false};

		const std::wstring m_wsIdStart = L"fontstyle";
	};
}


