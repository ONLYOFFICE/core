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
		//чтобы не удалялся на destructor
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
