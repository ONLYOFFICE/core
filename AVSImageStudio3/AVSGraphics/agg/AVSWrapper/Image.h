#pragma once

#include "../ap_AggPlusEnums.h"
#include "../ap_AggPlusTypes.h"


#include "../svg/agg_svg_exception.h"
#include "../svg/agg_svg_parser.h"
#include "../svg/agg_svg_indexation_interpreter.h"
#include "../svg/agg_svg_rendering_interpreter.h"
#include "../svg/agg_svg_attributes_map.h"
#include "../svg/agg_svg_rasterizer.h"

//#include "../X__CacheFile.h"
#include "../ap_Mem.h"
#include "../../stdafx.h"

namespace Aggplus
{
class CImage  
{
public:
	friend class CGraphics;
	friend class CBrushTexture;

public:
	CImage() : m_dwWidth(0), m_dwHeight(0), m_nStride(0), m_pImgData(NULL), m_bExternalBuffer(false), m_Status(WrongState), m_pFrame(NULL)
	{
	}

	CImage(const WCHAR *filename) : m_dwWidth(0), m_dwHeight(0), m_nStride(0), m_pImgData(NULL), m_bExternalBuffer(false), m_pFrame(NULL)
	{
		//Create(filename);
		CreateFromImageStudio(filename);
	}
	virtual ~CImage()
	{
		Destroy();
	}

	inline DWORD GetWidth() const { return(m_dwWidth); }
	inline DWORD GetHeight() const { return(m_dwHeight); }
	 
	inline Status GetLastStatus() const { return(m_Status); }

public:
	
	Status m_Status;
	
	DWORD m_dwWidth;
	DWORD m_dwHeight;
	long m_nStride;

	BYTE* m_pImgData;
	bool m_bExternalBuffer;

public:
	MediaCore::IAVSUncompressedVideoFrame* m_pFrame;
	 
	CImage* Clone() const
	{
		CImage* pRet = new CImage();
		pRet->CopyFrom(this);
		return pRet;
	}

	void Create(const WCHAR *filename);
	void CreateFromImageStudio(const WCHAR* filename);
	void CopyFrom(const CImage *pSource);
	void Destroy();
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
	PixelFormat PixelFormat;
	void* Scan0;
	UINT_PTR Reserved;
};

class CBitmap : public CImage
{
public:
	CBitmap(LONG width, LONG height, PixelFormat format = PixelFormat32bppARGB) : CImage()
	{
		assert(format == PixelFormat32bppARGB);

		if(width <= 0 || height <= 0) 
		{ 
			m_Status=InvalidParameter; 
			return; 
		}

		LONG lSize = 4 * width * height;
		m_pImgData = (BYTE*)ap_malloc16(lSize);
		
		assert(m_pImgData);

		if (m_pImgData)
		{
			memset(m_pImgData, 0, lSize);
			m_dwWidth	= width;
			m_dwHeight	= height;
			m_nStride	= 4 * m_dwWidth;
			m_Status	= Ok;
		}
	}
	
	CBitmap(LONG width, LONG height, LONG stride, PixelFormat format, BYTE* scan0) : CImage()
	{
		assert(format == PixelFormat32bppARGB);
		assert(scan0);

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
	 

	CBitmap(HBITMAP hbm, HPALETTE hpal) : CImage()
	{
	    Create(hbm, hpal);
	}
	
	CBitmap(HINSTANCE hInstance, const WCHAR* bitmapName) : CImage()
	{
		HBITMAP hBMRes = ::LoadBitmapW(hInstance, bitmapName);

		if (hBMRes)
		{
			Create(hBMRes, NULL);
			::DeleteObject(hBMRes);
		}
	}
	
	CBitmap(const WCHAR *filename) : CImage(filename)
	{
	}
	
	~CBitmap()
	{
	}

	static inline CBitmap* FromHBITMAP(HBITMAP hbm, HPALETTE hpal) { return new CBitmap(hbm, hpal); }

	void LockBits(const RectF* rect, PixelFormat format, CBitmapData* lockedBitmapData)
	{
		// надо реализовать
		return;
	}

protected:
	CBitmap() : CImage(NULL) { assert(FALSE); }

	void Create(HBITMAP hbm, HPALETTE hpal)
	{
		Destroy();

		BITMAPINFO bi;
		bool b32bitImg = false;
		//Step 1
		memset(&bi, 0, sizeof(BITMAPINFO));
		bi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);

		HDC hdc=::GetDC(NULL);
		int nChk=GetDIBits(hdc,				// handle to device context
							hbm,			// handle to bitmap
							0,				// first scan line to set in destination bitmap
							1,				// number of scan lines to copy
							NULL,			// address of array for bitmap bits
							&bi,			// address of structure with bitmap data
							DIB_RGB_COLORS  // RGB or palette index
							);

		if (!nChk || bi.bmiHeader.biWidth <= 0 || bi.bmiHeader.biHeight <=0 ) 
			return;

		if(bi.bmiHeader.biBitCount == 32 && bi.bmiHeader.biCompression == BI_RGB) 
			b32bitImg = true;


		//Step 2
		m_dwWidth			= bi.bmiHeader.biWidth;
		m_dwHeight			= bi.bmiHeader.biHeight;
		m_bExternalBuffer	= false;
		m_pImgData			= (BYTE*)ap_malloc16(4 * m_dwWidth * m_dwHeight);

		memset(&bi, 0, sizeof(BITMAPINFO));
		bi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biWidth = m_dwWidth;				// fill in width from parameter
		bi.bmiHeader.biHeight = -int(m_dwHeight);       // fill in height from parameter
		bi.bmiHeader.biPlanes = 1;						// must be 1
		bi.bmiHeader.biBitCount = 32;					// from parameter
		bi.bmiHeader.biCompression = BI_RGB;    
		bi.bmiHeader.biSizeImage = m_dwWidth * m_dwHeight * 4;
		//bi.bmiHeader.biXPelsPerMeter = 0;
		//bi.bmiHeader.biYPelsPerMeter = 0;
		//bi.bmiHeader.biClrUsed = 0;
		//bi.bmiHeader.biClrImportant = 0;


		nChk=GetDIBits(hdc,								// handle to device context
						hbm,							// handle to bitmap
						0,								// first scan line to set in destination bitmap
						m_dwHeight,						// number of scan lines to copy
						m_pImgData,						// address of array for bitmap bits
						&bi,							// address of structure with bitmap data
						DIB_RGB_COLORS					// RGB or palette index
						);

		//Opaque Alpha!
		if(!b32bitImg) 
		{ 
			DWORD dwSize = m_dwWidth * m_dwHeight;
			DWORD* pData = (DWORD*)m_pImgData;
			for (DWORD i = 0; i < dwSize; i++) 
			{ 
				pData[i] |= 0xFF000000; 
			} 
		}

		m_Status=Ok;

		::ReleaseDC(NULL, hdc);
	}
};

class CSVGImage
{
	friend class CGraphics;

	agg::svg::dom_storage             m_storage;
	agg::svg::dom_storage::map_type   m_id2elem_map;
	agg::svg::global_settings         m_settings;
	agg::svg::attributes              m_attributes;
	agg::svg::attributes_map          m_attr_map;

	agg::svg::pipeline                m_pipeline;
	agg::svg::gradient_lut_cache      m_gradient_lut_cache;

	void indexation()
	{
		agg::svg::indexation_interpreter consumer(m_id2elem_map);

		m_storage.traverse(consumer);

		m_id2elem_map.sort_state(agg::svg::on);	
	}
	void reset()
	{
		m_id2elem_map.clear();
		m_attributes.clear();
	}

public:
	CSVGImage() : m_attributes(m_settings)
	{
	}

   ~CSVGImage()
   {
	   Clear();
   }

	void  Clear()
	{
		m_storage.clear();	
		reset();
	}

	bool Open(const WCHAR *filename)
	{
		if(!filename)
			return false;

		m_attributes.initial_zoom(); 
		m_storage.clear();
		reset();

		agg::svg::parser p(m_storage, m_attr_map);
		p.parse(filename);
		indexation();
	
		return true;
	}
};

}