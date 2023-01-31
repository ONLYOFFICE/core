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
#include "../../../raster/ImageFileFormatChecker.h"
#include "../../../graphics/Image.h"

#include "../Common/MetaFileUtils.h"

#include "WmfObjects.h"

#include "../../BgraFrame.h"

namespace MetaFile
{
	CWmfBrush::CWmfBrush() : Color(255, 255, 255)
	{
		BrushStyle     = BS_SOLID;
		BrushHatch     = HS_HORIZONTAL;
		DibPatternPath = L"";
		DibBuffer      = NULL;
		DibWidth       = 0;
		DibHeigth      = 0;
	}
	CWmfBrush::CWmfBrush(TWmfLogBrush& oBrush)
	{
		BrushStyle     = oBrush.BrushStyle;
		Color          = oBrush.Color;
		BrushHatch     = oBrush.BurshHatch;
		DibPatternPath = L"";
		DibBuffer      = NULL;
		DibWidth       = 0;
		DibHeigth      = 0;
	}
	CWmfBrush::~CWmfBrush()
	{
		if (BS_DIBPATTERN == BrushStyle && L"" != DibPatternPath)
		{
			NSFile::CFileBinary::Remove(DibPatternPath);
		}

		if (DibBuffer)
			delete[] DibBuffer;
	}
	void         CWmfBrush::SetDibPattern(unsigned char* pBuffer, unsigned int ulWidth, unsigned int ulHeight)
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
			BrushStyle     = BS_DIBPATTERN;
			DibPatternPath = wsTempFileName;

			BYTE* pNewBuffer = NULL;
			int nNewSize = 0;

			oBgraFrame.Encode(pNewBuffer, nNewSize, _CXIMAGE_FORMAT_PNG);

			DibBuffer	= pNewBuffer;
			DibWidth	= oBgraFrame.get_Width();
			DibHeigth	= oBgraFrame.get_Height();
		}

		oBgraFrame.put_Data(NULL);
#endif
	}
	int          CWmfBrush::GetColor()
	{
		return METAFILE_RGBA(Color.r, Color.g, Color.b);
	}
	unsigned int CWmfBrush::GetStyle()
	{
		return BrushStyle;
	}
	unsigned int CWmfBrush::GetHatch()
	{
		return BrushHatch;
	}
	unsigned int CWmfBrush::GetAlpha()
	{
		return 255;
	}
	std::wstring CWmfBrush::GetDibPatterPath()
	{
		return DibPatternPath;
	}

	int CWmfPen::GetColor()
	{
		return METAFILE_RGBA(Color.r, Color.g, Color.b);
	}
}
