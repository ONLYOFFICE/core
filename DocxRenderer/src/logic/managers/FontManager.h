#pragma once
#include <list>
#include <vector>

#include "../../../../DesktopEditor/graphics/structures.h"
#include "../../../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../../../DesktopEditor/common/StringUTF32.h"

namespace NSDocxRenderer
{
	class CUnicodeRange
	{
	public:
		BYTE RangeNum {0};
		BYTE Range {0};

		int Start {0};
		int End {0};

		CUnicodeRange(const int& _start = 0, const int& _end = 0, const BYTE& _range = 0, const BYTE& _rangenum = 0);
	};

	// класс для проставления Ranges для подбора шрифта по символу
	class CUnicodeRanges
	{
	public:
		std::list<CUnicodeRange> m_arRanges;

	public:
		CUnicodeRanges();
		void CheckRange(const int& symbol, BYTE& Range, BYTE& RangeNum);
		void CheckRange(const int& symbol, int& Range1, int& Range2, int& Range3, int& Range4);
	};

	struct CFontMetrics
	{
		double dAscent {0.0};
		double dDescent {0.0};
		double dLineSpacing {0.0};
		double dEmHeight {0.0};
		double dBaselineOffset {0.0};
	};

	struct CFontSelectParams
	{
		// изначальные параметры, которые могут быть нам известны
		std::wstring wsDefaultName{L""};
		bool bDefaultBold{false};
		bool bDefaultItalic{false};

		SHORT lAvgWidth{-1};
		bool bIsFixedWidth{false};

		BYTE arPANOSE[10]{};
		std::vector<UINT> arSignature;

		CFontSelectParams() = default;
		CFontSelectParams(const CFontSelectParams& oOther);
		CFontSelectParams& operator=(const CFontSelectParams& oOther);
		bool operator==(const CFontSelectParams& oOther);
	};

	// подбирает шрифт по параметрам
	class CFontSelector
	{
	public:
		// структура для хранения уже подобранных шрифтов
		struct CFontSelectInfo
		{
			CFontSelectParams oFontSelectParams;
			BYTE lRangeNum;
			BYTE lRange;

			std::wstring wsSelectedName;
			bool bIsSelectedBold;
			bool bIsSelectedItalic;
		};

		CFontSelector(NSFonts::IApplicationFonts* pApplication);
		~CFontSelector();

		void SelectFont(const CFontSelectParams& oFontSelectParams,
						const CFontMetrics& oFontMetrics,
						const NSStringUtils::CStringUTF32& oText);
		std::wstring GetSelectedName() const noexcept;
		bool IsSelectedBold() const noexcept;
		bool IsSelectedItalic() const noexcept;

		const std::list<CFontSelectInfo>& GetCache() const;
		void ClearCache();

	private:
		std::list<CFontSelectInfo> m_arParamsCache;

		NSFonts::IFontManager* m_pManager;
		std::wstring m_wsSelectedName;
		bool m_bIsSelectedBold;
		bool m_bIsSelectedItalic;

		CUnicodeRanges m_oRanges;
		void CheckRanges(UINT& lRange1, UINT& lRange2, UINT& lRange3, UINT& lRange4, BYTE& lRangeNum, BYTE& lRange);

		void CheckFontNamePDF(std::wstring& wsName, bool& bBold, bool& bItalic);
		bool CheckFontNameStyle(std::wstring& wsName, const std::wstring& sStyle);
	};

	// грузит шрифт, его параметры и метрики + измеряет шрифт
	class CFontManager
	{
	public:
		enum MeasureType
		{
			mtGlyph	= 0,
			mtPosition	= 1
		};

		CFontManager(NSFonts::IApplicationFonts* pFonts);
		~CFontManager();

		void LoadFontByFile(const NSStructures::CFont& oFont);
		void LoadFontByName(const NSStructures::CFont& oFont);

		const CFontSelectParams& GetFontSelectParams() const noexcept;
		const CFontMetrics& GetFontMetrics() const noexcept;

		double GetFontHeight() const;
		double GetSpaceWidthMM() const;

		void SetStringGid(const LONG& lGid);

		void MeasureString(
			const std::wstring& wsText,
			double x,
			double y,
			double& dBoxX,
			double& dBoxY,
			double& dBoxWidth,
			double& dBoxHeight,
			MeasureType measureType) const;

		void MeasureStringGids(
			unsigned int* pGids,
			unsigned int count,
			double x,
			double y,
			double& dBoxX,
			double& dBoxY,
			double& dBoxWidth,
			double& dBoxHeight,
			MeasureType measureType) const;

		void ClearCache();
	private:
		NSFonts::IFontManager* m_pManager;

		NSStructures::CFont m_oFont;
		CFontMetrics m_oFontMetrics;
		CFontSelectParams m_oFontSelectParams;

		void LoadFontMetrics();
		void LoadFontSelectParams();

		void CheckPdfResources();

	};
}
