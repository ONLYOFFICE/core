#ifndef _ASC_HTMLRENDERER_FM_H_
#define _ASC_HTMLRENDERER_FM_H_

#include "FontManagerBase.h"

namespace NSHtmlRenderer
{
	using namespace NSFontManager;

	const long g_lNewNoJustifySpace = 5;

	class CFontManager : public CFontManagerBase
	{
	public:
        NSStructures::CFont*	m_pFont;
        Aggplus::CMatrix*		m_pTransform;

        double					m_dSpaceWidthMM;

	public:
		CFontManager() : m_pFont(NULL), CFontManagerBase()
		{
			m_pTransform = NULL;
			m_dSpaceWidthMM = 0;
		}
		virtual ~CFontManager()
		{
		}

	public:
		virtual void LoadFont(long lFaceIndex = 0, bool bIsNeedAddToMap = true)
		{
			if (NULL == m_pManager)
				return;

			double dSize = m_pFont->Size;
			double dSizeFont = dSize * ((m_pTransform->m_agg_mtx.sx + m_pTransform->m_agg_mtx.sy) / 2);

			m_pFont->Size = dSizeFont;

            if (IsEqual2(m_pFont, &m_oFont.m_oFont))
			{
				m_pFont->Size = dSize;
				return;
			}

			m_oFont.m_oFont	= *m_pFont;
			m_pFont->Size = dSize;

			bool bIsPath = false;

            if (L"" == m_pFont->Path)
			{
				CFontManagerBase::LoadFontByName(m_oFont.m_oFont.Name, m_oFont.m_oFont.Size, m_oFont.m_oFont.GetStyle());
			}
			else
			{
				CFontManagerBase::LoadFontByFile(m_oFont.m_oFont.Path, m_oFont.m_oFont.Size, lFaceIndex);

				m_pFont->SetStyle(m_oFont.m_oProperties.m_lStyle);
				m_oFont.m_oFont.SetStyle(m_oFont.m_oProperties.m_lStyle);

				bIsPath = true;
			}

			CalculateSpace();
		}

        inline void CalculateSpace()
		{
            LONG lGid = m_pManager->m_bStringGID;
            m_pManager->SetStringGID(FALSE);
			
            m_pManager->LoadString1(L" ", 0, 0);

            TBBox _box = m_pManager->MeasureString2();

            m_dSpaceWidthMM = (double)(_box.fMaxX - _box.fMinX) * c_dPixToMM;
			if (0 >= m_dSpaceWidthMM)
			{
				m_dSpaceWidthMM = 1.0;
			}

			m_pManager->SetStringGID(lGid);
		}
	};
}

#endif // _ASC_HTMLRENDERER_FM_H_
