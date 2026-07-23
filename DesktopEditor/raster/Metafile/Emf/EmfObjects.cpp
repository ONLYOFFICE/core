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
#include "../../../raster/ImageFileFormatChecker.h"

#include "../Common/MetaFileUtils.h"

#include "EmfObjects.h"

#include "../../BgraFrame.h"

namespace MetaFile
{
	CEmfObjectBase::CEmfObjectBase()
	{}

	CEmfObjectBase::~CEmfObjectBase()
	{}

	EEmfObjectType CEmfObjectBase::GetType() const
	{
		return EMF_OBJECT_UNKNOWN;
	}

	CEmfLogBrushEx::CEmfLogBrushEx() 
		: unBrushStyle(BS_SOLID), oColor(255, 255, 255, 255), unBrushHatch(HS_HORIZONTAL), pDibBuffer(NULL), unDibWidth(0), unDibHeigth(0)
	{}

	CEmfLogBrushEx::~CEmfLogBrushEx()
	{
		if (BS_DIBPATTERN == unBrushStyle && !wsDibPatternPath.empty())
			NSFile::CFileBinary::Remove(wsDibPatternPath);

		RELEASEOBJECT(pDibBuffer);
	}
	
	EEmfObjectType CEmfLogBrushEx::GetType() const
	{
		return EMF_OBJECT_BRUSH;
	}

	void CEmfLogBrushEx::SetDibPattern(unsigned char* pBuffer, unsigned int ulWidth, unsigned int ulHeight)
	{
		pDibBuffer = pBuffer;
		unDibWidth  = ulWidth;
		unDibHeigth = ulHeight;		

		if (ulWidth <= 0 || ulHeight <= 0)
			return;

	#ifdef METAFILE_DISABLE_FILESYSTEM
		// without using the file system, it's not yet possible to implement raster conversion,
		// since currently the brush image can only be passed to the renderer using the file system
		// (CMetaFileRenderer::UpdateBrush()) m_pRenderer->put_BrushTexturePath(pBrush->GetDibPatterPath());
	#else
		std::wstring wsTempFileName = GetTempFilename();
		if (wsTempFileName.empty())
			return;

		CBgraFrame oBgraFrame;

		oBgraFrame.put_Data(pBuffer);
		oBgraFrame.put_Width(ulWidth);
		oBgraFrame.put_Height(ulHeight);

		if (oBgraFrame.SaveFile(wsTempFileName, _CXIMAGE_FORMAT_PNG))
		{
			unBrushStyle     = BS_DIBPATTERN;
			wsDibPatternPath = wsTempFileName;
		}

		oBgraFrame.put_Data(NULL);
	#endif
	}

	int CEmfLogBrushEx::GetColor() const
	{
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b, 0);
	}

	int CEmfLogBrushEx::GetColor2() const
	{
		return 0;
	}

	unsigned int CEmfLogBrushEx::GetStyle() const
	{
		return unBrushStyle;
	}

	unsigned int CEmfLogBrushEx::GetStyleEx() const
	{
		return 0;
	}

	unsigned int CEmfLogBrushEx::GetHatch() const
	{
		return unBrushHatch;
	}

	unsigned int CEmfLogBrushEx::GetAlpha() const
	{
		return 0xff;
	}

	unsigned int CEmfLogBrushEx::GetAlpha2() const
	{
		return 0xff;
	}

	std::wstring CEmfLogBrushEx::GetDibPatterPath() const
	{
		return wsDibPatternPath;
	}

	void CEmfLogBrushEx::GetBounds(double &left, double &top, double &width, double &height) const
	{}

	void CEmfLogBrushEx::GetCenterPoint(double &dX, double &dY) const
	{}

	void CEmfLogBrushEx::GetDibPattern(unsigned char **pBuffer, unsigned int &unWidth, unsigned int &unHeight) const
	{
		*pBuffer = pDibBuffer;
		unWidth  = unDibWidth;
		unHeight = unDibHeigth;
	}

	void CEmfLogBrushEx::GetGradientColors(std::vector<long>& arColors, std::vector<double>& arPositions) const
	{
		arColors    = {(long)(GetColor()  + (GetAlpha()  << 24)), (long)(GetColor2() + (GetAlpha2() << 24))};
		arPositions = {0., 1.};
	}

	CEmfLogFont::CEmfLogFont(bool bFixedLength)
	    : m_bFixedLength(bFixedLength)
	{
		oDesignVector.pValues = NULL;

		oLogFontEx.oLogFont.nHeight           = DEFAULT_FONT_SIZE;
		oLogFontEx.oLogFont.nWidth            = 0;
		oLogFontEx.oLogFont.nEscapement       = 0;
		oLogFontEx.oLogFont.nOrientation      = 0;
		oLogFontEx.oLogFont.nWeight           = 400;
		oLogFontEx.oLogFont.uchItalic         = 0x00;
		oLogFontEx.oLogFont.uchUnderline      = 0x00;
		oLogFontEx.oLogFont.uchStrikeOut      = 0x00;
		oLogFontEx.oLogFont.uchCharSet        = 0x01;
		oLogFontEx.oLogFont.uchOutPrecision   = 0x00;
		oLogFontEx.oLogFont.uchClipPrecision  = 0x00;
		oLogFontEx.oLogFont.uchQuality        = 0x00;
		oLogFontEx.oLogFont.uchPitchAndFamily = 0x00;

		memset(oLogFontEx.oLogFont.ushFaceName, 0x00, 32);

		oLogFontEx.oLogFont.ushFaceName[0] = 'A';
		oLogFontEx.oLogFont.ushFaceName[1] = 'r';
		oLogFontEx.oLogFont.ushFaceName[2] = 'i';
		oLogFontEx.oLogFont.ushFaceName[3] = 'a';
		oLogFontEx.oLogFont.ushFaceName[4] = 'l';
	}

	CEmfLogFont::~CEmfLogFont()
	{
		RELEASEOBJECT(oDesignVector.pValues);
	}

	EEmfObjectType CEmfLogFont::GetType() const
	{
		return EMF_OBJECT_FONT;
	}

	double CEmfLogFont::GetHeight() const
	{
		return (double)oLogFontEx.oLogFont.nHeight;
	}

	std::wstring CEmfLogFont::GetFaceName() const
	{
		const std::wstring wsFaceName{NSFile::CUtf8Converter::GetWStringFromUTF16(oLogFontEx.oLogFont.ushFaceName, 32)};
		return wsFaceName.substr(0, wsFaceName.find(L'\0'));
	}
	
	int CEmfLogFont::GetWeight() const
	{
		return oLogFontEx.oLogFont.nWeight;
	}

	bool CEmfLogFont::IsItalic() const
	{
		return 0x01 == oLogFontEx.oLogFont.uchItalic;
	}

	bool CEmfLogFont::IsStrikeOut() const
	{
		return 0x01 == oLogFontEx.oLogFont.uchStrikeOut;
	}

	bool CEmfLogFont::IsUnderline() const
	{
		return 0x01 == oLogFontEx.oLogFont.uchUnderline ;
	}

	int CEmfLogFont::GetEscapement() const
	{
		return oLogFontEx.oLogFont.nEscapement;
	}

	int CEmfLogFont::GetCharSet() const
	{
		return oLogFontEx.oLogFont.uchCharSet;
	}

	bool CEmfLogFont::IsFixedLength() const
	{
		return m_bFixedLength;
	}

	int CEmfLogFont::GetOrientation() const
	{
		return oLogFontEx.oLogFont.nOrientation;
	}

	CEmfLogPen::CEmfLogPen()
		: unPenStyle(PS_SOLID), unWidth(1), oColor(0, 0, 0), unNumStyleEntries(0), pStyleEntry(NULL)
	{}

	CEmfLogPen::~CEmfLogPen()
	{
		RELEASEOBJECT(pStyleEntry);
	}

	EEmfObjectType CEmfLogPen::GetType() const
	{
		return EMF_OBJECT_PEN;
	}

	int  CEmfLogPen::GetColor() const
	{
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b, oColor.a);
	}

	unsigned int CEmfLogPen::GetStyle() const
	{
		return unPenStyle;
	}

	double CEmfLogPen::GetWidth() const
	{
		return (double)unWidth;
	}

	unsigned int CEmfLogPen::GetAlpha() const
	{
		return 0xff;
	}

	double CEmfLogPen::GetMiterLimit() const
	{
		return 0;
	}

	double CEmfLogPen::GetDashOffset() const
	{
		return 0;
	}

	void CEmfLogPen::GetDashData(double *&arDatas, unsigned int &unSize) const
	{
		arDatas = NULL;
		unSize  = 0;
	}

	const ILineCap* CEmfLogPen::GetStartLineCap() const
	{
		return NULL;
	}

	const ILineCap* CEmfLogPen::GetEndLineCap() const
	{
		return NULL;
	}

	CEmfLogPalette::CEmfLogPalette() : ushNumberOfEntries(0), pPaletteEntries(NULL)
	{}

	CEmfLogPalette::~CEmfLogPalette()
	{
		RELEASEOBJECT(pPaletteEntries);
	}

	EEmfObjectType CEmfLogPalette::GetType() const
	{
		return EMF_OBJECT_PALETTE;
	}
}
