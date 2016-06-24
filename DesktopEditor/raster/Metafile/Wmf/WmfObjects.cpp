/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
		DibBuffer = pBuffer;
		DibWidth  = ulWidth;
		DibHeigth = ulHeight;

		if (ulWidth <= 0 || ulHeight <= 0)
			return;

		unsigned int ulBufferSize = 4 * ulWidth * ulHeight;
		Aggplus::CImage oImage;
		BYTE* pBufferPtr = new BYTE[ulBufferSize];
		oImage.Create(pBufferPtr, ulWidth, ulHeight, 4 * ulWidth);

		// Пишем данные в pBufferPtr
		for (unsigned int ulIndex = 0; ulIndex < ulBufferSize; ulIndex += 4)
		{
			pBufferPtr[0] = (unsigned char)pBuffer[ulIndex + 0];
			pBufferPtr[1] = (unsigned char)pBuffer[ulIndex + 1];
			pBufferPtr[2] = (unsigned char)pBuffer[ulIndex + 2];
			pBufferPtr[3] = (unsigned char)pBuffer[ulIndex + 3];
			pBufferPtr += 4;
		}

		FILE *pTempFile = NULL;
		std::wstring wsTempFileName;
		if (!OpenTempFile(&wsTempFileName, &pTempFile, L"wb", L".emf0", NULL))
			return;

		::fclose(pTempFile);

		oImage.SaveFile(wsTempFileName, _CXIMAGE_FORMAT_PNG);

		BrushStyle     = BS_DIBPATTERN;
		DibPatternPath = wsTempFileName;
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
