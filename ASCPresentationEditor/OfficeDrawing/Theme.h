#pragma once
#include "Structures.h"
#include "Layout.h"
//#include "../PPTXWriter/CSS.h"

namespace NSPresentationEditor
{
	const long g_ThemeTextStylesCount	= 4;

	class CTheme
	{
	public:
		std::map<_UINT64, LONG>			m_mapGeomToLayout;// типовые шаблоны
		std::map<DWORD, LONG>			m_mapTitleLayout; // заголовочные шаблоны
		
		std::map<int,int>				m_mapPlaceholders;

		std::vector<CColor>				m_arColorScheme;
		std::vector<CFont>				m_arFonts;
		std::vector<CBrush>				m_arBrushes;
		std::vector<CPen>				m_arPens;

		std::vector<CEffects>			m_arEffects;
		CTextStyles						m_pStyles[g_ThemeTextStylesCount];
		std::vector<CLayout>			m_arLayouts;

		bool							m_bIsBackground;
		CBrush							m_oBackground;

		std::vector<IElement*>			m_arElements;

		CMetricInfo						m_oInfo;

		std::wstring					m_sThemeName;
		
		std::vector<std::vector<CColor>>m_arExtraColorScheme;

		long m_lOriginalWidth;
		long m_lOriginalHeight;
	
		bool m_bHasDate;
		bool m_bHasSlideNumber;
		bool m_bHasHeader;
		bool m_bHasFooter;

		std::vector<std::wstring>		m_oFootersHeaderString;

//------------------------------------------------------------------------------------
		CTheme() : m_arColorScheme(), m_arFonts(), m_arBrushes(),
			m_arPens(), m_arEffects(), m_arLayouts()
		{
			m_sThemeName		= L"Default";
			m_lOriginalWidth	= m_lOriginalHeight = 0;
			
			m_bHasDate			= false;
			m_bHasSlideNumber	= false;
			m_bHasHeader		= false;
			m_bHasFooter		= false;
		}

		CTheme(const CTheme& oSrc)
		{
			*this = oSrc;
		}

		CTheme& operator=(const CTheme& oSrc)
		{
			m_arColorScheme		=	oSrc.m_arColorScheme;
			m_arFonts			=	oSrc.m_arFonts;
			m_arBrushes			=	oSrc.m_arBrushes;
			m_arPens			=	oSrc.m_arPens;
			m_arEffects			=	oSrc.m_arEffects;
			
			m_sThemeName		=	oSrc.m_sThemeName;

			m_lOriginalWidth	=	oSrc.m_lOriginalWidth ;
			m_lOriginalHeight	=	oSrc.m_lOriginalHeight;

			m_bHasDate			=	oSrc.m_bHasDate;
			m_bHasSlideNumber	=	oSrc.m_bHasSlideNumber;
			m_bHasHeader		=	oSrc.m_bHasHeader;
			m_bHasFooter		=	oSrc.m_bHasFooter;

			for (int i = 0; i < oSrc.m_arExtraColorScheme.size(); ++i)
			{
				m_arExtraColorScheme.push_back(oSrc.m_arExtraColorScheme[i]);
			}

			for (int i = 0; i < g_ThemeTextStylesCount; ++i)
				m_pStyles[i] = oSrc.m_pStyles[i];

			m_arLayouts			= oSrc.m_arLayouts;
			m_mapTitleLayout	= oSrc.m_mapTitleLayout;
			m_mapGeomToLayout	= oSrc.m_mapGeomToLayout;
			m_mapPlaceholders	= oSrc.m_mapPlaceholders;
	
			m_bIsBackground		= oSrc.m_bIsBackground;
			m_oBackground		= oSrc.m_oBackground;

			// теперь сделаем копию всех элементов layout'ов
			size_t nCount = m_arLayouts.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				m_arLayouts[i].CreateDublicateElements();
			}

			m_arElements = oSrc.m_arElements;

			nCount = m_arElements.size();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				ADDREFINTERFACE((m_arElements[nIndex]));
			}
			CreateDublicateElements();

			for (long nIndexStyle = 0; nIndexStyle < g_ThemeTextStylesCount; ++nIndexStyle)
			{
				m_pStyles[nIndexStyle] = oSrc.m_pStyles[nIndexStyle];
			}

			return (*this);
		}

		void CreateDublicateElements()
		{
			// просто из всех своих элементов делаем дубликата
			size_t nCount = m_arElements.size();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				IElement* pElem = m_arElements[nIndex];
				if (NULL != pElem)
				{
					m_arElements[nIndex] = pElem->CreateDublicate();
				}
				RELEASEINTERFACE(pElem);
			}
		}

		void Clear()
		{
			size_t nCount = m_arElements.size();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				RELEASEINTERFACE((m_arElements[nIndex]));
			}

			m_arElements.clear();
			m_arLayouts.clear();
			m_mapTitleLayout.clear();
			m_mapGeomToLayout.clear();
			m_mapPlaceholders.clear();

			m_lOriginalWidth = m_lOriginalHeight = 0;

			m_sThemeName = L"Default";
			
			m_bHasDate = false;
			m_bHasSlideNumber = false;
			m_bHasHeader = false;
			m_bHasFooter = false;
		}

		~CTheme()
		{
		}

	public:

		NSPresentationEditor::CColor GetColor(const LONG& lIndexScheme)
		{
			if (lIndexScheme < (LONG)m_arColorScheme.size())
			{
				return m_arColorScheme[lIndexScheme];
			}
			return NSPresentationEditor::CColor();
		}

		void CalculateStyles()
		{
			LONG lCountColors = (LONG)m_arColorScheme.size();

			for (int pos = 0; pos < g_ThemeTextStylesCount; ++pos)
			{
				NSPresentationEditor::CTextStyles& oStyle = m_pStyles[pos];

				size_t nLevels = 10;
				for (size_t i = 0; i < nLevels; ++i)
				{
					if (!oStyle.m_pLevels[i].is_init())
						continue;

					CTextStyleLevel* pLevel = oStyle.m_pLevels[i].operator ->();

					if (pLevel->m_oPFRun.bulletColor.is_init())
					{
						CColor& oColor = pLevel->m_oPFRun.bulletColor.get();
						if ((0 <= oColor.m_lSchemeIndex) && (oColor.m_lSchemeIndex < lCountColors))
						{
							oColor.R	= m_arColorScheme[oColor.m_lSchemeIndex].R;
							oColor.G	= m_arColorScheme[oColor.m_lSchemeIndex].G;
							oColor.B	= m_arColorScheme[oColor.m_lSchemeIndex].B;
							oColor.A	= m_arColorScheme[oColor.m_lSchemeIndex].A;
						}
					}
					if (pLevel->m_oCFRun.Color.is_init())
					{
						CColor& oColor = pLevel->m_oCFRun.Color.get();
						if ((0 <= oColor.m_lSchemeIndex) && (oColor.m_lSchemeIndex < lCountColors))
						{
							oColor.R	= m_arColorScheme[oColor.m_lSchemeIndex].R;
							oColor.G	= m_arColorScheme[oColor.m_lSchemeIndex].G;
							oColor.B	= m_arColorScheme[oColor.m_lSchemeIndex].B;
							oColor.A	= m_arColorScheme[oColor.m_lSchemeIndex].A;
						}
					}
					if (pLevel->m_oCFRun.Typeface.IsInit())
					{
						WORD lFontIndex = pLevel->m_oCFRun.Typeface.get();
						if (lFontIndex < (WORD)m_arFonts.size())
						{
							if (!pLevel->m_oCFRun.FontProperties.is_init())
								pLevel->m_oCFRun.FontProperties = new CFontProperties();

							pLevel->m_oCFRun.FontProperties->SetFont(m_arFonts[lFontIndex]);
						}
					}
					if (pLevel->m_oPFRun.bulletFontRef.IsInit())
					{
						WORD lFontIndex = pLevel->m_oPFRun.bulletFontRef.get();
						if (lFontIndex < (WORD)m_arFonts.size())
						{
							if (!pLevel->m_oPFRun.bulletFontProperties.is_init())
								pLevel->m_oPFRun.bulletFontProperties = new CFontProperties();

							pLevel->m_oPFRun.bulletFontProperties->SetFont(m_arFonts[lFontIndex]);
						}
					}
				}
			}
		}

		static void CalculateStyle(CTheme* pTheme, CTextStyles& oStyle)
		{
			LONG lCountColors = (LONG)pTheme->m_arColorScheme.size();

			// пока здесь расчитываются только цвета
			size_t nLevels = 10;
			for (size_t i = 0; i < nLevels; ++i)
			{
				if (!oStyle.m_pLevels[i].is_init())
					continue;

				CTextStyleLevel* pLevel = oStyle.m_pLevels[i].operator ->();

				if (pLevel->m_oPFRun.bulletColor.is_init())
				{
					CColor& oColor = pLevel->m_oPFRun.bulletColor.get();
					if ((0 <= oColor.m_lSchemeIndex) && (oColor.m_lSchemeIndex < lCountColors))
					{
						oColor.R	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].R;
						oColor.G	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].G;
						oColor.B	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].B;
						oColor.A	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].A;
					}
				}
				if (pLevel->m_oCFRun.Color.is_init())
				{
					CColor& oColor = pLevel->m_oCFRun.Color.get();
					if ((0 <= oColor.m_lSchemeIndex) && (oColor.m_lSchemeIndex < lCountColors))
					{
						oColor.R	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].R;
						oColor.G	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].G;
						oColor.B	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].B;
						oColor.A	= pTheme->m_arColorScheme[oColor.m_lSchemeIndex].A;
					}
				}
				if (pLevel->m_oCFRun.Typeface.IsInit())
				{
					WORD lFontIndex = pLevel->m_oCFRun.Typeface.get();
					if (lFontIndex < (WORD)pTheme->m_arFonts.size())
					{
						if (!pLevel->m_oCFRun.FontProperties.is_init())
							pLevel->m_oCFRun.FontProperties = new CFontProperties();

						pLevel->m_oCFRun.FontProperties->SetFont(pTheme->m_arFonts[lFontIndex]);
					}
				}
			}
		}
	};
}