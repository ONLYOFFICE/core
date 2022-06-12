#pragma once
#include "Common.h"
#include "FontManagerBase.h"
#include "../../DocxRenderer.h"

namespace NSDocxRenderer
{
    using namespace NSFontManager;

    class CFontTableEntry
    {
        public:
            std::wstring    	m_strFamilyName;
            std::wstring		m_strPANOSE;
            LONG				m_lStyle;
            std::vector<UINT>	m_arSignature;
            bool				m_bIsFixedWidth;

        public:
            CFontTableEntry();
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
            NSStructures::CFont*	m_pFont;
            Aggplus::CMatrix*		m_pTransform;
            double					m_dSpaceWidthMM;

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

            void MeasureString(std::wstring sText, double x, double y, double& dBoxX, double& dBoxY,
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
            std::wstring m_strFontName;
            LONG	m_lFontStyle;
            double	m_dSize;

            double	m_dSpaceWidth;

            NSFonts::IFontManager*	m_pManager;

        public:
            CFontManagerLight(NSFonts::IApplicationFonts* pFonts);
            virtual ~CFontManagerLight();

            double GetSpaceWidth();

        public:
            void LoadFont(std::wstring& strFontName, LONG& lStyle, double& dSize, const bool& bIsGID);

            double MeasureStringWidth(const std::wstring& sText);
    };

}
