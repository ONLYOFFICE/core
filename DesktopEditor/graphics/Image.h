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
#ifndef _BUILD_IMAGE_H_
#define _BUILD_IMAGE_H_

#include "AggPlusEnums.h"
#include "IRenderer.h"

#include "config.h"

namespace Aggplus
{

class GRAPHICS_DECL CImage : public IGrObject
{
public:
	friend class CGraphics;
	friend class CBrushTexture;

public:
	CImage();
	CImage(const std::wstring& filename);
	virtual ~CImage();

	DWORD GetWidth() const;
	DWORD GetHeight() const;
	long GetStride() const;
	BYTE* GetData() const;
	 
	Status GetLastStatus() const;

	void Create(const std::wstring& filename);
	void Create(BYTE* pImgData, const DWORD& dwWidth, const DWORD& dwHeight, const long& nStride, bool bExternalBuffer = false);
	void Decode(BYTE *pBuffer, unsigned int unSize);
	bool SaveFile(const std::wstring& strFileName, UINT nFileType);
	void Destroy();

protected:
	Status m_Status;
	
	DWORD m_dwWidth;
	DWORD m_dwHeight;
	long m_nStride;

	BYTE* m_pImgData;
	bool m_bExternalBuffer;
};

//---------------------------------------------------------------------------
// Information about image pixel data
//---------------------------------------------------------------------------

class CBitmapData
{
public:
	UINT Width;
	UINT Height;
	LONG Stride;
    Aggplus::PixelFormat PixelFormat;
	void* Scan0;
	UINT* Reserved;
};

class GRAPHICS_DECL CBitmap : public CImage
{
public:
	CBitmap(LONG width, LONG height, PixelFormat format = PixelFormat32bppARGB);	
	CBitmap(LONG width, LONG height, LONG stride, PixelFormat format, BYTE* scan0);	 
	CBitmap(const std::wstring& filename);
	
	virtual ~CBitmap();

	void LockBits(const RectF* rect, PixelFormat format, CBitmapData* lockedBitmapData);
};

}

#endif // _BUILD_IMAGE_H_
