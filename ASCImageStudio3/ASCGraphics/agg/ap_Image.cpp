//----------------------------------------------------------------------------
// Image class  - Version 0.1.1 - 31-10-2004
// Marlon Mircevski
//----------------------------------------------------------------------------
//#include "stdafx.h"
#include <string.h>
#include "ap_Image.h"
#include "X__CacheFile.h"
#include "ap_Mem.h"

inline LPVOID ZC_GetCreateItemBuff(LPCWSTR szwFilename, DWORD *pdwRetWidth, DWORD *pdwRetHeight) 
{ 
	return Z__CacheFile.GetCreateItemBuff(szwFilename, pdwRetWidth, pdwRetHeight); 
}
inline LPVOID ZC_GetCreateCopy(LPVOID pSourceBuff, DWORD dwWidth, DWORD dwHeight) 
{ 
	return Z__CacheFile.GetCreateCopy(pSourceBuff, dwWidth, dwHeight); 
}
inline void   ZC_Release(LPVOID pBuff) 
{ 
	Z__CacheFile.Release(pBuff); 
}

namespace Aggplus
{

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	Image::Image() : m_dwWidth(0), m_dwHeight(0), m_nStride(0), m_pImgData(NULL), m_bExternalBuffer(false), m_Status(WrongState)
	{
	}

	Image::Image(const WCHAR *filename, BOOL useEmbeddedColorManagement) : m_dwWidth(0), m_dwHeight(0), m_nStride(0), m_pImgData(NULL), m_bExternalBuffer(false)
	{
		Create(filename);
	}

	Image::~Image()
	{
		Destroy();
	}

	void Image::Create(const WCHAR *filename)
	{
		Destroy();
		m_pImgData=ZC_GetCreateItemBuff(filename, &m_dwWidth, &m_dwHeight);
		m_nStride=4*m_dwWidth;
		if(!m_pImgData) 
		{ 
			Destroy(); 
		}
		else 
			m_Status=Ok;
	}

	void Image::CopyFrom(const Image *pSource)
	{
		Destroy();
		if(pSource && pSource->m_Status == Ok)
		{
			m_Status=Ok;
			m_dwWidth=pSource->m_dwWidth;
			m_dwHeight=pSource->m_dwHeight;
			m_nStride=pSource->m_nStride;
			m_bExternalBuffer=false;
			if(pSource->m_bExternalBuffer)
				{
					assert((unsigned)abs(m_nStride)==4*m_dwWidth);
					m_pImgData=ap_malloc16(4*m_dwWidth*m_dwHeight);
					memcpy(m_pImgData, pSource->m_pImgData, 4*m_dwWidth*m_dwHeight);
				}
			else
				{
					m_pImgData=ZC_GetCreateCopy(pSource->m_pImgData, m_dwWidth, m_dwHeight);
					if(!m_pImgData) { Destroy(); }
				}
		}
	}

	Image *Image::Clone() const
	{
		Image *pRet=new Image;
		pRet->CopyFrom(this);
		return(pRet);
	}

	void Image::Destroy()
	{
		if(m_pImgData)
		{
			if(!m_bExternalBuffer)
			{
				ZC_Release(m_pImgData);
			}
		}

		m_Status=WrongState;
		m_pImgData=NULL;
		m_dwWidth=m_dwHeight=m_nStride=0;
		m_bExternalBuffer=false;
	}
	
	Bitmap::Bitmap(INT width, INT height, PixelFormat format) : Image()
	{
		assert(format==PixelFormat32bppARGB);

		if(width<=0 || height<=0) 
		{ 
			m_Status=InvalidParameter; return; 
		}

		m_pImgData=ap_malloc16(4*width*height);
		assert(m_pImgData);
		if(m_pImgData)
		{
			memset(m_pImgData, 0, 4*width*height);
			m_dwWidth=width;
			m_dwHeight=height;
			m_nStride=4*m_dwWidth;
			m_Status=Ok;
		}
	}

	Bitmap::Bitmap(INT width, INT height, INT stride, PixelFormat format, BYTE* scan0) : Image()
	{
		assert(format==PixelFormat32bppARGB);
		assert(scan0);

		//Warning! This is not Gdiplus behavior; it returns Ok!
		if(width<=0 || height<=0 || stride==0) 
		{ m_Status=InvalidParameter; return; }

		m_bExternalBuffer=true;

		if(stride>0) m_pImgData=scan0;
		else         m_pImgData=scan0+(height-1)*stride;
		m_dwWidth=width;
		m_dwHeight=height;
		m_nStride=stride;
		m_Status=Ok;
	}

	Bitmap::Bitmap(const WCHAR *filename, BOOL useIcm) : Image(filename, useIcm)
	{
	}

	Bitmap::Bitmap(HBITMAP hbm, HPALETTE hpal) : Image()
	{
	    Create(hbm, hpal);
	}

	Bitmap::Bitmap(HINSTANCE hInstance, const WCHAR* bitmapName) : Image()
	{
		HBITMAP hBMRes=::LoadBitmapW(hInstance, bitmapName);

		if(hBMRes)
		{
			Create(hBMRes, NULL);
			::DeleteObject(hBMRes);
		}
	}

	Bitmap::~Bitmap()
	{
	}


	void Bitmap::Create(HBITMAP hbm, HPALETTE hpal)
	{
			Destroy();

			BITMAPINFO bi;
			bool b32bitImg=false;
			//Step 1
			memset(&bi, 0, sizeof(BITMAPINFO));
			bi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);

			HDC hdc=::GetDC(NULL);
			int nChk=GetDIBits(hdc,           // handle to device context
								hbm,      // handle to bitmap
								0,   // first scan line to set in destination bitmap
								1,   // number of scan lines to copy
								NULL,    // address of array for bitmap bits
								&bi, // address of structure with bitmap data
								DIB_RGB_COLORS        // RGB or palette index
								);

			if(!nChk || bi.bmiHeader.biWidth<=0 || bi.bmiHeader.biHeight<=0) return;
			if(bi.bmiHeader.biBitCount==32 && bi.bmiHeader.biCompression == BI_RGB) b32bitImg=true;


			//Step 2
			m_dwWidth=bi.bmiHeader.biWidth;
			m_dwHeight=bi.bmiHeader.biHeight;
			m_bExternalBuffer=false;
			m_pImgData=ap_malloc16(4*m_dwWidth*m_dwHeight);

			memset(&bi, 0, sizeof(BITMAPINFO));
			bi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
			bi.bmiHeader.biWidth = m_dwWidth;         // fill in width from parameter
			bi.bmiHeader.biHeight = -int(m_dwHeight);       // fill in height from parameter
			bi.bmiHeader.biPlanes = 1;              // must be 1
			bi.bmiHeader.biBitCount = 32;    // from parameter
			bi.bmiHeader.biCompression = BI_RGB;    
			bi.bmiHeader.biSizeImage = m_dwWidth * m_dwHeight * 4;
			//bi.bmiHeader.biXPelsPerMeter = 0;
			//bi.bmiHeader.biYPelsPerMeter = 0;
			//bi.bmiHeader.biClrUsed = 0;
			//bi.bmiHeader.biClrImportant = 0;


			nChk=GetDIBits(hdc,           // handle to device context
							hbm,      // handle to bitmap
							0,   // first scan line to set in destination bitmap
							m_dwHeight,   // number of scan lines to copy
							m_pImgData,    // address of array for bitmap bits
							&bi, // address of structure with bitmap data
							DIB_RGB_COLORS        // RGB or palette index
							);

			//Opaque Alpha!
			if(!b32bitImg) 
			{ 
				for(DWORD i=0;i<m_dwWidth*m_dwHeight;i++) { ((DWORD *)m_pImgData)[i] |= 0xFF000000; } 
			}

			m_Status=Ok;

			::ReleaseDC(NULL, hdc);
	}




	SVGImage::SVGImage():m_attributes(m_settings)
	{

	}


	SVGImage::~SVGImage()
	{
    	Clear();
	}


	void  SVGImage::Clear()
	{
		m_storage.clear();	
		reset();
	}


	void SVGImage::indexation()
	{
		agg::svg::indexation_interpreter consumer(m_id2elem_map);

		m_storage.traverse(consumer);

		m_id2elem_map.sort_state(agg::svg::on);	
	}

	void SVGImage::reset()
	{
		m_id2elem_map.clear();
		m_attributes.clear();
	}

	bool  SVGImage::Open(const WCHAR *filename)
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


}
