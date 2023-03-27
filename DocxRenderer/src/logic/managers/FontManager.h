#pragma once
#include "FontManagerBase.h"
#include "../DesktopEditor/graphics/Matrix.h"

namespace NSDocxRenderer
{
    using namespace NSFontManager;

    class CFontTableEntry
    {
        public:
            std::wstring    	m_strFamilyName {L""};
            std::wstring		m_strPANOSE {L""};
            LONG				m_lStyle {0};
            std::vector<UINT>	m_arSignature;
            bool				m_bIsFixedWidth {false};

        public:
            CFontTableEntry(){}
            virtual ~CFontTableEntry(){}

            CFontTableEntry(const CFontTableEntry& oSrc);

            CFontTableEntry& operator =(const CFontTableEntry& oSrc);
    };

    class CFontTable
    {
        public:
            std::map<std::wstring, CFontTableEntry> m_mapTable;

        public:
            CFontTable() : m_mapTable(){}
    };

    class CFontManager : public CFontManagerBase
    {
        public:
            NSStructures::CFont*	m_pFont {nullptr};
            Aggplus::CMatrix*		m_pTransform {nullptr};
            double					m_dSpaceWidthMM {0.0};

        public:
            CFontTable				m_oFontTable;

        public:
            CFontManager(NSFonts::IApplicationFonts* pFonts);
            virtual ~CFontManager(){}

        public:
            void Init();

            void AddFontToMap();

        public:
            virtual void LoadFont(long lFaceIndex = 0, bool bNeedAddToMap = true);

            virtual void MeasureString(const std::wstring& sText, double x, double y, double& dBoxX, double& dBoxY,
                               double& dBoxWidth, double& dBoxHeight, MeasureType measureType);

            void MeasureStringGids(unsigned int* pGids, unsigned int count, double x, double y,
                                   double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight, MeasureType measureType);

            double GetBaseLineOffset();

            double GetFontHeight();

            void SetStringGid(const LONG& lGid);

            void GenerateFontName2(NSStringUtils::CStringUTF32& oText);
    };

    class CFontManagerLight
    {
        private:
            std::wstring m_strFontName {L""};
            LONG	m_lFontStyle {0};
            double	m_dSize {0.0};

            double	m_dSpaceWidth {0.0};

            NSFonts::IFontManager*	m_pManager {nullptr};

        public:
            CFontManagerLight(NSFonts::IApplicationFonts* pFonts);
            virtual ~CFontManagerLight();

            double GetSpaceWidth();

        public:
            void LoadFont(std::wstring& strFontName, LONG& lStyle, const double &dSize, const bool& bIsGID);

            double MeasureStringWidth(const std::wstring& sText);
    };

}
