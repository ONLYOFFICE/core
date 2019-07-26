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

#include "../../../common/StringExt.h"
#include "../../../fontengine/FontManager.h"

#include "EmfFile.h"

#ifdef _DEBUG
    #include <iostream>
#endif

#if defined(DrawText)
#undef DrawText
#endif

namespace MetaFile
{
static const struct ActionNamesEmf
{
    int     actionNumber;
    std::wstring actionName;
} actionNamesEmf[] =
{
{	0,							L"Unknown"},
{	EMR_HEADER,					L"EMR_HEADER"},
{	EMR_POLYBEZIER,				L"EMR_POLYBEZIER"},
{	EMR_POLYGON,				L"EMR_POLYGON"},
{	EMR_POLYLINE,				L"EMR_POLYLINE"},
{	EMR_POLYBEZIERTO,			L"EMR_POLYBEZIERTO"},
{	EMR_POLYLINETO,				L"EMR_POLYLINETO"},
{	EMR_POLYPOLYLINE,			L"EMR_POLYPOLYLINE"},
{	EMR_POLYPOLYGON,			L"EMR_POLYPOLYGON"},
{	EMR_SETWINDOWEXTEX,			L"EMR_SETWINDOWEXTEX"},
{	EMR_SETWINDOWORGEX,			L"EMR_SETWINDOWORGEX"},
{	EMR_SETVIEWPORTEXTEX,		L"EMR_SETVIEWPORTEXTEX"},
{	EMR_SETVIEWPORTORGEX,		L"EMR_SETVIEWPORTORGEX"},
{	EMR_SETBRUSHORGEX,			L"EMR_SETBRUSHORGEX"},
{	EMR_EOF,					L"EMR_EOF"},
{	EMR_SETPIXELV,				L"EMR_SETPIXELV"},
{	EMR_SETMAPPERFLAGS,			L"EMR_SETMAPPERFLAGS"},
{	EMR_SETMAPMODE,				L"EMR_SETMAPMODE"},
{	EMR_SETBKMODE,				L"EMR_SETBKMODE"},
{	EMR_SETPOLYFILLMODE,		L"EMR_SETPOLYFILLMODE"},
{	EMR_SETROP2,				L"EMR_SETROP2"},
{	EMR_SETSTRETCHBLTMODE,		L"EMR_SETSTRETCHBLTMODE"},
{	EMR_SETTEXTALIGN,			L"EMR_SETTEXTALIGN"},
{	EMR_SETCOLORADJUSTMENT,		L"EMR_SETCOLORADJUSTMENT"},
{	EMR_SETTEXTCOLOR,			L"EMR_SETTEXTCOLOR"},
{	EMR_SETBKCOLOR,				L"EMR_SETBKCOLOR"},
{	EMR_OFFSETCLIPRGN,			L"EMR_OFFSETCLIPRGN"},
{	EMR_MOVETOEX,				L"EMR_MOVETOEX"},
{	EMR_SETMETARGN,				L"EMR_SETMETARGN"},
{	EMR_EXCLUDECLIPRECT,		L"EMR_EXCLUDECLIPRECT"},
{	EMR_INTERSECTCLIPRECT,		L"EMR_INTERSECTCLIPRECT"},
{	EMR_SCALEVIEWPORTEXTEX,		L"EMR_SCALEVIEWPORTEXTEX"},
{	EMR_SCALEWINDOWEXTEX,		L"EMR_SCALEWINDOWEXTEX"},
{	EMR_SAVEDC,					L"EMR_SAVEDC"},
{	EMR_RESTOREDC,				L"EMR_RESTOREDC"},
{	EMR_SETWORLDTRANSFORM,		L"EMR_SETWORLDTRANSFORM"},
{	EMR_MODIFYWORLDTRANSFORM,	L"EMR_MODIFYWORLDTRANSFORM"},
{	EMR_SELECTOBJECT,			L"EMR_SELECTOBJECT"},
{	EMR_CREATEPEN,				L"EMR_CREATEPEN"},
{	EMR_CREATEBRUSHINDIRECT,	L"EMR_CREATEBRUSHINDIRECT"},
{	EMR_DELETEOBJECT,			L"EMR_DELETEOBJECT"},
{	EMR_ANGLEARC,				L"EMR_ANGLEARC"},
{	EMR_ELLIPSE,				L"EMR_ELLIPSE"},
{	EMR_RECTANGLE,				L"EMR_RECTANGLE"},
{	EMR_ROUNDRECT,				L"EMR_ROUNDRECT"},
{	EMR_ARC,					L"EMR_ARC"},
{	EMR_CHORD,					L"EMR_CHORD"},
{	EMR_PIE,					L"EMR_PIE"},
{	EMR_SELECTPALETTE,			L"EMR_SELECTPALETTE"},
{	EMR_CREATEPALETTE,			L"EMR_CREATEPALETTE"},
{	EMR_SETPALETTEENTRIES,		L"EMR_SETPALETTEENTRIES"},
{	EMR_RESIZEPALETTE,			L"EMR_RESIZEPALETTE"},
{	EMR_REALIZEPALETTE,			L"EMR_REALIZEPALETTE"},
{	EMR_EXTFLOODFILL,			L"EMR_EXTFLOODFILL"},
{	EMR_LINETO,					L"EMR_LINETO"},
{	EMR_ARCTO,					L"EMR_ARCTO"},
{	EMR_POLYDRAW,				L"EMR_POLYDRAW"},
{	EMR_SETARCDIRECTION,		L"EMR_SETARCDIRECTION"},
{	EMR_SETMITERLIMIT,			L"EMR_SETMITERLIMIT"},
{	EMR_BEGINPATH,				L"EMR_BEGINPATH"},
{	EMR_ENDPATH,				L"EMR_ENDPATH"},
{	EMR_CLOSEFIGURE,			L"EMR_CLOSEFIGURE"},
{	EMR_FILLPATH,				L"EMR_FILLPATH"},
{	EMR_STROKEANDFILLPATH,		L"EMR_STROKEANDFILLPATH"},
{	EMR_STROKEPATH,				L"EMR_STROKEPATH"},
{	EMR_FLATTENPATH,			L"EMR_FLATTENPATH"},
{	EMR_WIDENPATH,				L"EMR_WIDENPATH"},
{	EMR_SELECTCLIPPATH,			L"EMR_SELECTCLIPPATH"},
{	EMR_ABORTPATH,				L"EMR_ABORTPATH"},
{	69,							L"Unknown"},
{	EMR_GDICOMMENT,				L"EMR_GDICOMMENT"},
{	EMR_FILLRGN,				L"EMR_FILLRGN"},
{	EMR_FRAMERGN,				L"EMR_FRAMERGN"},
{	EMR_INVERTRGN,				L"EMR_INVERTRGN"},
{	EMR_PAINTRGN,				L"EMR_PAINTRGN"},
{	EMR_EXTSELECTCLIPRGN,		L"EMR_EXTSELECTCLIPRGN"},
{	EMR_BITBLT,					L"EMR_BITBLT"},
{	EMR_STRETCHBLT,				L"EMR_STRETCHBLT"},
{	EMR_MASKBLT,				L"EMR_MASKBLT"},
{	EMR_PLGBLT,					L"EMR_PLGBLT"},
{	EMR_SETDIBITSTODEVICE,		L"EMR_SETDIBITSTODEVICE"},
{	EMR_STRETCHDIBITS,			L"EMR_STRETCHDIBITS"},
{	EMR_EXTCREATEFONTINDIRECTW,	L"EMR_EXTCREATEFONTINDIRECTW"},
{	EMR_EXTTEXTOUTA,			L"EMR_EXTTEXTOUTA"},
{	EMR_EXTTEXTOUTW,			L"EMR_EXTTEXTOUTW"},
{	EMR_POLYBEZIER16,			L"EMR_POLYBEZIER16"},
{	EMR_POLYGON16,				L"EMR_POLYGON16"},
{	EMR_POLYLINE16,				L"EMR_POLYLINE16"},
{	EMR_POLYBEZIERTO16,			L"EMR_POLYBEZIERTO16"},
{	EMR_POLYLINETO16,			L"EMR_POLYLINETO16"},
{	EMR_POLYPOLYLINE16,			L"EMR_POLYPOLYLINE16"},
{	EMR_POLYPOLYGON16,			L"EMR_POLYPOLYGON16"},
{	EMR_POLYDRAW16,				L"EMR_POLYDRAW16"},
{	EMR_CREATEMONOBRUSH,		L"EMR_CREATEMONOBRUSH"},
{	EMR_CREATEDIBPATTERNBRUSHPT,L"EMR_CREATEDIBPATTERNBRUSHPT"},
{	EMR_EXTCREATEPEN,			L"EMR_EXTCREATEPEN"},
{	EMR_POLYTEXTOUTA,			L"EMR_POLYTEXTOUTA"},
{	EMR_POLYTEXTOUTW,			L"EMR_POLYTEXTOUTW"},
{	EMR_SETICMMODE,				L"EMR_SETICMMODE"},
{	EMR_CREATECOLORSPACE,		L"EMR_CREATECOLORSPACE"},
{	EMR_SETCOLORSPACE,			L"EMR_SETCOLORSPACE"},
{	EMR_DELETECOLORSPACE,		L"EMR_DELETECOLORSPACE"},
{	EMR_GLSRECORD,				L"EMR_GLSRECORD"},
{	EMR_GLSBOUNDEDRECORD,		L"EMR_GLSBOUNDEDRECORD"},
{	EMR_PIXELFORMAT,			L"EMR_PIXELFORMAT"},
{	EMR_RESERVED_105,			L"EMR_RESERVED_105"},
{	EMR_RESERVED_106,			L"EMR_RESERVED_106"},
{	EMR_RESERVED_107,			L"EMR_RESERVED_107"},
{	EMR_RESERVED_108,			L"EMR_RESERVED_108"},
{	EMR_RESERVED_109,			L"EMR_RESERVED_109"},
{	EMR_RESERVED_110,			L"EMR_RESERVED_110"},
{	EMR_COLORCORRECTPALETTE,	L"EMR_COLORCORRECTPALETTE"},
{	EMR_SETICMPROFILEA,			L"EMR_SETICMPROFILEA"},
{	EMR_SETICMPROFILEW,			L"EMR_SETICMPROFILEW"},
{	EMR_ALPHABLEND,				L"EMR_ALPHABLEND"},
{	EMR_SETLAYOUT,				L"EMR_SETLAYOUT"},
{	EMR_TRANSPARENTBLT,			L"EMR_TRANSPARENTBLT"},
{	EMR_RESERVED_117,			L"EMR_RESERVED_117"},
{	EMR_GRADIENTFILL,			L"EMR_GRADIENTFILL"},
{	EMR_RESERVED_119,			L"EMR_RESERVED_119"},
{	EMR_RESERVED_120,			L"EMR_RESERVED_120"},
{	EMR_COLORMATCHTOTARGETW,	L"EMR_COLORMATCHTOTARGETW"},
{	EMR_CREATECOLORSPACEW,		L"EMR_CREATECOLORSPACEW	"}
};
	void CEmfFile::PlayMetaFile()
	{
		if (!m_oStream.IsValid())
			SetError();

		unsigned int ulSize, ulType;
		unsigned int ulNumber = 0;

		bool bEof = false;

		unsigned int ulRecordIndex	= 0;
		unsigned int m_ulRecordPos	= 0;

		if (m_pOutput)
			m_pOutput->Begin();

		do
		{
            if (m_oStream.IsEof())
                break;
            if (m_oStream.CanRead() < 8)
				return SetError();

			m_oStream >> ulType;
			m_oStream >> ulSize;

            if (ulSize < 1)
				continue;

			m_ulRecordPos	= m_oStream.Tell();
			m_ulRecordSize	= ulSize - 8;

			if (ulType < EMR_MIN || ulType > EMR_MAX)
            {
                if (ENHMETA_SIGNATURE != m_oHeader.ulSignature || 0x00010000 != m_oHeader.ulVersion)
                    return SetError();
                else
                    break;
            }

			if (0 == ulRecordIndex && EMR_HEADER != ulType)
				return SetError();

			switch (ulType)
			{
				//-----------------------------------------------------------
				// 2.3.1 Bitmap
				//-----------------------------------------------------------
				case EMR_ALPHABLEND:        Read_EMR_ALPHABLEND(); break;
				case EMR_BITBLT:            Read_EMR_BITBLT(); break;
				case EMR_STRETCHDIBITS:     Read_EMR_STRETCHDIBITS(); break;
				case EMR_SETDIBITSTODEVICE: Read_EMR_SETDIBITSTODEVICE(); break;
				case EMR_STRETCHBLT:        Read_EMR_STRETCHBLT(); break;
					//-----------------------------------------------------------
					// 2.3.2 Clipping
					//-----------------------------------------------------------
				case EMR_EXCLUDECLIPRECT:   Read_EMR_EXCLUDECLIPRECT(); break;
				case EMR_EXTSELECTCLIPRGN:  Read_EMR_EXTSELECTCLIPRGN(); break;
				case EMR_INTERSECTCLIPRECT: Read_EMR_INTERSECTCLIPRECT(); break;
				case EMR_SELECTCLIPPATH:    Read_EMR_SELECTCLIPPATH(); break;
				case EMR_SETMETARGN:        Read_EMR_SETMETARGN(); break;
					//-----------------------------------------------------------
					// 2.3.4 Control
					//-----------------------------------------------------------
				case EMR_HEADER: Read_EMR_HEADER(); break;
				case EMR_EOF:    Read_EMR_EOF(); bEof = true; break;
					//-----------------------------------------------------------
					// 2.3.5 Drawing
					//-----------------------------------------------------------
				case EMR_ANGLEARC:          Read_EMR_ANGLEARC(); break;
				case EMR_ARC:				Read_EMR_ARC(); break;
				case EMR_ARCTO:             Read_EMR_ARCTO(); break;
				case EMR_CHORD:             Read_EMR_CHORD(); break;
				case EMR_ELLIPSE:           Read_EMR_ELLIPSE(); break;
				case EMR_EXTTEXTOUTA:       Read_EMR_EXTTEXTOUTA(); break;
				case EMR_EXTTEXTOUTW:       Read_EMR_EXTTEXTOUTW(); break;
				case EMR_FILLPATH:          Read_EMR_FILLPATH(); break;
				case EMR_LINETO:            Read_EMR_LINETO(); break;
				case EMR_PIE:				Read_EMR_PIE(); break;
				case EMR_POLYBEZIER:        Read_EMR_POLYBEZIER(); break;
				case EMR_POLYBEZIER16:      Read_EMR_POLYBEZIER16(); break;
				case EMR_POLYBEZIERTO:      Read_EMR_POLYBEZIERTO(); break;
				case EMR_POLYBEZIERTO16:    Read_EMR_POLYBEZIERTO16(); break;
				case EMR_POLYDRAW:          Read_EMR_POLYDRAW(); break;
				case EMR_POLYDRAW16:        Read_EMR_POLYDRAW16(); break;
				case EMR_POLYGON:           Read_EMR_POLYGON(); break;
				case EMR_POLYGON16:         Read_EMR_POLYGON16(); break;
				case EMR_POLYLINE:          Read_EMR_POLYLINE(); break;
				case EMR_POLYLINE16:        Read_EMR_POLYLINE16(); break;
				case EMR_POLYLINETO:        Read_EMR_POLYLINETO(); break;
				case EMR_POLYLINETO16:      Read_EMR_POLYLINETO16(); break;
				case EMR_POLYPOLYGON:       Read_EMR_POLYPOLYGON(); break;
				case EMR_POLYPOLYGON16:     Read_EMR_POLYPOLYGON16(); break;
				case EMR_POLYPOLYLINE:      Read_EMR_POLYPOLYLINE(); break;
				case EMR_POLYPOLYLINE16:    Read_EMR_POLYPOLYLINE16(); break;
				case EMR_POLYTEXTOUTA:      Read_EMR_POLYTEXTOUTA(); break;
				case EMR_POLYTEXTOUTW:      Read_EMR_POLYTEXTOUTW(); break;
				case EMR_RECTANGLE:         Read_EMR_RECTANGLE(); break;
				case EMR_ROUNDRECT:         Read_EMR_ROUNDRECT(); break;
				case EMR_SETPIXELV:         Read_EMR_SETPIXELV(); break;
				case EMR_SMALLTEXTOUT:      Read_EMR_SMALLTEXTOUT(); break;
				case EMR_STROKEANDFILLPATH: Read_EMR_STROKEANDFILLPATH(); break;
				case EMR_STROKEPATH:        Read_EMR_STROKEPATH(); break;
					//-----------------------------------------------------------
					// 2.3.7 Object Creation
					//-----------------------------------------------------------
				case EMR_CREATEBRUSHINDIRECT:     Read_EMR_CREATEBRUSHINDIRECT(); break;
				case EMR_CREATEDIBPATTERNBRUSHPT: Read_EMR_CREATEDIBPATTERNBRUSHPT(); break;
				case EMR_CREATEPALETTE:           Read_EMR_CREATEPALETTE(); break;
				case EMR_CREATEPEN:				Read_EMR_CREATEPEN(); break;
				case EMR_EXTCREATEFONTINDIRECTW:  Read_EMR_EXTCREATEFONTINDIRECTW(); break;
				case EMR_EXTCREATEPEN:            Read_EMR_EXTCREATEPEN(); break;
					//-----------------------------------------------------------
					// 2.3.8 Object Manipulation
					//-----------------------------------------------------------
				case EMR_SELECTOBJECT:  Read_EMR_SELECTOBJECT(); break;
				case EMR_DELETEOBJECT:  Read_EMR_DELETEOBJECT(); break;
				case EMR_SELECTPALETTE: Read_EMR_SELECTPALETTE(); break;
					//-----------------------------------------------------------
					// 2.3.10 Path Bracket
					//-----------------------------------------------------------
				case EMR_BEGINPATH:   Read_EMR_BEGINPATH(); break;
				case EMR_ENDPATH:     Read_EMR_ENDPATH(); break;
				case EMR_CLOSEFIGURE: Read_EMR_CLOSEFIGURE(); break;
				case EMR_FLATTENPATH: Read_EMR_FLATTENPATH(); break;
				case EMR_WIDENPATH:   Read_EMR_WIDENPATH(); break;
				case EMR_ABORTPATH:   Read_EMR_ABORTPATH(); break;
					//-----------------------------------------------------------
					// 2.3.11 State
					//-----------------------------------------------------------
				case EMR_MOVETOEX:          Read_EMR_MOVETOEX(); break;
				case EMR_SETARCDIRECTION:   Read_EMR_SETARCDIRECTION(); break;
				case EMR_SAVEDC:            Read_EMR_SAVEDC(); break;
				case EMR_RESTOREDC:         Read_EMR_RESTOREDC(); break;
				case EMR_SETTEXTCOLOR:      Read_EMR_SETTEXTCOLOR(); break;
				case EMR_SETTEXTALIGN:      Read_EMR_SETTEXTALIGN(); break;
				case EMR_SETBKMODE:         Read_EMR_SETBKMODE(); break;
				case EMR_SETMITERLIMIT:     Read_EMR_SETMITERLIMIT(); break;
				case EMR_SETPOLYFILLMODE:   Read_EMR_SETPOLYFILLMODE(); break;
				case EMR_SETMAPMODE:        Read_EMR_SETMAPMODE(); break;
				case EMR_SETWINDOWORGEX:    Read_EMR_SETWINDOWORGEX(); break;
				case EMR_SETWINDOWEXTEX:    Read_EMR_SETWINDOWEXTEX(); break;
				case EMR_SETVIEWPORTORGEX:  Read_EMR_SETVIEWPORTORGEX(); break;
				case EMR_SETVIEWPORTEXTEX:  Read_EMR_SETVIEWPORTEXTEX(); break;
				case EMR_SETBKCOLOR:        Read_EMR_SETBKCOLOR(); break;
				case EMR_SETSTRETCHBLTMODE: Read_EMR_SETSTRETCHBLTMODE(); break;
				case EMR_SETICMMODE:        Read_EMR_SETICMMODE(); break;
				case EMR_SETROP2:           Read_EMR_SETROP2(); break;
				case EMR_REALIZEPALETTE:    Read_EMR_REALIZEPALETTE(); break;
				case EMR_SETLAYOUT:         Read_EMR_SETLAYOUT(); break;
				case EMR_SETBRUSHORGEX:     Read_EMR_SETBRUSHORGEX(); break;
					//-----------------------------------------------------------
					// 2.3.12 Transform
					//-----------------------------------------------------------
				case EMR_SETWORLDTRANSFORM: Read_EMR_SETWORLDTRANSFORM(); break;
				case EMR_MODIFYWORLDTRANSFORM: Read_EMR_MODIFYWORLDTRANSFORM(); break;
					//-----------------------------------------------------------
					// Неподдерживаемые записи
					//-----------------------------------------------------------
				case EMR_GDICOMMENT: Read_EMR_UNKNOWN(); break;
					//-----------------------------------------------------------
					// Неизвестные записи
					//-----------------------------------------------------------
				default:
				{
					Read_EMR_UNKNOWN();
					break;
				}
			}

			if (bEof)
				break;

			int need_skip = m_ulRecordSize - (m_oStream.Tell() - m_ulRecordPos);
			m_oStream.Skip(need_skip);
			
#ifdef _DEBUG
			if ( need_skip != 0 && !m_pOutput)
			{
				std::wstring name = actionNamesEmf[ulType].actionName;

				std::wcout << name << L"\t\t(" << ulType << L")\t; skiped = " << need_skip << L"\n";
			}			
#endif   
			ulRecordIndex++;

		} while (!CheckError());

		if (!CheckError())
			m_oStream.SeekToStart();

		if (m_pOutput)
			m_pOutput->End();
	}
	void CEmfFile::TranslatePoint(TEmfPointL& oPoint, double& dX, double& dY)
	{
		TranslatePoint(oPoint.x, oPoint.y, dX, dY);
	}
	void CEmfFile::TranslatePoint(int nX, int nY, double& dX, double &dY)
	{
		dX = (double)nX;
		dY = (double)nY;
	}
	bool CEmfFile::ReadImage(unsigned int offBmi, unsigned int cbBmi, unsigned int offBits, unsigned int cbBits, unsigned int ulSkip, BYTE** ppBgraBuffer, unsigned int* pulWidth, unsigned int* pulHeight)
	{
		int lHeaderOffset         = offBmi - ulSkip;
		unsigned int ulHeaderSize = cbBmi;
		int lBitsOffset           = offBits - offBmi - cbBmi;
		unsigned int ulBitsSize   = cbBits;
		if (ulHeaderSize <= 0 || ulBitsSize <= 0 || lHeaderOffset < 0 || lBitsOffset < 0)
		{
			// TODO: Если попали сюда, значит надо смотреть BitBltRasterOperation
			if (lHeaderOffset > 0)
				m_oStream.Skip(lHeaderOffset);

			m_oStream.Skip(ulHeaderSize);

			if (lBitsOffset > 0)
				m_oStream.Skip(lBitsOffset);

			m_oStream.Skip(ulBitsSize);

			return false;
		}

		m_oStream.Skip(lHeaderOffset);

		BYTE* pHeaderBuffer = m_oStream.GetCurPtr();
		m_oStream.Skip(ulHeaderSize + lBitsOffset);
		BYTE* pBitsBuffer = m_oStream.GetCurPtr();

		unsigned int ulBitsSizeSkip = 0 == ulBitsSize ? 0 : ((int)(((double)ulBitsSize - 0.5) / 4) + 1) * 4;
		m_oStream.Skip(ulBitsSizeSkip);

		MetaFile::ReadImage(pHeaderBuffer, ulHeaderSize, pBitsBuffer, ulBitsSize, ppBgraBuffer, pulWidth, pulHeight);

		return true;
	}
	void CEmfFile::DrawImage(int nX, int nY, int nW, int nH, BYTE* pImageBuffer, unsigned int unImageW, unsigned int unImageH)
	{
		if (m_pOutput)
		{
			double dX, dY, dR, dB;
			TranslatePoint(nX, nY, dX, dY);
			TranslatePoint(nX + nW, nY + nH, dR, dB);
			m_pOutput->DrawBitmap(dX, dY, dR - dX, dB - dY, pImageBuffer, unImageW, unImageH);
		}
	}
	void CEmfFile::MoveTo(int nX, int nY)
	{
		double dX, dY;
		TranslatePoint(nX, nY, dX, dY);

		if (m_pPath)
		{
			if (!m_pPath->MoveTo(dX, dY))
				return SetError();
		}
		else if (m_pOutput)
		{
			m_pOutput->MoveTo(dX, dY);
		}

		m_pDC->SetCurPos(nX, nY);
	}
	void CEmfFile::LineTo(int nX, int nY)
	{
		double dX, dY;
		TranslatePoint(nX, nY, dX, dY);

		if (m_pPath)
		{
			if (!m_pPath->LineTo(dX, dY))
				return SetError();
		}
		else if (m_pOutput)
		{
			m_pOutput->LineTo(dX, dY);
		}

		m_pDC->SetCurPos(nX, nY);
	}
	void CEmfFile::CurveTo(int nX1, int nY1, int nX2, int nY2, int nXe, int nYe)
	{
		double dX1, dY1, dX2, dY2, dXe, dYe;
		TranslatePoint(nX1, nY1, dX1, dY1);
		TranslatePoint(nX2, nY2, dX2, dY2);
		TranslatePoint(nXe, nYe, dXe, dYe);

		if (m_pPath)
		{
			if (!m_pPath->CurveTo(dX1, dY1, dX2, dY2, dXe, dYe))
				return SetError();
		}
		else if (m_pOutput)
		{
			m_pOutput->CurveTo(dX1, dY1, dX2, dY2, dXe, dYe);
		}

		m_pDC->SetCurPos(nXe, nYe);
	}
	void CEmfFile::ClosePath()
	{
		if (m_pPath)
		{
			if (!m_pPath->Close())
				return SetError();
		}
		else if (m_pOutput)
			m_pOutput->ClosePath();
	}
	void CEmfFile::ArcTo(int nL, int nT, int nR, int nB, double dStart, double dSweep)
	{
		double dL, dT, dR, dB;
		TranslatePoint(nL, nT, dL, dT);
		TranslatePoint(nR, nB, dR, dB);

		if (m_pPath)
		{
			if (!m_pPath->ArcTo(dL, dT, dR, dB, dStart, dSweep))
				return SetError();
		}
		else if (m_pOutput)
		{
			m_pOutput->ArcTo(dL, dT, dR, dB, dStart, dSweep);
		}

		// Пересчет текущей позиции делается в каждой функции отдельно после вызова данной
	}
	void CEmfFile::DrawPath(bool bStroke, bool bFill)
	{
		if (m_pPath && m_pOutput)
		{				
		}
		else if (m_pOutput)
		{
			int lType = (bStroke ? 1 : 0) + (bFill ? 2 : 0);
			m_pOutput->DrawPath(lType);
			m_pOutput->EndPath();
		}
	}
	void CEmfFile::DrawText(std::wstring& wsString, unsigned int unCharsCount, int _nX, int _nY, int* pnDx, int iGraphicsMode)
	{
		int nX = _nX;
		int nY = _nY;

		if (m_pDC->GetTextAlign() & TA_UPDATECP)
		{
			nX = m_pDC->GetCurPos().x;
			nY = m_pDC->GetCurPos().y;
		}

		if (m_pOutput)
		{
			double dX, dY;
			TranslatePoint(nX, nY, dX, dY);

			double* pdDx = NULL;
			if (pnDx)
			{
				pdDx = new double[unCharsCount];
				if (pdDx)
				{
					int nCurX = nX;
					double dCurX = dX;

					for (unsigned int unCharIndex = 0; unCharIndex < unCharsCount; unCharIndex++)
					{
						int nX1 = nCurX + pnDx[unCharIndex];
						double dX1, dY1;
						TranslatePoint(nX1, nY, dX1, dY1);
						pdDx[unCharIndex] = dX1 - dCurX;
						nCurX = nX1;
						dCurX = dX1;
					}
				}
			}

			m_pOutput->DrawString(wsString, unCharsCount, dX, dY, pdDx, iGraphicsMode);

			if (pdDx)
				delete[] pdDx;
		}
	}
	void CEmfFile::DrawTextA(TEmfEmrText& oText, int iGraphicsMode)
	{
		if (!oText.OutputString)
			return SetError();

		IFont* pFont = GetFont();
		NSStringExt::CConverter::ESingleByteEncoding eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_DEFAULT;
		if (pFont)
		{
			// Соответствие Charset -> Codepage: http://support.microsoft.com/kb/165478
			// http://msdn.microsoft.com/en-us/library/cc194829.aspx
			//  Charset Name       Charset Value(hex)  Codepage number
			//  ------------------------------------------------------
			//
			//  DEFAULT_CHARSET           1 (x01)
			//  SYMBOL_CHARSET            2 (x02)
			//  OEM_CHARSET             255 (xFF)
			//  ANSI_CHARSET              0 (x00)            1252
			//  RUSSIAN_CHARSET         204 (xCC)            1251
			//  EASTEUROPE_CHARSET      238 (xEE)            1250
			//  GREEK_CHARSET           161 (xA1)            1253
			//  TURKISH_CHARSET         162 (xA2)            1254
			//  BALTIC_CHARSET          186 (xBA)            1257
			//  HEBREW_CHARSET          177 (xB1)            1255
			//  ARABIC _CHARSET         178 (xB2)            1256
			//  SHIFTJIS_CHARSET        128 (x80)             932
			//  HANGEUL_CHARSET         129 (x81)             949
			//  GB2313_CHARSET          134 (x86)             936
			//  CHINESEBIG5_CHARSET     136 (x88)             950
			//  THAI_CHARSET            222 (xDE)             874	
			//  JOHAB_CHARSET	        130 (x82)            1361
			//  VIETNAMESE_CHARSET      163 (xA3)            1258

			switch (pFont->GetCharSet())
			{
				default:
				case DEFAULT_CHARSET:       eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_DEFAULT; break;
				case SYMBOL_CHARSET:        eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_DEFAULT; break;
				case ANSI_CHARSET:          eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1252; break;
				case RUSSIAN_CHARSET:       eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1251; break;
				case EASTEUROPE_CHARSET:    eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1250; break;
				case GREEK_CHARSET:         eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1253; break;
				case TURKISH_CHARSET:       eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1254; break;
				case BALTIC_CHARSET:        eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1257; break;
				case HEBREW_CHARSET:        eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1255; break;
				case ARABIC_CHARSET:        eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1256; break;
				case SHIFTJIS_CHARSET:      eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP932; break;
				case HANGEUL_CHARSET:       eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP949; break;
				case 134/*GB2313_CHARSET*/: eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP936; break;
				case CHINESEBIG5_CHARSET:   eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP950; break;
				case THAI_CHARSET:          eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP874; break;
				case JOHAB_CHARSET:         eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1361; break;
				case VIETNAMESE_CHARSET:    eCharSet = NSStringExt::CConverter::ESingleByteEncoding::SINGLE_BYTE_ENCODING_CP1258; break;
			}
		}

		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromSingleByteString((unsigned char*)oText.OutputString, oText.Chars, eCharSet);

		int* pDx = NULL;
		if (oText.OutputDx)
		{
			pDx = new int[oText.Chars];
			if (pDx)
			{
				for (unsigned int unIndex = 0; unIndex < oText.Chars; unIndex++)
				{
					pDx[unIndex] = oText.OutputDx[unIndex];

					// Пропускаем сдвиги по Y если они есть
					if (oText.Options & ETO_PDY)
						unIndex++;
				}
			}
		}

		DrawText(wsText, oText.Chars, oText.Reference.x, oText.Reference.y, pDx, iGraphicsMode);

		if (pDx)
			delete[] pDx;
	}
	void CEmfFile::DrawTextW(TEmfEmrText& oText, int iGraphicsMode)
	{
		if (!oText.OutputString)
			return SetError();

		std::wstring wsText = NSStringExt::CConverter::GetUnicodeFromUTF16((unsigned short*)oText.OutputString, oText.Chars);

		unsigned int unLen = 0;
		int* pDx = NULL;
		if (oText.OutputDx && oText.Chars)
		{
			// Здесь мы эмулируем конвертацию Utf16 в Utf32, чтобы правильно получить массив pDx
			pDx = new int[oText.Chars];
			unLen = 0;

			unsigned short* pUtf16 = (unsigned short*)oText.OutputString;
			wchar_t wLeading, wTrailing;
			unsigned int unCode;
			unsigned int unPos = 0;
			while (unPos < oText.Chars)
			{
				wLeading = pUtf16[unPos++];
				if (wLeading < 0xD800 || wLeading > 0xDFFF)
				{
					pDx[unLen++] = oText.OutputDx[unPos - 1];
				}
				else if (wLeading >= 0xDC00)
				{
					// Такого не должно быть
					continue;
				}
				else
				{
					unCode = (wLeading & 0x3FF) << 10;
					wTrailing = pUtf16[unPos++];
					if (wTrailing < 0xDC00 || wTrailing > 0xDFFF)
					{
						// Такого не должно быть
						continue;
					}
					else
					{
						pDx[unLen++] = oText.OutputDx[unPos - 2] + oText.OutputDx[unPos - 1];
					}

					// Пропускаем сдвиги по Y если они есть
					if (oText.Options & ETO_PDY)
						unPos++;
				}

				// Пропускаем сдвиги по Y если они есть
				if (oText.Options & ETO_PDY)
					unPos++;
			}
		}
		else
		{
			unLen = 0;
			unsigned int* pUnicodes = NSStringExt::CConverter::GetUtf32FromUnicode(wsText, unLen);
			if (pUnicodes)
				delete[] pUnicodes;
		}

		if (unLen)
			DrawText(wsText, unLen, oText.Reference.x, oText.Reference.y, pDx, iGraphicsMode);

		if (pDx)
			delete[] pDx;
	}

	void CEmfFile::Read_EMR_HEADER()
	{
		m_oStream >> m_oHeader.oBounds;
		m_oStream >> m_oHeader.oFrame;
		m_oStream >> m_oHeader.ulSignature;
		m_oStream >> m_oHeader.ulVersion;
		m_oStream >> m_oHeader.ulSize;
		m_oStream >> m_oHeader.ulRecords;
		m_oStream >> m_oHeader.ushObjects;
		m_oStream >> m_oHeader.ushReserved;
		m_oStream >> m_oHeader.ulSizeDescription;
		m_oStream >> m_oHeader.ulOffsetDescription;
		m_oStream >> m_oHeader.ulPalEntries;
		m_oStream >> m_oHeader.oDevice;
		m_oStream >> m_oHeader.oMillimeters;

		if (ENHMETA_SIGNATURE != m_oHeader.ulSignature || 0x00010000 != m_oHeader.ulVersion)
			return SetError();

		// Пропускаем остальную часть заголовка, т.к. она нас пока не интересует
		unsigned int ulRemaining = m_ulRecordSize - 80; // sizeof(TEmfHeader)
		m_oStream.Skip(ulRemaining);

		double dL = m_oHeader.oFrame.lLeft / 100.0 / m_oHeader.oMillimeters.cx * m_oHeader.oDevice.cx;
		double dR = m_oHeader.oFrame.lRight / 100.0 / m_oHeader.oMillimeters.cx * m_oHeader.oDevice.cx;
		double dT = m_oHeader.oFrame.lTop / 100.0 / m_oHeader.oMillimeters.cy * m_oHeader.oDevice.cy;
		double dB = m_oHeader.oFrame.lBottom / 100.0 / m_oHeader.oMillimeters.cy * m_oHeader.oDevice.cy;

		double dW = dR - dL;
		double dH = dB - dT;

		int nL = (int)floor(dL + 0.5);
		int nT = (int)floor(dT + 0.5);
		int nR = (int)floor(dW + 0.5) + nL;
		int nB = (int)floor(dH + 0.5) + nT;

		// По логике мы должны получать рект, точно такой же как и oBounds, но есть файлы, где это не так.
		m_oHeader.oFrameToBounds.nLeft   = nL;
		m_oHeader.oFrameToBounds.nRight  = nR;
		m_oHeader.oFrameToBounds.nTop    = nT;
		m_oHeader.oFrameToBounds.nBottom = nB;

		m_oHeader.oFramePx = m_oHeader.oFrameToBounds;
	}
	void CEmfFile::Read_EMR_ALPHABLEND()
	{
		TEmfAlphaBlend oBitmap;
		m_oStream >> oBitmap;

		BYTE* pBgraBuffer = NULL;
		unsigned int unWidth, unHeight;

		if (ReadImage(oBitmap.offBmiSrc, oBitmap.cbBmiSrc, oBitmap.offBitsSrc, oBitmap.cbBitsSrc, c_nTEmfAlphaBlendSize + 8, &pBgraBuffer, &unWidth, &unHeight))
		{
			if (m_pOutput)
			{
				if (0x00 == oBitmap.AlphaFormat)
				{
					for (unsigned int unY = 0; unY < unHeight; unY++)
					{
						for (unsigned int unX = 0; unX < unWidth; unX++)
						{
							unsigned int unIndex = (unX + unY * unWidth) * 4;
							pBgraBuffer[unIndex + 3] = oBitmap.SrcConstantAlpha;
						}
					}
				}
				else
				{
					double dAlphaKoef = oBitmap.SrcConstantAlpha / 255.0;
					for (unsigned int unY = 0; unY < unHeight; unY++)
					{
						for (unsigned int unX = 0; unX < unWidth; unX++)
						{
							unsigned int unIndex = (unX + unY * unWidth) * 4;
							pBgraBuffer[unIndex + 3] = (unsigned char)(dAlphaKoef * pBgraBuffer[unIndex + 3]);
						}
					}

				}

				DrawImage(oBitmap.xDest, oBitmap.yDest, oBitmap.cxDest, oBitmap.cyDest, pBgraBuffer, unWidth, unHeight);
			}
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}
	void CEmfFile::Read_EMR_STRETCHDIBITS()
	{
		TEmfStretchDIBITS oBitmap;
		m_oStream >> oBitmap;

		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;

		if (ReadImage(oBitmap.offBmiSrc, oBitmap.cbBmiSrc, oBitmap.offBitsSrc, oBitmap.cbBitsSrc, sizeof(TEmfStretchDIBITS) + 8, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			if (m_pOutput)
			{
				ProcessRasterOperation(oBitmap.BitBltRasterOperation, &pBgraBuffer, ulWidth, ulHeight);
				DrawImage(oBitmap.xDest, oBitmap.yDest, oBitmap.cxDest, oBitmap.cyDest, pBgraBuffer, ulWidth, ulHeight);
			}
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}
	void CEmfFile::Read_EMR_BITBLT()
	{
		TEmfBitBlt oBitmap;
		m_oStream >> oBitmap;

		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;

		// sizeof использовать нельзя, т.к. мы используем double в структуре TEmfBitBlt, а читаем его 4-байтовым
		unsigned int unEmfBitBltRecordSize = 100; // sizeof(TEmfBitBlt) + 8

		if (ReadImage(oBitmap.offBmiSrc, oBitmap.cbBmiSrc, oBitmap.offBitsSrc, oBitmap.cbBitsSrc, unEmfBitBltRecordSize, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			DrawImage(oBitmap.xDest, oBitmap.yDest, oBitmap.cxDest, oBitmap.cyDest, pBgraBuffer, ulWidth, ulHeight);
		}

		if (m_pOutput)
		{
			if (0x00000042 == oBitmap.BitBltRasterOperation) // BLACKNESS
			{
				// Делаем все черным цветом
				pBgraBuffer = new BYTE[4];
				pBgraBuffer[0] = 0x00;
				pBgraBuffer[1] = 0x00;
				pBgraBuffer[2] = 0x00;
				pBgraBuffer[3] = 0xff;

				ulWidth  = 1;
				ulHeight = 1;
			}
			if (0x00FF0062 == oBitmap.BitBltRasterOperation) // WHITENESS
			{
				// Делаем все черным цветом
				pBgraBuffer = new BYTE[4];
				pBgraBuffer[0] = 0xff;
				pBgraBuffer[1] = 0xff;
				pBgraBuffer[2] = 0xff;
				pBgraBuffer[3] = 0xff;

				ulWidth  = 1;
				ulHeight = 1;
			}
			else if (0x00f00021 == oBitmap.BitBltRasterOperation) // PATCOPY
			{
				CEmfLogBrushEx* pBrush = m_pDC->GetBrush();
				if (pBrush)
				{					
					// Делаем цветом кисти
					pBgraBuffer = new BYTE[4];
					pBgraBuffer[0] = pBrush->Color.b;
					pBgraBuffer[1] = pBrush->Color.g;
					pBgraBuffer[2] = pBrush->Color.r;
					pBgraBuffer[3] = 0xff;

					ulWidth  = 1;
					ulHeight = 1;
				}
			}
			else if (0x005a0049 == oBitmap.BitBltRasterOperation) // PATINVERT
			{
				CEmfLogBrushEx* pBrush = m_pDC->GetBrush();
				if (pBrush)
				{
					// Делаем цветом кисти
					pBgraBuffer = new BYTE[4];
				     pBgraBuffer[0] = pBrush->Color.b;
					pBgraBuffer[1] = pBrush->Color.g;
				     pBgraBuffer[2] = pBrush->Color.r;
					pBgraBuffer[3] = 30;

					ulWidth  = 1;
					ulHeight = 1;
				}
			}
			else if (0x00A000C9 == oBitmap.BitBltRasterOperation) // PATINVERT
			{
				CEmfLogBrushEx* pBrush = m_pDC->GetBrush();
				if (pBrush)
				{
					// Делаем цветом кисти
					pBgraBuffer = new BYTE[4];
					pBgraBuffer[0] = pBrush->Color.b;
					pBgraBuffer[1] = pBrush->Color.g;
					pBgraBuffer[2] = pBrush->Color.r;
					pBgraBuffer[3] = 30;

					ulWidth  = 1;
					ulHeight = 1;
				}
			}

			if (pBgraBuffer)
				DrawImage(oBitmap.xDest, oBitmap.yDest, oBitmap.cxDest, oBitmap.cyDest, pBgraBuffer, ulWidth, ulHeight);				
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}
	void CEmfFile::Read_EMR_SETDIBITSTODEVICE()
	{
		TEmfSetDiBitsToDevice oBitmap;
		m_oStream >> oBitmap;

		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;
		if (ReadImage(oBitmap.offBmiSrc, oBitmap.cbBmiSrc, oBitmap.offBitsSrc, oBitmap.cbBitsSrc, sizeof(TEmfSetDiBitsToDevice) + 8, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			// TODO: Нужно реализовать обрезку картинки по параметрам oBitmap.iStartScan и oBitmap.cScans
			DrawImage(oBitmap.Bounds.lLeft, oBitmap.Bounds.lTop, oBitmap.Bounds.lRight - oBitmap.Bounds.lLeft, oBitmap.Bounds.lBottom - oBitmap.Bounds.lTop, pBgraBuffer, ulWidth, ulHeight);
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}
	void CEmfFile::Read_EMR_STRETCHBLT()
	{
		TEmfStretchBLT oBitmap;
		m_oStream >> oBitmap;

		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;

		unsigned int unSkip = 108; // sizeof(TEmfStretchBLT) + 8 неправильно считает, из-з TXForm, там double, а в Emf они по 4 байта
		if (ReadImage(oBitmap.offBmiSrc, oBitmap.cbBmiSrc, oBitmap.offBitsSrc, oBitmap.cbBitsSrc, unSkip, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			if (m_pOutput)
			{
				ProcessRasterOperation(oBitmap.BitBltRasterOperation, &pBgraBuffer, ulWidth, ulHeight);
				DrawImage(oBitmap.xDest, oBitmap.yDest, oBitmap.cxDest, oBitmap.cyDest, pBgraBuffer, ulWidth, ulHeight);
			}
		}

		if (pBgraBuffer)
			delete[] pBgraBuffer;
	}
	void CEmfFile::Read_EMR_EOF()
	{
		unsigned int ulCount, ulOffset, ulSizeLast;

		m_oStream >> ulCount;
		m_oStream >> ulOffset;

		m_oStream.Skip(m_ulRecordSize - 8 - 4);

		m_oStream >> ulSizeLast;
	}
	void CEmfFile::Read_EMR_UNKNOWN()
	{
		// Неизвестные и нереализованные записи мы пропускаем
		m_oStream.Skip(m_ulRecordSize);
	}
	void CEmfFile::Read_EMR_SAVEDC()
	{
		m_pDC = m_oPlayer.SaveDC();
	}
	void CEmfFile::Read_EMR_RESTOREDC()
	{
		int lSavedDC;
		m_oStream >> lSavedDC;

		if (lSavedDC >= 0)
		{
			SetError();
			return;
		}

		int lCount = -lSavedDC;
		for (int lIndex = 0; lIndex < lCount; lIndex++)
			m_oPlayer.RestoreDC();

		m_pDC = m_oPlayer.GetDC();
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_MODIFYWORLDTRANSFORM()
	{
		TEmfXForm oXForm;
		unsigned int ulMode;

		m_oStream >> oXForm;
		m_oStream >> ulMode;

		m_pDC->MultiplyTransform(oXForm, ulMode);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_SETWORLDTRANSFORM()
	{
		TEmfXForm oXForm;

		m_oStream >> oXForm;

		m_pDC->MultiplyTransform(oXForm, MWT_SET);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_CREATEBRUSHINDIRECT()
	{
		unsigned int ulBrushIndex;
		CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();
		if (!pBrush)
			return SetError();

		m_oStream >> ulBrushIndex;
		m_oStream >> *pBrush;

		m_oPlayer.RegisterObject(ulBrushIndex, (CEmfObjectBase*)pBrush);
	}
	void CEmfFile::Read_EMR_SETTEXTCOLOR()
	{
		TEmfColor oColor;
		m_oStream >> oColor;

		m_pDC->SetTextColor(oColor);
		UpdateOutputDC();
	}		
	void CEmfFile::Read_EMR_SELECTOBJECT()
	{
		unsigned int ulObjectIndex;
		m_oStream >> ulObjectIndex;

		m_oPlayer.SelectObject(ulObjectIndex);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_EXTCREATEFONTINDIRECTW()
	{
		unsigned int unSize = m_ulRecordSize - 4;
		bool bFixedLength = unSize <= 0x0140 ? true : false;

		unsigned int ulIndex;						
		CEmfLogFont* pFont = new CEmfLogFont(bFixedLength);
		if (!pFont)
			return SetError();

		m_oStream >> ulIndex;
		m_oStream >> *pFont;

		m_oPlayer.RegisterObject(ulIndex, (CEmfObjectBase*)pFont);
	}
	void CEmfFile::Read_EMR_SETTEXTALIGN()
	{
		unsigned int ulAlign;
		m_oStream >> ulAlign;

		m_pDC->SetTextAlign(ulAlign);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_SETBKMODE()
	{
		unsigned int ulBgMode;
		m_oStream >> ulBgMode;
		m_pDC->SetBgMode(ulBgMode);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_DELETEOBJECT()
	{
		unsigned int ulIndex;
		m_oStream >> ulIndex;
		m_oPlayer.DeleteObject(ulIndex);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_SETMITERLIMIT()
	{
		unsigned int ulMiterLimit;
		m_oStream >> ulMiterLimit;
		m_pDC->SetMiterLimit(ulMiterLimit);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_EXTCREATEPEN()
	{
		unsigned int ulPenIndex;
		m_oStream >> ulPenIndex;

		m_oStream.Skip(4); // offBmi
		m_oStream.Skip(4); // cbBmi
		m_oStream.Skip(4); // offBits
		m_oStream.Skip(4); // cbBits

		unsigned int current_size = m_ulRecordSize - 20;

		CEmfLogPen* pPen = new CEmfLogPen();
		if (!pPen)
			return SetError();

		// LogPenEx
		m_oStream >> pPen->PenStyle;
		m_oStream >> pPen->Width;
		m_oStream.Skip(4); // BrushStyle
		m_oStream >> pPen->Color;
		m_oStream.Skip(4); // BrushHatch
		m_oStream >> pPen->NumStyleEntries;

		current_size -= 24;
		if (pPen->NumStyleEntries > 0)
		{
			current_size -= pPen->NumStyleEntries * 4;
			pPen->StyleEntry = new unsigned int[pPen->NumStyleEntries];
			if (!pPen->StyleEntry)
			{
				delete pPen;
				return SetError();
			}

			for (unsigned int ulIndex = 0; ulIndex < pPen->NumStyleEntries; ulIndex++)
			{
				m_oStream >> pPen->StyleEntry[ulIndex];
			}
		}
		else
		{
			pPen->StyleEntry = NULL;
		}

		// Пропускаем часть с картинкой, если она была
		m_oStream.Skip(current_size);

		m_oPlayer.RegisterObject(ulPenIndex, (CEmfObjectBase*)pPen);
	}
	void CEmfFile::Read_EMR_CREATEPEN()
	{
		unsigned int ulPenIndex;
		m_oStream >> ulPenIndex;
		CEmfLogPen* pPen = new CEmfLogPen();
		if (!pPen)
			return SetError();

		m_oStream >> pPen->PenStyle;

		unsigned int widthX, widthY;
		m_oStream >> widthX >> widthY;

		if (!widthX)	
		{//emf from Bonetti Martínez. cálculo estructural de pilotes y pilas.xlsx
			widthX = 1 / m_pDC->GetPixelWidth();
		}
		pPen->Width = widthX;

		m_oStream >> pPen->Color;
		m_oPlayer.RegisterObject(ulPenIndex, (CEmfObjectBase*)pPen);
	}
	void CEmfFile::Read_EMR_SETPOLYFILLMODE()
	{
		unsigned int ulFillMode;
		m_oStream >> ulFillMode;
		m_pDC->SetFillMode(ulFillMode);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_BEGINPATH()
	{
		if (m_pPath)
			delete m_pPath;

		m_pPath = new CEmfPath();
		if (!m_pPath)
			SetError();

		// Иногда MoveTo идет до BeginPath
		TEmfPointL oPoint = m_pDC->GetCurPos();
		double dX, dY;
		TranslatePoint(oPoint, dX, dY);
		m_pPath->MoveTo(dX, dY);
	}
	void CEmfFile::Read_EMR_ENDPATH()
	{
		// Ничего не делаем
	}
	void CEmfFile::Read_EMR_CLOSEFIGURE()
	{
		if (m_pPath)
		{
			if (!m_pPath->Close())
				return SetError();
		}
	}
	void CEmfFile::Read_EMR_FLATTENPATH()
	{
		// Ничего не делаем
	}
	void CEmfFile::Read_EMR_WIDENPATH()
	{
		// TODO: реализовать
	}
	void CEmfFile::Read_EMR_ABORTPATH()
	{
		if (m_pPath)
		{
			delete m_pPath;
			m_pPath = NULL;
		}
	}
	void CEmfFile::Read_EMR_MOVETOEX()
	{
		TEmfPointL oPoint;
		m_oStream >> oPoint;
		MoveTo(oPoint);
	}	
	void CEmfFile::Read_EMR_SETARCDIRECTION()
	{
		unsigned int unDirection;
		m_oStream >> unDirection;
		m_pDC->SetArcDirection(unDirection);
		// Здесь не обновляем DC у Output, т.к. этот параметр разруливается внутри данного класса.
	}
	void CEmfFile::Read_EMR_FILLPATH()
	{
		TEmfRectL oBounds;
		m_oStream >> oBounds;

		if (m_pPath)
		{
			m_pPath->Draw(m_pOutput, false, true);
			RELEASEOBJECT(m_pPath);
		}
	}
	void CEmfFile::Read_EMR_SETMAPMODE()
	{
		unsigned int ulMapMode;
		m_oStream >> ulMapMode;

		m_pDC->SetMapMode(ulMapMode);
	}
	void CEmfFile::Read_EMR_SETWINDOWORGEX()
	{
		TEmfPointL oOrigin;
		m_oStream >> oOrigin;
		m_pDC->SetWindowOrigin(oOrigin);
	}
	void CEmfFile::Read_EMR_SETWINDOWEXTEX()
	{
		TEmfSizeL oExtent;
		m_oStream >> oExtent;
		m_pDC->SetWindowExtents(oExtent);
	}
	void CEmfFile::Read_EMR_SETVIEWPORTORGEX()
	{
		TEmfPointL oOrigin;
		m_oStream >> oOrigin;
		m_pDC->SetViewportOrigin(oOrigin);
	}
	void CEmfFile::Read_EMR_SETVIEWPORTEXTEX()
	{
		TEmfSizeL oExtent;
		m_oStream >> oExtent;
		m_pDC->SetViewportExtents(oExtent);
	}
	void CEmfFile::Read_EMR_SETSTRETCHBLTMODE()
	{
		unsigned int ulStretchMode;
		m_oStream >> ulStretchMode;
		m_pDC->SetStretchMode(ulStretchMode);
	}
	void CEmfFile::Read_EMR_SETICMMODE()
	{
		unsigned int ulICMMode;
		m_oStream >> ulICMMode;
	}
	void CEmfFile::Read_EMR_CREATEDIBPATTERNBRUSHPT()
	{
		unsigned int ulBrushIndex;
		TEmfDibPatternBrush oDibBrush;
		m_oStream >> ulBrushIndex;
		m_oStream >> oDibBrush;

		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;

		if (ReadImage(oDibBrush.offBmi, oDibBrush.cbBmi, oDibBrush.offBits, oDibBrush.cbBits, sizeof(TEmfDibPatternBrush) + 12, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();
			if (!pBrush)
				return SetError();

			pBrush->SetDibPattern(pBgraBuffer, ulWidth, ulHeight);
			m_oPlayer.RegisterObject(ulBrushIndex, (CEmfObjectBase*)pBrush);
		}
	}
	void CEmfFile::Read_EMR_SELECTCLIPPATH()
	{
		unsigned int unRegionMode;
		m_oStream >> unRegionMode;

		if (m_pPath)
		{
			m_pDC->ClipToPath(m_pPath, unRegionMode, GetDC()->GetFinalTransform(GM_ADVANCED));
			RELEASEOBJECT(m_pPath);

			UpdateOutputDC();
		}
	}
	void CEmfFile::Read_EMR_SETBKCOLOR()
	{
		TEmfColor oColor;
		m_oStream >> oColor;
		m_pDC->SetBgColor(oColor);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_EXCLUDECLIPRECT()
	{
		// TODO: Проверить как найдется файл
		TEmfRectL oClip;
		m_oStream >> oClip;

		TRectD oClipRect, oBB;

		// Поскольку мы реализовываем данный тип клипа с помощью разницы внешнего ректа и заданного, и
		// пересечением с полученной областью, то нам надо вычесть границу заданного ректа.
		if (oClip.lLeft < oClip.lRight)
		{
			oClip.lLeft--;
			oClip.lRight++;
		}
		else
		{
			oClip.lLeft++;
			oClip.lRight--;
		}

		if (oClip.lTop < oClip.lBottom)
		{
			oClip.lTop--;
			oClip.lBottom++;
		}
		else
		{
			oClip.lTop++;
			oClip.lBottom--;
		}

		TranslatePoint(oClip.lLeft, oClip.lTop, oClipRect.dLeft, oClipRect.dTop);
		TranslatePoint(oClip.lRight, oClip.lBottom, oClipRect.dRight, oClipRect.dBottom);

		TRect* pRect = GetDCBounds();
		TranslatePoint(pRect->nLeft, pRect->nTop, oBB.dLeft, oBB.dTop);
		TranslatePoint(pRect->nRight, pRect->nBottom, oBB.dRight, oBB.dBottom);

		m_pDC->GetClip()->Exclude(oClipRect, oBB);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_EXTSELECTCLIPRGN()
	{
		unsigned int ulRgnDataSize, ulRegionMode;
		m_oStream >> ulRgnDataSize >> ulRegionMode;

		m_oStream.Skip(m_ulRecordSize - 8);

		// Тут просто сбрасываем текущий клип. Ничего не добавляем в клип, т.е. реализовать регионы с
	    // текущим интерфейсом рендерера невозможно.
		m_pDC->GetClip()->Reset();
	}
	void CEmfFile::Read_EMR_SETMETARGN()
	{
		m_pDC->GetClip()->Reset();
		UpdateOutputDC();
	}		
	void CEmfFile::Read_EMR_SETROP2()
	{
		unsigned int ulRop2Mode;
		m_oStream >> ulRop2Mode;
		m_pDC->SetRop2Mode(ulRop2Mode);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_CREATEPALETTE()
	{
		unsigned int ulPaletteIndex;
		CEmfLogPalette* pPalette = new CEmfLogPalette();
		if (!pPalette)
			return SetError();

		m_oStream >> ulPaletteIndex;
		m_oStream >> *pPalette;
		m_oPlayer.RegisterObject(ulPaletteIndex, (CEmfObjectBase*)pPalette);
	}
	void CEmfFile::Read_EMR_SELECTPALETTE()
	{
		unsigned int ulIndex;
		m_oStream >> ulIndex;
		m_oPlayer.SelectPalette(ulIndex);
	}
	void CEmfFile::Read_EMR_REALIZEPALETTE()
	{
		// TODO: Реализовать
	}
	void CEmfFile::Read_EMR_INTERSECTCLIPRECT()
	{
		TEmfRectL oClip;
		m_oStream >> oClip;

		TRectD oClipRect;
		TranslatePoint(oClip.lLeft, oClip.lTop, oClipRect.dLeft, oClipRect.dTop);
		TranslatePoint(oClip.lRight, oClip.lBottom, oClipRect.dRight, oClipRect.dBottom);
		m_pDC->GetClip()->Intersect(oClipRect);
	}
	void CEmfFile::Read_EMR_SETLAYOUT()
	{
		unsigned int ulLayoutMode;
		m_oStream >> ulLayoutMode;

		// TODO: реализовать
	}
	void CEmfFile::Read_EMR_SETBRUSHORGEX()
	{
		TEmfPointL oOrigin;
		m_oStream >> oOrigin;

		// TODO: реализовать
	}		
	void CEmfFile::Read_EMR_ANGLEARC()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfPointL oCenter;
		unsigned int unRadius;
		double dStartAngle, dSweepAngle;
		m_oStream >> oCenter >> unRadius >> dStartAngle >> dSweepAngle;

		ArcTo(oCenter.x - unRadius, oCenter.y - unRadius, oCenter.x + unRadius, oCenter.y + unRadius, dStartAngle, dSweepAngle);
		DrawPath(true, false);
	}
	void CEmfFile::Read_EMR_ARC_BASE(TEmfRectL& oBox, TEmfPointL& oStart, TEmfPointL& oEnd, double& dStartAngle, double& dSweepAngle)
	{
		m_oStream >> oBox >> oStart >> oEnd;

		dStartAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oStart.x, oStart.y);
		dSweepAngle = GetEllipseAngle(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, oEnd.x, oEnd.y) - dStartAngle;

		// TODO: Проверить здесь
		if (dSweepAngle < 0.001)
			dSweepAngle += 360;

		// TODO: Проверить здесь
		if (AD_COUNTERCLOCKWISE != m_pDC->GetArcDirection())
		{
			dSweepAngle = dSweepAngle - 360;
		}
	}
	void CEmfFile::Read_EMR_ARC()
	{
		TEmfRectL oBox;
		TEmfPointL oStart, oEnd;
		double dStartAngle, dSweep;
		Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

		MoveTo(oStart);
		ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweep);
		DrawPath(true, false);
	}
	void CEmfFile::Read_EMR_ARCTO()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfRectL oBox;
		TEmfPointL oStart, oEnd;
		double dStartAngle, dSweep;
		Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

		ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweep);
	}
	void CEmfFile::Read_EMR_CHORD()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfRectL oBox;
		TEmfPointL oStart, oEnd;
		double dStartAngle, dSweep;
		Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

		MoveTo(oStart);
		ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweep);
		LineTo(oStart);
		DrawPath(true, true);
	}
	void CEmfFile::Read_EMR_ELLIPSE()
	{
		TEmfRectL oBox;
		m_oStream >> oBox;
		ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, 0, 360);
		DrawPath(true, true);
	}
	void CEmfFile::Read_EMR_EXTTEXTOUTA()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfExtTextoutA oText;
		m_oStream >> oText;	

		DrawTextA(oText.aEmrText, oText.iGraphicsMode);
	}
	void CEmfFile::Read_EMR_EXTTEXTOUTW()
	{
		TEmfExtTextoutW oText;
		m_oStream >> oText;
		DrawTextW(oText.wEmrText, oText.iGraphicsMode);
	}
	void CEmfFile::Read_EMR_LINETO()
	{
		TEmfPointL oPoint;
		m_oStream >> oPoint;
		LineTo(oPoint);
	}
	void CEmfFile::Read_EMR_PIE()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfRectL oBox;
		TEmfPointL oStart, oEnd;
		double dStartAngle, dSweep;
		Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

		ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweep);
		LineTo((oBox.lLeft + oBox.lRight) / 2, (oBox.lTop + oBox.lBottom) / 2);
		ClosePath();
		DrawPath(true, true);
	}
	void CEmfFile::Read_EMR_POLYBEZIER()
	{
		Read_EMR_POLYBEZIER_BASE<TEmfPointL>();
	}
	void CEmfFile::Read_EMR_POLYBEZIER16()
	{
		Read_EMR_POLYBEZIER_BASE<TEmfPointS>();
	}
	template<typename T>void CEmfFile::Read_EMR_POLYBEZIER_BASE()
	{
		TEmfRectL oBounds;
		m_oStream >> oBounds;

		unsigned int ulCount;
		m_oStream >> ulCount;

		if (0 == ulCount)
			return;

		T oStartPoint;
		m_oStream >> oStartPoint;
		MoveTo(oStartPoint);

		T oPoint1, oPoint2, oPointE;
		for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex += 3)
		{
			m_oStream >> oPoint1 >> oPoint2 >> oPointE;
			CurveTo(oPoint1, oPoint2, oPointE);
		}
		DrawPath(true, false);
	}
	void CEmfFile::Read_EMR_POLYBEZIERTO()
	{
		Read_EMR_POLYBEZIERTO_BASE<TEmfPointL>();
	}
	void CEmfFile::Read_EMR_POLYBEZIERTO16()
	{
		Read_EMR_POLYBEZIERTO_BASE<TEmfPointS>();
	}
	template<typename T>void CEmfFile::Read_EMR_POLYBEZIERTO_BASE()
	{
		TEmfRectL oBounds;
		m_oStream >> oBounds;

		unsigned int ulCount;
		m_oStream >> ulCount;

		for (unsigned int ulIndex = 0; ulIndex < ulCount; ulIndex += 3)
		{
			if (ulCount - ulIndex < 2)
				return SetError();

			T oPoint1, oPoint2, oPointE;
			m_oStream >> oPoint1 >> oPoint2 >> oPointE;
			CurveTo(oPoint1, oPoint2, oPointE);
		}
	}
	void CEmfFile::Read_EMR_POLYDRAW()
	{
		Read_EMR_POLYDRAW_BASE<TEmfPointL>();
	}
	void CEmfFile::Read_EMR_POLYDRAW16()
	{
		Read_EMR_POLYDRAW_BASE<TEmfPointS>();
	}
	template<typename T>void CEmfFile::Read_EMR_POLYDRAW_BASE()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		//bug #35006 - не прочитывается весь рекорд ... выравнивание?

		TEmfRectL oBounds;
		m_oStream >> oBounds;

		unsigned int unCount;
		m_oStream >> unCount;

		if (0 == unCount)
			return;

		T* pPoints = new T[unCount];
		if (!pPoints)
			return SetError();

		for (unsigned int unIndex = 0; unIndex < unCount; unIndex++)
		{
			m_oStream >> pPoints[unIndex];
		}

		unsigned char* pAbTypes = new unsigned char[unCount];
		if (!pAbTypes)
		{
			delete[] pPoints;
			return SetError();
		}

		for (unsigned int unIndex = 0; unIndex < unCount; unIndex++)
		{
			m_oStream >> pAbTypes[unIndex];
		}

		T* pPoint1 = NULL, *pPoint2 = NULL;
		for (unsigned int unIndex = 0, unPointIndex = 0; unIndex < unCount; unIndex++)
		{
			unsigned char unType = pAbTypes[unIndex];
			T* pPoint = pPoints + unIndex;
			if (PT_MOVETO == unType)
			{
				MoveTo(*pPoint);
				unPointIndex = 0;
			}
			else if (PT_LINETO & unType)
			{
				LineTo(*pPoint);
				if (PT_CLOSEFIGURE & unType)
					ClosePath();
				unPointIndex = 0;
			}
			else if (PT_BEZIERTO & unType)
			{
				if (0 == unPointIndex)
				{
					pPoint1 = pPoint;
					unPointIndex = 1;
				}
				else if (1 == unPointIndex)
				{
					pPoint2 = pPoint;
					unPointIndex = 2;
				}
				else if (2 == unPointIndex)
				{
					CurveTo(*pPoint1, *pPoint2, *pPoint);
					unPointIndex = 0;

					if (PT_CLOSEFIGURE & unType)
						ClosePath();
				}
				else
				{
					SetError();
					break;
				}
			}

		}

		delete[] pPoints;
		delete[] pAbTypes;
	}
	void CEmfFile::Read_EMR_POLYGON()
	{
		Read_EMR_POLYGON_BASE<TEmfPointL>();
	}
	void CEmfFile::Read_EMR_POLYGON16()
	{
		Read_EMR_POLYGON_BASE<TEmfPointS>();
	}
	template<typename T>void CEmfFile::Read_EMR_POLYGON_BASE()
	{
		TEmfRectL oBounds;
		m_oStream >> oBounds;
		unsigned int ulCount;
		m_oStream >> ulCount;

		if (ulCount <= 0)
			return;

		T oPoint;
		m_oStream >> oPoint;
		MoveTo(oPoint);
		for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex++)
		{
			m_oStream >> oPoint;
			LineTo(oPoint);
		}
		ClosePath();
		DrawPath(true, true);
	}
	void CEmfFile::Read_EMR_POLYLINE()
	{
		Read_EMR_POLYLINE_BASE<TEmfPointL>();
	}
	void CEmfFile::Read_EMR_POLYLINE16()
	{
		Read_EMR_POLYLINE_BASE<TEmfPointS>();
	}
	template<typename T>void CEmfFile::Read_EMR_POLYLINE_BASE()
	{
		TEmfRectL oBounds;
		m_oStream >> oBounds;
		unsigned int ulCount;
		m_oStream >> ulCount;

		if (0 == ulCount)
			return;

		T oPoint;
		m_oStream >> oPoint;
		MoveTo(oPoint);

		for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex++)
		{
			m_oStream >> oPoint;
			LineTo(oPoint);
		}

		DrawPath(true, false);
	}
	void CEmfFile::Read_EMR_POLYLINETO()
	{
		Read_EMR_POLYLINETO_BASE<TEmfPointL>();
	}
	void CEmfFile::Read_EMR_POLYLINETO16()
	{
		Read_EMR_POLYLINETO_BASE<TEmfPointS>();
	}
	template<typename T>void CEmfFile::Read_EMR_POLYLINETO_BASE()
	{
		TEmfRectL oBounds;
		m_oStream >> oBounds;

		unsigned int ulCount;
		m_oStream >> ulCount;

		for (unsigned int ulIndex = 0; ulIndex < ulCount; ulIndex++)
		{
			T oPoint;
			m_oStream >> oPoint;
			LineTo(oPoint);
		}
	}
	void CEmfFile::Read_EMR_POLYPOLYGON()
	{
		Read_EMR_POLYPOLYGON_BASE<TEmfPointL>();
	}
	void CEmfFile::Read_EMR_POLYPOLYGON16()
	{
		Read_EMR_POLYPOLYGON_BASE<TEmfPointS>();			
	}
	template<typename T>void CEmfFile::Read_EMR_POLYPOLYGON_BASE()
	{
		TEmfRectL oBounds;
		m_oStream >> oBounds;
		unsigned int ulNumberOfPolygons;
		m_oStream >> ulNumberOfPolygons;
		unsigned int ulTotalPointsCount;
		m_oStream >> ulTotalPointsCount;

		unsigned int* pPolygonPointCount = new unsigned int[ulNumberOfPolygons];
		if (!pPolygonPointCount)
			return SetError();

		for (unsigned int ulIndex = 0; ulIndex < ulNumberOfPolygons; ulIndex++)
		{
			m_oStream >> pPolygonPointCount[ulIndex];
		}

		for (unsigned int ulPolygonIndex = 0, unStartPointIndex = 0; ulPolygonIndex < ulNumberOfPolygons; ulPolygonIndex++)
		{
			unsigned int ulCurrentPolygonPointsCount = pPolygonPointCount[ulPolygonIndex];
			if (0 == ulCurrentPolygonPointsCount)
				continue;

			T oPoint;
			m_oStream >> oPoint;
			MoveTo(oPoint);

			for (unsigned int ulPointIndex = 1; ulPointIndex < ulCurrentPolygonPointsCount; ulPointIndex++)
			{
				unsigned int ulRealPointIndex = ulPointIndex + unStartPointIndex;
				if (ulRealPointIndex >= ulTotalPointsCount)
				{
					delete[] pPolygonPointCount;
					return SetError();
				}

				m_oStream >> oPoint;
				LineTo(oPoint);
			}

			ClosePath();
		}
		DrawPath(true, true);

		delete[] pPolygonPointCount;
	}
	void CEmfFile::Read_EMR_POLYPOLYLINE()
	{
		Read_EMR_POLYPOLYLINE_BASE<TEmfPointL>();
	}
	void CEmfFile::Read_EMR_POLYPOLYLINE16()
	{
		Read_EMR_POLYPOLYLINE_BASE<TEmfPointS>();
	}
	template<typename T>void CEmfFile::Read_EMR_POLYPOLYLINE_BASE()
	{
		TEmfRectL oBounds;
		m_oStream >> oBounds;

		unsigned int ulNumberOfPolylines;
		m_oStream >> ulNumberOfPolylines;

		unsigned int ulTotalPointsCount;
		m_oStream >> ulTotalPointsCount;

		if (0 == ulNumberOfPolylines && 0 == ulTotalPointsCount)
			return;
		else if (0 == ulNumberOfPolylines || 0 == ulTotalPointsCount)
			return SetError();

		unsigned int* pPolylinePointCount = new unsigned int[ulNumberOfPolylines];
		for (unsigned int ulIndex = 0; ulIndex < ulNumberOfPolylines; ulIndex++)
		{
			m_oStream >> pPolylinePointCount[ulIndex];
		}

		for (unsigned int ulPolyIndex = 0, ulStartPointIndex = 0; ulPolyIndex < ulNumberOfPolylines; ulPolyIndex++)
		{
			unsigned int ulCurrentPolylinePointsCount = pPolylinePointCount[ulPolyIndex];
			if (0 == ulCurrentPolylinePointsCount)
				continue;

			T oPoint;
			m_oStream >> oPoint;
			MoveTo(oPoint);

			for (unsigned int ulPointIndex = 1; ulPointIndex < ulCurrentPolylinePointsCount; ulPointIndex++)
			{
				unsigned int ulRealPointIndex = ulPointIndex + ulStartPointIndex;
				if (ulRealPointIndex >= ulTotalPointsCount)
				{
					delete[] pPolylinePointCount;
					return SetError();
				}

				m_oStream >> oPoint;
				LineTo(oPoint);
			}				
		}
		DrawPath(true, false);

		delete[] pPolylinePointCount;
	}
	void CEmfFile::Read_EMR_POLYTEXTOUTA()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfPolyTextoutA oText;
		m_oStream >> oText;
		
		if (0 == oText.cStrings)
			return;

		if (!oText.aEmrText)
			return SetError();

		for (unsigned int unIndex = 0; unIndex < oText.cStrings; unIndex++)
		{
			DrawTextA(oText.aEmrText[unIndex], oText.iGraphicsMode);
		}
	}
	void CEmfFile::Read_EMR_POLYTEXTOUTW()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfPolyTextoutW oText;
		m_oStream >> oText;

		if (0 == oText.cStrings)
			return;

		if (!oText.wEmrText)
			return SetError();

		for (unsigned int unIndex = 0; unIndex < oText.cStrings; unIndex++)
		{
			DrawTextA(oText.wEmrText[unIndex], oText.iGraphicsMode);
		}
	}
	void CEmfFile::Read_EMR_RECTANGLE()
	{
		TEmfRectL oBox;
		m_oStream >> oBox;

		if (AD_COUNTERCLOCKWISE == m_pDC->GetArcDirection())
		{
			MoveTo(oBox.lLeft, oBox.lTop);
			LineTo(oBox.lLeft, oBox.lBottom);
			LineTo(oBox.lRight, oBox.lBottom);
			LineTo(oBox.lRight, oBox.lTop);
		}
		else
		{
			MoveTo(oBox.lLeft, oBox.lTop);
			LineTo(oBox.lRight, oBox.lTop);
			LineTo(oBox.lRight, oBox.lBottom);
			LineTo(oBox.lLeft, oBox.lBottom);
		}
		ClosePath();
		DrawPath(true, true);
	}
	void CEmfFile::Read_EMR_ROUNDRECT()
	{
		TEmfRectL oBox;
		TEmfSizeL oCorner;
		m_oStream >> oBox >> oCorner;

		int lBoxW = oBox.lRight - oBox.lLeft;
		int lBoxH = oBox.lBottom - oBox.lTop;

		int lRoundW = (std::min)((int)oCorner.cx, lBoxW / 2);
		int lRoundH = (std::min)((int)oCorner.cy, lBoxH / 2);

		if (AD_COUNTERCLOCKWISE == m_pDC->GetArcDirection())
		{
			MoveTo(oBox.lLeft + lRoundW, oBox.lTop);
			ArcTo(oBox.lLeft, oBox.lTop, oBox.lLeft + lRoundW, oBox.lTop + lRoundH, 270, -90);
			LineTo(oBox.lLeft, oBox.lBottom - lRoundH);
			ArcTo(oBox.lLeft, oBox.lBottom - lRoundH, oBox.lLeft + lRoundW, oBox.lBottom, 180, -90);
			LineTo(oBox.lRight - lRoundW, oBox.lBottom);
			ArcTo(oBox.lRight - lRoundW, oBox.lBottom - lRoundH, oBox.lRight, oBox.lBottom, 90, -90);
			LineTo(oBox.lRight, oBox.lTop + lRoundH);
			ArcTo(oBox.lRight - lRoundW, oBox.lTop, oBox.lRight, oBox.lTop + lRoundH, 0, -90);
			LineTo(oBox.lLeft + lRoundW, oBox.lTop);
		}
		else
		{
			MoveTo(oBox.lLeft + lRoundW, oBox.lTop);
			LineTo(oBox.lRight - lRoundW, oBox.lTop);
			ArcTo(oBox.lRight - lRoundW, oBox.lTop, oBox.lRight, oBox.lTop + lRoundH, -90, 90);
			LineTo(oBox.lRight, oBox.lBottom - lRoundH);
			ArcTo(oBox.lRight - lRoundW, oBox.lBottom - lRoundH, oBox.lRight, oBox.lBottom, 0, 90);
			LineTo(oBox.lLeft + lRoundW, oBox.lBottom);
			ArcTo(oBox.lLeft, oBox.lBottom - lRoundH, oBox.lLeft + lRoundW, oBox.lBottom, 90, 90);
			LineTo(oBox.lLeft, oBox.lTop + lRoundH);
			ArcTo(oBox.lLeft, oBox.lTop, oBox.lLeft + lRoundW, oBox.lTop + lRoundH, 180, 90);
		}

		ClosePath();
		DrawPath(true, true);
	}
	void CEmfFile::Read_EMR_SETPIXELV()
	{
		TEmfPointL oPoint;
		TEmfColor oColor;

		m_oStream >> oPoint;
		m_oStream >> oColor;

		// Делаем цветом кисти
		BYTE pBgraBuffer[4];
        pBgraBuffer[0] = oColor.b;
		pBgraBuffer[1] = oColor.g;
        pBgraBuffer[2] = oColor.r;
		pBgraBuffer[3] = 0xff;

		DrawImage(oPoint.x, oPoint.y, 1, 1, pBgraBuffer, 1, 1);
	}
	void CEmfFile::Read_EMR_SMALLTEXTOUT()
	{
		TEmfSmallTextout oText;
		m_oStream >> oText;

		// Переводим oText в TEmfEmrText
		TEmfEmrText oEmrText;
		oEmrText.Chars        = oText.cChars;
		oEmrText.offDx        = 0;
		oEmrText.offString    = 0;
		oEmrText.Options      = oText.fuOptions;
		oEmrText.OutputString = oText.TextString;
		oEmrText.Reference.x  = oText.x;
		oEmrText.Reference.y  = oText.y;
		oEmrText.OutputDx     = NULL;

		// Запись не документированна нормально, остается несколько байт в конце, непонятно почему.
		unsigned int unSize = oText.GetSize();
		if (m_ulRecordSize - unSize > 0)
			m_oStream.Skip(m_ulRecordSize - unSize);
		else if (m_ulRecordSize - unSize < 0)
			m_oStream.SeekBack(unSize - m_ulRecordSize);

		DrawTextW(oEmrText, oText.iGraphicsMode);

		// Поскольку мы просто скопировали ссылку на строку, а не скопировали сами строку обнуляем здесь, потому 
		// что на деструкторе структуры освобождается память.
		oEmrText.OutputString = NULL;
	}
	void CEmfFile::Read_EMR_STROKEANDFILLPATH()
	{
		TEmfRectL oBounds;
		m_oStream >> oBounds;
		if (m_pOutput && m_pPath)
		{
			m_pPath->Draw(m_pOutput, true, true);
			RELEASEOBJECT(m_pPath);
		}
	}
	void CEmfFile::Read_EMR_STROKEPATH()
	{
		TEmfRectL oBounds;
		m_oStream >> oBounds;
		if (m_pOutput && m_pPath)
		{
			m_pPath->Draw(m_pOutput, true, false);
			RELEASEOBJECT(m_pPath);
		}
	}
}
