/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "WmfPlayer.h"
#include "WmfFile.h"

#include "WmfParser/CWmfParserBase.h"

namespace MetaFile
{
	CWmfPlayer::CWmfPlayer(CWmfParserBase* pFile) : m_pParser(pFile)
	{
		CWmfDC* pDC = new CWmfDC();
		if (!pDC)
		{
			m_pParser->SetError();
			return;
		}

		m_pDC = pDC;
		m_vDCStack.push_back(pDC);
		m_ushIndex = 0;
	};
	CWmfPlayer::~CWmfPlayer()
	{
		for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
		{
			CWmfDC* pDC = m_vDCStack.at(nIndex);
			delete pDC;
		}
		m_vDCStack.clear();

		for (CWmfObjectMap::iterator oIterator = m_mObjects.begin(); oIterator != m_mObjects.end(); oIterator++)
		{
			CWmfObjectBase* pOldObject = oIterator->second;
			delete pOldObject;
		}
		m_mObjects.clear();
	}
	void    CWmfPlayer::Clear()
	{
		for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
		{
			CWmfDC* pDC = m_vDCStack.at(nIndex);
			delete pDC;
		}
		m_vDCStack.clear();

		for (CWmfObjectMap::iterator oIterator = m_mObjects.begin(); oIterator != m_mObjects.end(); oIterator++)
		{
			CWmfObjectBase* pOldObject = oIterator->second;
			delete pOldObject;
		}
		m_mObjects.clear();

		CWmfDC* pDC = new CWmfDC();
		if (!pDC)
		{
			m_pParser->SetError();
			return;
		}

		m_pDC = pDC;
		m_vDCStack.push_back(pDC);
		m_ushIndex = 0;
		m_vAvailableIndexes.clear();
	}
	CWmfDC* CWmfPlayer::SaveDC()
	{
		if (!m_pDC)
		{
			m_pParser->SetError();
			return NULL;
		}

		CWmfDC* pNewDC = m_pDC->Copy();
		if (!pNewDC)
		{
			m_pParser->SetError();
			return NULL;
		}

		m_vDCStack.push_back(pNewDC);
		m_pDC = pNewDC;
		return pNewDC;
	}
	CWmfDC* CWmfPlayer::RestoreDC()
	{
		if (m_vDCStack.size() <= 1)
		{
			m_pParser->SetError();
			return m_pDC;
		}

		CWmfDC* pDC = m_vDCStack.at(m_vDCStack.size() - 1);
		m_vDCStack.pop_back();
		delete pDC;

		pDC = m_vDCStack.at(m_vDCStack.size() - 1);
		m_pDC = pDC;
		return m_pDC;
	}
	CWmfDC* CWmfPlayer::GetDC()
	{
		return m_pDC;
	}
	void    CWmfPlayer::RegisterObject(CWmfObjectBase* pObject)
	{
		// Найдем наименьший доступный индекс
		unsigned short ushIndex = m_ushIndex;
		int nAvailableIndex = -1;
		if (m_vAvailableIndexes.size() > 0)
		{
			for (std::size_t nIterator = 0; nIterator != m_vAvailableIndexes.size(); ++nIterator)
			{
				if (ushIndex > m_vAvailableIndexes[nIterator])
				{
					ushIndex = m_vAvailableIndexes[nIterator];
					nAvailableIndex = (int)nIterator;
				}
			}
		}

		CWmfObjectMap::const_iterator oPos = m_mObjects.find(ushIndex);

		if (m_mObjects.end() != oPos)
			DeleteObject(ushIndex);

		m_mObjects.insert(std::pair<unsigned int, CWmfObjectBase*>(ushIndex, pObject));

		if (-1 != nAvailableIndex)
			m_vAvailableIndexes.erase(m_vAvailableIndexes.begin() + nAvailableIndex);

		if (ushIndex == m_ushIndex)
			m_ushIndex++;
	}
	void    CWmfPlayer::SelectObject(unsigned short ushIndex)
	{
		CWmfObjectMap::const_iterator oPos = m_mObjects.find(ushIndex);
		if (m_mObjects.end() != oPos)
		{
			CWmfObjectBase* pObject = oPos->second;


			switch (pObject->GetType())
			{
			case WMF_OBJECT_BRUSH: m_pDC->SetBrush((CWmfBrush*)pObject); break;
			case WMF_OBJECT_FONT: m_pDC->SetFont((CWmfFont*)pObject); break;
			case WMF_OBJECT_PEN: m_pDC->SetPen((CWmfPen*)pObject); break;
			case WMF_OBJECT_PALETTE: m_pDC->SetPalette((CWmfPalette*)pObject); break;
			case WMF_OBJECT_REGION: m_pDC->SetRegion((CWmfRegion*)pObject); break;
			}
		}
	}
	void    CWmfPlayer::SelectPalette(unsigned short ushIndex)
	{
		CWmfObjectMap::const_iterator oPos = m_mObjects.find(ushIndex);
		if (m_mObjects.end() != oPos)
		{
			CWmfObjectBase* pObject = oPos->second;
			if (WMF_OBJECT_PALETTE == pObject->GetType())
				m_pDC->SetPalette((CWmfPalette*)pObject);
		}
	}
	void    CWmfPlayer::DeleteObject(unsigned short ushIndex)
	{
		CWmfObjectMap::const_iterator oPos = m_mObjects.find(ushIndex);
		if (m_mObjects.end() != oPos)
		{
			CWmfObjectBase* pObject = oPos->second;

			for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
			{
				CWmfDC* pDC = m_vDCStack.at(nIndex);

				switch (pObject->GetType())
				{
				case WMF_OBJECT_BRUSH: pDC->RemoveBrush((CWmfBrush*)pObject); break;
				case WMF_OBJECT_FONT: pDC->RemoveFont((CWmfFont*)pObject); break;
				case WMF_OBJECT_PEN: pDC->RemovePen((CWmfPen*)pObject); break;
				case WMF_OBJECT_PALETTE: pDC->RemovePalette((CWmfPalette*)pObject); break;
				case WMF_OBJECT_REGION: pDC->RemoveRegion((CWmfRegion*)pObject); break;
				}
			}

			delete pObject;
			m_mObjects.erase(ushIndex);
		}

		if (ushIndex < m_ushIndex)
		{
			if (std::find(m_vAvailableIndexes.begin(), m_vAvailableIndexes.end(), ushIndex) != m_vAvailableIndexes.end())
			{
				// Данный индекс уже есть в списке доступных
			}
			else
			{
				m_vAvailableIndexes.push_back(ushIndex);
			}
		}
	}

//---------------------------------------------------------------------------------------------------------------
	CWmfDC::CWmfDC()
	{
		m_pBrush       = &m_oDefaultBrush;
		m_pPen         = &m_oDefaultPen;
		m_pPalette     = NULL;
		m_pFont        = NULL;
		m_pRegion      = NULL;
		m_ushMapMode   = MM_TEXT;
		m_dPixelWidth  = 1;
		m_dPixelHeight = 1;
		m_oWindow.Init();
		m_oViewport.Init();
		m_oTextColor.Set(0, 0, 0);
		m_oTextBgColor.Set(255, 255, 255);
		m_oCurPos.Set(0, 0);
		m_ushTextBgMode     = OPAQUE;
		m_ushLayout         = LAYOUT_LTR;
		m_ushPolyFillMode   = ALTERNATE;
		m_ushRop2Mode       = R2_COPYPEN;
		m_ushStretchBltMode = COLORONCOLOR;
		m_ushTextAlign      = TA_TOP | TA_LEFT | TA_NOUPDATECP;
		m_ushCharSpacing    = 0;
		m_oTransform.Init();
		m_oFinalTransform.Init();
		m_oFinalTransform2.Init();
	}
	CWmfDC::~CWmfDC()
	{

	}
	CWmfDC*        CWmfDC::Copy()
	{
		CWmfDC* pNewDC = new CWmfDC();
		if (!pNewDC)
			return NULL;

		pNewDC->m_pBrush            = (m_pBrush == &m_oDefaultBrush ? &pNewDC->m_oDefaultBrush : m_pBrush);
		pNewDC->m_pPen              = (m_pPen == &m_oDefaultPen ? &pNewDC->m_oDefaultPen : m_pPen);
		pNewDC->m_pPalette          = m_pPalette;
		pNewDC->m_pFont             = m_pFont;
		pNewDC->m_pRegion           = m_pRegion;
		pNewDC->m_ushMapMode        = m_ushMapMode;
		pNewDC->m_dPixelWidth       = m_dPixelWidth;
		pNewDC->m_dPixelHeight      = m_dPixelHeight;
		pNewDC->m_oWindow.Copy(m_oWindow);
		pNewDC->m_oViewport.Copy(m_oViewport);
		pNewDC->m_oTextColor        = m_oTextColor;
		pNewDC->m_oTextBgColor      = m_oTextBgColor;
		pNewDC->m_oCurPos           = m_oCurPos;
		pNewDC->m_ushTextBgMode     = m_ushTextBgMode;
		pNewDC->m_ushLayout         = m_ushLayout;
		pNewDC->m_ushPolyFillMode   = m_ushPolyFillMode;
		pNewDC->m_ushRop2Mode       = m_ushRop2Mode;
		pNewDC->m_ushStretchBltMode = m_ushStretchBltMode;
		pNewDC->m_ushTextAlign      = m_ushTextAlign;
		pNewDC->m_ushCharSpacing    = m_ushCharSpacing;
		pNewDC->m_oTransform.Init();
		pNewDC->m_oClip             = m_oClip;
		pNewDC->m_oFinalTransform.Copy(&m_oFinalTransform);
		pNewDC->m_oFinalTransform2.Copy(&m_oFinalTransform2);

		return pNewDC;
	}
	void           CWmfDC::SetBrush(CWmfBrush* pBrush)
	{
		m_pBrush = pBrush;
	}
	void           CWmfDC::RemoveBrush(CWmfBrush* pBrush)
	{
		if (pBrush == m_pBrush)
			m_pBrush = NULL;
	}
	CWmfBrush*     CWmfDC::GetBrush()
	{
		return m_pBrush;
	}
	void           CWmfDC::SetPen(CWmfPen* pPen)
	{
		m_pPen = pPen;
	}
	void           CWmfDC::RemovePen(CWmfPen* pPen)
	{
		if (m_pPen == pPen)
			m_pPen = NULL;
	}
	CWmfPen*       CWmfDC::GetPen()
	{
		return m_pPen;
	}
	void           CWmfDC::SetPalette(CWmfPalette* pPalette)
	{
		m_pPalette = pPalette;
	}
	void           CWmfDC::RemovePalette(CWmfPalette* pPalette)
	{
		if (m_pPalette == pPalette)
			m_pPalette = NULL;
	}
	CWmfPalette*   CWmfDC::GetPalette()
	{
		return m_pPalette;
	}
	void           CWmfDC::SetFont(CWmfFont* pFont)
	{
		m_pFont = pFont;
	}
	void           CWmfDC::RemoveFont(CWmfFont* pFont)
	{
		if (m_pFont == pFont)
			m_pFont = NULL;
	}
	CWmfFont*      CWmfDC::GetFont()
	{
		return m_pFont;
	}
	void           CWmfDC::SetRegion(CWmfRegion* pRegion)
	{
		m_pRegion = pRegion;
	}
	void           CWmfDC::RemoveRegion(CWmfRegion* pRegion)
	{
		if (m_pRegion == pRegion)
			m_pRegion = NULL;
	}
	CWmfRegion*    CWmfDC::GetRegion()
	{
		return m_pRegion;
	}
	void           CWmfDC::SetMapMode(unsigned short ushMapMode)
	{
		m_ushMapMode = ushMapMode;

		switch (m_ushMapMode)
		{
			case MM_TEXT: // 1 unit = 1pt
			{
				SetPixelWidth(1);
				SetPixelHeight(1);
				break;
			}
			case MM_LOMETRIC: // 1 unit = 0.1mm
			{
				double dPixel = 0.1 * 72 / 25.4;
				SetPixelWidth(dPixel);
				SetPixelHeight(dPixel);
				break;
			}
			case MM_HIMETRIC: // 1 unit = 0.01mm
			{
				double dPixel = 0.01 * 72 / 25.4;
				SetPixelWidth(dPixel);
				SetPixelHeight(dPixel);
				break;
			}
			case MM_LOENGLISH: // 1 unit = 0.01 inch
			{
				double dPixel = 0.01 * 72;
				SetPixelWidth(dPixel);
				SetPixelHeight(dPixel);
				break;
			}
			case MM_HIENGLISH: // 1 unit = 0.001 inch
			{
				double dPixel = 0.001 * 72;
				SetPixelWidth(dPixel);
				SetPixelHeight(dPixel);
				break;
			}
			case MM_TWIPS: // 1 unit = 1/1440 inch
			{
				SetPixelWidth(0.05);
				SetPixelHeight(0.05);
				break;
			}
			case MM_ISOTROPIC:
			case MM_ANISOTROPIC:
			{
				UpdatePixelMetrics();
				break;
			}
		}

		UpdateFinalTransform();
	}
	unsigned int   CWmfDC::GetMapMode()
	{
		return m_ushMapMode;
	}
	double         CWmfDC::GetPixelWidth()
	{
		return m_dPixelWidth;
	}
	double         CWmfDC::GetPixelHeight()
	{
		return m_dPixelHeight;
	}
	void           CWmfDC::SetPixelWidth(double dW)
	{
		m_dPixelWidth = dW;
	}
	void           CWmfDC::SetPixelHeight(double dH)
	{
		m_dPixelHeight = dH;
	}
	TWmfWindow* CWmfDC::GetWindow()
	{
		return &m_oWindow;
	}
	void CWmfDC::SetWindowOrg(short shX, short shY)
	{
		m_oWindow.x = shX;
		m_oWindow.y = shY;
		m_oWindow.bUnchangedOrg = false;

		if (m_oViewport.bUnchangedOrg)
		{
			m_oViewport.x = shX;
			m_oViewport.y = shY;
		}

		UpdatePixelMetrics();
		UpdateFinalTransform();
	}
	void CWmfDC::SetWindowExt(short shW, short shH)
	{
		m_oWindow.w = shW;
		m_oWindow.h = shH;
		m_oWindow.bUnchangedExt = false;

		if (m_oViewport.bUnchangedExt)
		{
			m_oViewport.w = std::abs(shW);
			m_oViewport.h = std::abs(shH);
		}

		if (MM_ISOTROPIC == m_ushMapMode)
			FixIsotropic();

		UpdatePixelMetrics();
		UpdateFinalTransform();
	}
	void CWmfDC::SetWindowOff(short shX, short shY)
	{
		m_oWindow.x += shX;
		m_oWindow.y += shY;
		UpdatePixelMetrics();
		UpdateFinalTransform();
	}
	void CWmfDC::SetWindowScale(double dX, double dY)
	{
		m_oWindow.w = (short)(m_oWindow.w * dX);
		m_oWindow.h = (short)(m_oWindow.h * dY);
		UpdatePixelMetrics();
		UpdateFinalTransform();
	}
	TWmfWindow*    CWmfDC::GetViewport()
	{
		return &m_oViewport;
	}
	void CWmfDC::SetViewportOrg(short shX, short shY)
	{
		m_oViewport.x = shX;
		m_oViewport.y = shY;
		m_oViewport.bUnchangedOrg = false;

		if (m_oWindow.bUnchangedOrg)
		{
			m_oWindow.x = shX;
			m_oWindow.y = shY;
		}

		UpdatePixelMetrics();
		UpdateFinalTransform();
	}
	void  CWmfDC::SetViewportExt(short shW, short shH)
	{
		m_oViewport.w = shW;
		m_oViewport.h = shH;
		m_oViewport.bUnchangedExt = false;

		if (m_oWindow.bUnchangedExt)
		{
			m_oWindow.w = shW;
			m_oWindow.h = shH;
		}

		if (MM_ISOTROPIC == m_ushMapMode)
			FixIsotropic();

		UpdatePixelMetrics();
		UpdateFinalTransform();
	}
	void CWmfDC::SetViewportOff(short shX, short shY)
	{
		m_oViewport.x += shX;
		m_oViewport.y += shY;
		UpdatePixelMetrics();
		UpdateFinalTransform();
	}
	void CWmfDC::SetViewportScale(double dX, double dY)
	{
		m_oViewport.w = (short)(m_oViewport.w * dX);
		m_oViewport.h = (short)(m_oViewport.h * dY);
		UpdatePixelMetrics();
		UpdateFinalTransform();
	}
	bool CWmfDC::UpdatePixelMetrics()
	{
		if (1 >= m_oWindow.w || 1 >= m_oViewport.w)
			return false;

		unsigned short ushMapMode = m_ushMapMode;
		if (MM_ISOTROPIC == ushMapMode)
		{
			if (0 == m_oWindow.w || 0 == m_oViewport.w)
				return false;

			double dPixel = (double)m_oViewport.w / (double)m_oWindow.w;
			SetPixelHeight(dPixel);
			SetPixelWidth(dPixel);
		}
		else if (MM_ANISOTROPIC == ushMapMode)
		{
			double dPixelX = (double)m_oViewport.w / (double)m_oWindow.w;
			double dPixelY = (double)m_oViewport.h / (double)m_oWindow.h;

			SetPixelWidth(dPixelX);
			SetPixelHeight(dPixelY);
		}

		return true;
	}
	void CWmfDC::UpdateFinalTransform()
	{
		TWmfWindow* pWindow   = GetWindow();
		TWmfWindow* pViewPort = GetViewport();

		double dM11 = (pViewPort->w >= 0) ? 1 : -1;
		double dM22 = (pViewPort->h >= 0) ? 1 : -1;

		TEmfXForm oWindowXForm(1, 0, 0, 1, -(pWindow->x * GetPixelWidth() * dM11), -(pWindow->y * GetPixelHeight() * dM22));
		TEmfXForm oViewportXForm(GetPixelWidth() * dM11, 0, 0, GetPixelHeight() * dM22, pViewPort->x, pViewPort->y);

		m_oFinalTransform.Init();
		m_oFinalTransform.Multiply(oViewportXForm, MWT_RIGHTMULTIPLY);
		m_oFinalTransform.Multiply(m_oTransform, MWT_RIGHTMULTIPLY);
		m_oFinalTransform.Multiply(oWindowXForm, MWT_RIGHTMULTIPLY);

		m_oFinalTransform2.Init();
		m_oFinalTransform2.Multiply(oViewportXForm, MWT_RIGHTMULTIPLY);
//		m_oFinalTransform2.Multiply(m_oTransform, MWT_RIGHTMULTIPLY);
		m_oFinalTransform2.Multiply(oWindowXForm, MWT_RIGHTMULTIPLY);
	}

	void CWmfDC::FixIsotropic()
	{
		double dXDim = std::fabs((double)m_oViewport.w / m_oWindow.w);
		double dYDim = std::fabs((double)m_oViewport.h / m_oWindow.h);

		if (dXDim > dYDim)
		{
			int nMinCx = (m_oViewport.w >= 0) ? 1 : -1;
			m_oViewport.w = std::floor(m_oViewport.w * dYDim / dXDim + 0.5);
			if (!m_oViewport.w) m_oViewport.w = nMinCx;
		}
		else
		{
			int nMinCy = (m_oViewport.h >= 0) ? 1 : -1;
			m_oViewport.h = std::floor(m_oViewport.h * dXDim / dYDim + 0.5);
			if (!m_oViewport.h) m_oViewport.h = nMinCy;
		}
	}
	void CWmfDC::SetTextColor(TWmfColor& oColor)
	{
		m_oTextColor = oColor;
	}
	TWmfColor&  CWmfDC::GetTextColor()
	{
		return m_oTextColor;
	}
	void  CWmfDC::SetTextBgColor(TWmfColor& oColor)
	{
		m_oTextBgColor = oColor;
	}
	TWmfColor& CWmfDC::GetTextBgColor()
	{
		return m_oTextBgColor;
	}
	TWmfPointS& CWmfDC::GetCurPos()
	{
		return m_oCurPos;
	}
	void           CWmfDC::SetCurPos(TWmfPointS& oPoint)
	{
		m_oCurPos = oPoint;
	}
	void           CWmfDC::SetCurPos(short shX, short shY)
	{
		m_oCurPos.x = shX;
		m_oCurPos.y = shY;
	}
	void           CWmfDC::SetTextBgMode(unsigned short ushMode)
	{
		m_ushTextBgMode = ushMode;
	}
	unsigned short CWmfDC::GetTextBgMode()
	{
		return m_ushTextBgMode;
	}
	void           CWmfDC::SetLayout(unsigned short ushLayout)
	{
		m_ushLayout = ushLayout;
	}
	unsigned short CWmfDC::GetLayout()
	{
		return m_ushLayout;
	}
	void           CWmfDC::SetPolyFillMode(unsigned short ushMode)
	{
		m_ushPolyFillMode = ushMode;
	}
	unsigned short CWmfDC::GetPolyFillMode()
	{
		return m_ushPolyFillMode;
	}
	void           CWmfDC::SetRop2Mode(unsigned short ushMode)
	{
		m_ushRop2Mode = ushMode;
	}
	unsigned short CWmfDC::GetRop2Mode()
	{
		return m_ushRop2Mode;
	}
	void           CWmfDC::SetStretchBltMode(unsigned short ushMode)
	{
		m_ushStretchBltMode = ushMode;
	}
	unsigned short CWmfDC::GetStretchBltMode()
	{
		return m_ushStretchBltMode;
	}
	void           CWmfDC::SetTextAlign(unsigned short ushTextAlign)
	{
		m_ushTextAlign = ushTextAlign;
	}
	unsigned short CWmfDC::GetTextAlign()
	{
		return m_ushTextAlign;
	}
	void           CWmfDC::SetCharSpacing(unsigned short ushCharSpacing)
	{
		m_ushCharSpacing = ushCharSpacing;
	}
	unsigned short CWmfDC::GetCharSpacing()
	{
		return m_ushCharSpacing;
	}
	TXForm*        CWmfDC::GetTransform()
	{
		return &m_oTransform;
	}
	TXForm*        CWmfDC::GetInverseTransform()
	{
		return &m_oTransform;
	}
	TXForm*        CWmfDC::GetFinalTransform(int iGraphicsMode)
	{
		if (GM_COMPATIBLE == iGraphicsMode)
			return &m_oFinalTransform2;

		return &m_oFinalTransform;
	}
	unsigned int   CWmfDC::GetMiterLimit()
	{
		return 0;
	}
	CWmfClip*      CWmfDC::GetClip()
	{
		return &m_oClip;
	}
}
