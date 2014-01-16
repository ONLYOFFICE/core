#include "Image.h"

#include "../raster/BgraFrame.h"

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