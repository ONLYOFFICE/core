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
#include "../../../graphics/Image.h"

#include "../Common/MetaFileUtils.h"

#include "WmfObjects.h"

#include "../../BgraFrame.h"

namespace MetaFile
{
	CWmfObjectBase::CWmfObjectBase()
	{}
	
	CWmfObjectBase::~CWmfObjectBase()
	{}

	EWmfObjectType CWmfObjectBase::GetType() const
	{
		return WMF_OBJECT_UNKNOWN;
	}

	CWmfBrush::CWmfBrush() 
		: ushBrushStyle(BS_SOLID), oColor(255, 255, 255), ushBrushHatch(HS_HORIZONTAL), pDibBuffer(NULL), unDibWidth(0), unDibHeigth(0)
	{}

	CWmfBrush::CWmfBrush(const TWmfLogBrush& oBrush)
		: ushBrushStyle(oBrush.ushBrushStyle), oColor(oBrush.oColor), ushBrushHatch(oBrush.ushBrushHatch), pDibBuffer(NULL), unDibWidth(0), unDibHeigth(0)
	{}

	CWmfBrush::~CWmfBrush()
	{
		if (BS_DIBPATTERN == ushBrushStyle && !wsDibPatternPath.empty())
			NSFile::CFileBinary::Remove(wsDibPatternPath);

		RELEASEOBJECT(pDibBuffer);
	}

	EWmfObjectType CWmfBrush::GetType() const
	{
		return WMF_OBJECT_BRUSH;
	}

	void CWmfBrush::SetDibPattern(unsigned char* pBuffer, unsigned int ulWidth, unsigned int ulHeight)
	{
#ifdef METAFILE_DISABLE_FILESYSTEM
		// without using filesystem this cannot be implemented yet when converting to raster,
		// since currently brush image can only be passed to renderer using the filesystem
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
			ushBrushStyle    = BS_DIBPATTERN;
			wsDibPatternPath = wsTempFileName;

			BYTE* pNewBuffer = NULL;
			int nNewSize = 0;

			oBgraFrame.Encode(pNewBuffer, nNewSize, _CXIMAGE_FORMAT_PNG);

			pDibBuffer  = pNewBuffer;
			unDibWidth  = oBgraFrame.get_Width();
			unDibHeigth = oBgraFrame.get_Height();
		}

		oBgraFrame.put_Data(NULL);
#endif
	}

	int CWmfBrush::GetColor() const
	{
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b, oColor.a);
	}

	int CWmfBrush::GetColor2() const
	{
		return 0;
	}

	unsigned int CWmfBrush::GetStyle() const
	{
		return ushBrushStyle;
	}

	unsigned int CWmfBrush::GetStyleEx() const
	{
		return 0;
	}

	unsigned int CWmfBrush::GetHatch() const
	{
		return ushBrushHatch;
	}

	unsigned int CWmfBrush::GetAlpha() const
	{
		return 0xff;
	}

	unsigned int CWmfBrush::GetAlpha2() const
	{
		return 0xff;
	}

	std::wstring CWmfBrush::GetDibPatterPath() const
	{
		return wsDibPatternPath;
	}

	void CWmfBrush::GetBounds(double &left, double &top, double &width, double &height) const
	{}

	void CWmfBrush::GetCenterPoint(double &dX, double &dY) const
	{}

	void CWmfBrush::GetDibPattern(unsigned char **pBuffer, unsigned int &unWidth, unsigned int &unHeight) const
	{
		*pBuffer = pDibBuffer;
		unWidth  = unDibWidth;
		unHeight = unDibHeigth;
	}

	void CWmfBrush::GetGradientColors(std::vector<long>& arColors, std::vector<double>& arPositions) const
	{
		arColors    = {(long)(GetColor()  + (GetAlpha()  << 24)), (long)(GetColor2() + (GetAlpha2() << 24))};
		arPositions = {0., 1.};
	}

	CWmfFont::CWmfFont()
	    : shHeight(DEFAULT_FONT_SIZE), shWidth(0), shEscapement(0), shOrientation(0), shWeight(400),
	      uchItalic(0x00), uchUnderline(0x00), uchStrikeOut(0x00), uchCharSet(0x01), uchOutPrecision(0x00),
	      uchClipPrecision(0x00), uchQuality(0x00), uchPitchAndFamily(0x00)
	{
		memset(uchFacename, 0x00, 32);

		uchFacename[0] = 'A';
		uchFacename[1] = 'r';
		uchFacename[2] = 'i';
		uchFacename[3] = 'a';
		uchFacename[4] = 'l';
	}

	CWmfFont::~CWmfFont()
	{}

	EWmfObjectType CWmfFont::GetType() const
	{
		return WMF_OBJECT_FONT;
	}

	double CWmfFont::GetHeight() const
	{
		return (double)shHeight;
	}

	std::wstring CWmfFont::GetFaceName() const
	{
		const std::wstring wsFontName(ConvertToUnicode((const unsigned char*)uchFacename, 32, uchCharSet));
		return wsFontName.substr(0, wsFontName.find(L'\0'));
	}

	int CWmfFont::GetWeight() const
	{
		return (int)shWidth;
	}

	bool CWmfFont::IsItalic() const
	{
		return 0x01 == uchItalic;
	}

	bool CWmfFont::IsStrikeOut() const
	{
		return 0x01 == uchStrikeOut;
	}

	bool CWmfFont::IsUnderline() const
	{
		return 0x01 == uchUnderline;
	}

	int CWmfFont::GetEscapement() const
	{
		return (int)shEscapement;
	}

	int CWmfFont::GetCharSet() const
	{
		return (int)uchCharSet;
	}

	int CWmfFont::GetOrientation() const
	{
		return (int)shOrientation;
	}

	CWmfPalette::CWmfPalette() : ushNumberOfEntries(0), pPaletteEntries(NULL)
	{}

	CWmfPalette::~CWmfPalette()
	{
		RELEASEOBJECT(pPaletteEntries);
	}

	EWmfObjectType CWmfPalette::GetType() const
	{
		return WMF_OBJECT_PALETTE;
	}

	CWmfPen::CWmfPen()
	{}

	CWmfPen::~CWmfPen()
	{}

	EWmfObjectType CWmfPen::GetType() const
	{
		return WMF_OBJECT_PEN;
	}

	int CWmfPen::GetColor() const
	{
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b, oColor.a);
	}

	unsigned int CWmfPen::GetStyle() const
	{
		return (unsigned int)ushPenStyle;
	}

	double CWmfPen::GetWidth() const
	{
		return (double)oWidth.X;
	}

	unsigned int CWmfPen::GetAlpha() const
	{
		return 0xff;
	}

	double CWmfPen::GetMiterLimit() const
	{
		return 0;
	}

	double CWmfPen::GetDashOffset() const
	{
		return 0;
	}

	void CWmfPen::GetDashData(double *&arDatas, unsigned int &unSize) const
	{
		arDatas = NULL;
		unSize  = 0;
	}

	const ILineCap* CWmfPen::GetStartLineCap() const
	{
		return NULL;
	}

	const ILineCap* CWmfPen::GetEndLineCap() const
	{
		return NULL;
	}

	CWmfRegion::CWmfRegion() : pScans(NULL)
	{}

	CWmfRegion::~CWmfRegion()
	{
		RELEASEOBJECT(pScans);
	}

	EWmfObjectType CWmfRegion::GetType() const
	{
		return WMF_OBJECT_REGION;
	}
}
