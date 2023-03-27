#pragma once
#include "../DesktopEditor/graphics/structures.h"
#include "../DesktopEditor/graphics/pro/Fonts.h"
#include "../DesktopEditor/common/StringUTF32.h"
#include <list>

namespace NSFontManager
{
    static NSFonts::IFontManager* CreateFontManager(NSFonts::IApplicationFonts* pApplication)
    {
        NSFonts::IFontManager* pManager = pApplication->GenerateFontManager();
        pManager->CreateOwnerCache(8);
        return pManager;
    }

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
            std::wstring					m_strPANOSE {L""};
            LONG							m_lStyle {0};
            std::vector<UINT>   			m_arSignature;
            bool							m_bIsFixedWidth {false};
            SHORT							m_lAvgWidth {-1};

        public:
            CFontAdvanced();

            CFontAdvanced(const CFontAdvanced& oSrc);

            CFontAdvanced& operator=(const CFontAdvanced& oSrc);
    };

    class CFontPickUp
    {
        public:
            CFontAdvanced	m_oFont;
            BYTE			m_lRangeNum {0xFF};
            BYTE			m_lRange {0xFF};
            std::wstring	m_strPickFont {L""};
            LONG			m_lPickStyle  {0};

        public:
            CFontPickUp();
            CFontPickUp(const CFontPickUp& oSrc);
            CFontPickUp& operator=(const CFontPickUp& oSrc);
    };

    class CFontManagerBase
    {
        public:
            enum MeasureType
            {
                mtGlyph	= 0,
                mtPosition	= 1
            };

        protected:
            NSFonts::IFontManager*          m_pManager;
            std::wstring					m_strDefaultFont;

        public:

            CFontAdvanced					m_oFont;

            //для подбора шрифтов
            CUnicodeRanges                  m_oRanges;

            std::list<CFontPickUp>			m_arListPicUps;
            std::wstring					m_strCurrentPickFont;
            LONG							m_lCurrentPictFontStyle;

        public:
            CFontManagerBase(NSFonts::IApplicationFonts* pFonts);
            virtual ~CFontManagerBase();

            void ClearPickUps();

        public:
            void SetDefaultFont(const std::wstring& strName);
            std::wstring GetDefaultFont();

            virtual void LoadFont(long lFaceIndex = 0, bool bIsNeedAddToMap = true);

            void LoadFontByName(const std::wstring& strName, const double& dSize, const LONG& lStyle, const double& dDpiX, const double& dDpiY);

            void LoadFontByFile(const std::wstring& strPath, const double& dSize, const double& dDpiX, const double& dDpiY, const LONG& lFaceIndex);

        public:
            virtual void MeasureString(const std::wstring& sText, double x, double y, double& dBoxX, double& dBoxY,
                                       double& dBoxWidth, double& dBoxHeight, MeasureType measureType) = 0;
            virtual void CalculateBaselineOffset();

        public:
            void LoadFontMetrics();

            std::wstring ToHexString( BYTE uc );

            BYTE FromHexString( wchar_t c1, wchar_t c2 );

            void LoadFontParams(bool bIsPath = true);

        private:
            void CheckRanges(UINT& lRange1, UINT& lRange2, UINT& lRange3, UINT& lRange4, BYTE& lRangeNum, BYTE& lRange);

        public:
            bool GenerateFontName(NSStringUtils::CStringUTF32& oText);

            bool CheckFontNameStyle(std::wstring& sName, const std::wstring& sStyle);

            void CheckFontNamePDF(std::wstring& sName, bool& bBold, bool& bItalic);
    };
}
