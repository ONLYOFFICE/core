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
#ifndef _METAFILE_EMF_EMFFILE_H
#define _METAFILE_EMF_EMFFILE_H

#include "../Common/MetaFile.h"

#include "EmfTypes.h"
#include "EmfPlayer.h"
#include "EmfPath.h"

#if defined(DrawText)
#undef DrawText
#endif

namespace MetaFile
{
	class CEmfFile : public IMetaFileBase
	{
	public:

		CEmfFile() : m_oPlayer(this)
		{
			m_pPath = NULL;
			m_pDC   = m_oPlayer.GetDC();
		}

		~CEmfFile()
		{
			ClearFile();
		}

		TEmfRectL* GetBounds()
		{
			//return &m_oHeader.oFrame;
			return &m_oHeader.oFramePx;
		}		
		void PlayMetaFile();
		void ClearFile()
		{
			RELEASEOBJECT(m_pPath);
			m_oPlayer.Clear();
			m_pDC = m_oPlayer.GetDC();
		}
		TRect* GetDCBounds()
		{
			return &m_oHeader.oFrameToBounds;
		}
		double GetPixelHeight()
		{
			return m_pDC->GetPixelHeight();
		}
		double GetPixelWidth()
		{
			return m_pDC->GetPixelWidth();
		}
		int GetTextColor()
		{
			TEmfColor& oColor = m_pDC->GetTextColor();
			return METAFILE_RGBA(oColor.r, oColor.g, oColor.b);
		}
		IFont* GetFont()
		{
			CEmfLogFont* pFont = m_pDC->GetFont();
			if (!pFont)
				return NULL;

			return (IFont*)pFont;
		}
		IBrush* GetBrush()
		{
			CEmfLogBrushEx* pBrush = m_pDC->GetBrush();
			if (!pBrush)
				return NULL;

			return (IBrush*)pBrush;
		}
		IPen* GetPen()
		{
			CEmfLogPen* pPen = m_pDC->GetPen();
			if (!pPen)
				return NULL;

			return (IPen*)pPen;
		}
		unsigned int GetTextAlign()
		{
			return m_pDC->GetTextAlign();
		}
		unsigned int GetTextBgMode()
		{
			return m_pDC->GetBgMode();
		}
		int GetTextBgColor()
		{
			TEmfColor& oColor = m_pDC->GetBgColor();
			return METAFILE_RGBA(oColor.r, oColor.g, oColor.b);
		}
		unsigned int GetFillMode()
		{
			return m_pDC->GetFillMode();
		}
		TPointD GetCurPos()
		{
			TPointL oPoint = m_pDC->GetCurPos();
			TPointD oRes;
			TranslatePoint(oPoint.x, oPoint.y, oRes.x, oRes.y);
			return oRes;
		}
		TXForm* GetInverseTransform()
		{
			return m_pDC->GetInverseTransform();
		}
		TXForm* GetTransform(int iGraphicsMode)
		{
			TRect* pBounds = GetDCBounds();
			double dT = pBounds->nTop;
			double dL = pBounds->nLeft;

			TXForm oShiftXForm(1, 0, 0, 1, -dL, -dT);
			m_oTransform.Copy(m_pDC->GetFinalTransform(iGraphicsMode));
			m_oTransform.Multiply(oShiftXForm, MWT_RIGHTMULTIPLY);
			return &m_oTransform;
		}
		unsigned int GetMiterLimit()
		{
			return m_pDC->GetMiterLimit();
		}
		unsigned int GetRop2Mode()
		{
			return m_pDC->GetRop2Mode();
		}
		IClip* GetClip()
		{
			CEmfClip* pClip = m_pDC->GetClip();
			if (!pClip)
				return NULL;

			return (IClip*)pClip;
		}
		int GetCharSpace()
		{
			return 0;
		}
		bool IsWindowFlippedY()
		{
			TEmfWindow* pWindow = m_pDC->GetWindow();
			return (pWindow->ulH < 0);
		}
		bool IsWindowFlippedX()
		{
			TEmfWindow* pWindow = m_pDC->GetWindow();
			return (pWindow->ulW < 0);
		}

	private:

		void TranslatePoint(TEmfPointL& oPoint, double& dX, double& dY);
		void TranslatePoint(int nX, int nY, double& dX, double &dY);
		CEmfDC* GetDC()
		{
			return m_pDC;
		}
		bool ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE** ppBgraBuffer, unsigned int* pulWidth, unsigned int* pulHeight);
		void DrawImage(int nX, int nY, int nW, int nH, BYTE* pImageBuffer, unsigned int unImageW, unsigned int unImageH);
		void MoveTo(TEmfPointL& oPoint)
		{
			MoveTo(oPoint.x, oPoint.y);
		}
		void MoveTo(TEmfPointS& oPoint)
		{
			MoveTo(oPoint.x, oPoint.y);
		}
		void MoveTo(int nX, int nY);
		void LineTo(int nX, int nY);
		void LineTo(TEmfPointL& oPoint)
		{
			LineTo(oPoint.x, oPoint.y);
		}
		void LineTo(TEmfPointS& oPoint)
		{
			LineTo(oPoint.x, oPoint.y);
		}
		void CurveTo(int nX1, int nY1, int nX2, int nY2, int nXe, int nYe);
		void CurveTo(TEmfPointS& oPoint1, TEmfPointS& oPoint2, TEmfPointS& oPointE)
		{
			CurveTo(oPoint1.x, oPoint1.y, oPoint2.x, oPoint2.y, oPointE.x, oPointE.y);
		}
		void CurveTo(TEmfPointL& oPoint1, TEmfPointL& oPoint2, TEmfPointL& oPointE)
		{
			CurveTo(oPoint1.x, oPoint1.y, oPoint2.x, oPoint2.y, oPointE.x, oPointE.y);
		}
		void ClosePath();
		void ArcTo(int nL, int nT, int nR, int nB, double dStart, double dSweep);
		void DrawPath(bool bStroke, bool bFill);
		void UpdateOutputDC()
		{
			if (m_pOutput)
				m_pOutput->UpdateDC();
		}
		void DrawText(std::wstring& wsString, unsigned int unCharsCount, int _nX, int _nY, int* pnDx = NULL, int iGraphicsMode = GM_ADVANCED);
		void DrawTextA(TEmfEmrText& oText, int iGraphicsMode);
		void DrawTextW(TEmfEmrText& oText, int iGraphicsMode);
		void Read_EMR_HEADER();
		void Read_EMR_ALPHABLEND();
		void Read_EMR_STRETCHDIBITS();
		void Read_EMR_BITBLT();
		void Read_EMR_SETDIBITSTODEVICE();
		void Read_EMR_STRETCHBLT();
		void Read_EMR_EOF();
		void Read_EMR_UNKNOWN();
		void Read_EMR_SAVEDC();
		void Read_EMR_RESTOREDC();
		void Read_EMR_MODIFYWORLDTRANSFORM();
		void Read_EMR_SETWORLDTRANSFORM();
		void Read_EMR_CREATEBRUSHINDIRECT();
		void Read_EMR_SETTEXTCOLOR();
		void Read_EMR_SELECTOBJECT();
		void Read_EMR_EXTCREATEFONTINDIRECTW();
		void Read_EMR_SETTEXTALIGN();
		void Read_EMR_SETBKMODE();
		void Read_EMR_DELETEOBJECT();
		void Read_EMR_SETMITERLIMIT();
		void Read_EMR_EXTCREATEPEN();
		void Read_EMR_CREATEPEN();
		void Read_EMR_SETPOLYFILLMODE();
		void Read_EMR_BEGINPATH();
		void Read_EMR_ENDPATH();
		void Read_EMR_CLOSEFIGURE();
		void Read_EMR_FLATTENPATH();
		void Read_EMR_WIDENPATH();
		void Read_EMR_ABORTPATH();
		void Read_EMR_MOVETOEX();
		void Read_EMR_SETARCDIRECTION();
		void Read_EMR_FILLPATH();
		void Read_EMR_SETMAPMODE();
		void Read_EMR_SETWINDOWORGEX();
		void Read_EMR_SETWINDOWEXTEX();
		void Read_EMR_SETVIEWPORTORGEX();
		void Read_EMR_SETVIEWPORTEXTEX();
		void Read_EMR_SETSTRETCHBLTMODE();
		void Read_EMR_SETICMMODE();
		void Read_EMR_CREATEDIBPATTERNBRUSHPT();
		void Read_EMR_SELECTCLIPPATH();
		void Read_EMR_SETBKCOLOR();
		void Read_EMR_EXCLUDECLIPRECT();
		void Read_EMR_EXTSELECTCLIPRGN();
		void Read_EMR_SETMETARGN();
		void Read_EMR_SETROP2();
		void Read_EMR_CREATEPALETTE();
		void Read_EMR_SELECTPALETTE();
		void Read_EMR_REALIZEPALETTE();
		void Read_EMR_INTERSECTCLIPRECT();
		void Read_EMR_SETLAYOUT();
		void Read_EMR_SETBRUSHORGEX();
		void Read_EMR_ANGLEARC();
		void Read_EMR_ARC_BASE(TEmfRectL& oBox, TEmfPointL& oStart, TEmfPointL& oEnd, double& dStartAngle, double& dSweepAngle);
		void Read_EMR_ARC();
		void Read_EMR_ARCTO();
		void Read_EMR_CHORD();
		void Read_EMR_ELLIPSE();
		void Read_EMR_EXTTEXTOUTA();
		void Read_EMR_EXTTEXTOUTW();
		void Read_EMR_LINETO();
		void Read_EMR_PIE();
		void Read_EMR_POLYBEZIER();
		void Read_EMR_POLYBEZIER16();
		template<typename T>void Read_EMR_POLYBEZIER_BASE();
		void Read_EMR_POLYBEZIERTO();
		void Read_EMR_POLYBEZIERTO16();
		template<typename T>void Read_EMR_POLYBEZIERTO_BASE();
		void Read_EMR_POLYDRAW();
		void Read_EMR_POLYDRAW16();
		template<typename T>void Read_EMR_POLYDRAW_BASE();
		void Read_EMR_POLYGON();
		void Read_EMR_POLYGON16();
		template<typename T>void Read_EMR_POLYGON_BASE();
		void Read_EMR_POLYLINE();
		void Read_EMR_POLYLINE16();
		template<typename T>void Read_EMR_POLYLINE_BASE();
		void Read_EMR_POLYLINETO();
		void Read_EMR_POLYLINETO16();
		template<typename T>void Read_EMR_POLYLINETO_BASE();
		void Read_EMR_POLYPOLYGON();
		void Read_EMR_POLYPOLYGON16();
		template<typename T>void Read_EMR_POLYPOLYGON_BASE();
		void Read_EMR_POLYPOLYLINE();
		void Read_EMR_POLYPOLYLINE16();
		template<typename T>void Read_EMR_POLYPOLYLINE_BASE();
		void Read_EMR_POLYTEXTOUTA();
		void Read_EMR_POLYTEXTOUTW();
		void Read_EMR_RECTANGLE();
		void Read_EMR_ROUNDRECT();
		void Read_EMR_SETPIXELV();
		void Read_EMR_SMALLTEXTOUT();
		void Read_EMR_STROKEANDFILLPATH();
		void Read_EMR_STROKEPATH();
	private:

		TEmfHeader        m_oHeader;
		unsigned int      m_ulRecordSize;
		CEmfDC*           m_pDC;
		CEmfPlayer        m_oPlayer;
		CEmfPath*         m_pPath;
		TEmfXForm         m_oTransform;

		friend class CEmfPlayer;
		friend class CEmfClip;
	};
}

#endif // _METAFILE_EMF_EMFFILE_H
