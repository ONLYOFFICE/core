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
#pragma once

#ifndef _METAFILE_EMF_EMFPLAYER_H
#define _METAFILE_EMF_EMFPLAYER_H

#include "EmfTypes.h"
#include "EmfObjects.h"
#include "EmfPlusObjects.h"
#include "EmfClip.h"

#include <vector>
#include <map>

namespace MetaFile
{
	class CEmfFile;
	class CEmfParserBase;
	class CEmfDC;
	class CEmfPath;

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

		CEmfDC*              m_pDC;
		EmfDCsMap            m_mDCs;
		CEmfParserBase*	     m_pParser;
		CEmfObjectMap        m_mObjects;
	};

	class CEmfDC
	{
	public:

		CEmfDC(CEmfPlayer* pPlayer);
		~CEmfDC();
		CEmfDC* Copy();

		void            SetMapMode(unsigned int ulMapMode);
		unsigned int    GetMapMode();
		void            ResetTransform();
		TEmfXForm*      GetTransform();
		TEmfXForm*      GetInverseTransform();
		TEmfXForm*      GetFinalTransform(int iGraphicsMode);
		void            MultiplyTransform(TEmfXForm& oForm, unsigned int ulMode);
		void            SetTextColor(TEmfColor& oColor);
		TEmfColor&      GetTextColor();
		void            SetBrush(IBrush* pBrush);
		void            RemoveBrush(IBrush* pBrush);
		IBrush*         GetBrush();
		void            SetFont(IFont* pFont);
		void            RemoveFont(IFont* pFont);
		IFont*          GetFont();
		void            SetTextAlign(unsigned int ulAlign);
		unsigned int    GetTextAlign();
		void            SetBgMode(unsigned int ulBgMode);
		unsigned int    GetBgMode();
		void            SetBgColor(TEmfColor& oColor);
		TEmfColor&      GetBgColor();
		void            SetMiterLimit(unsigned int ulMiter);
		unsigned int    GetMiterLimit();
		void            SetFillMode(unsigned int ulFillMode);
		unsigned int    GetFillMode();
		void            SetPen(IPen* pPen);
		void            RemovePen(IPen* pPen);
		IPen*           GetPen();
		void            SetStretchMode(unsigned int& oMode);
		unsigned int    GetStretchMode();
		double          GetPixelWidth();
		double          GetPixelHeight();
		void            SetWindowOrigin(TEmfPointL& oPoint);
		void            SetWindowExtents(TEmfSizeL& oPoint);
		void            ScaleWindow(double dXScale, double dYScale);
		TEmfWindow*     GetWindow();
		void            SetViewportOrigin(TEmfPointL& oPoint);
		void            SetViewportExtents(TEmfSizeL& oPoint);
		void            ScaleViewport(double dXScale, double dYScale);
		TEmfWindow*     GetViewport();
		void            SetRop2Mode(unsigned int& nMode);
		unsigned int    GetRop2Mode();
		void            SetPalette(CEmfLogPalette* pPalette);
		void            RemovePalette(CEmfLogPalette* pPalette);
		CEmfLogPalette* GetPalette();
		void            SetCurPos(TEmfPointL& oPoint);
		void            SetCurPos(int lX, int lY);
		TEmfPointL&     GetCurPos();
		CEmfClip*       GetClip();
		void            ClipToPath(CEmfPath* pPath, unsigned int unMode, TEmfXForm* pTransform);
		void            SetArcDirection(unsigned int unDirection);
		unsigned int    GetArcDirection();

	private:

		void            SetPixelWidth(double dPixelW);
		void            SetPixelHeight(double dPixelH);
		bool            UpdatePixelMetrics();
		void            UpdateFinalTransform();
		void            FixIsotropic();

	private:

		CEmfPlayer*     m_pPlayer;
		unsigned int    m_ulMapMode;
		IBrush*         m_pBrush;
		IPen*           m_pPen;
		IFont*          m_pFont;
		CEmfLogFont     m_oDefaultFont;
		CEmfLogPalette* m_pPalette;
		TEmfXForm       m_oTransform;
		TEmfXForm       m_oInverseTransform;
		TEmfXForm       m_oFinalTransform;
		TEmfXForm       m_oFinalTransform2; // Преобразование без учета m_oTransform, т.е. чисто преобразование окна
		TEmfColor       m_oTextColor;
		TEmfColor       m_oBgColor;
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
		TEmfPointL      m_oCurPos;
		CEmfClip        m_oClip;
		unsigned int    m_unArcDirection;
	};
}

#endif //_METAFILE_EMF_EMFPLAYER_H
