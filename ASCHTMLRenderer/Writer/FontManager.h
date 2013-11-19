#pragma once

#include "..\stdafx.h"
#include "Const.h"
#include "Common.h"
#include "StringWriter.h"
#include "..\Graphics\Structures.h"
#include "..\Graphics\Matrix.h"

#include "FontManagerBase.h"

namespace NSHtmlRenderer
{
	using namespace NSFontManager;

	const long g_lNewNoJustifySpace = 5;

	inline LONG ConvertColor(LONG lBGR)
	{
		return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
	}
	inline CString GetStringColor(LONG lBGR)
	{
		BYTE R = (BYTE)(lBGR & 0xFF);
		BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
		BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

		CString strTemp = _T("");
		strTemp.Format(g_string_css_color, R, G, B);
		
		return strTemp;
	}
	inline CString SetStrokeColor(LONG lBGR, LONG lA)
	{
		BYTE R = (BYTE)(lBGR & 0xFF);
		BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
		BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

		CString strTemp = _T("");
		strTemp.Format(g_string_canvas_StrokeColorA, R, G, B, (double)lA / 255);
		
		return strTemp;
	}
	inline CString SetFillColor(LONG lBGR, LONG lA)
	{
		BYTE R = (BYTE)(lBGR & 0xFF);
		BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
		BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

		CString strTemp = _T("");
		strTemp.Format(g_string_canvas_FillColorA, R, G, B, (double)lA / 255);

		return strTemp;
	}


	
	class CFontManager : public CFontManagerBase
	{
	public:
		NSStructures::CFont*			m_pFont;
		NSHtmlRenderer::CMatrix*		m_pTransform;

		double							m_dSpaceWidthMM;

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

			if (m_pFont->IsEqual2(&m_oFont.m_oFont))
			{
				m_pFont->Size = dSize;
				return;
			}

			m_oFont.m_oFont	= *m_pFont;
			m_pFont->Size = dSize;

			bool bIsPath = false;

			if (_T("") == m_pFont->Path)
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

		AVSINLINE void CalculateSpace()
		{
			LONG lGid = 0;
			m_pManager->GetStringGID(&lGid);
			m_pManager->SetStringGID(FALSE);
			
			m_pManager->LoadString(L" ", 0, 0);
			float _x = 0;
			float _y = 0;
			float _w = 0;
			float _h = 0;

			m_pManager->MeasureString2(&_x, &_y, &_w, &_h);

			m_dSpaceWidthMM = (double)_w * c_dPixToMM;
			if (0 >= m_dSpaceWidthMM)
			{
				m_dSpaceWidthMM = 1.0;
			}

			m_pManager->SetStringGID(lGid);
		}
	};
}