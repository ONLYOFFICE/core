/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
#ifndef _METAFILE_WMF_WMFPLAYER_H
#define _METAFILE_WMF_WMFPLAYER_H

#include "../Common/CClip.h"

#include "WmfObjects.h"

#include <map>
#include <vector>

namespace MetaFile
{
	class CWmfDC;
	class CWmfFile;
	class CWmfParserBase;

	class CWmfPlayer
	{
	public:

		CWmfPlayer(CWmfParserBase* pFile);
		~CWmfPlayer();
		void Clear();
		CWmfDC* SaveDC();
		CWmfDC* RestoreDC();
		CWmfDC* GetDC();
		
		void RegisterObject(CWmfObjectBase* pObject);
		void SelectObject(unsigned short ushIndex);
		void SelectPalette(unsigned short ushIndex);
		void DeleteObject(unsigned short ushIndex);

	private:

		typedef std::map <unsigned int, CWmfObjectBase*> CWmfObjectMap;

		CWmfDC*                     m_pDC;
		std::vector<CWmfDC*>        m_vDCStack;
		CWmfParserBase*             m_pParser;
		CWmfObjectMap               m_mObjects;
		unsigned short              m_ushIndex;
		std::vector<unsigned short> m_vAvailableIndexes;
	};

	class CWmfDC
	{
	public:
		CWmfDC();
		~CWmfDC();
		CWmfDC*            Copy() const;
		CClip*             GetClip();
		void               SetBrush(CWmfBrush* pBrush);
		void               RemoveBrush(CWmfBrush* pBrush);
		const CWmfBrush*   GetBrush() const;
		void               SetPen(CWmfPen* pPen);
		void               RemovePen(CWmfPen* pPen);
		const CWmfPen*     GetPen() const;
		void               SetPalette(CWmfPalette* pPalette);
		void               RemovePalette(CWmfPalette* pPalette);
		const CWmfPalette* GetPalette() const;
		void               SetFont(CWmfFont* pFont);
		void               RemoveFont(CWmfFont* pFont);
		const CWmfFont*    GetFont() const;
		void               SetRegion(CWmfRegion* pRegion);
		void               RemoveRegion(CWmfRegion* pRegion);
		const CWmfRegion*  GetRegion() const;
		void               SetMapMode(unsigned short ushMapMode);
		unsigned int       GetMapMode() const;
		double             GetPixelWidth() const;
		double             GetPixelHeight() const;
		const TWmfWindow&  GetWindow() const;
		void               SetWindowOrg(short shX, short shY);
		void               SetWindowExt(short shW, short shH);
		void               SetWindowOff(short shX, short shY);
		void               SetWindowScale(double dX, double dY);
		const TWmfWindow&  GetViewport() const;
		void               SetViewportOrg(short shX, short shY);
		void               SetViewportExt(short shW, short shH);
		void               SetViewportOff(short shX, short shY);
		void               SetViewportScale(double dX, double dY);
		void               SetTextColor(TRGBA& oColor);
		const TRGBA&       GetTextColor() const;
		void               SetTextBgColor(TRGBA& oColor);
		const TRGBA&       GetTextBgColor() const;
		const TPointS&     GetCurPos() const;
		void               SetCurPos(TPointS& oPoint);
		void               SetCurPos(short shX, short shY);
		void               SetTextBgMode(unsigned short ushMode);
		unsigned short     GetTextBgMode() const;
		void               SetLayout(unsigned short ushLayout);
		unsigned short     GetLayout() const;
		void               SetPolyFillMode(unsigned short ushMode);
		unsigned short     GetPolyFillMode() const;
		void               SetRop2Mode(unsigned short ushMode);
		unsigned short     GetRop2Mode() const;
		void               SetStretchBltMode(unsigned short ushMode);
		unsigned short     GetStretchBltMode() const;
		void               SetTextAlign(unsigned short ushTextAlign);
		unsigned short     GetTextAlign() const;
		void               SetCharSpacing(unsigned short ushCharSpacing);
		unsigned short     GetCharSpacing() const;
		const TXForm&      GetTransform() const;
		const TXForm&      GetInverseTransform() const;
		const TXForm&      GetFinalTransform(int iGraphicsMode) const;
		unsigned int       GetMiterLimit() const;

	private:

		void           SetPixelWidth(double dW);
		void           SetPixelHeight(double dH);
		bool           UpdatePixelMetrics();
		void           UpdateFinalTransform();
		void           FixIsotropic();

	private:

		CClip          m_oClip;
		CWmfBrush      m_oDefaultBrush;
		CWmfFont       m_oDefaultFont;
		CWmfPen        m_oDefaultPen;
		CWmfBrush*     m_pBrush;
		CWmfPen*       m_pPen;
		CWmfPalette*   m_pPalette;
		CWmfFont*      m_pFont;
		CWmfRegion*    m_pRegion;
		unsigned short m_ushMapMode;
		double         m_dPixelWidth;
		double         m_dPixelHeight;
		TWmfWindow     m_oWindow;
		TWmfWindow     m_oViewport;
		TRGBA          m_oTextColor;
		TRGBA          m_oTextBgColor;
		TPointS        m_oCurPos;
		unsigned short m_ushTextBgMode;
		unsigned short m_ushLayout;
		unsigned short m_ushPolyFillMode;
		unsigned short m_ushRop2Mode;
		unsigned short m_ushStretchBltMode;
		unsigned short m_ushTextAlign;
		unsigned short m_ushCharSpacing;
		TXForm         m_oTransform;
		TXForm         m_oFinalTransform;
		TXForm         m_oFinalTransform2;
	};
}

#endif //_METAFILE_WMF_WMFPLAYER_H
