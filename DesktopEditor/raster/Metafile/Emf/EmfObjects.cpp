/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
#include "../../../raster/ImageFileFormatChecker.h"
#include "../../../graphics/Image.h"

#include "../Common/MetaFileUtils.h"

#include "EmfObjects.h"

#include "../../BgraFrame.h"

namespace MetaFile
{
	CEmfObjectBase::CEmfObjectBase()
	{}

	CEmfObjectBase::~CEmfObjectBase()
	{}

	EEmfObjectType CEmfObjectBase::GetType()
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
	
	EEmfObjectType CEmfLogBrushEx::GetType()
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
		// без использования файловой системы пока реализовать не получится при конвертации в растр,
		// так как на данный момент картинку кисти передать в рендер можно только  с помощью использования файловой системы
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

	int CEmfLogBrushEx::GetColor()
	{
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b, 0);
	}

	int CEmfLogBrushEx::GetColor2()
	{
		return 0;
	}

	unsigned int CEmfLogBrushEx::GetStyle()
	{
		return unBrushStyle;
	}

	unsigned int CEmfLogBrushEx::GetStyleEx()
	{
		return 0;
	}

	unsigned int CEmfLogBrushEx::GetHatch()
	{
		return unBrushHatch;
	}

	unsigned int CEmfLogBrushEx::GetAlpha()
	{
		return 0xff;
	}

	unsigned int CEmfLogBrushEx::GetAlpha2()
	{
		return 0xff;
	}

	std::wstring CEmfLogBrushEx::GetDibPatterPath()
	{
		return wsDibPatternPath;
	}

	void CEmfLogBrushEx::GetBounds(double &left, double &top, double &width, double &height)
	{}

	void CEmfLogBrushEx::GetCenterPoint(double &dX, double &dY)
	{}

	void CEmfLogBrushEx::GetDibPattern(unsigned char **pBuffer, unsigned int &unWidth, unsigned int &unHeight)
	{
		*pBuffer	= pDibBuffer;
		unWidth		= unDibWidth;
		unHeight	= unDibHeigth;
	}

	CEmfLogFont::CEmfLogFont(bool bFixedLength) : m_bFixedLength(bFixedLength)
	{
		oDesignVector.pValues = NULL;
	}

	CEmfLogFont::~CEmfLogFont()
	{
		RELEASEOBJECT(oDesignVector.pValues);
	}

	EEmfObjectType CEmfLogFont::GetType()
	{
		return EMF_OBJECT_FONT;
	}

	double CEmfLogFont::GetHeight()
	{
		return (double)oLogFontEx.oLogFont.nHeight;
	}

	std::wstring CEmfLogFont::GetFaceName()
	{
		return std::wstring(NSFile::CUtf8Converter::GetWStringFromUTF16(oLogFontEx.oLogFont.ushFaceName, 32).c_str());
	}
	
	int CEmfLogFont::GetWeight()
	{
		return oLogFontEx.oLogFont.nWeight;
	}

	bool CEmfLogFont::IsItalic()
	{
		return (0x01 == oLogFontEx.oLogFont.uchItalic ? true : false);
	}

	bool CEmfLogFont::IsStrikeOut()
	{
		return (0x01 == oLogFontEx.oLogFont.uchStrikeOut ? true : false);
	}

	bool CEmfLogFont::IsUnderline()
	{
		return (0x01 == oLogFontEx.oLogFont.uchUnderline ? true : false);
	}

	int CEmfLogFont::GetEscapement()
	{
		return oLogFontEx.oLogFont.nEscapement;
	}

	int CEmfLogFont::GetCharSet()
	{
		return oLogFontEx.oLogFont.uchCharSet;
	}

	bool CEmfLogFont::IsFixedLength()
	{
		return m_bFixedLength;
	}

	int CEmfLogFont::GetOrientation()
	{
		return oLogFontEx.oLogFont.nOrientation;
	}

	CEmfLogPen::CEmfLogPen()
		: unPenStyle(PS_SOLID), unWidth(1), oColor(0, 0, 0), pStyleEntry(NULL)
	{}

	CEmfLogPen::~CEmfLogPen()
	{
		RELEASEOBJECT(pStyleEntry);
	}

	EEmfObjectType CEmfLogPen::GetType()
	{
		return EMF_OBJECT_PEN;
	}

	int  CEmfLogPen::GetColor()
	{
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b, oColor.a);
	}

	unsigned int CEmfLogPen::GetStyle()
	{
		return unPenStyle;
	}

	double CEmfLogPen::GetWidth()
	{
		return (double)unWidth;
	}

	unsigned int CEmfLogPen::GetAlpha()
	{
		return 0xff;
	}

	double CEmfLogPen::GetMiterLimit()
	{
		return 0;
	}

	double CEmfLogPen::GetDashOffset()
	{
		return 0;
	}

	void CEmfLogPen::GetDashData(double *&arDatas, unsigned int &unSize)
	{
		arDatas = NULL;
		unSize  = 0;
	}

	CEmfLogPalette::CEmfLogPalette() : ushNumberOfEntries(0), pPaletteEntries(NULL)
	{}

	CEmfLogPalette::~CEmfLogPalette()
	{
		RELEASEOBJECT(pPaletteEntries);
	}

	EEmfObjectType CEmfLogPalette::GetType()
	{
		return EMF_OBJECT_PALETTE;
	}
}
