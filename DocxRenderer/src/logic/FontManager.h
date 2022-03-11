#pragma once
#include "Common.h"
#include "FontManagerBase.h"
#include "../../DocxRenderer.h"

namespace NSDocxRenderer
{
	using namespace NSFontManager;

	const double c_dDpiX		= 72.0;
	const double c_dDpiY		= 72.0;

	class CFontTableEntry
	{
	public:
        std::wstring    	m_strFamilyName;
        std::wstring		m_strPANOSE;
        LONG				m_lStyle;
        std::vector<UINT>	m_arSignature;
        bool				m_bIsFixedWidth;

	public:
		CFontTableEntry() : m_arSignature()
		{
            m_strFamilyName		= L"";
            m_strPANOSE			= L"";
			m_lStyle			= 0;
			m_bIsFixedWidth		= false;
		}
		~CFontTableEntry()
		{
		}
		CFontTableEntry(const CFontTableEntry& oSrc)
		{
			*this = oSrc;
		}
		CFontTableEntry& operator =(const CFontTableEntry& oSrc)
		{
			m_strFamilyName	= oSrc.m_strFamilyName;
			m_strPANOSE		= oSrc.m_strPANOSE;
			m_lStyle		= oSrc.m_lStyle;
            m_arSignature   = oSrc.m_arSignature;
			m_bIsFixedWidth	= oSrc.m_bIsFixedWidth;

			return *this;
		}
	};

	class CFontTable
	{
	public:
        std::map<std::wstring, CFontTableEntry> m_mapTable;

	public:
		CFontTable() : m_mapTable()
		{
		}
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
        CFontManager(NSFonts::IApplicationFonts* pFonts) : m_pFont(NULL), CFontManagerBase(pFonts)
		{
			m_pTransform = NULL;
			m_dSpaceWidthMM = 0;
		}
		virtual ~CFontManager()
		{
		}

	public:
        void Init()
		{
            m_oFontTable.m_mapTable.clear();
		}
        void AddFontToMap()
		{
            if (m_oFontTable.m_mapTable.end() != m_oFontTable.m_mapTable.find(m_oFont.m_strFamilyName))
            {
                CFontTableEntry oEntry;
                oEntry.m_strFamilyName	= m_oFont.m_strFamilyName;
                oEntry.m_strPANOSE		= m_oFont.m_strPANOSE;
                oEntry.m_lStyle			= m_oFont.m_lStyle;
                oEntry.m_bIsFixedWidth	= m_oFont.m_bIsFixedWidth;
                oEntry.m_arSignature    = m_oFont.m_arSignature;

                m_oFontTable.m_mapTable.insert(std::pair<std::wstring, CFontTableEntry>(m_oFont.m_strFamilyName, oEntry));
            }
		}

	public:
		virtual void LoadFont(long lFaceIndex = 0, bool bNeedAddToMap = true)
		{
			if (NULL == m_pManager)
				return;

			double dSize = m_pFont->Size;
            double dSizeFont = dSize * ((m_pTransform->sx() + m_pTransform->sy()) / 2);

			double dPix = m_pFont->CharSpace / c_dPixToMM;

			m_pFont->Size = dSizeFont;

            if (m_pFont->IsEqual2(&m_oFont.m_oFont))
			{
				m_pFont->Size = dSize;
				m_pManager->SetCharSpacing(dPix);
				m_pManager->SetStringGID(m_oFont.m_oFont.StringGID);
				return;
			}

			m_oFont.m_oFont	= *m_pFont;
			m_pFont->Size = dSize;

			bool bIsPath = false;

            if (m_pFont->Path.empty())
			{
				CFontManagerBase::LoadFontByName(m_oFont.m_oFont.Name, m_oFont.m_oFont.Size, m_oFont.m_oFont.GetStyle(), c_dDpiX, c_dDpiY);
			}
			else
			{
				CFontManagerBase::LoadFontByFile(m_oFont.m_oFont.Path, m_oFont.m_oFont.Size, c_dDpiX, c_dDpiY, lFaceIndex);

				m_pFont->SetStyle(m_oFont.m_lStyle);
				m_oFont.m_oFont.SetStyle(m_oFont.m_lStyle);

				bIsPath = true;
			}

            int bIsGID = m_pManager->GetStringGID();
			m_pManager->SetStringGID(FALSE);
			
            m_pManager->LoadString2(L" ", 0, 0);
            TBBox bbox = m_pManager->MeasureString2();

            m_dSpaceWidthMM = (double)(bbox.fMaxX - bbox.fMinX) * c_dPixToMM;
			if (0 >= m_dSpaceWidthMM)
			{
				m_dSpaceWidthMM = 1.0;
			}

            m_pManager->SetStringGID(bIsGID);

			if (bNeedAddToMap)
				AddFontToMap();
		}
		
        void MeasureString(std::wstring sText, double x, double y, double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight, MeasureType measureType)
		{
			LoadFont();

			dBoxX		= 0;
			dBoxY		= 0;
			dBoxWidth	= 0;
			dBoxHeight	= 0;

			if (NULL == m_pManager)
				return;

            m_pManager->LoadString1(sText, (float)x, (float)y);

            TBBox bbox;
			if (MeasureTypeGlyph == measureType)
			{
                bbox = m_pManager->MeasureString();
			}
			else if (MeasureTypePosition == measureType)
			{
                bbox = m_pManager->MeasureString2();
			}

            dBoxX		= (double)bbox.fMinX;
            dBoxY		= (double)bbox.fMinY;
            dBoxWidth	= (double)(bbox.fMaxX - bbox.fMinX);
            dBoxHeight	= (double)(bbox.fMaxY - bbox.fMinY);

			// переводим в миллиметры
			dBoxX		*= c_dPixToMM;
			dBoxY		*= c_dPixToMM;
			dBoxWidth	*= c_dPixToMM;
			dBoxHeight	*= c_dPixToMM;
		}
        void MeasureStringGids(unsigned int* pGids, unsigned int count, double x, double y, double& dBoxX, double& dBoxY, double& dBoxWidth, double& dBoxHeight, MeasureType measureType)
        {
            LoadFont();

            dBoxX		= 0;
            dBoxY		= 0;
            dBoxWidth	= 0;
            dBoxHeight	= 0;

            if (NULL == m_pManager)
                return;

            m_pManager->LoadString1(pGids, count, (float)x, (float)y);

            TBBox bbox;
            if (MeasureTypeGlyph == measureType)
            {
                bbox = m_pManager->MeasureString();
            }
            else if (MeasureTypePosition == measureType)
            {
                bbox = m_pManager->MeasureString2();
            }

            dBoxX		= (double)bbox.fMinX;
            dBoxY		= (double)bbox.fMinY;
            dBoxWidth	= (double)(bbox.fMaxX - bbox.fMinX);
            dBoxHeight	= (double)(bbox.fMaxY - bbox.fMinY);

            // переводим в миллиметры
            dBoxX		*= c_dPixToMM;
            dBoxY		*= c_dPixToMM;
            dBoxWidth	*= c_dPixToMM;
            dBoxHeight	*= c_dPixToMM;
        }

        inline double GetBaseLineOffset()
		{
			LoadFont();

			double d1 = 3 * (m_oFont.m_dLineSpacing - m_oFont.m_dDescent) - m_oFont.m_dAscent;
			d1 /= 2.0;

			d1 *= (m_oFont.m_oFont.Size / m_oFont.m_dEmHeight);
			return d1;
		}

        inline double GetFontHeight()
		{
			return c_dPtToMM * (m_oFont.m_dLineSpacing * m_oFont.m_oFont.Size ) / m_oFont.m_dEmHeight;
		}

        inline void SetStringGid(const LONG& lGid)
		{
			if (NULL != m_pManager)
				m_pManager->SetStringGID(lGid);
		}

        inline void GenerateFontName2(NSStringUtils::CStringUTF32& oText)
		{
            bool bIsNeedAddToMap = CFontManagerBase::GenerateFontName(oText);

			if (bIsNeedAddToMap)
			{
                if (m_oFontTable.m_mapTable.end() != m_oFontTable.m_mapTable.find(m_strCurrentPickFont))
                {
                    CFontTableEntry oEntry;
                    oEntry.m_strFamilyName	= m_strCurrentPickFont;
                    oEntry.m_strPANOSE		= m_oFont.m_strPANOSE;
                    oEntry.m_lStyle			= m_oFont.m_lStyle;
                    oEntry.m_bIsFixedWidth	= m_oFont.m_bIsFixedWidth;
                    oEntry.m_arSignature    = m_oFont.m_arSignature;

                    m_oFontTable.m_mapTable.insert(std::pair<std::wstring, CFontTableEntry>(m_oFont.m_strFamilyName, oEntry));
                }
			}
		}
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
        CFontManagerLight(NSFonts::IApplicationFonts* pFonts)
		{
            m_strFontName	= L"";
			m_lFontStyle	= 0;
			m_dSize			= 0;
            m_dSpaceWidth	= 0;

            m_pManager = NSFontManager::CreateFontManager(pFonts);
		}
		~CFontManagerLight()
		{
			RELEASEINTERFACE(m_pManager);
		}

        inline double GetSpaceWidth()
		{
			return m_dSpaceWidth;
		}

	public:
        void LoadFont(std::wstring& strFontName, LONG& lStyle, double& dSize, const bool& bIsGID)
		{
			if ((strFontName == m_strFontName) && (lStyle == m_lFontStyle) && (dSize == m_dSize))
			{
				m_pManager->SetStringGID(bIsGID);
				return;
			}

			m_strFontName	= strFontName;
			m_lFontStyle	= lStyle;
			m_dSize			= dSize;

            m_pManager->LoadFontByName(strFontName, (float)m_dSize, m_lFontStyle, c_dDpiX, c_dDpiY);
            m_dSpaceWidth = MeasureStringWidth(L" ");

			m_pManager->SetStringGID(bIsGID);
		}

        double MeasureStringWidth(const std::wstring& sText)
		{
            m_pManager->LoadString2(sText, (float)0, (float)0);
            TBBox bbox = m_pManager->MeasureString2();

            return (bbox.fMaxX - bbox.fMinX) * c_dPixToMM;
		}
	};

}
