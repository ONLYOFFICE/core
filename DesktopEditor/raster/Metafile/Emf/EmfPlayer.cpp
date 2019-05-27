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
#include "EmfPlayer.h"
#include "EmfFile.h"

namespace MetaFile
{
	CEmfPlayer::CEmfPlayer(CEmfFile* pFile)
	{
		CEmfDC* pDC = new CEmfDC(this);
		if (!pDC)
		{
			pFile->SetError();
			return;
		}

		m_pEmfFile = pFile;
		m_pDC = pDC;
		m_vDCStack.push_back(pDC);

		InitStockObjects();
	};
	CEmfPlayer::~CEmfPlayer()
	{
		for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
		{
			CEmfDC* pDC = m_vDCStack.at(nIndex);
			delete pDC;
		}
		m_vDCStack.clear();

		for (CEmfObjectMap::iterator oIterator = m_mObjects.begin(); oIterator != m_mObjects.end(); oIterator++)
		{
			CEmfObjectBase* pOldObject = oIterator->second;
			delete pOldObject;
		}
		m_mObjects.clear();
	}
	void    CEmfPlayer::Clear()
	{
		for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
		{
			CEmfDC* pDC = m_vDCStack.at(nIndex);
			delete pDC;
		}
		m_vDCStack.clear();

		for (CEmfObjectMap::iterator oIterator = m_mObjects.begin(); oIterator != m_mObjects.end(); oIterator++)
		{
			CEmfObjectBase* pOldObject = oIterator->second;
			delete pOldObject;
		}
		m_mObjects.clear();

		CEmfDC* pDC = new CEmfDC(this);
		if (!pDC)
		{
			m_pEmfFile->SetError();
			return;
		}

		m_pDC = pDC;
		m_vDCStack.push_back(pDC);
		InitStockObjects();

		SelectObject(0x80000007); // BLACK_PEN
		SelectObject(0x80000000); // WHITE_BRUSH
	}
	CEmfDC* CEmfPlayer::SaveDC()
	{
		if (!m_pDC)
		{
			m_pEmfFile->SetError();
			return NULL;
		}

		CEmfDC* pNewDC = m_pDC->Copy();
		if (!pNewDC)
		{
			m_pEmfFile->SetError();
			return NULL;
		}

		m_vDCStack.push_back(pNewDC);
		m_pDC = pNewDC;
		return pNewDC;
	}
	CEmfDC* CEmfPlayer::RestoreDC()
	{
		if (m_vDCStack.size() <= 1)
		{
			m_pEmfFile->SetError();
			return m_pDC;
		}

		CEmfDC* pDC = m_vDCStack.at(m_vDCStack.size() - 1);
		m_vDCStack.pop_back();
		delete pDC;

		pDC = m_vDCStack.at(m_vDCStack.size() - 1);
		m_pDC = pDC;
		return m_pDC;
	}
	CEmfDC* CEmfPlayer::GetDC()
	{
		return m_pDC;
	}
	void    CEmfPlayer::RegisterObject(unsigned int ulIndex, CEmfObjectBase* pObject)
	{
		CEmfObjectMap::const_iterator oPos = m_mObjects.find(ulIndex);
		if (m_mObjects.end() != oPos)
		{ 
			CEmfObjectBase* pOldObject = oPos->second;
			delete pOldObject;
			m_mObjects.erase(ulIndex);
		}

		m_mObjects.insert(std::pair<unsigned int, CEmfObjectBase*>(ulIndex, pObject));
	}
	void    CEmfPlayer::SelectObject(unsigned int ulIndex)
	{
		CEmfObjectMap::const_iterator oPos = m_mObjects.find(ulIndex);
		if (m_mObjects.end() != oPos)
		{
			CEmfObjectBase* pObject = oPos->second;

			switch (pObject->GetType())
			{
			case EMF_OBJECT_BRUSH: m_pDC->SetBrush((CEmfLogBrushEx*)pObject); break;
			case EMF_OBJECT_FONT: m_pDC->SetFont((CEmfLogFont*)pObject); break;
			case EMF_OBJECT_PEN: m_pDC->SetPen((CEmfLogPen*)pObject); break;
			}
		}
	}
	void    CEmfPlayer::SelectPalette(unsigned int ulIndex)
	{
		// DEFAULT_PALETTE
		if (ulIndex == 0x8000000F)
			m_pDC->SetPalette(NULL);

		CEmfObjectMap::const_iterator oPos = m_mObjects.find(ulIndex);
		if (m_mObjects.end() != oPos)
		{
			CEmfObjectBase* pObject = oPos->second;
			if (EMF_OBJECT_PALETTE == pObject->GetType())
				m_pDC->SetPalette((CEmfLogPalette*)pObject);
		}
	}
	void    CEmfPlayer::DeleteObject(unsigned int ulIndex)
	{
		// TODO: Сделать поиск по DC_BRUSH и DC_PEN

		CEmfObjectMap::const_iterator oPos = m_mObjects.find(ulIndex);
		if (m_mObjects.end() != oPos)
		{
			CEmfObjectBase* pObject = oPos->second;

			for (int nIndex = 0; nIndex < m_vDCStack.size(); nIndex++)
			{
				CEmfDC* pDC = m_vDCStack.at(nIndex);

				switch (pObject->GetType())
				{
				case EMF_OBJECT_BRUSH: pDC->RemoveBrush((CEmfLogBrushEx*)pObject); break;
				case EMF_OBJECT_FONT: pDC->RemoveFont((CEmfLogFont*)pObject); break;
				case EMF_OBJECT_PEN: pDC->RemovePen((CEmfLogPen*)pObject); break;
				}
			}

			delete pObject;
			m_mObjects.erase(ulIndex);
		}
	}
	void    CEmfPlayer::InitStockObjects()
	{
		InitStockBrush(false, 0xff, 0xff, 0xff, 0x80000000);
		InitStockBrush(false, 0xc0, 0xc0, 0xc0, 0x80000001);
		InitStockBrush(false, 0x80, 0x80, 0x80, 0x80000002);
		InitStockBrush(false, 0x40, 0x40, 0x40, 0x80000003);
		InitStockBrush(false, 0x00, 0x00, 0x00, 0x80000004);
		InitStockBrush(true, 0x00, 0x00, 0x00, 0x80000005);
		InitStockPen(false, 0xff, 0xff, 0xff, 0x80000006);
		InitStockPen(false, 0x00, 0x00, 0x00, 0x80000007);
		InitStockPen(true, 0x00, 0x00, 0x00, 0x80000008);
		// TODO: Сделать шрифты

		// DC_BRUSH и DC_PEN не надо выставлять
	}
	void    CEmfPlayer::InitStockBrush(bool bNull, unsigned char r, unsigned char g, unsigned char b, unsigned int ulIndex)
	{
		CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();
		if (!pBrush)
			return;

		if (bNull)
			pBrush->BrushStyle = BS_NULL;
		else
		{
			pBrush->BrushStyle = BS_SOLID;
			pBrush->Color.Set(r, g, b);
		}

		RegisterObject(ulIndex, (CEmfObjectBase*)pBrush);
	}
	void    CEmfPlayer::InitStockPen(bool bNull, unsigned char r, unsigned char g, unsigned char b, unsigned int ulIndex)
	{
		CEmfLogPen* pPen = new CEmfLogPen();
		if (!pPen)
			return;

		if (bNull)
			pPen->PenStyle = PS_NULL;
		else
		{
			pPen->PenStyle = PS_COSMETIC | PS_SOLID;
			pPen->Color.Set(r, g, b);
		}

		RegisterObject(ulIndex, (CEmfObjectBase*)pPen);
	}

	CEmfDC::CEmfDC(CEmfPlayer* pPlayer)
	{
		m_pPlayer   = pPlayer;
		m_ulMapMode = MM_TEXT;
		m_pBrush    = NULL;
		m_pPen      = NULL;
		m_pFont     = NULL;
		m_oTransform.Init();
		m_oInverseTransform.Init();
		m_oTextColor.Init();
		m_oFinalTransform.Init();
		m_oBgColor.Init();
		m_ulTextAlign   = TA_TOP | TA_LEFT | TA_NOUPDATECP;
		m_ulBgMode      = TRANSPARENT;
		m_ulMiterLimit  = 0;
		m_ulFillMode    = WINDING;
		m_ulStretchMode = 0;
		m_oWindow.Init();
		m_oViewport.Init();
		m_dPixelHeight = 1;
		m_dPixelWidth  = 1;
		m_oCurPos.x = 0;
		m_oCurPos.y = 0;
		m_unArcDirection = AD_COUNTERCLOCKWISE;
	}
	CEmfDC::~CEmfDC()
	{
	}
	CEmfDC*         CEmfDC::Copy()
	{
		CEmfDC* pNewDC = new CEmfDC(m_pPlayer);
		if (!pNewDC)
			return NULL;

		pNewDC->m_ulMapMode      = m_ulMapMode;
		pNewDC->m_pBrush         = m_pBrush;
		pNewDC->m_pPen           = m_pPen;
		pNewDC->m_pFont          = m_pFont;
		pNewDC->m_pPalette       = m_pPalette;
		pNewDC->m_oTransform.Copy(&m_oTransform);
		pNewDC->m_oInverseTransform.Copy(&m_oInverseTransform);
		pNewDC->m_oFinalTransform.Copy(&m_oFinalTransform);
		pNewDC->m_oTextColor.Copy(&m_oTextColor);
		pNewDC->m_oBgColor.Copy(&m_oBgColor);
		pNewDC->m_ulTextAlign    = m_ulTextAlign;
		pNewDC->m_ulBgMode       = m_ulBgMode;
		pNewDC->m_ulMiterLimit   = m_ulMiterLimit;
		pNewDC->m_ulFillMode     = m_ulFillMode;
		pNewDC->m_ulStretchMode  = m_ulStretchMode;
		pNewDC->m_ulRop2Mode     = m_ulRop2Mode;
		pNewDC->m_dPixelHeight   = m_dPixelHeight;
		pNewDC->m_dPixelWidth    = m_dPixelWidth;
		pNewDC->m_oWindow.Copy(&m_oWindow);
		pNewDC->m_oViewport.Copy(&m_oViewport);
		pNewDC->m_oCurPos        = m_oCurPos;
		pNewDC->m_oClip          = m_oClip;
		pNewDC->m_unArcDirection = m_unArcDirection;

		return pNewDC;
	}
	void            CEmfDC::SetMapMode(unsigned int ulMapMode)
	{
		m_ulMapMode = ulMapMode;

		switch (ulMapMode)
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
	unsigned int    CEmfDC::GetMapMode()
	{
		return m_ulMapMode;
	}
	TEmfXForm*      CEmfDC::GetTransform()
	{
		return &m_oTransform;
	}
	TEmfXForm*      CEmfDC::GetInverseTransform()
	{
		return &m_oInverseTransform;
	}
	TEmfXForm*      CEmfDC::GetFinalTransform(int iGraphicsMode)
	{
		if (GM_COMPATIBLE == iGraphicsMode)
			return &m_oFinalTransform2;

		return &m_oFinalTransform;
	}
	void            CEmfDC::MultiplyTransform(TEmfXForm& oForm, unsigned int ulMode)
	{
		m_oTransform.Multiply(oForm, ulMode);

		// Обновляем обратную матрицу
		TEmfXForm* pT = &m_oTransform;
		double dDet = pT->M11 * pT->M22 - pT->M12 * pT->M21;
		if (dDet < 0.0001 && dDet > 0.0001)
		{
			m_oInverseTransform.M11 = 1;
			m_oInverseTransform.M12 = 0;
			m_oInverseTransform.M21 = 0;
			m_oInverseTransform.M22 = 1;
			m_oInverseTransform.Dx  = 0;
			m_oInverseTransform.Dy  = 0;
		}

		m_oInverseTransform.M11 = pT->M22 / dDet;
		m_oInverseTransform.M12 = -pT->M12 / dDet;
		m_oInverseTransform.M21 = -pT->M21 / dDet;
		m_oInverseTransform.M22 = pT->M22 / dDet;
		m_oInverseTransform.Dx  = pT->Dy * pT->M21 / dDet - pT->Dx * pT->M22 / dDet;
		m_oInverseTransform.Dy  = pT->Dx * pT->M12 / dDet - pT->Dy * pT->M11 / dDet;

		UpdateFinalTransform();
	}
	void            CEmfDC::SetTextColor(TEmfColor& oColor)
	{
		m_oTextColor.Copy(&oColor);
	}
	TEmfColor&      CEmfDC::GetTextColor()
	{
		return m_oTextColor;
	}
	void            CEmfDC::SetBrush(CEmfLogBrushEx* pBrush)
	{
		m_pBrush = pBrush;
	}
	void            CEmfDC::RemoveBrush(CEmfLogBrushEx* pBrush)
	{
		if (pBrush == m_pBrush)
			m_pBrush = NULL;
	}
	CEmfLogBrushEx* CEmfDC::GetBrush()
	{
		return m_pBrush;
	}
	void            CEmfDC::SetFont(CEmfLogFont* pFont)
	{
		m_pFont = pFont;
	}
	void            CEmfDC::RemoveFont(CEmfLogFont* pFont)
	{
		if (pFont == m_pFont)
			m_pFont = NULL;
	}
	CEmfLogFont*    CEmfDC::GetFont()
	{
		return m_pFont;
	}
	void            CEmfDC::SetTextAlign(unsigned int ulAlign)
	{
		m_ulTextAlign = ulAlign;
	}
	unsigned int    CEmfDC::GetTextAlign()
	{
		return m_ulTextAlign;
	}
	void            CEmfDC::SetBgMode(unsigned int ulBgMode)
	{
		m_ulBgMode = ulBgMode;
	}
	unsigned int    CEmfDC::GetBgMode()
	{
		return m_ulBgMode;
	}
	void            CEmfDC::SetBgColor(TEmfColor& oColor)
	{
		m_oBgColor.Copy(&oColor);
	}
	TEmfColor&      CEmfDC::GetBgColor()
	{
		return m_oBgColor;
	}
	void            CEmfDC::SetMiterLimit(unsigned int ulMiter)
	{
		m_ulMiterLimit = ulMiter;
	}
	unsigned int    CEmfDC::GetMiterLimit()
	{
		return m_ulMiterLimit;
	}
	void            CEmfDC::SetFillMode(unsigned int ulFillMode)
	{
		m_ulFillMode = ulFillMode;
	}
	unsigned int    CEmfDC::GetFillMode()
	{
		return m_ulFillMode;
	}
	void            CEmfDC::SetPen(CEmfLogPen* pPen)
	{
		m_pPen = pPen;
	}
	void            CEmfDC::RemovePen(CEmfLogPen* pPen)
	{
		if (pPen == m_pPen)
			m_pPen = NULL;
	}
	CEmfLogPen*     CEmfDC::GetPen()
	{
		return m_pPen;
	}
	void            CEmfDC::SetStretchMode(unsigned int& oMode)
	{
		m_ulStretchMode = oMode;
	}
	unsigned int    CEmfDC::GetStretchMode()
	{
		return m_ulStretchMode;
	}
	double          CEmfDC::GetPixelWidth()
	{
		return m_dPixelWidth;
	}
	double          CEmfDC::GetPixelHeight()
	{
		return m_dPixelHeight;
	}
	void            CEmfDC::SetPixelWidth(double dPixelW)
	{
		m_dPixelWidth = dPixelW;
	}
	void            CEmfDC::SetPixelHeight(double dPixelH)
	{
		m_dPixelHeight = dPixelH;
	}
	void            CEmfDC::SetWindowOrigin(TEmfPointL& oPoint)
	{
		m_oWindow.lX = oPoint.x;
		m_oWindow.lY = oPoint.y;
		UpdatePixelMetrics();
		UpdateFinalTransform();
	}
	void            CEmfDC::SetWindowExtents(TEmfSizeL& oPoint)
	{
		m_oWindow.ulW = oPoint.cx;
		m_oWindow.ulH = oPoint.cy;

		UpdatePixelMetrics();
		UpdateFinalTransform();
	}
	TEmfWindow*     CEmfDC::GetWindow()
	{
		return &m_oWindow;
	}
	void            CEmfDC::SetViewportOrigin(TEmfPointL& oPoint)
	{
		m_oViewport.lX = oPoint.x;
		m_oViewport.lY = oPoint.y;
		UpdatePixelMetrics();
		UpdateFinalTransform();
	}
	void            CEmfDC::SetViewportExtents(TEmfSizeL& oPoint)
	{
		m_oViewport.ulW = oPoint.cx;
		m_oViewport.ulH = oPoint.cy;
		UpdatePixelMetrics();
		UpdateFinalTransform();
	}
	TEmfWindow*     CEmfDC::GetViewport()
	{
		return &m_oViewport;
	}
	bool            CEmfDC::UpdatePixelMetrics()
	{
		unsigned int ulMapMode = m_ulMapMode;
		if (MM_ISOTROPIC == ulMapMode)
		{
			if (0 == m_oWindow.ulW || 0 == m_oViewport.ulW)
				return false;

			double dPixel = (double)m_oViewport.ulW / (double)m_oWindow.ulW;
			SetPixelHeight(dPixel);
			SetPixelWidth(dPixel);
		}
		else if (MM_ANISOTROPIC == ulMapMode)
		{
			double dPixelX = (double)m_oViewport.ulW / (double)m_oWindow.ulW;
			double dPixelY = (double)m_oViewport.ulH / (double)m_oWindow.ulH;

			SetPixelWidth(dPixelX);
			SetPixelHeight(dPixelY);
		}

		return true;
	}
	void            CEmfDC::UpdateFinalTransform()
	{
		TEmfWindow* pWindow   = GetWindow();
		TEmfWindow* pViewPort = GetViewport();

		TEmfXForm oWindowXForm(1, 0, 0, 1, -pWindow->lX, -pWindow->lY);
		TEmfXForm oViewportXForm((double)GetPixelWidth(), 0, 0, (double)GetPixelHeight(), pViewPort->lX, pViewPort->lY);

		m_oFinalTransform.Init();
		m_oFinalTransform.Multiply(oWindowXForm, MWT_RIGHTMULTIPLY);
		m_oFinalTransform.Multiply(m_oTransform, MWT_RIGHTMULTIPLY);
		m_oFinalTransform.Multiply(oViewportXForm, MWT_RIGHTMULTIPLY);

		m_oFinalTransform2.Init();
		m_oFinalTransform2.Multiply(oWindowXForm, MWT_RIGHTMULTIPLY);
		m_oFinalTransform2.Multiply(oViewportXForm, MWT_RIGHTMULTIPLY);
	}
	void            CEmfDC::SetRop2Mode(unsigned int& nMode)
	{
		m_ulRop2Mode = nMode;
	}
	unsigned int    CEmfDC::GetRop2Mode()
	{
		return m_ulRop2Mode;
	}
	void            CEmfDC::SetPalette(CEmfLogPalette* pPalette)
	{
		m_pPalette = pPalette;
	}
	void            CEmfDC::RemovePalette(CEmfLogPalette* pPalette)
	{
		if (m_pPalette == pPalette)
			m_pPalette = NULL;
	}
	CEmfLogPalette* CEmfDC::GetPalette()
	{
		return m_pPalette;
	}
	void            CEmfDC::SetCurPos(TEmfPointL& oPoint)
	{
		SetCurPos(oPoint.x, oPoint.y);
	}
	void            CEmfDC::SetCurPos(int lX, int lY)
	{
		m_oCurPos.x = lX;
		m_oCurPos.y = lY;
	}
	TEmfPointL&     CEmfDC::GetCurPos()
	{
		return m_oCurPos;
	}
	CEmfClip*       CEmfDC::GetClip()
	{
		return &m_oClip;
	}
	void            CEmfDC::ClipToPath(CEmfPath* pPath, unsigned int unMode, TEmfXForm* pTransform)
	{
		m_oClip.SetPath(pPath, unMode, pTransform);
	}
	void            CEmfDC::SetArcDirection(unsigned int unDirection)
	{
		m_unArcDirection = unDirection;
	}
	unsigned int    CEmfDC::GetArcDirection()
	{
		return m_unArcDirection;
	}
}
