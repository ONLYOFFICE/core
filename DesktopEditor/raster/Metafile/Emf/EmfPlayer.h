/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#ifndef _METAFILE_EMF_EMFPLAYER_H
#define _METAFILE_EMF_EMFPLAYER_H

#include "EmfTypes.h"
#include "EmfObjects.h"
#include "EmfClip.h"

#include <vector>
#include <map>

namespace MetaFile
{
	class CEmfFile;
	class CEmfDC;
	class CEmfPath;

	class CEmfPlayer
	{
	public:

		CEmfPlayer(CEmfFile* pFile);
		~CEmfPlayer();
		void Clear();
		CEmfDC* SaveDC();
		CEmfDC* RestoreDC();
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

		CEmfDC*              m_pDC;
		std::vector<CEmfDC*> m_vDCStack;
		CEmfFile*            m_pEmfFile;		
		CEmfObjectMap        m_mObjects;
	};

	class CEmfDC
	{
	public:

		CEmfDC();
		~CEmfDC();
		CEmfDC* Copy();

		void            SetMapMode(unsigned int ulMapMode);
		unsigned int    GetMapMode();
		TEmfXForm*      GetTransform();
		TEmfXForm*      GetInverseTransform();
		void            MultiplyTransform(TEmfXForm& oForm, unsigned int ulMode);
		void            SetTextColor(TEmfColor& oColor);
		TEmfColor&      GetTextColor();
		void            SetBrush(CEmfLogBrushEx* pBrush);
		void            RemoveBrush(CEmfLogBrushEx* pBrush);
		CEmfLogBrushEx* GetBrush();
		void            SetFont(CEmfLogFont* pFont);
		void            RemoveFont(CEmfLogFont* pFont);
		CEmfLogFont*    GetFont();
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
		void            SetPen(CEmfLogPen* pPen);
		void            RemovePen(CEmfLogPen* pPen);
		CEmfLogPen*     GetPen();
		void            SetStretchMode(unsigned int& oMode);
		unsigned int    GetStretchMode();
		double          GetPixelWidth();
		double          GetPixelHeight();
		void            SetWindowOrigin(TEmfPointL& oPoint);
		void            SetWindowExtents(TEmfSizeL& oPoint);
		TEmfWindow*     GetWindow();
		void            SetViewportOrigin(TEmfPointL& oPoint);
		void            SetViewportExtents(TEmfSizeL& oPoint);
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
		void            ClipToPath(CEmfPath* pPath, unsigned int unMode);
		void            SetArcDirection(unsigned int unDirection);
		unsigned int    GetArcDirection();

	private:

		void            SetPixelWidth(double dPixelW);
		void            SetPixelHeight(double dPixelH);
		bool            UpdatePixelMetrics();

	private:

		unsigned int    m_ulMapMode;
		CEmfLogBrushEx* m_pBrush;
		CEmfLogPen*     m_pPen;
		CEmfLogFont*    m_pFont;
		CEmfLogPalette* m_pPalette;
		TEmfXForm       m_oTransform;
		TEmfXForm       m_oInverseTransform;
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