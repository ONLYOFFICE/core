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
