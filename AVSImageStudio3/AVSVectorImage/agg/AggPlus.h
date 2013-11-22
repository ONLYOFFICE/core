// Matrix.h: interface for the Matrix class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AggPlus__INCLUDED_)
#define AggPlus__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ap_AggPlusEnums.h"
#include "ap_Graphics.h"
#include "ap_GraphicsPath.h"
#include "ap_Matrix.h"
#include "ap_Pen.h"
#include "ap_Image.h"
#include "ap_Brush.h"
#include "ap_Font.h"
#include "ap_StringFormat.h"

#pragma comment( lib, "Msimg32.lib" )

namespace Aggplus
{

	class CBlitBuffer
	{
	public:
		CBlitBuffer(int nWidth, int nHeight) : m_pRGBA_buffer(NULL)
		{
			if(nWidth<=0 || nHeight<=0) return;
			m_dwWidth=nWidth;
			m_dwHeight=nHeight;
			m_pRGBA_buffer=new DWORD[m_dwWidth * m_dwHeight * 4];
			if(m_pRGBA_buffer) { memset(m_pRGBA_buffer, 0, m_dwWidth * m_dwHeight * 4); }
		}
		void Clear(DWORD dwRGBA)
		{
			int nCnt=m_dwWidth*m_dwHeight;
			DWORD *pIt=((DWORD *)m_pRGBA_buffer)+nCnt;
			while(nCnt)
			{
				*(--pIt)=dwRGBA;
				nCnt--;
			};
		}
		void PatternFill()
		{
			DWORD dwColors[3]={0xFF606060, 0xFF808080, 0xFF606060};
			DWORD *pIt=(DWORD *)m_pRGBA_buffer;
			for(unsigned y=0;y<m_dwHeight;y++)
			{
				for(unsigned x=0;x<m_dwWidth;x++)
				{
					*pIt=dwColors[(x/8)%2 + (y/8)%2];
					pIt++;
				}
			}
		}
		void BlitTo(HDC hdc, int nLeft, int nTop)
		{
			if(!m_pRGBA_buffer || !hdc) return;

			BITMAPINFO dib;
			memset(&dib, 0, sizeof(BITMAPINFO));
			dib.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			dib.bmiHeader.biWidth = m_dwWidth;
			dib.bmiHeader.biHeight = m_dwHeight;
			dib.bmiHeader.biPlanes = 1;
			dib.bmiHeader.biBitCount = 32;
			dib.bmiHeader.biCompression = BI_RGB;
			dib.bmiHeader.biSizeImage = m_dwWidth * m_dwHeight * 4;

			SetDIBitsToDevice(hdc, nLeft, nTop, m_dwWidth, m_dwHeight,
				0, 0, 0, m_dwHeight,
				m_pRGBA_buffer, &dib, DIB_RGB_COLORS);

		}
		~CBlitBuffer()
		{
			if(m_pRGBA_buffer)
			{
				delete(m_pRGBA_buffer);
			}
		}

		void *m_pRGBA_buffer;
		DWORD m_dwWidth, m_dwHeight;
	};














}

#endif // !defined(AggPlus__INCLUDED_)
