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
