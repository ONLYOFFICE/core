#pragma once
#include <list>
#include <vector>

#include "../DesktopEditor/graphics/Matrix.h"
#include "../DesktopEditor/graphics/structures.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"
#include "../DesktopEditor/common/StringUTF32.h"

namespace NSDocxRenderer
{
	class CUnicodeRange
	{
	public:
		BYTE RangeNum {0};
		BYTE Range {0};

		int Start {0};
		int End {0};

		CUnicodeRange(const int& _start = 0,
					  const int& _end = 0,
					  const BYTE& _range = 0,
					  const BYTE& _rangenum = 0);
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

	class CFontAdvanced
	{
	public:
		NSStructures::CFont m_oFont;

		// font metrics
		double							m_dAscent {0.0};
		double							m_dDescent {0.0};
		double							m_dLineSpacing {0.0};
		double							m_dEmHeight {0.0};
		double							m_dBaselineOffset {0.0};
		double							m_dSpaceWidthMM {0.0};

		// font params
		std::wstring					m_strFamilyName {L""};
		BYTE							m_arPANOSE[10] {};
		std::vector<UINT>   			m_arSignature;
		SHORT							m_lAvgWidth {-1};
		bool							m_bIsFixedWidth {false};

	public:
		CFontAdvanced();
		CFontAdvanced(const CFontAdvanced& oSrc);
		CFontAdvanced& operator=(const CFontAdvanced& oSrc);
	};

	class CFontManager
	{
	public:
		enum MeasureType
		{
			mtGlyph	= 0,
			mtPosition	= 1
		};

		CFontManager(NSFonts::IApplicationFonts* pFonts);
		virtual ~CFontManager();

		void LoadFont();

		void LoadFontByName(const std::wstring& strName, const double& dSize, const LONG& lStyle, const double& dDpiX, const double& dDpiY);
		void LoadFontByFile(const std::wstring& strPath, const double& dSize, const double& dDpiX, const double& dDpiY, const LONG& lFaceIndex);

		void SetFont(const NSStructures::CFont& oFont);
		void SetTransform(const Aggplus::CMatrix* pTransform);

		CFontAdvanced GetFontAdvanced() const noexcept;

		void SetDefaultFont(const std::wstring& strName);

		bool GenerateFontName(NSStringUtils::CStringUTF32& oText);


		void MeasureString(const std::wstring& sText, double x, double y, double& dBoxX, double& dBoxY,
						   double& dBoxWidth, double& dBoxHeight, MeasureType measureType);

		void MeasureStringGids(unsigned int* pGids, unsigned int count, double x, double y,
							   double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight, MeasureType measureType);

		double GetFontHeight();
		void SetStringGid(const LONG& lGid);

	private:
		NSFonts::IFontManager*          m_pManager;
		std::wstring					m_strDefaultFont;

		CFontAdvanced					m_oFontAdvanced;
		const Aggplus::CMatrix*			m_pTransform;

		// для подбора шрифтов
		CUnicodeRanges                  m_oRanges;

		void CheckRanges(UINT& lRange1, UINT& lRange2, UINT& lRange3, UINT& lRange4, BYTE& lRangeNum, BYTE& lRange);

		void LoadFontMetrics();
		void LoadFontParams();

		bool CheckFontNameStyle(std::wstring& sName, const std::wstring& sStyle);
		void CheckFontNamePDF(std::wstring& sName, bool& bBold, bool& bItalic);
	};
}
