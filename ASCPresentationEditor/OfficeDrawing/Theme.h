#pragma once
#include "Structures.h"
#include "Layout.h"
#include "../PPTXWriter/CSS.h"

namespace NSPresentationEditor
{
	const long g_ThemeTextStylesCount	= 4;

	class CTheme : public IBase
	{
	public:
		CAtlArray<CColor>			m_arColorScheme;
		CAtlArray<CFont>			m_arFonts;
		CAtlArray<CBrush>			m_arBrushes;
		CAtlArray<CPen>				m_arPens;

		CAtlArray<CEffects>			m_arEffects;
		CTextStyles					m_pStyles[g_ThemeTextStylesCount];
		CAtlArray<CLayout>			m_arLayouts;

		bool						m_bIsBackground;
		CBrush						m_oBackground;

		CAtlArray<IElement*>		m_arElements;

		CMetricInfo					m_oInfo;

	public:
		CTheme() : m_arColorScheme(), m_arFonts(), m_arBrushes(),
			m_arPens(), m_arEffects(), m_arLayouts()
		{
		}

		CTheme(const CTheme& oSrc)
		{
			*this = oSrc;
		}

		CTheme& operator=(const CTheme& oSrc)
		{
			m_arColorScheme.Copy(oSrc.m_arColorScheme);
			m_arFonts.Copy(oSrc.m_arFonts);
			m_arBrushes.Copy(oSrc.m_arBrushes);
			m_arPens.Copy(oSrc.m_arPens);
			m_arEffects.Copy(oSrc.m_arEffects);
			
			for (int i = 0; i < g_ThemeTextStylesCount; ++i)
				m_pStyles[i] = oSrc.m_pStyles[i];
			
			m_arLayouts.Copy(oSrc.m_arLayouts);

			m_bIsBackground = oSrc.m_bIsBackground;
			m_oBackground	= oSrc.m_oBackground;

			// теперь сделаем копию всех элементов layout'ов
			size_t nCount = m_arLayouts.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				m_arLayouts[i].CreateDublicateElements();
			}

			Clear();
			m_arElements.Copy(oSrc.m_arElements);

			nCount = m_arElements.GetCount();
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
			size_t nCount = m_arElements.GetCount();
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
			size_t nCount = m_arElements.GetCount();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				RELEASEINTERFACE((m_arElements[nIndex]));
			}

			m_arElements.RemoveAll();
		}

		~CTheme()
		{
		}

	public:
		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode);
		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
		}

		CString ToXmlEditor(const CMetricInfo& oInfo);

		NSPresentationEditor::CColor GetColor(const LONG& lIndexScheme)
		{
			if (lIndexScheme < (LONG)m_arColorScheme.GetCount())
			{
				return m_arColorScheme[lIndexScheme];
			}
			return NSPresentationEditor::CColor();
		}

		void CalculateStyles()
		{
			LONG lCountColors = (LONG)m_arColorScheme.GetCount();

			// пока здесь расчитываются только цвета
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
						if (lFontIndex < (WORD)m_arFonts.GetCount())
						{
							if (!pLevel->m_oCFRun.FontProperties.is_init())
								pLevel->m_oCFRun.FontProperties = new CFontProperties();

							pLevel->m_oCFRun.FontProperties->SetFont(m_arFonts[lFontIndex]);
						}
					}
				}
			}
		}

		static void CalculateStyle(CTheme* pTheme, CTextStyles& oStyle)
		{
			LONG lCountColors = (LONG)pTheme->m_arColorScheme.GetCount();

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
					if (lFontIndex < (WORD)pTheme->m_arFonts.GetCount())
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