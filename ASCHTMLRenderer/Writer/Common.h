#pragma once
//#include "../stdafx.h"
#include "Const.h"

namespace NSHtmlRenderer
{
		inline LONG ConvertColor(LONG lBGR)
	{
		return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
	}
	inline CString GetStringColor(LONG lBGR)
	{
		BYTE R = (BYTE)(lBGR & 0xFF);
		BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
		BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

		CString strTemp = _T("");
		strTemp.Format(g_string_css_color, R, G, B);
		
		return strTemp;
	}
	inline CString SetStrokeColor(LONG lBGR, LONG lA)
	{
		BYTE R = (BYTE)(lBGR & 0xFF);
		BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
		BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

		CString strTemp = _T("");
		strTemp.Format(g_string_canvas_StrokeColorA, R, G, B, (double)lA / 255);
		
		return strTemp;
	}
	inline CString SetFillColor(LONG lBGR, LONG lA)
	{
		BYTE R = (BYTE)(lBGR & 0xFF);
		BYTE G = (BYTE)((lBGR >> 8) & 0xFF);
		BYTE B = (BYTE)((lBGR >> 16) & 0xFF);

		CString strTemp = _T("");
		strTemp.Format(g_string_canvas_FillColorA, R, G, B, (double)lA / 255);

		return strTemp;
	}



	static RECT GetImageBounds(MediaCore::IAVSUncompressedVideoFrame* pFrame)
	{
		BYTE* pBuffer = NULL;
		LONG lWidth = 0;
		LONG lHeight = 0;

		pFrame->get_Buffer(&pBuffer);
		pFrame->get_Width(&lWidth);
		pFrame->get_Height(&lHeight);

		RECT rect;
		rect.left	= 0;
		rect.top	= 0;
		rect.right	= lWidth - 1;
		rect.bottom = lHeight - 1;

		// top
		DWORD* pData = (DWORD*)pBuffer;
		for (; rect.top < lHeight; rect.top++)
		{
			bool bIsBreak = false;
			for (LONG i = 0; i < lWidth; ++i, ++pData)
			{
				if (*pData != 0x00)
				{
					bIsBreak = true;
					break;
				}
			}
			if (bIsBreak)
				break;
		}
		if (rect.top >= lHeight)
			rect.top = (lHeight - 1);

		// bottom
		for (; rect.bottom >= rect.top; rect.bottom--)
		{
			pData = (DWORD*)pBuffer;
			pData += (lWidth * rect.bottom);

			bool bIsBreak = false;
			for (LONG i = 0; i < lWidth; ++i, ++pData)
			{
				if (*pData != 0x00)
				{
					bIsBreak = true;
					break;
				}
			}
			if (bIsBreak)
				break;
		}
		if (rect.bottom < rect.top)
			rect.bottom = rect.top;

		LONG lDelta = rect.bottom - rect.top + 1;

		// left
		for (; rect.left < lWidth; rect.left++)
		{
			pData = (DWORD*)(pBuffer + 4 * lWidth * rect.top);
			pData += rect.left;

			bool bIsBreak = false;
			for (LONG i = 0; i < lDelta; ++i, pData += lWidth)
			{
				if (*pData != 0x00)
				{
					bIsBreak = true;
					break;
				}
			}
			if (bIsBreak)
				break;
		}
		if (rect.left >= lWidth)
			rect.left = lWidth - 1;

		// right
		for (; rect.right >= rect.left; rect.right--)
		{
			pData = (DWORD*)(pBuffer + 4 * lWidth * rect.top);
			pData += rect.right;

			bool bIsBreak = false;
			for (LONG i = 0; i < lDelta; ++i, pData += lWidth)
			{
				if (*pData != 0x00)
				{
					bIsBreak = true;
					break;
				}
			}
			if (bIsBreak)
				break;
		}
		if (rect.right < rect.left)
			rect.right = rect.left;

		return rect;
	}

	static RECT GetImageBounds2(MediaCore::IAVSUncompressedVideoFrame* pFrame, BYTE* pCache)
	{
		BYTE* pBuffer = NULL;
		LONG lWidth = 0;
		LONG lHeight = 0;

		pFrame->get_Buffer(&pBuffer);
		pFrame->get_Width(&lWidth);
		pFrame->get_Height(&lHeight);

		RECT rect;
		rect.left	= 0;
		rect.top	= 0;
		rect.right	= lWidth - 1;
		rect.bottom = lHeight - 1;

		// top
		DWORD* pData = (DWORD*)pBuffer;
		DWORD* pDataSrc = (DWORD*)pCache;
		for (; rect.top < lHeight; rect.top++)
		{
			if (0 != memcmp((void*)pData, (void*)pDataSrc, 4 * lWidth))
				break;

			pData += lWidth;
			pDataSrc += lWidth;
		}
		if (rect.top >= lHeight)
			rect.top = (lHeight - 1);

		// bottom
		for (; rect.bottom >= rect.top; rect.bottom--)
		{
			pData = (DWORD*)pBuffer;
			pData += (lWidth * rect.bottom);
			pDataSrc = (DWORD*)pCache;
			pDataSrc += (lWidth * rect.bottom);

			if (0 != memcmp((void*)pData, (void*)pDataSrc, 4 * lWidth))
				break;
		}
		if (rect.bottom < rect.top)
			rect.bottom = rect.top;

		LONG lDelta = rect.bottom - rect.top + 1;

		// left
		for (; rect.left < lWidth; rect.left++)
		{
			pData = (DWORD*)(pBuffer + 4 * lWidth * rect.top);
			pData += rect.left;
			pDataSrc = (DWORD*)(pCache + 4 * lWidth * rect.top);
			pDataSrc += rect.left;

			bool bIsBreak = false;
			for (LONG i = 0; i < lDelta; ++i, pData += lWidth, pDataSrc += lWidth)
			{
				if (*pData != *pDataSrc)
				{
					bIsBreak = true;
					break;
				}
			}
			if (bIsBreak)
				break;
		}
		if (rect.left >= lWidth)
			rect.left = lWidth - 1;

		// right
		for (; rect.right >= rect.left; rect.right--)
		{
			pData = (DWORD*)(pBuffer + 4 * lWidth * rect.top);
			pData += rect.right;

			pDataSrc = (DWORD*)(pCache + 4 * lWidth * rect.top);
			pDataSrc += rect.right;

			bool bIsBreak = false;
			for (LONG i = 0; i < lDelta; ++i, pData += lWidth)
			{
				if (*pData != *pDataSrc)
				{
					bIsBreak = true;
					break;
				}
			}
			if (bIsBreak)
				break;
		}
		if (rect.right < rect.left)
			rect.right = rect.left;

		return rect;
	}


	const double c_ag_Inch_to_MM	= 25.4;
	const double c_ag_1pxWidth		= 25.4 / 96;

	enum ImageType
	{
		itJPG	= 0,
		itPNG	= 1
	};

	class CImageInfo
	{
	public:
		ImageType	m_eType;
		LONG		m_lID;

		CImageInfo()
		{
			m_eType	= itJPG;
			m_lID	= -1;
		}
		CImageInfo(const CImageInfo& oSrc)
		{
			*this = oSrc;
		}
		CImageInfo& operator=(const CImageInfo& oSrc)
		{
			m_eType		= oSrc.m_eType;
			m_lID		= oSrc.m_lID;

			return *this;
		}
	};

	__forceinline static double FABS(double dVal)
	{
		return (dVal >= 0) ? dVal : -dVal;
	}
	__forceinline static int round(double dVal)
	{
		return (int)(dVal + 0.5);
	}
}