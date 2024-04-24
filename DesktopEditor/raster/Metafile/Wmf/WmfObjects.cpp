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

#include "WmfObjects.h"

#include "../../BgraFrame.h"

namespace MetaFile
{
	CWmfObjectBase::CWmfObjectBase()
	{}
	
	CWmfObjectBase::~CWmfObjectBase()
	{}

	EWmfObjectType CWmfObjectBase::GetType()
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

	EWmfObjectType CWmfBrush::GetType()
	{
		return WMF_OBJECT_BRUSH;
	}

	void CWmfBrush::SetDibPattern(unsigned char* pBuffer, unsigned int ulWidth, unsigned int ulHeight)
	{
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

	int CWmfBrush::GetColor()
	{
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b, oColor.a);
	}

	int CWmfBrush::GetColor2()
	{
		return 0;
	}

	unsigned int CWmfBrush::GetStyle()
	{
		return ushBrushStyle;
	}

	unsigned int CWmfBrush::GetStyleEx()
	{
		return 0;
	}

	unsigned int CWmfBrush::GetHatch()
	{
		return ushBrushHatch;
	}

	unsigned int CWmfBrush::GetAlpha()
	{
		return 255;
	}

	unsigned int CWmfBrush::GetAlpha2()
	{
		return 0xff;
	}

	std::wstring CWmfBrush::GetDibPatterPath()
	{
		return wsDibPatternPath;
	}

	void CWmfBrush::GetBounds(double &left, double &top, double &width, double &height)
	{
		
	}

	void CWmfBrush::GetCenterPoint(double &dX, double &dY)
	{
		
	}

	void CWmfBrush::GetDibPattern(unsigned char **pBuffer, unsigned int &unWidth, unsigned int &unHeight)
	{
		*pBuffer	= pDibBuffer;
		unWidth		= unDibWidth;
		unHeight	= unDibHeigth;
	}

	CWmfFont::CWmfFont()
	{
		memset(uchFacename, 0x00, 32);
	}

	CWmfFont::~CWmfFont()
	{}

	EWmfObjectType CWmfFont::GetType()
	{
		return WMF_OBJECT_FONT;
	}

	double CWmfFont::GetHeight()
	{
		return (double)shHeight;
	}

	std::wstring CWmfFont::GetFaceName()
	{
		return std::wstring(NSStringExt::CConverter::GetUnicodeFromSingleByteString((const unsigned char*)uchFacename, 32).c_str());
	}

	int CWmfFont::GetWeight()
	{
		return (int)shWidth;
	}

	bool CWmfFont::IsItalic()
	{
		return (0x01 == uchItalic ? true : false);
	}

	bool CWmfFont::IsStrikeOut()
	{
		return (0x01 == uchStrikeOut ? true : false);
	}

	bool CWmfFont::IsUnderline()
	{
		return (0x01 == uchUnderline ? true : false);
	}

	int CWmfFont::GetEscapement()
	{
		return (int)shEscapement;
	}

	int CWmfFont::GetCharSet()
	{
		return (int)uchCharSet;
	}

	int CWmfFont::GetOrientation()
	{
		return (int)shOrientation;
	}

	CWmfPalette::CWmfPalette() : ushNumberOfEntries(0), pPaletteEntries(NULL)
	{}

	CWmfPalette::~CWmfPalette()
	{
		RELEASEOBJECT(pPaletteEntries);
	}

	EWmfObjectType CWmfPalette::GetType()
	{
		return WMF_OBJECT_PALETTE;
	}

	CWmfPen::CWmfPen()
	{}

	CWmfPen::~CWmfPen()
	{}

	EWmfObjectType CWmfPen::GetType()
	{
		return WMF_OBJECT_PEN;
	}

	int CWmfPen::GetColor()
	{
		return METAFILE_RGBA(oColor.r, oColor.g, oColor.b, oColor.a);
	}

	unsigned int CWmfPen::GetStyle()
	{
		return (unsigned int)ushPenStyle;
	}

	double CWmfPen::GetWidth()
	{
		return (double)oWidth.X;
	}

	unsigned int CWmfPen::GetAlpha()
	{
		return 0xff;
	}

	double CWmfPen::GetMiterLimit()
	{
		return 0;
	}

	double CWmfPen::GetDashOffset()
	{
		return 0;
	}

	void CWmfPen::GetDashData(double *&arDatas, unsigned int &unSize)
	{
		arDatas = NULL;
		unSize  = 0;
	}

	CWmfRegion::CWmfRegion() : pScans(NULL)
	{}

	CWmfRegion::~CWmfRegion()
	{
		RELEASEOBJECT(pScans);
	}

	EWmfObjectType CWmfRegion::GetType()
	{
		return WMF_OBJECT_REGION;
	}
}
