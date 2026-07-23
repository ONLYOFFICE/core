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
#include "Image.h"

#include "../raster/BgraFrame.h"

#include <cstring>  // memset
namespace Aggplus
{
	////////////////////////////////////////////////////////////////////////////////////////

	CImage::CImage() : m_dwWidth(0), m_dwHeight(0), 
		m_nStride(0), m_pImgData(NULL), 
		m_bExternalBuffer(false), m_Status(WrongState)
	{
	}

	CImage::CImage(const std::wstring& filename) : m_dwWidth(0), m_dwHeight(0), 
			m_nStride(0), m_pImgData(NULL), 
			m_bExternalBuffer(false)
	{
		Create(filename);		
	}

	CImage::~CImage()
	{
		Destroy();
	}

	void CImage::Create(const std::wstring& filename)
	{
		Destroy();
		
		CBgraFrame oFrame;
		bool bOpen = oFrame.OpenFile(filename);
		
		if (bOpen)
		{
			m_pImgData = oFrame.get_Data();
			m_dwWidth = (DWORD)oFrame.get_Width();
			m_dwHeight = (DWORD)oFrame.get_Height();

			m_nStride = oFrame.get_Stride();
			m_Status = Ok;
		}

		oFrame.ClearNoAttack();
	}
	void CImage::Decode(BYTE* pBuffer, unsigned int unSize)
	{
		Destroy();

		CBgraFrame oFrame;
		bool bOpen = oFrame.Decode(pBuffer, (int)unSize);

		if (bOpen)
		{
			m_pImgData = oFrame.get_Data();
			m_dwWidth = (DWORD)oFrame.get_Width();
			m_dwHeight = (DWORD)oFrame.get_Height();

			m_nStride = oFrame.get_Stride();
			m_Status = Ok;
		}

		oFrame.ClearNoAttack();
	}
	void CImage::Create(BYTE* pImgData, const DWORD& dwWidth, const DWORD& dwHeight, const long& nStride, bool bExternalBuffer)
	{
		m_pImgData = pImgData;
		m_dwWidth = dwWidth;
		m_dwHeight = dwHeight;
		m_nStride = nStride;

		m_bExternalBuffer = bExternalBuffer;
		m_Status = Ok;
	}

	bool CImage::SaveFile(const std::wstring& strFileName, UINT nFileType)
	{
		CBgraFrame oBgraFrame;
		oBgraFrame.put_Width(m_dwWidth);
		oBgraFrame.put_Height(m_dwHeight);
		oBgraFrame.put_Stride(m_nStride);
		oBgraFrame.put_Data(m_pImgData);
		bool bRes = oBgraFrame.SaveFile(strFileName, nFileType);
		// to prevent deletion in destructor
		oBgraFrame.put_Data(NULL);
		return bRes;
	}
	void CImage::Destroy()
	{
		if (NULL != m_pImgData)
		{
			if (!m_bExternalBuffer)
			{
				delete [] m_pImgData;
			}
		}

		m_Status	= WrongState;
		m_pImgData	= NULL;
		m_dwWidth	= 0;
		m_dwHeight	= 0;
		m_nStride	= 0;
		
		m_bExternalBuffer = false;
	}

	DWORD CImage::GetWidth() const { return(m_dwWidth); }
	DWORD CImage::GetHeight() const { return(m_dwHeight); }
	long CImage::GetStride() const { return(m_nStride); }
	BYTE* CImage::GetData() const { return(m_pImgData); }
	 
	Status CImage::GetLastStatus() const { return(m_Status); }

	////////////////////////////////////////////////////////////////////////////////////////
	CBitmap::CBitmap(LONG width, LONG height, PixelFormat format) : CImage()
	{
		if(width <= 0 || height <= 0) 
		{ 
			m_Status=InvalidParameter; 
			return; 
		}

		LONG lSize = 4 * width * height;
		m_pImgData = new BYTE[lSize];
		
		if (m_pImgData)
		{
			memset(m_pImgData, 0, lSize);
			m_dwWidth	= width;
			m_dwHeight	= height;
			m_nStride	= 4 * m_dwWidth;
			m_Status	= Ok;
		}
	}
	
	CBitmap::CBitmap(LONG width, LONG height, LONG stride, PixelFormat format, BYTE* scan0) : CImage()
	{
		//Warning! This is not Gdiplus behavior; it returns Ok!
		if(width <= 0 || height <= 0 || stride == 0) 
		{ 
			m_Status = InvalidParameter; 
			return; 
		}

		m_bExternalBuffer = true;

		if (stride > 0) 
		{
			m_pImgData = scan0;
		}
		else
		{
			m_pImgData = scan0 + (height - 1) * (-stride);
		}
		
		m_dwWidth	= width;
		m_dwHeight	= height;
		m_nStride	= stride;
		m_Status	= Ok;
	}
	 
	CBitmap::CBitmap(const std::wstring& filename) : CImage(filename)
	{
	}
	
	CBitmap::~CBitmap()
	{
	}

	void CBitmap::LockBits(const RectF* rect, PixelFormat format, CBitmapData* lockedBitmapData)
	{
		// TODO:
		return;
	}
}
