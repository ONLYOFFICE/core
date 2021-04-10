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
    #include "../../../xml/include/xmlutils.h"
    #include  <algorithm>
    #include <cstdlib>

    std::wstring StringNormalization(std::wstring wsString)
    {
        std::wstring wsText;
        for (wchar_t wChar : wsString)
            if (wChar == L'<')
                   wsText += L"&lt;";
            else if (wChar == L'>')
                   wsText += L"&gt;";
            else if (wChar == L'&')
                   wsText += L"&amp;";
            else if (wChar == L'\'')
                   wsText += L"&apos;";
            else if (wChar == L'"')
                   wsText += L"&quot;";
            else if (wChar == 0x00)
                return wsText;
            else wsText += wChar;
        return wsText;
    }
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
		{
			m_pOutput->Begin();
			#ifdef _DEBUG
				oXmlWriter.WriteString(L"<?xml version=\"1.0\"?>\n");
				oXmlWriter.WriteNodeBegin(L"EMF");
			#endif
		}

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
		{
			m_pOutput->End();

			#ifdef _DEBUG
				oXmlWriter.WriteNodeEnd(L"EMF");
				oXmlWriter.SaveToFile(NSFile::GetProcessDirectory() + L"/tests.xml");
			#endif
		}
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

		#ifdef _DEBUG
			if (m_pOutput)
			{
					oXmlWriter.WriteNode(L"Text", StringNormalization(wsText));
			}
		#endif

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

                #ifdef _DEBUG
                        if (m_pOutput)
                        {
                                        oXmlWriter.WriteNode(L"Text", StringNormalization(wsText));
                        }
                #endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_HEADER", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_HEADER);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_HEADER", true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	m_oHeader.oBounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	m_oHeader.oBounds.lTop);
						oXmlWriter.WriteNode(L"Right",	m_oHeader.oBounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", m_oHeader.oBounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNodeBegin(L"Frame");
						oXmlWriter.WriteNode(L"Left",	m_oHeader.oFrame.lLeft);
						oXmlWriter.WriteNode(L"Top",	m_oHeader.oFrame.lTop);
						oXmlWriter.WriteNode(L"Right",	m_oHeader.oFrame.lRight);
						oXmlWriter.WriteNode(L"Bottom", m_oHeader.oFrame.lBottom);
						oXmlWriter.WriteNodeEnd(L"Frame");

					oXmlWriter.WriteNode(L"Signature",	    (int)m_oHeader.ulSignature);
					oXmlWriter.WriteNode(L"Version",	    (int)m_oHeader.ulVersion);
					oXmlWriter.WriteNode(L"Size",		    (int)m_oHeader.ulSize);
					oXmlWriter.WriteNode(L"Records",	    (int)m_oHeader.ulRecords);
					oXmlWriter.WriteNode(L"CountObjects",	    (int)m_oHeader.ushObjects);
					oXmlWriter.WriteNode(L"Reserved",	    (int)m_oHeader.ushReserved);
					oXmlWriter.WriteNode(L"SizeDescription",    (int)m_oHeader.ulSizeDescription);
					oXmlWriter.WriteNode(L"OffsetDescription",  (int)m_oHeader.ulOffsetDescription);
					oXmlWriter.WriteNode(L"PalletEntries",	    (int)m_oHeader.ulPalEntries);

					oXmlWriter.WriteNodeBegin(L"Frame");
						oXmlWriter.WriteNode(L"X", (int)m_oHeader.oDevice.cx);
						oXmlWriter.WriteNode(L"Y", (int)m_oHeader.oDevice.cy);
						oXmlWriter.WriteNodeEnd(L"Frame");

					oXmlWriter.WriteNodeBegin(L"Millimeters");
						oXmlWriter.WriteNode(L"X", (int)m_oHeader.oMillimeters.cx);
						oXmlWriter.WriteNode(L"Y", (int)m_oHeader.oMillimeters.cy);
						oXmlWriter.WriteNodeEnd(L"Millimeters");

					//TODO: не хватает сохранения остальных записей, которые не используются

					oXmlWriter.WriteNodeEnd(L"EMR_HEADER");
			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_ALPHABLEND", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_ALPHABLEND);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_ALPHABLEND", true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oBitmap.Bounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBitmap.Bounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oBitmap.Bounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBitmap.Bounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNode(L"xDest",	oBitmap.xDest);
					oXmlWriter.WriteNode(L"yDest",	oBitmap.yDest);
					oXmlWriter.WriteNode(L"cxDest", oBitmap.cxDest);
					oXmlWriter.WriteNode(L"cyDest", oBitmap.cyDest);

					oXmlWriter.WriteNodeBegin(L"BLENDFUNCTION");
						oXmlWriter.WriteNode(L"BlendOperation",	    oBitmap.BlendOperation);
						oXmlWriter.WriteNode(L"BlendFlags",	    oBitmap.BlendFlags);
						oXmlWriter.WriteNode(L"SrcConstantAlpha",   oBitmap.SrcConstantAlpha);
						oXmlWriter.WriteNode(L"AlphaFormat",	    oBitmap.AlphaFormat);
						oXmlWriter.WriteNodeEnd(L"BLENDFUNCTION");

					oXmlWriter.WriteNode(L"xSrc", oBitmap.xSrc);
					oXmlWriter.WriteNode(L"ySrc", oBitmap.ySrc);

					oXmlWriter.WriteNodeBegin(L"XformSrc");
						oXmlWriter.WriteNode(L"M11",	oBitmap.XformSrc.M11);
						oXmlWriter.WriteNode(L"M12",	oBitmap.XformSrc.M12);
						oXmlWriter.WriteNode(L"M21",	oBitmap.XformSrc.M21);
						oXmlWriter.WriteNode(L"M22",	oBitmap.XformSrc.M22);
						oXmlWriter.WriteNode(L"Dx",	oBitmap.XformSrc.Dx);
						oXmlWriter.WriteNode(L"Dy",	oBitmap.XformSrc.Dy);
						oXmlWriter.WriteNodeEnd(L"XformSrc");

					oXmlWriter.WriteNodeBegin(L"BkColorSrc");
						oXmlWriter.WriteNode(L"Red",	    oBitmap.BkColor.r);
						oXmlWriter.WriteNode(L"Green",	    oBitmap.BkColor.g);
						oXmlWriter.WriteNode(L"Blue",	    oBitmap.BkColor.b);
						oXmlWriter.WriteNode(L"Reserved",   oBitmap.BkColor.a);
						oXmlWriter.WriteNodeEnd(L"BkColorSrc");

					oXmlWriter.WriteNode(L"UsageSrc",   (int)oBitmap.UsageSrc);
					oXmlWriter.WriteNode(L"offBmiSrc",  (int)oBitmap.offBmiSrc);
					oXmlWriter.WriteNode(L"cbBmiSrc",   (int)oBitmap.cbBmiSrc);
					oXmlWriter.WriteNode(L"offBitsSrc", (int)oBitmap.offBitsSrc);
					oXmlWriter.WriteNode(L"cbBitsSrc",  (int)oBitmap.cbBitsSrc);
					oXmlWriter.WriteNode(L"cxSrc",	    oBitmap.cxSrc);
					oXmlWriter.WriteNode(L"cySrc",	    oBitmap.cySrc);

					//TODO: не хватает записи переменной BitmapBuffer

					oXmlWriter.WriteNodeEnd(L"EMR_ALPHABLEND");

			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_EOF", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_EOF);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_EOF", true, false);

					oXmlWriter.WriteNode(L"nPalEntries",	(int)ulCount);
					oXmlWriter.WriteNode(L"offPalEntries",	(int)ulOffset);
					oXmlWriter.WriteNode(L"SizeLast",	(int)ulSizeLast);

					oXmlWriter.WriteNodeEnd(L"EMR_EOF");

			}
		#endif

	}
	void CEmfFile::Read_EMR_UNKNOWN()
	{
		// Неизвестные и нереализованные записи мы пропускаем
		m_oStream.Skip(m_ulRecordSize);
	}
	void CEmfFile::Read_EMR_SAVEDC()
	{
		m_pDC = m_oPlayer.SaveDC();

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SAVEDC", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SAVEDC);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SAVEDC", true, true);

			}
		#endif
	}
	void CEmfFile::Read_EMR_RESTOREDC()
	{
		int lSavedDC;
		m_oStream >> lSavedDC;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_RESTOREDC", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_EOF);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_RESTOREDC", true, false);

					oXmlWriter.WriteNode(L"SavedDC", lSavedDC);

					oXmlWriter.WriteNodeEnd(L"EMR_RESTOREDC");

			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_MODIFYWORLDTRANSFORM", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_MODIFYWORLDTRANSFORM);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_MODIFYWORLDTRANSFORM", true, false);

					oXmlWriter.WriteNodeBegin(L"XForm");
						oXmlWriter.WriteNode(L"M11", oXForm.M11);
						oXmlWriter.WriteNode(L"M12", oXForm.M12);
						oXmlWriter.WriteNode(L"M21", oXForm.M21);
						oXmlWriter.WriteNode(L"M22", oXForm.M22);
						oXmlWriter.WriteNode(L"Dx",  oXForm.Dx);
						oXmlWriter.WriteNode(L"Dy",  oXForm.Dy);
						oXmlWriter.WriteNodeEnd(L"XForm");

					oXmlWriter.WriteNode(L"ModifyWorldTransformMode", (int)ulMode);

					oXmlWriter.WriteNodeEnd(L"EMR_MODIFYWORLDTRANSFORM");

			}
		#endif

		m_pDC->MultiplyTransform(oXForm, ulMode);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_SETWORLDTRANSFORM()
	{
		TEmfXForm oXForm;

		m_oStream >> oXForm;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETWORLDTRANSFORM", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETWORLDTRANSFORM);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETWORLDTRANSFORM", true, false);

					oXmlWriter.WriteNodeBegin(L"XForm");
						oXmlWriter.WriteNode(L"M11", oXForm.M11);
						oXmlWriter.WriteNode(L"M12", oXForm.M12);
						oXmlWriter.WriteNode(L"M21", oXForm.M21);
						oXmlWriter.WriteNode(L"M22", oXForm.M22);
						oXmlWriter.WriteNode(L"Dx",  oXForm.Dx);
						oXmlWriter.WriteNode(L"Dy",  oXForm.Dy);
						oXmlWriter.WriteNodeEnd(L"XForm");

					oXmlWriter.WriteNodeEnd(L"EMR_SETWORLDTRANSFORM");

			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_CREATEBRUSHINDIRECT", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_CREATEBRUSHINDIRECT);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_CREATEBRUSHINDIRECT", true, false);

					oXmlWriter.WriteNode(L"ihBrush", (int)ulBrushIndex);
					oXmlWriter.WriteNodeBegin(L"LogBrush");
						oXmlWriter.WriteNode(L"BrushStyle", (int)pBrush->BrushStyle);
						oXmlWriter.WriteNodeBegin(L"Color");
							oXmlWriter.WriteNode(L"Red",	    pBrush->Color.r);
							oXmlWriter.WriteNode(L"Green",	    pBrush->Color.g);
							oXmlWriter.WriteNode(L"Blue",	    pBrush->Color.b);
							oXmlWriter.WriteNode(L"Reserved",   pBrush->Color.a);
							oXmlWriter.WriteNodeEnd(L"Color");
						oXmlWriter.WriteNode(L"BrushHatch", (int)pBrush->BrushHatch);

						oXmlWriter.WriteNodeEnd(L"LogBrush");

					oXmlWriter.WriteNodeEnd(L"EMR_CREATEBRUSHINDIRECT");

			}
		#endif

		m_oPlayer.RegisterObject(ulBrushIndex, (CEmfObjectBase*)pBrush);
	}
	void CEmfFile::Read_EMR_SETTEXTCOLOR()
	{
		TEmfColor oColor;
		m_oStream >> oColor;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETTEXTCOLOR", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETTEXTCOLOR);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETTEXTCOLOR", true, false);

					oXmlWriter.WriteNodeBegin(L"Color");
						oXmlWriter.WriteNode(L"Red",	    oColor.r);
						oXmlWriter.WriteNode(L"Green",	    oColor.g);
						oXmlWriter.WriteNode(L"Blue",	    oColor.b);
						oXmlWriter.WriteNode(L"Reserved",   oColor.a);
						oXmlWriter.WriteNodeEnd(L"Color");

					oXmlWriter.WriteNodeEnd(L"EMR_SETTEXTCOLOR");

			}
		#endif

		m_pDC->SetTextColor(oColor);
		UpdateOutputDC();
	}		
	void CEmfFile::Read_EMR_SELECTOBJECT()
	{
		unsigned int ulObjectIndex;
		m_oStream >> ulObjectIndex;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SELECTOBJECT", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SELECTOBJECT);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SELECTOBJECT", true, false);

					oXmlWriter.WriteNode(L"Index", (int)ulObjectIndex);

					oXmlWriter.WriteNodeEnd(L"EMR_SELECTOBJECT");

			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_EXTCREATEFONTINDIRECTW", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_EXTCREATEFONTINDIRECTW);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_EXTCREATEFONTINDIRECTW", true, false);

					oXmlWriter.WriteNode(L"ihFonts", (int)ulIndex);

					if (!pFont->IsFixedLength())
					{
						oXmlWriter.WriteNodeBegin(L"LogFontExDv");
					}

					oXmlWriter.WriteNodeBegin(L"LogFont");
						oXmlWriter.WriteNode(L"Height",		pFont->LogFontEx.LogFont.Height);
						oXmlWriter.WriteNode(L"Width",		pFont->LogFontEx.LogFont.Width);
						oXmlWriter.WriteNode(L"Escapement",	pFont->LogFontEx.LogFont.Escapement);
						oXmlWriter.WriteNode(L"Orientation",	pFont->LogFontEx.LogFont.Orientation);
						oXmlWriter.WriteNode(L"Weight",		pFont->LogFontEx.LogFont.Weight);
						oXmlWriter.WriteNode(L"Italic",		pFont->LogFontEx.LogFont.Italic);
						oXmlWriter.WriteNode(L"Underline",	pFont->LogFontEx.LogFont.Underline);
						oXmlWriter.WriteNode(L"StrikeOut",	pFont->LogFontEx.LogFont.StrikeOut);
						oXmlWriter.WriteNode(L"CharSet",	pFont->LogFontEx.LogFont.CharSet);
						oXmlWriter.WriteNode(L"OutPrecision",	pFont->LogFontEx.LogFont.OutPrecision);
						oXmlWriter.WriteNode(L"ClipPrecision",	pFont->LogFontEx.LogFont.ClipPrecision);
						oXmlWriter.WriteNode(L"Quality",	pFont->LogFontEx.LogFont.Quality);
						oXmlWriter.WriteNode(L"PitchAndFamily", pFont->LogFontEx.LogFont.PitchAndFamily);
						oXmlWriter.WriteNode(L"FaceName",	StringNormalization(NSStringExt::CConverter::GetUnicodeFromUTF16(pFont->LogFontEx.LogFont.FaceName, 32)));
						oXmlWriter.WriteNodeEnd(L"LogFont");

					oXmlWriter.WriteNode(L"FullName",   StringNormalization(NSStringExt::CConverter::GetUnicodeFromUTF16(pFont->LogFontEx.FullName, 64)));
					oXmlWriter.WriteNode(L"Style",	    StringNormalization(NSStringExt::CConverter::GetUnicodeFromUTF16(pFont->LogFontEx.Style,	32)));
					oXmlWriter.WriteNode(L"Script",	    StringNormalization(NSStringExt::CConverter::GetUnicodeFromUTF16(pFont->LogFontEx.Script,	32)));

					if (!pFont->IsFixedLength())
					{
						oXmlWriter.WriteNodeBegin(L"DesignVector");
							oXmlWriter.WriteNode(L"Signature",  (int)pFont->DesignVector.Signature);
							oXmlWriter.WriteNode(L"NumAxes",    (int)pFont->DesignVector.NumAxes);

							for (unsigned int i = 0; i < pFont->DesignVector.NumAxes; ++i)
								oXmlWriter.WriteNode(L"NumAxes" + std::to_wstring(i + 1),    pFont->DesignVector.Values[i]);

							oXmlWriter.WriteNodeEnd(L"DesignVector");
							oXmlWriter.WriteNodeEnd(L"LogFontExDv");
					}

					oXmlWriter.WriteNodeEnd(L"EMR_EXTCREATEFONTINDIRECTW");

			}
		#endif

		m_oPlayer.RegisterObject(ulIndex, (CEmfObjectBase*)pFont);
	}
	void CEmfFile::Read_EMR_SETTEXTALIGN()
	{
		unsigned int ulAlign;
		m_oStream >> ulAlign;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETTEXTALIGN", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETTEXTALIGN);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETTEXTALIGN", true, false);

					oXmlWriter.WriteNode(L"TextAlign", (int)ulAlign);

					oXmlWriter.WriteNodeEnd(L"EMR_SETTEXTALIGN");

			}
		#endif

		m_pDC->SetTextAlign(ulAlign);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_SETBKMODE()
	{
		unsigned int ulBgMode;
		m_oStream >> ulBgMode;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETBKMODE", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETBKMODE);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETBKMODE", true, false);

					oXmlWriter.WriteNode(L"BgMode", (int)ulBgMode);

					oXmlWriter.WriteNodeEnd(L"EMR_SETBKMODE");

			}
		#endif

		m_pDC->SetBgMode(ulBgMode);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_DELETEOBJECT()
	{
		unsigned int ulIndex;
		m_oStream >> ulIndex;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_DELETEOBJECT", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_DELETEOBJECT);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_DELETEOBJECT", true, false);

					oXmlWriter.WriteNode(L"Index", (int)ulIndex);

					oXmlWriter.WriteNodeEnd(L"EMR_DELETEOBJECT");

			}
		#endif

		m_oPlayer.DeleteObject(ulIndex);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_SETMITERLIMIT()
	{
		unsigned int ulMiterLimit;
		m_oStream >> ulMiterLimit;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETMITERLIMIT", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETMITERLIMIT);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETMITERLIMIT", true, false);

					oXmlWriter.WriteNode(L"MiterLimit", (int)ulMiterLimit);

					oXmlWriter.WriteNodeEnd(L"EMR_SETMITERLIMIT");

			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_EXTCREATEPEN", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_EXTCREATEPEN);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_EXTCREATEPEN", true, false);

					m_oStream.SeekBack(16);

					oXmlWriter.WriteNode(L"ihPen",	(int)ulPenIndex);
					oXmlWriter.WriteNode(L"offBmi", (int)m_oStream.ReadULong());
					oXmlWriter.WriteNode(L"cbBmi",	(int)m_oStream.ReadULong());
					oXmlWriter.WriteNode(L"offBits",(int)m_oStream.ReadULong());
					oXmlWriter.WriteNode(L"cbBits",	(int)m_oStream.ReadULong());
			}
		#endif

		unsigned int current_size = m_ulRecordSize - 20;

		CEmfLogPen* pPen = new CEmfLogPen();
		if (!pPen)
			return SetError();

		// LogPenEx
		m_oStream >> pPen->PenStyle;
		m_oStream >> pPen->Width;

		#ifdef _DEBUG
			unsigned int BrushStyle, BrushHatch;
			m_oStream >> BrushStyle;
		#else
			m_oStream.Skip(4); // BrushStyle
		#endif

		m_oStream >> pPen->Color;

		#ifdef _DEBUG
			m_oStream >> BrushHatch;
		#else
			m_oStream.Skip(4); // BrushHatch
		#endif

		m_oStream >> pPen->NumStyleEntries;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"LogPenEx");
					oXmlWriter.WriteNode(L"PenStyle",	    (int)pPen->PenStyle);
					oXmlWriter.WriteNode(L"Width",		    (int)pPen->Width);
					oXmlWriter.WriteNode(L"BrushStyle",	    (int)BrushStyle);
					oXmlWriter.WriteNodeBegin(L"ColorRef");
						oXmlWriter.WriteNode(L"Red",	    pPen->Color.r);
						oXmlWriter.WriteNode(L"Green",	    pPen->Color.g);
						oXmlWriter.WriteNode(L"Blue",	    pPen->Color.b);
						oXmlWriter.WriteNode(L"Reserved",   pPen->Color.a);
						oXmlWriter.WriteNodeEnd(L"ColorRef");

					oXmlWriter.WriteNode(L"BrushHatch",	    (int)BrushHatch);
					oXmlWriter.WriteNode(L"NumStyleEntries",    (int)pPen->NumStyleEntries);

			}
		#endif

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
				#ifdef _DEBUG
					if (m_pOutput)
						oXmlWriter.WriteNode(L"StyleEntry" + std::to_wstring(ulIndex + 1),  (int)pPen->StyleEntry[ulIndex]);
				#endif
			}
		}
		else
		{
			pPen->StyleEntry = NULL;
		}
		#ifdef _DEBUG
			if (m_pOutput)
					oXmlWriter.WriteNodeEnd(L"LogPenEx");
		#endif


		// Пропускаем часть с картинкой, если она была
		m_oStream.Skip(current_size);

		#ifdef _DEBUG
			if (m_pOutput)
			{
				//TODO: не хватает сохранения
				oXmlWriter.WriteNodeEnd(L"EMR_EXTCREATEPEN");
			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_CREATEPEN", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_CREATEPEN);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_CREATEPEN", true, false);

					oXmlWriter.WriteNode(L"ihPen", (int)ulPenIndex);
					oXmlWriter.WriteNodeBegin(L"LogPen");
						oXmlWriter.WriteNode(L"PenStyle", (int)pPen->PenStyle);
						oXmlWriter.WriteNodeBegin(L"Width");
							oXmlWriter.WriteNode(L"X", (int)widthX);
							oXmlWriter.WriteNode(L"Y", (int)widthY);
							oXmlWriter.WriteNodeEnd(L"Width");
						oXmlWriter.WriteNodeEnd(L"LogPen");

			}
		#endif

		if (!widthX)	
		{//emf from Bonetti Martínez. cálculo estructural de pilotes y pilas.xlsx
			widthX = 1 / m_pDC->GetPixelWidth();
		}
		pPen->Width = widthX;

		m_oStream >> pPen->Color;

		#ifdef _DEBUG
			if (m_pOutput)
			{

					oXmlWriter.WriteNodeBegin(L"COLORREF");
						oXmlWriter.WriteNode(L"Red",	    pPen->Color.r);
						oXmlWriter.WriteNode(L"Green",	    pPen->Color.g);
						oXmlWriter.WriteNode(L"Blue",	    pPen->Color.b);
						oXmlWriter.WriteNode(L"Reserved",   pPen->Color.a);
						oXmlWriter.WriteNodeEnd(L"COLORREF");

					oXmlWriter.WriteNodeEnd(L"EMR_CREATEPEN");

			}
		#endif
		m_oPlayer.RegisterObject(ulPenIndex, (CEmfObjectBase*)pPen);
	}
	void CEmfFile::Read_EMR_SETPOLYFILLMODE()
	{
		unsigned int ulFillMode;
		m_oStream >> ulFillMode;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETPOLYFILLMODE", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETPOLYFILLMODE);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETPOLYFILLMODE", true, false);

					oXmlWriter.WriteNode(L"PolygonFillMode", (int)ulFillMode);

					oXmlWriter.WriteNodeEnd(L"EMR_SETPOLYFILLMODE");

			}
		#endif

		m_pDC->SetFillMode(ulFillMode);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_BEGINPATH()
	{
		if (m_pPath)
			delete m_pPath;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_BEGINPATH", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_BEGINPATH);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_BEGINPATH", true, true);
			}
		#endif

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
		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_ENDPATH", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_ENDPATH);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_ENDPATH", true, true);
			}
		#endif
		// Ничего не делаем
	}
	void CEmfFile::Read_EMR_CLOSEFIGURE()
	{
		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_CLOSEFIGURE", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_CLOSEFIGURE);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_CLOSEFIGURE", true, true);
			}
		#endif
		if (m_pPath)
		{
			if (!m_pPath->Close())
				return SetError();
		}
	}
	void CEmfFile::Read_EMR_FLATTENPATH()
	{
		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_FLATTENPATH", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_FLATTENPATH);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_FLATTENPATH", true, true);
			}
		#endif
		// Ничего не делаем
	}
	void CEmfFile::Read_EMR_WIDENPATH()
	{
		// TODO: реализовать

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_WIDENPATH", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_WIDENPATH);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_WIDENPATH", true, true);
			}
		#endif
	}
	void CEmfFile::Read_EMR_ABORTPATH()
	{
		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_ABORTPATH", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_ABORTPATH);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_ABORTPATH", true, true);
			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_MOVETOEX", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_MOVETOEX);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_MOVETOEX", true, false);

					oXmlWriter.WriteNode(L"X", oPoint.x);
					oXmlWriter.WriteNode(L"Y", oPoint.y);

					oXmlWriter.WriteNodeEnd(L"EMR_MOVETOEX");

			}
		#endif

		MoveTo(oPoint);
	}	
	void CEmfFile::Read_EMR_SETARCDIRECTION()
	{
		unsigned int unDirection;
		m_oStream >> unDirection;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETARCDIRECTION", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETARCDIRECTION);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETARCDIRECTION", true, false);

					oXmlWriter.WriteNode(L"arcDirection", (int)unDirection);

					oXmlWriter.WriteNodeEnd(L"EMR_SETARCDIRECTION");

			}
		#endif

		m_pDC->SetArcDirection(unDirection);
		// Здесь не обновляем DC у Output, т.к. этот параметр разруливается внутри данного класса.
	}
	void CEmfFile::Read_EMR_FILLPATH()
	{
		TEmfRectL oBounds;
		m_oStream >> oBounds;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_FILLPATH", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_FILLPATH);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_FILLPATH", true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oBounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oBounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNodeEnd(L"EMR_FILLPATH");

			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETMAPMODE", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETMAPMODE);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETMAPMODE", true, false);

					oXmlWriter.WriteNode(L"MapMode", (int)ulMapMode);

					oXmlWriter.WriteNodeEnd(L"EMR_SETMAPMODE");

			}
		#endif

		m_pDC->SetMapMode(ulMapMode);
	}
	void CEmfFile::Read_EMR_SETWINDOWORGEX()
	{
		TEmfPointL oOrigin;
		m_oStream >> oOrigin;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETWINDOWORGEX", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETWINDOWORGEX);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETWINDOWORGEX", true, false);

					oXmlWriter.WriteNodeBegin(L"Origin");
						oXmlWriter.WriteNode(L"X", (int)oOrigin.x);
						oXmlWriter.WriteNode(L"Y", (int)oOrigin.y);
						oXmlWriter.WriteNodeEnd(L"Origin");

					oXmlWriter.WriteNodeEnd(L"EMR_SETWINDOWORGEX");

			}
		#endif

		m_pDC->SetWindowOrigin(oOrigin);
	}
	void CEmfFile::Read_EMR_SETWINDOWEXTEX()
	{
		TEmfSizeL oExtent;
		m_oStream >> oExtent;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETWINDOWEXTEX", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETWINDOWEXTEX);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETWINDOWEXTEX", true, false);

					oXmlWriter.WriteNodeBegin(L"Extent");
						oXmlWriter.WriteNode(L"cX", (int)oExtent.cx);
						oXmlWriter.WriteNode(L"cY", (int)oExtent.cy);
						oXmlWriter.WriteNodeEnd(L"Extent");

					oXmlWriter.WriteNodeEnd(L"EMR_SETWINDOWEXTEX");

			}
		#endif

		m_pDC->SetWindowExtents(oExtent);
	}
	void CEmfFile::Read_EMR_SETVIEWPORTORGEX()
	{
		TEmfPointL oOrigin;
		m_oStream >> oOrigin;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETVIEWPORTORGEX", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETVIEWPORTORGEX);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETVIEWPORTORGEX", true, false);

					oXmlWriter.WriteNodeBegin(L"Origin");
						oXmlWriter.WriteNode(L"X", (int)oOrigin.x);
						oXmlWriter.WriteNode(L"Y", (int)oOrigin.y);
						oXmlWriter.WriteNodeEnd(L"Origin");

					oXmlWriter.WriteNodeEnd(L"EMR_SETVIEWPORTORGEX");

			}
		#endif

		m_pDC->SetViewportOrigin(oOrigin);
	}
	void CEmfFile::Read_EMR_SETVIEWPORTEXTEX()
	{
		TEmfSizeL oExtent;
		m_oStream >> oExtent;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETVIEWPORTEXTEX", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETVIEWPORTEXTEX);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETVIEWPORTEXTEX", true, false);

					oXmlWriter.WriteNodeBegin(L"Extent");
						oXmlWriter.WriteNode(L"cX", (int)oExtent.cx);
						oXmlWriter.WriteNode(L"cY", (int)oExtent.cy);
						oXmlWriter.WriteNodeEnd(L"Extent");

					oXmlWriter.WriteNodeEnd(L"EMR_SETVIEWPORTEXTEX");

			}
		#endif

		m_pDC->SetViewportExtents(oExtent);
	}
	void CEmfFile::Read_EMR_SETSTRETCHBLTMODE()
	{
		unsigned int ulStretchMode;
		m_oStream >> ulStretchMode;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETSTRETCHBLTMODE", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETSTRETCHBLTMODE);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETSTRETCHBLTMODE", true, false);

					oXmlWriter.WriteNode(L"StretchMode", (int)ulStretchMode);

					oXmlWriter.WriteNodeEnd(L"EMR_SETSTRETCHBLTMODE");

			}
		#endif

		m_pDC->SetStretchMode(ulStretchMode);
	}
	void CEmfFile::Read_EMR_SETICMMODE()
	{
		unsigned int ulICMMode;
		m_oStream >> ulICMMode;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETICMMODE", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETICMMODE);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETICMMODE", true, false);

					oXmlWriter.WriteNode(L"ICMMode", (int)ulICMMode);

					oXmlWriter.WriteNodeEnd(L"EMR_SETICMMODE");

			}
		#endif

	}
	void CEmfFile::Read_EMR_CREATEDIBPATTERNBRUSHPT()
	{
		unsigned int ulBrushIndex;
		TEmfDibPatternBrush oDibBrush;
		m_oStream >> ulBrushIndex;
		m_oStream >> oDibBrush;

		BYTE* pBgraBuffer = NULL;
		unsigned int ulWidth, ulHeight;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_CREATEDIBPATTERNBRUSHPT", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_CREATEDIBPATTERNBRUSHPT);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_CREATEDIBPATTERNBRUSHPT", true, false);

					oXmlWriter.WriteNode(L"ihBrush",    (int)ulBrushIndex);
					oXmlWriter.WriteNode(L"Usage",	    (int)oDibBrush.Usage);
					oXmlWriter.WriteNode(L"offBmi",	    (int)oDibBrush.offBmi);
					oXmlWriter.WriteNode(L"cbBmi",	    (int)oDibBrush.cbBmi);
					oXmlWriter.WriteNode(L"offBits",    (int)oDibBrush.offBits);
					oXmlWriter.WriteNode(L"cbBits",	    (int)oDibBrush.cbBits);

					oXmlWriter.WriteNodeBegin(L"DeviceIndependentBitmap");

					//TODO: не хватает сохранения переменной DeviceIndependentBitmap

			}
		#endif

		if (ReadImage(oDibBrush.offBmi, oDibBrush.cbBmi, oDibBrush.offBits, oDibBrush.cbBits, sizeof(TEmfDibPatternBrush) + 12, &pBgraBuffer, &ulWidth, &ulHeight))
		{
			CEmfLogBrushEx* pBrush = new CEmfLogBrushEx();
			if (!pBrush)
				return SetError();

			pBrush->SetDibPattern(pBgraBuffer, ulWidth, ulHeight);
			m_oPlayer.RegisterObject(ulBrushIndex, (CEmfObjectBase*)pBrush);
		}

		#ifdef _DEBUG
			if (m_pOutput)
			{
					oXmlWriter.WriteNodeEnd(L"DeviceIndependentBitmap");
					oXmlWriter.WriteNodeEnd(L"EMR_CREATEDIBPATTERNBRUSHPT");

			}
		#endif

	}
	void CEmfFile::Read_EMR_SELECTCLIPPATH()
	{
		unsigned int unRegionMode;
		m_oStream >> unRegionMode;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SELECTCLIPPATH", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SELECTCLIPPATH);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SELECTCLIPPATH", true, false);

					oXmlWriter.WriteNode(L"RegionMode", (int)unRegionMode);

					oXmlWriter.WriteNodeEnd(L"EMR_SELECTCLIPPATH", true);

			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETBKCOLOR", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETBKCOLOR);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETBKCOLOR", true, false);

					oXmlWriter.WriteNodeBegin(L"Color");
						oXmlWriter.WriteNode(L"Red",	    oColor.r);
						oXmlWriter.WriteNode(L"Green",	    oColor.g);
						oXmlWriter.WriteNode(L"Blue",	    oColor.b);
						oXmlWriter.WriteNode(L"Reserved",   oColor.a);
						oXmlWriter.WriteNodeEnd(L"Color");


					oXmlWriter.WriteNodeEnd(L"EMR_SETBKCOLOR");

			}
		#endif

		m_pDC->SetBgColor(oColor);
		UpdateOutputDC();
	}
	void CEmfFile::Read_EMR_EXCLUDECLIPRECT()
	{
		// TODO: Проверить как найдется файл
		TEmfRectL oClip;
		m_oStream >> oClip;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_EXCLUDECLIPRECT", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_EXCLUDECLIPRECT);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_EXCLUDECLIPRECT", true, false);

					oXmlWriter.WriteNodeBegin(L"Clip");
						oXmlWriter.WriteNode(L"Left",	oClip.lLeft);
						oXmlWriter.WriteNode(L"Top",	oClip.lTop);
						oXmlWriter.WriteNode(L"Right",	oClip.lRight);
						oXmlWriter.WriteNode(L"Bottom", oClip.lBottom);
						oXmlWriter.WriteNodeEnd(L"Clip");

					oXmlWriter.WriteNodeEnd(L"EMR_EXCLUDECLIPRECT");

			}
		#endif

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

                #ifdef _DEBUG
                        if (m_pOutput)
                        {
                                oXmlWriter.WriteNodeBegin(L"EMR_EXTSELECTCLIPRGN", true);
                                        oXmlWriter.WriteAttribute(L"Id", EMR_EXTSELECTCLIPRGN);
                                        oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
                                        oXmlWriter.WriteNodeEnd(L"EMR_EXTSELECTCLIPRGN", true, false);

                                        oXmlWriter.WriteNode(L"RgnDataSize",    (int)ulRgnDataSize);
                                        oXmlWriter.WriteNode(L"RegionMode",     (int)ulRegionMode);

                                        //TODO: не хватает сохранения переменной RgnData, которая в реализации пропускается

                                        oXmlWriter.WriteNodeEnd(L"EMR_EXTSELECTCLIPRGN");

                        }
                #endif

		m_oStream.Skip(m_ulRecordSize - 8);

		// Тут просто сбрасываем текущий клип. Ничего не добавляем в клип, т.е. реализовать регионы с
	    // текущим интерфейсом рендерера невозможно.
		m_pDC->GetClip()->Reset();
	}
	void CEmfFile::Read_EMR_SETMETARGN()
	{
		m_pDC->GetClip()->Reset();
		UpdateOutputDC();

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETMETARGN", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETMETARGN);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETMETARGN", true, false);
					oXmlWriter.WriteNodeEnd(L"EMR_SETMETARGN");

			}
		#endif
	}		
	void CEmfFile::Read_EMR_SETROP2()
	{
		unsigned int ulRop2Mode;
		m_oStream >> ulRop2Mode;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETROP2", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETROP2);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETROP2", true, false);

					oXmlWriter.WriteNode(L"Rop2Mode", (int)ulRop2Mode);

					oXmlWriter.WriteNodeEnd(L"EMR_SETROP2");

			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_CREATEPALETTE", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_CREATEPALETTE);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_CREATEPALETTE", true, false);

					oXmlWriter.WriteNode(L"ihPal", (int)ulPaletteIndex);
					oXmlWriter.WriteNodeBegin(L"LogPalette");
						oXmlWriter.WriteNode(L"Version",	    768);
						oXmlWriter.WriteNode(L"NumberOfEntries",    (int)pPalette->NumberOfEntries);

						for(unsigned int i = 0; i < pPalette->NumberOfEntries; ++i)
						{
							oXmlWriter.WriteNodeBegin(L"LogPaletteEntry" + std::to_wstring(i + 1));
								oXmlWriter.WriteNode(L"Reserved",   pPalette->PaletteEntries[i].Reserved);
								oXmlWriter.WriteNode(L"Blue",	    pPalette->PaletteEntries[i].Blue);
								oXmlWriter.WriteNode(L"Green",	    pPalette->PaletteEntries[i].Green);
								oXmlWriter.WriteNode(L"Red",	    pPalette->PaletteEntries[i].Red);
								oXmlWriter.WriteNodeEnd(L"LogPaletteEntry" + std::to_wstring(i + 1));

						}

						oXmlWriter.WriteNodeEnd(L"LogPalette");

					oXmlWriter.WriteNodeEnd(L"EMR_CREATEPALETTE");

			}
		#endif

		m_oPlayer.RegisterObject(ulPaletteIndex, (CEmfObjectBase*)pPalette);
	}
	void CEmfFile::Read_EMR_SELECTPALETTE()
	{
		unsigned int ulIndex;
		m_oStream >> ulIndex;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SELECTPALETTE", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SELECTPALETTE);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SELECTPALETTE", true, false);

					oXmlWriter.WriteNode(L"ihPal", (int)ulIndex);

					oXmlWriter.WriteNodeEnd(L"EMR_SELECTPALETTE");

			}
		#endif

		m_oPlayer.SelectPalette(ulIndex);
	}
	void CEmfFile::Read_EMR_REALIZEPALETTE()
	{
		// TODO: Реализовать
		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_REALIZEPALETTE", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_REALIZEPALETTE);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_REALIZEPALETTE", true, true);

			}
		#endif
	}
	void CEmfFile::Read_EMR_INTERSECTCLIPRECT()
	{
		TEmfRectL oClip;
		m_oStream >> oClip;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_INTERSECTCLIPRECT", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_INTERSECTCLIPRECT);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_INTERSECTCLIPRECT", true, false);

					oXmlWriter.WriteNodeBegin(L"Clip");
						oXmlWriter.WriteNode(L"Left",	oClip.lLeft);
						oXmlWriter.WriteNode(L"Top",	oClip.lTop);
						oXmlWriter.WriteNode(L"Right",	oClip.lRight);
						oXmlWriter.WriteNode(L"Bottom", oClip.lBottom);
						oXmlWriter.WriteNodeEnd(L"Clip");

					oXmlWriter.WriteNodeEnd(L"EMR_INTERSECTCLIPRECT");

			}
		#endif

		TRectD oClipRect;
		TranslatePoint(oClip.lLeft, oClip.lTop, oClipRect.dLeft, oClipRect.dTop);
		TranslatePoint(oClip.lRight, oClip.lBottom, oClipRect.dRight, oClipRect.dBottom);
		m_pDC->GetClip()->Intersect(oClipRect);
	}
	void CEmfFile::Read_EMR_SETLAYOUT()
	{
		unsigned int ulLayoutMode;
		m_oStream >> ulLayoutMode;

                #ifdef _DEBUG
                        if (m_pOutput)
                        {
                                oXmlWriter.WriteNodeBegin(L"EMR_SETLAYOUT", true);
                                        oXmlWriter.WriteAttribute(L"Id", EMR_SETLAYOUT);
                                        oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
                                        oXmlWriter.WriteNodeEnd(L"EMR_SETLAYOUT", true, false);

                                        oXmlWriter.WriteNode(L"LayoutMode", (int)ulLayoutMode);

                                        oXmlWriter.WriteNodeEnd(L"EMR_SETLAYOUT");

                        }
                #endif

		// TODO: реализовать
	}
	void CEmfFile::Read_EMR_SETBRUSHORGEX()
	{
		TEmfPointL oOrigin;
		m_oStream >> oOrigin;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SETBRUSHORGEX", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SETBRUSHORGEX);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SETBRUSHORGEX", true, false);

					oXmlWriter.WriteNodeBegin(L"Origin");
						oXmlWriter.WriteNode(L"X",  oOrigin.x);
						oXmlWriter.WriteNode(L"Y",  oOrigin.y);
						oXmlWriter.WriteNodeEnd(L"Origin");

					oXmlWriter.WriteNodeEnd(L"EMR_SETBRUSHORGEX");

			}
		#endif

		// TODO: реализовать
	}		
	void CEmfFile::Read_EMR_ANGLEARC()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfPointL oCenter;
		unsigned int unRadius;
		double dStartAngle, dSweepAngle;
		m_oStream >> oCenter >> unRadius >> dStartAngle >> dSweepAngle;

                #ifdef _DEBUG
                        if (m_pOutput)
                        {
                                oXmlWriter.WriteNodeBegin(L"EMR_ANGLEARC", true);
                                        oXmlWriter.WriteAttribute(L"Id", EMR_ANGLEARC);
                                        oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
                                        oXmlWriter.WriteNodeEnd(L"EMR_ANGLEARC", true, false);

                                        oXmlWriter.WriteNodeBegin(L"Center");
                                                oXmlWriter.WriteNode(L"X", oCenter.x);
                                                oXmlWriter.WriteNode(L"Y", oCenter.y);
                                                oXmlWriter.WriteNodeEnd(L"Center");

                                        oXmlWriter.WriteNode(L"Radius",     (int)unRadius);
                                        oXmlWriter.WriteNode(L"StartAngle", dStartAngle);
                                        oXmlWriter.WriteNode(L"SweepAngle", dSweepAngle);

                                        oXmlWriter.WriteNodeEnd(L"EMR_ANGLEARC");

                        }
                #endif

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

                #ifdef _DEBUG
                        if (m_pOutput)
                        {
                                oXmlWriter.WriteNodeBegin(L"EMR_ARC", true);
                                        oXmlWriter.WriteAttribute(L"Id", EMR_ARC);
                                        oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
                                        oXmlWriter.WriteNodeEnd(L"EMR_ARC", true, false);

                                        oXmlWriter.WriteNodeBegin(L"Rectangle");
                                                oXmlWriter.WriteNode(L"Left",	oBox.lLeft);
                                                oXmlWriter.WriteNode(L"Top",	oBox.lTop);
                                                oXmlWriter.WriteNode(L"Right",	oBox.lRight);
                                                oXmlWriter.WriteNode(L"Bottom", oBox.lBottom);
                                                oXmlWriter.WriteNodeEnd(L"Rectangle");

                                        oXmlWriter.WriteNodeBegin(L"StartPoint");
                                                oXmlWriter.WriteNode(L"X",  oStart.x);
                                                oXmlWriter.WriteNode(L"Y",  oStart.y);
                                                oXmlWriter.WriteNodeEnd(L"StartPoint");

                                        oXmlWriter.WriteNodeBegin(L"EndPoint");
                                                oXmlWriter.WriteNode(L"X",  oEnd.x);
                                                oXmlWriter.WriteNode(L"Y",  oEnd.y);
                                                oXmlWriter.WriteNodeEnd(L"EndPoint");

                                        oXmlWriter.WriteNodeEnd(L"EMR_ARC");
                        }
                #endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_ARCTO", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_ARCTO);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_ARCTO", true, false);

					oXmlWriter.WriteNodeBegin(L"Rectangle");
						oXmlWriter.WriteNode(L"Left",	oBox.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBox.lTop);
						oXmlWriter.WriteNode(L"Right",	oBox.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBox.lBottom);
						oXmlWriter.WriteNodeEnd(L"Rectangle");

					oXmlWriter.WriteNodeBegin(L"StartPoint");
						oXmlWriter.WriteNode(L"X",  oStart.x);
						oXmlWriter.WriteNode(L"Y",  oStart.y);
						oXmlWriter.WriteNodeEnd(L"StartPoint");

					oXmlWriter.WriteNodeBegin(L"EndPoint");
						oXmlWriter.WriteNode(L"X",  oEnd.x);
						oXmlWriter.WriteNode(L"Y",  oEnd.y);
						oXmlWriter.WriteNodeEnd(L"EndPoint");

					oXmlWriter.WriteNodeEnd(L"EMR_ARCTO");
			}
		#endif

		ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweep);
	}
	void CEmfFile::Read_EMR_CHORD()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfRectL oBox;
		TEmfPointL oStart, oEnd;
		double dStartAngle, dSweep;
		Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

                #ifdef _DEBUG
                        if (m_pOutput)
                        {
                                oXmlWriter.WriteNodeBegin(L"EMR_CHORD", true);
                                        oXmlWriter.WriteAttribute(L"Id", EMR_CHORD);
                                        oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
                                        oXmlWriter.WriteNodeEnd(L"EMR_CHORD", true, false);

                                        oXmlWriter.WriteNodeBegin(L"Rectangle");
                                                oXmlWriter.WriteNode(L"Left",	oBox.lLeft);
                                                oXmlWriter.WriteNode(L"Top",	oBox.lTop);
                                                oXmlWriter.WriteNode(L"Right",	oBox.lRight);
                                                oXmlWriter.WriteNode(L"Bottom", oBox.lBottom);
                                                oXmlWriter.WriteNodeEnd(L"Rectangle");

                                        oXmlWriter.WriteNodeBegin(L"StartPoint");
                                                oXmlWriter.WriteNode(L"X",  oStart.x);
                                                oXmlWriter.WriteNode(L"Y",  oStart.y);
                                                oXmlWriter.WriteNodeEnd(L"StartPoint");

                                        oXmlWriter.WriteNodeBegin(L"EndPoint");
                                                oXmlWriter.WriteNode(L"X",  oEnd.x);
                                                oXmlWriter.WriteNode(L"Y",  oEnd.y);
                                                oXmlWriter.WriteNodeEnd(L"EndPoint");

                                        oXmlWriter.WriteNodeEnd(L"EMR_CHORD");
                        }
                #endif

		MoveTo(oStart);
		ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, dStartAngle, dSweep);
		LineTo(oStart);
		DrawPath(true, true);
	}
	void CEmfFile::Read_EMR_ELLIPSE()
	{
		TEmfRectL oBox;
		m_oStream >> oBox;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_ELLIPSE", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_ELLIPSE);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_ELLIPSE", true, false);

					oXmlWriter.WriteNodeBegin(L"Rectangle");
						oXmlWriter.WriteNode(L"Left",	oBox.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBox.lTop);
						oXmlWriter.WriteNode(L"Right",	oBox.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBox.lBottom);
						oXmlWriter.WriteNodeEnd(L"Rectangle");

					oXmlWriter.WriteNodeEnd(L"EMR_ELLIPSE");
			}
		#endif

		if (m_pDC->GetArcDirection() == AD_COUNTERCLOCKWISE)
		    ArcTo(oBox.lLeft, oBox.lTop, oBox.lRight, oBox.lBottom, 0, 360);
		else
		    ArcTo(oBox.lLeft, oBox.lBottom, oBox.lRight, oBox.lTop, 0, 360);


		DrawPath(true, true);
	}
	void CEmfFile::Read_EMR_EXTTEXTOUTA()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfExtTextoutA oText;
		m_oStream >> oText;	

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_EXTTEXTOUTA", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_EXTTEXTOUTA);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_EXTTEXTOUTA", true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oText.Bounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oText.Bounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oText.Bounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oText.Bounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNode(L"GraphicsMode",	(int)oText.iGraphicsMode);
					oXmlWriter.WriteNode(L"exScale",	oText.exScale);
					oXmlWriter.WriteNode(L"eyScale",	oText.eyScale);

					if (NULL != oText.aEmrText.OutputString)
					{
						oXmlWriter.WriteNodeBegin(L"EmrText");
							oXmlWriter.WriteNodeBegin(L"Reference");
								oXmlWriter.WriteNode(L"X", oText.aEmrText.Reference.x);
								oXmlWriter.WriteNode(L"Y", oText.aEmrText.Reference.y);
								oXmlWriter.WriteNodeEnd(L"Reference");

							oXmlWriter.WriteNode(L"CountChars", (int)oText.aEmrText.Chars);
							oXmlWriter.WriteNode(L"offString",  (int)oText.aEmrText.offString);
							oXmlWriter.WriteNode(L"Options",    (int)oText.aEmrText.Options);

							oXmlWriter.WriteNodeBegin(L"Rectangle");
								oXmlWriter.WriteNode(L"Left",	oText.aEmrText.Rectangle.lLeft);
								oXmlWriter.WriteNode(L"Top",	oText.aEmrText.Rectangle.lTop);
								oXmlWriter.WriteNode(L"Right",	oText.aEmrText.Rectangle.lRight);
								oXmlWriter.WriteNode(L"Bottom", oText.aEmrText.Rectangle.lBottom);
								oXmlWriter.WriteNodeEnd(L"Rectangle");

							oXmlWriter.WriteNode(L"offDx", (int)oText.aEmrText.offDx);
							//TODO: сделать сохранение DX
							//возможно текст нужно сохранять иначе (сохранение в DrawTextA)
					}
			    }
		#endif

		DrawTextA(oText.aEmrText, oText.iGraphicsMode);

		#ifdef _DEBUG
			if (m_pOutput)
			{
				if (oText.aEmrText.OutputString != NULL)
					oXmlWriter.WriteNodeEnd(L"EmrText");

				oXmlWriter.WriteNodeEnd(L"EMR_EXTTEXTOUTA");
			}
		#endif
	}
	void CEmfFile::Read_EMR_EXTTEXTOUTW()
	{
		TEmfExtTextoutW oText;
		m_oStream >> oText;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_EXTTEXTOUTW", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_EXTTEXTOUTW);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_EXTTEXTOUTW", true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oText.Bounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oText.Bounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oText.Bounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oText.Bounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNode(L"GraphicsMode",	(int)oText.iGraphicsMode);
					oXmlWriter.WriteNode(L"exScale",	oText.exScale);
					oXmlWriter.WriteNode(L"eyScale",	oText.eyScale);

					if (oText.wEmrText.OutputString != NULL)
					{
						oXmlWriter.WriteNodeBegin(L"EmrText");
							oXmlWriter.WriteNodeBegin(L"Reference");
								oXmlWriter.WriteNode(L"X", oText.wEmrText.Reference.x);
								oXmlWriter.WriteNode(L"Y", oText.wEmrText.Reference.y);
								oXmlWriter.WriteNodeEnd(L"Reference");

							oXmlWriter.WriteNode(L"CountChars", (int)oText.wEmrText.Chars);
							oXmlWriter.WriteNode(L"offString",  (int)oText.wEmrText.offString);
							oXmlWriter.WriteNode(L"Options",    (int)oText.wEmrText.Options);

							oXmlWriter.WriteNodeBegin(L"Rectangle");
								oXmlWriter.WriteNode(L"Left",	oText.wEmrText.Rectangle.lLeft);
								oXmlWriter.WriteNode(L"Top",	oText.wEmrText.Rectangle.lTop);
								oXmlWriter.WriteNode(L"Right",	oText.wEmrText.Rectangle.lRight);
								oXmlWriter.WriteNode(L"Bottom", oText.wEmrText.Rectangle.lBottom);
								oXmlWriter.WriteNodeEnd(L"Rectangle");

							oXmlWriter.WriteNode(L"offDx", (int)oText.wEmrText.offDx);
							//TODO: сделать сохранение DX
							//возможно текст нужно сохранять иначе (сохранение в DrawTextW)
					}
			    }
		#endif

		DrawTextW(oText.wEmrText, oText.iGraphicsMode);

		#ifdef _DEBUG
			if (m_pOutput)
			{
				if (oText.wEmrText.OutputString != NULL)
					oXmlWriter.WriteNodeEnd(L"EmrText");

				oXmlWriter.WriteNodeEnd(L"EMR_EXTTEXTOUTW");
			}
		#endif
	}
	void CEmfFile::Read_EMR_LINETO()
	{
		TEmfPointL oPoint;
		m_oStream >> oPoint;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_LINETO", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_LINETO);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_LINETO", true, false);

					oXmlWriter.WriteNode(L"X",  oPoint.x);
					oXmlWriter.WriteNode(L"Y",  oPoint.y);

					oXmlWriter.WriteNodeEnd(L"EMR_LINETO");

			    }
		#endif

		LineTo(oPoint);
	}
	void CEmfFile::Read_EMR_PIE()
	{
		// TODO: Как найдутся файлы проверить данную запись.
		TEmfRectL oBox;
		TEmfPointL oStart, oEnd;
		double dStartAngle, dSweep;
		Read_EMR_ARC_BASE(oBox, oStart, oEnd, dStartAngle, dSweep);

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_PIE", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_PIE);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_PIE", true, false);

					oXmlWriter.WriteNodeBegin(L"Rectangle");
						oXmlWriter.WriteNode(L"Left",	oBox.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBox.lTop);
						oXmlWriter.WriteNode(L"Right",	oBox.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBox.lBottom);
						oXmlWriter.WriteNodeEnd(L"Rectangle");

					oXmlWriter.WriteNodeBegin(L"StartPoint");
						oXmlWriter.WriteNode(L"X",  oStart.x);
						oXmlWriter.WriteNode(L"Y",  oStart.y);
						oXmlWriter.WriteNodeEnd(L"StartPoint");

					oXmlWriter.WriteNodeBegin(L"EndPoint");
						oXmlWriter.WriteNode(L"X",  oEnd.x);
						oXmlWriter.WriteNode(L"Y",  oEnd.y);
						oXmlWriter.WriteNodeEnd(L"EndPoint");

					oXmlWriter.WriteNodeEnd(L"EMR_PIE");
			}
		#endif

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

		#ifdef _DEBUG
			std::wstring sName;
			unsigned int unId;

			if (m_pOutput)
			{
				if (typeid (T).name() == "TEmfPointL")
				{
					sName = L"EMR_POLYBEZIER";
					unId = EMR_POLYBEZIER;
				}
				else
				{
					sName = L"EMR_POLYBEZIER16";
					unId = EMR_POLYBEZIER16;
				}

				oXmlWriter.WriteNodeBegin(sName, true);
					oXmlWriter.WriteAttribute(L"Id",    (int)unId);
					oXmlWriter.WriteAttribute(L"Size",  (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(sName, true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oBounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oBounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNode(L"NumberPoints", (int)ulCount);

					oXmlWriter.WriteNodeBegin(L"Point1");
						oXmlWriter.WriteNode(L"X", oStartPoint.x);
						oXmlWriter.WriteNode(L"Y", oStartPoint.y);
						oXmlWriter.WriteNodeEnd(L"Point1");
			}
		#endif

		T oPoint1, oPoint2, oPointE;
		for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex += 3)
		{
			m_oStream >> oPoint1 >> oPoint2 >> oPointE;

			#ifdef _DEBUG
				if (m_pOutput)
				{
					oXmlWriter.WriteNodeBegin(L"Point" + std::to_wstring(ulIndex + 1));
						oXmlWriter.WriteNode(L"X", oPoint1.x);
						oXmlWriter.WriteNode(L"Y", oPoint1.y);
						oXmlWriter.WriteNodeEnd(L"Point" + std::to_wstring(ulIndex + 1));

					oXmlWriter.WriteNodeBegin(L"Point" + std::to_wstring(ulIndex + 2));
						oXmlWriter.WriteNode(L"X", oPoint2.x);
						oXmlWriter.WriteNode(L"Y", oPoint2.y);
						oXmlWriter.WriteNodeEnd(L"Point" + std::to_wstring(ulIndex + 2));

					oXmlWriter.WriteNodeBegin(L"Point" + std::to_wstring(ulIndex + 3));
						oXmlWriter.WriteNode(L"X", oPointE.x);
						oXmlWriter.WriteNode(L"Y", oPointE.y);
						oXmlWriter.WriteNodeEnd(L"Point" + std::to_wstring(ulIndex + 3));

				}
			#endif

			CurveTo(oPoint1, oPoint2, oPointE);
		}

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeEnd(sName);
			}
		#endif

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

		#ifdef _DEBUG
			std::wstring sName;
			unsigned int unId;

			if (m_pOutput)
			{
				if (typeid (T).name() == "TEmfPointL")
				{
					sName = L"EMR_POLYBEZIERTO";
					unId = EMR_POLYBEZIERTO;
				}
				else
				{
					sName = L"EMR_POLYBEZIERTO16";
					unId = EMR_POLYBEZIERTO16;
				}

				oXmlWriter.WriteNodeBegin(sName, true);
					oXmlWriter.WriteAttribute(L"Id",    (int)unId);
					oXmlWriter.WriteAttribute(L"Size",  (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(sName, true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oBounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oBounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNode(L"NumberPoints", (int)ulCount);

			}
		#endif

		for (unsigned int ulIndex = 0; ulIndex < ulCount; ulIndex += 3)
		{
			if (ulCount - ulIndex < 2)
				return SetError();

			T oPoint1, oPoint2, oPointE;
			m_oStream >> oPoint1 >> oPoint2 >> oPointE;

			#ifdef _DEBUG
				if (m_pOutput)
				{
					oXmlWriter.WriteNodeBegin(L"Point" + std::to_wstring(ulIndex + 1));
						oXmlWriter.WriteNode(L"X", oPoint1.x);
						oXmlWriter.WriteNode(L"Y", oPoint1.y);
						oXmlWriter.WriteNodeEnd(L"Point" + std::to_wstring(ulIndex + 1));

					oXmlWriter.WriteNodeBegin(L"Point" + std::to_wstring(ulIndex + 2));
						oXmlWriter.WriteNode(L"X", oPoint2.x);
						oXmlWriter.WriteNode(L"Y", oPoint2.y);
						oXmlWriter.WriteNodeEnd(L"Point" + std::to_wstring(ulIndex + 2));

					oXmlWriter.WriteNodeBegin(L"Point" + std::to_wstring(ulIndex + 3));
						oXmlWriter.WriteNode(L"X", oPointE.x);
						oXmlWriter.WriteNode(L"Y", oPointE.y);
						oXmlWriter.WriteNodeEnd(L"Point" + std::to_wstring(ulIndex + 3));

				}
			#endif

			CurveTo(oPoint1, oPoint2, oPointE);
		}

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeEnd(sName);
			}
		#endif
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

		#ifdef _DEBUG
			std::wstring sName;
			unsigned int unId;

			if (m_pOutput)
			{
				if (typeid (T).name() == "TEmfPointL")
				{
					sName = L"EMR_POLYDRAW";
					unId = EMR_POLYDRAW;
				}
				else
				{
					sName = L"EMR_POLYDRAW16";
					unId = EMR_POLYDRAW16;
				}

				oXmlWriter.WriteNodeBegin(sName, true);
					oXmlWriter.WriteAttribute(L"Id",    (int)unId);
					oXmlWriter.WriteAttribute(L"Size",  (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(sName, true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oBounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oBounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNode(L"NumberPoints", (int)unCount);

			}
		#endif

		for (unsigned int unIndex = 0; unIndex < unCount; unIndex++)
		{
			m_oStream >> pPoints[unIndex];
			#ifdef _DEBUG
				if (m_pOutput)
				{
					oXmlWriter.WriteNodeBegin(L"Point" + std::to_wstring(unIndex + 1));
						oXmlWriter.WriteNode(L"X", pPoints[unIndex].x);
						oXmlWriter.WriteNode(L"Y", pPoints[unIndex].y);
						oXmlWriter.WriteNodeEnd(L"Point" + std::to_wstring(unIndex + 1));
				}
			#endif
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

			#ifdef _DEBUG
				if (m_pOutput)
					oXmlWriter.WriteNode(L"abTypes" + std::to_wstring(unIndex + 1), pAbTypes[unIndex]);
			#endif
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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeEnd(sName);
			}
		#endif

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

		#ifdef _DEBUG
			std::wstring sName;
			unsigned int unId;

			if (m_pOutput)
			{
				if (typeid (T).name() == "TEmfPointL")
				{
					sName = L"EMR_POLYGON";
					unId = EMR_POLYGON;
				}
				else
				{
					sName = L"EMR_POLYGON16";
					unId = EMR_POLYGON16;
				}

				oXmlWriter.WriteNodeBegin(sName, true);
					oXmlWriter.WriteAttribute(L"Id",    (int)unId);
					oXmlWriter.WriteAttribute(L"Size",  (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(sName, true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oBounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oBounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNode(L"NumberPoints", (int)ulCount);

					oXmlWriter.WriteNodeBegin(L"Point1");
					    oXmlWriter.WriteNode(L"X",  oPoint.x);
					    oXmlWriter.WriteNode(L"Y",  oPoint.y);
					    oXmlWriter.WriteNodeEnd(L"Point1");

			}
		#endif

		MoveTo(oPoint);


		for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex++)
		{
			m_oStream >> oPoint;
			LineTo(oPoint);

			#ifdef _DEBUG
				if (m_pOutput)
				{
					const std::wstring sNamePoint = L"Point" + std::to_wstring(ulIndex + 1);
					oXmlWriter.WriteNodeBegin(sNamePoint);
					    oXmlWriter.WriteNode(L"X",  oPoint.x);
					    oXmlWriter.WriteNode(L"Y",  oPoint.y);
					    oXmlWriter.WriteNodeEnd(sNamePoint);
				}
			#endif
		}

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeEnd(sName);
			}
		#endif

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

		#ifdef _DEBUG
			std::wstring sName;
			unsigned int unId;

			if (m_pOutput)
			{
				if (typeid (T).name() == "TEmfPointL")
				{
					sName = L"EMR_POLYLINE";
					unId = EMR_POLYLINE;
				}
				else
				{
					sName = L"EMR_POLYLINE16";
					unId = EMR_POLYLINE16;
				}

				oXmlWriter.WriteNodeBegin(sName, true);
					oXmlWriter.WriteAttribute(L"Id",    (int)unId);
					oXmlWriter.WriteAttribute(L"Size",  (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(sName, true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oBounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oBounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNode(L"NumberPoints", (int)ulCount);

					oXmlWriter.WriteNodeBegin(L"Point1");
					    oXmlWriter.WriteNode(L"X",  oPoint.x);
					    oXmlWriter.WriteNode(L"Y",  oPoint.y);
					    oXmlWriter.WriteNodeEnd(L"Point1");

			    }
		#endif

		for (unsigned int ulIndex = 1; ulIndex < ulCount; ulIndex++)
		{
			m_oStream >> oPoint;
			LineTo(oPoint);

			#ifdef _DEBUG
				if (m_pOutput)
				{
					const std::wstring sNamePoint = L"Point" + std::to_wstring(ulIndex + 1);
					oXmlWriter.WriteNodeBegin(sNamePoint);
					    oXmlWriter.WriteNode(L"X",  oPoint.x);
					    oXmlWriter.WriteNode(L"Y",  oPoint.y);
					    oXmlWriter.WriteNodeEnd(sNamePoint);
				}
			#endif
		}

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeEnd(sName);
			}
		#endif

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

		#ifdef _DEBUG
			std::wstring sName;
			unsigned int unId;

			if (m_pOutput)
			{
				if (typeid (T).name() == "TEmfPointL")
				{
					sName = L"EMR_POLYLINETO";
					unId = EMR_POLYLINETO;
				}
				else
				{
					sName = L"EMR_POLYLINETO16";
					unId = EMR_POLYLINETO16;
				}

				oXmlWriter.WriteNodeBegin(sName, true);
					oXmlWriter.WriteAttribute(L"Id",    (int)unId);
					oXmlWriter.WriteAttribute(L"Size",  (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(sName, true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oBounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oBounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNode(L"NumberPoints", (int)ulCount);
			    }
		#endif

		for (unsigned int ulIndex = 0; ulIndex < ulCount; ulIndex++)
		{
			T oPoint;
			m_oStream >> oPoint;
			LineTo(oPoint);

			#ifdef _DEBUG
				if (m_pOutput)
				{
					const std::wstring sNamePoint = L"Point" + std::to_wstring(ulIndex + 1);
					oXmlWriter.WriteNodeBegin(sNamePoint);
						oXmlWriter.WriteNode(L"X",  oPoint.x);
						oXmlWriter.WriteNode(L"Y",  oPoint.y);
						oXmlWriter.WriteNodeEnd(sNamePoint);
				}
			#endif
		}

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeEnd(sName);
			}
		#endif
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

		#ifdef _DEBUG
			std::wstring sName;
			unsigned int unId;

			if (m_pOutput)
			{
				if (typeid (T).name() == "TEmfPointL")
				{
					sName = L"EMR_POLYPOLYGON";
					unId = EMR_POLYPOLYGON;
				}
				else
				{
					sName = L"EMR_POLYPOLYGON16";
					unId = EMR_POLYPOLYGON16;
				}

				oXmlWriter.WriteNodeBegin(sName, true);
					oXmlWriter.WriteAttribute(L"Id",    (int)unId);
					oXmlWriter.WriteAttribute(L"Size",  (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(sName, true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oBounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oBounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNode(L"NumberOfPolygons",   (int)ulNumberOfPolygons);					oXmlWriter.WriteNode(L"NumberOfPolygons", (int)ulNumberOfPolygons);
					oXmlWriter.WriteNode(L"Count",		    (int)ulTotalPointsCount);

			    }
		#endif

		for (unsigned int ulIndex = 0; ulIndex < ulNumberOfPolygons; ulIndex++)
		{
			m_oStream >> pPolygonPointCount[ulIndex];

			#ifdef _DEBUG
				if (m_pOutput)
					oXmlWriter.WriteNode(L"PolygonPointCount" + std::to_wstring(ulIndex + 1), (int)pPolygonPointCount[ulIndex]);
			#endif
		}

                #ifdef _DEBUG
                        unsigned int unIndexPoint = 0;
                #endif

		for (unsigned int ulPolygonIndex = 0, unStartPointIndex = 0; ulPolygonIndex < ulNumberOfPolygons; ulPolygonIndex++)
		{
			unsigned int ulCurrentPolygonPointsCount = pPolygonPointCount[ulPolygonIndex];
			if (0 == ulCurrentPolygonPointsCount)
				continue;

			T oPoint;
			m_oStream >> oPoint;

			#ifdef _DEBUG
				if (m_pOutput)
				{
					++unIndexPoint;
					oXmlWriter.WriteNodeBegin(L"Point" + std::to_wstring(unIndexPoint));
						oXmlWriter.WriteNode(L"X",  oPoint.x);
						oXmlWriter.WriteNode(L"Y",  oPoint.y);
						oXmlWriter.WriteNodeEnd(L"Point" + std::to_wstring(unIndexPoint));
				}
			#endif

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

				#ifdef _DEBUG
					if (m_pOutput)
					{
						++unIndexPoint;
						oXmlWriter.WriteNodeBegin(L"Point" + std::to_wstring(unIndexPoint));
							oXmlWriter.WriteNode(L"X",  oPoint.x);
							oXmlWriter.WriteNode(L"Y",  oPoint.y);
							oXmlWriter.WriteNodeEnd(L"Point" + std::to_wstring(unIndexPoint));
					}
				#endif
				LineTo(oPoint);
			}

			ClosePath();
		}

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeEnd(sName);
			}
		#endif

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


		#ifdef _DEBUG
			std::wstring sName;
			unsigned int unId;

			if (m_pOutput)
			{
				if (typeid (T).name() == "TEmfPointL")
				{
					sName = L"EMR_POLYPOLYLINE";
					unId = EMR_POLYPOLYLINE;
				}
				else
				{
					sName = L"EMR_POLYPOLYLINE16";
					unId = EMR_POLYPOLYLINE16;
				}

				oXmlWriter.WriteNodeBegin(sName, true);
					oXmlWriter.WriteAttribute(L"Id",    (int)unId);
					oXmlWriter.WriteAttribute(L"Size",  (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(sName, true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oBounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oBounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNode(L"NumberOfPolylines",  (int)ulNumberOfPolylines);
					oXmlWriter.WriteNode(L"Count",		    (int)ulTotalPointsCount);

			    }
		#endif

		unsigned int* pPolylinePointCount = new unsigned int[ulNumberOfPolylines];
		for (unsigned int ulIndex = 0; ulIndex < ulNumberOfPolylines; ulIndex++)
		{
			m_oStream >> pPolylinePointCount[ulIndex];

			#ifdef _DEBUG
				if (m_pOutput)
					oXmlWriter.WriteNode(L"PolylinePointCount" + std::to_wstring(ulIndex + 1), (int)pPolylinePointCount[ulIndex]);
			#endif
		}

                #ifdef _DEBUG
                        unsigned int unIndexPoint = 0;
                #endif

		for (unsigned int ulPolyIndex = 0, ulStartPointIndex = 0; ulPolyIndex < ulNumberOfPolylines; ulPolyIndex++)
		{
			unsigned int ulCurrentPolylinePointsCount = pPolylinePointCount[ulPolyIndex];
			if (0 == ulCurrentPolylinePointsCount)
				continue;

			T oPoint;
			m_oStream >> oPoint;

			#ifdef _DEBUG
				if (m_pOutput)
				{
					++unIndexPoint;
					oXmlWriter.WriteNodeBegin(L"Point" + std::to_wstring(unIndexPoint));
						oXmlWriter.WriteNode(L"X",  oPoint.x);
						oXmlWriter.WriteNode(L"Y",  oPoint.y);
						oXmlWriter.WriteNodeEnd(L"Point" + std::to_wstring(unIndexPoint));
				}
			#endif

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

				#ifdef _DEBUG
					if (m_pOutput)
					{
						++unIndexPoint;
						oXmlWriter.WriteNodeBegin(L"Point" + std::to_wstring(unIndexPoint));
							oXmlWriter.WriteNode(L"X",  oPoint.x);
							oXmlWriter.WriteNode(L"Y",  oPoint.y);
							oXmlWriter.WriteNodeEnd(L"Point" + std::to_wstring(unIndexPoint));
					}
				#endif

				LineTo(oPoint);
			}				
		}

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeEnd(sName);
			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_RECTANGLE", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_RECTANGLE);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_RECTANGLE", true, false);

					oXmlWriter.WriteNodeBegin(L"Rectangle");
						oXmlWriter.WriteNode(L"Left",	oBox.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBox.lTop);
						oXmlWriter.WriteNode(L"Right",	oBox.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBox.lBottom);
						oXmlWriter.WriteNodeEnd(L"Rectangle");

					oXmlWriter.WriteNodeEnd(L"EMR_RECTANGLE");
			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_ROUNDRECT", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_ROUNDRECT);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_ROUNDRECT", true, false);

					oXmlWriter.WriteNodeBegin(L"Rectangle");
						oXmlWriter.WriteNode(L"Left",	oBox.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBox.lTop);
						oXmlWriter.WriteNode(L"Right",	oBox.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBox.lBottom);
						oXmlWriter.WriteNodeEnd(L"Rectangle");

					oXmlWriter.WriteNodeBegin(L"Corner");
						oXmlWriter.WriteNode(L"cX", (int)oCorner.cx);
						oXmlWriter.WriteNode(L"cY", (int)oCorner.cy);
						oXmlWriter.WriteNodeEnd(L"Corner");

					oXmlWriter.WriteNodeEnd(L"EMR_ROUNDRECT");
			}
		#endif


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

                #ifdef _DEBUG
                        if (m_pOutput)
                        {
                                oXmlWriter.WriteNodeBegin(L"EMR_SETPIXELV", true);
                                        oXmlWriter.WriteAttribute(L"Id", EMR_SETPIXELV);
                                        oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
                                        oXmlWriter.WriteNodeEnd(L"EMR_SETPIXELV", true, false);

                                        oXmlWriter.WriteNodeBegin(L"Pixel");
                                            oXmlWriter.WriteNode(L"X", oPoint.x);
                                            oXmlWriter.WriteNode(L"Y", oPoint.x);
                                            oXmlWriter.WriteNodeEnd(L"Pixel");

                                        oXmlWriter.WriteNodeBegin(L"Color");
                                            oXmlWriter.WriteNode(L"Red",        oColor.r);
                                            oXmlWriter.WriteNode(L"Green",      oColor.g);
                                            oXmlWriter.WriteNode(L"Blue",       oColor.b);
                                            oXmlWriter.WriteNode(L"Reserved",   oColor.a);
                                            oXmlWriter.WriteNodeEnd(L"Color");

                                        oXmlWriter.WriteNodeEnd(L"EMR_SETPIXELV");

                        }
                #endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_SMALLTEXTOUT", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_SMALLTEXTOUT);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_SMALLTEXTOUT", true, false);

					oXmlWriter.WriteNode(L"X",	oText.x);
					oXmlWriter.WriteNode(L"X",	oText.y);
					oXmlWriter.WriteNode(L"cChars", (int)oText.cChars);
					oXmlWriter.WriteNode(L"fuOptions", (int)oText.fuOptions);
					oXmlWriter.WriteNode(L"iGraphicsMode", (int)oText.iGraphicsMode);
					oXmlWriter.WriteNode(L"exScale", oText.exScale);
					oXmlWriter.WriteNode(L"eyScale", oText.eyScale);

					if (!(oText.fuOptions & ETO_NO_RECT))
					{
						oXmlWriter.WriteNodeBegin(L"Bounds");
							oXmlWriter.WriteNode(L"Left",	oText.Bounds.lLeft);
							oXmlWriter.WriteNode(L"Top",	oText.Bounds.lTop);
							oXmlWriter.WriteNode(L"Right",	oText.Bounds.lRight);
							oXmlWriter.WriteNode(L"Bottom", oText.Bounds.lBottom);
							oXmlWriter.WriteNodeEnd(L"Bounds");
					}

			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeEnd(L"EMR_SMALLTEXTOUT");
			}
		#endif

		// Поскольку мы просто скопировали ссылку на строку, а не скопировали сами строку обнуляем здесь, потому 
		// что на деструкторе структуры освобождается память.
		oEmrText.OutputString = NULL;
	}
	void CEmfFile::Read_EMR_STROKEANDFILLPATH()
	{
		TEmfRectL oBounds;
		m_oStream >> oBounds;

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_STROKEANDFILLPATH", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_STROKEANDFILLPATH);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_STROKEANDFILLPATH", true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oBounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oBounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNodeEnd(L"EMR_STROKEANDFILLPATH");
			}
		#endif

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

		#ifdef _DEBUG
			if (m_pOutput)
			{
				oXmlWriter.WriteNodeBegin(L"EMR_STROKEPATH", true);
					oXmlWriter.WriteAttribute(L"Id", EMR_STROKEPATH);
					oXmlWriter.WriteAttribute(L"Size", (int)m_ulRecordSize);
					oXmlWriter.WriteNodeEnd(L"EMR_STROKEPATH", true, false);

					oXmlWriter.WriteNodeBegin(L"Bounds");
						oXmlWriter.WriteNode(L"Left",	oBounds.lLeft);
						oXmlWriter.WriteNode(L"Top",	oBounds.lTop);
						oXmlWriter.WriteNode(L"Right",	oBounds.lRight);
						oXmlWriter.WriteNode(L"Bottom", oBounds.lBottom);
						oXmlWriter.WriteNodeEnd(L"Bounds");

					oXmlWriter.WriteNodeEnd(L"EMR_STROKEPATH");
			}
		#endif

		if (m_pOutput && m_pPath)
		{
			m_pPath->Draw(m_pOutput, true, false);
			RELEASEOBJECT(m_pPath);
		}
	}
}
