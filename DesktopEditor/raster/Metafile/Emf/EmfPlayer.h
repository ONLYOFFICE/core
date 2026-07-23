/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#ifndef _METAFILE_EMF_EMFPLAYER_H
#define _METAFILE_EMF_EMFPLAYER_H

#include "../Common/CClip.h"

#include "EmfTypes.h"
#include "EmfObjects.h"

#include <vector>
#include <map>

namespace MetaFile
{
	class CEmfFile;
	class CEmfParserBase;
	class CEmfDC;

	class CEmfPlayer
	{
	public:
		CEmfPlayer(CEmfParserBase* pParser);
		~CEmfPlayer();
		void Clear();
		void SaveDC(int nIndex = -1);
		void RestoreDC(int nIndex);
		CEmfDC* GetDC();
		void RegisterObject(unsigned int ulIndex, CEmfObjectBase* pObject);
		void SelectObject(unsigned int ulIndex);
		void DeleteObject(unsigned int ulIndex);
		void SelectPalette(unsigned int ulIndex);

	private:

		void InitStockObjects();
		void InitStockBrush(bool bNull, unsigned char r, unsigned char g, unsigned char b, unsigned int ulIndex);
		void InitStockPen(bool bNull, unsigned char r, unsigned char g, unsigned char b, unsigned int ulIndex);

	private:

		typedef std::map < unsigned int, CEmfObjectBase* > CEmfObjectMap;
		typedef std::map < int, CEmfDC* > EmfDCsMap;

		CEmfDC*         m_pDC;
		EmfDCsMap       m_mDCs;
		CEmfParserBase* m_pParser;
		CEmfObjectMap   m_mObjects;
	};

	class CEmfDC
	{
	public:

		CEmfDC(CEmfPlayer* pPlayer);
		~CEmfDC();
		CEmfDC* Copy();

		CClip*          GetClip();

		void                  SetMapMode(unsigned int ulMapMode);
		unsigned int          GetMapMode() const;
		void                  ResetTransform();
		const TEmfXForm&      GetTransform() const;
		const TEmfXForm&      GetInverseTransform() const ;
		const TEmfXForm&      GetFinalTransform(int iGraphicsMode) const;
		void                  MultiplyTransform(TEmfXForm& oForm, unsigned int ulMode);
		void                  SetTextColor(const TRGBA& oColor);
		const TRGBA&          GetTextColor() const;
		void                  SetBrush(IBrush* pBrush);
		void                  RemoveBrush(IBrush* pBrush);
		const IBrush*         GetBrush() const;
		void                  SetFont(IFont* pFont);
		void                  RemoveFont(IFont* pFont);
		const IFont*          GetFont() const;
		void                  SetTextAlign(unsigned int ulAlign);
		unsigned int          GetTextAlign() const;
		void                  SetBgMode(unsigned int ulBgMode);
		unsigned int          GetBgMode() const;
		void                  SetBgColor(TRGBA& oColor);
		const TRGBA*          GetBgColor() const;
		void                  SetMiterLimit(unsigned int ulMiter);
		unsigned int          GetMiterLimit() const;
		void                  SetFillMode(unsigned int ulFillMode);
		unsigned int          GetFillMode() const;
		void                  SetPen(IPen* pPen);
		void                  RemovePen(IPen* pPen);
		const IPen*           GetPen() const;
		void                  SetStretchMode(unsigned int& oMode);
		unsigned int          GetStretchMode() const;
		double                GetPixelWidth() const;
		double                GetPixelHeight() const;
		void                  SetWindowOrigin(const TPointL& oPoint);
		void                  SetWindowExtents(const TSizeL& oPoint);
		void                  ScaleWindow(const double& dXScale, const double& dYScale);
		const TEmfWindow&     GetWindow() const;
		void                  SetViewportOrigin(const TPointL& oPoint);
		void                  SetViewportExtents(const TSizeL& oPoint);
		void                  ScaleViewport(const double& dXScale, const double& dYScale);
		const TEmfWindow&     GetViewport() const;
		void                  SetRop2Mode(unsigned int nMode);
		unsigned int          GetRop2Mode() const;
		void                  SetPalette(CEmfLogPalette* pPalette);
		void                  RemovePalette(CEmfLogPalette* pPalette);
		const CEmfLogPalette* GetPalette() const;
		void                  SetCurPos(const TPointL& oPoint);
		void                  SetCurPos(int lX, int lY);
		const TPointL&        GetCurPos() const;
		void                  SetArcDirection(unsigned int unDirection);
		unsigned int          GetArcDirection() const;

	private:

		void            SetPixelWidth(double dPixelW);
		void            SetPixelHeight(double dPixelH);
		bool            UpdatePixelMetrics();
		void            UpdateFinalTransform();
		void            FixIsotropic();

	private:

		CEmfPlayer*     m_pPlayer;
		CClip           m_oClip;
		unsigned int    m_ulMapMode;
		IBrush*         m_pBrush;
		IPen*           m_pPen;
		IFont*          m_pFont;
		CEmfLogFont     m_oDefaultFont;
		CEmfLogPalette* m_pPalette;
		TEmfXForm       m_oTransform;
		TEmfXForm       m_oInverseTransform;
		TEmfXForm       m_oFinalTransform;
		TEmfXForm       m_oFinalTransform2; // Transformation without m_oTransform, i.e. pure window transformation
		TRGBA           m_oTextColor;
		TRGBA           m_oBgColor;
		unsigned int    m_ulTextAlign;
		unsigned int    m_ulBgMode;
		unsigned int    m_ulMiterLimit;
		unsigned int    m_ulFillMode;
		unsigned int    m_ulStretchMode;
		unsigned int    m_ulRop2Mode;
		double          m_dPixelWidth;
		double          m_dPixelHeight;
		TEmfWindow      m_oWindow;
		TEmfWindow      m_oViewport;
		TPointL         m_oCurPos;
		unsigned int    m_unArcDirection;
	};
}

#endif //_METAFILE_EMF_EMFPLAYER_H
