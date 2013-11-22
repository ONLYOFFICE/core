// AVSVideoFrameColorTransform.h : Declaration of the CAVSVideoFrameColorTransform

#pragma once
#include "resource.h"       // main symbols
#include "ASCUncompressedVideoFrame.h"
#include <math.h>


// IAVSVideoFrameColorTransform
[
	object,
	uuid("E84E31CD-914C-4442-BFB3-FF81D34CF887"),
	dual,	helpstring("IAVSVideoFrameColorTransform Interface"),
	pointer_default(unique)
]
__interface IAVSVideoFrameColorTransform : IDispatch
{
	[id(205000 + 1)] HRESULT AdjustBrightness([in] IAVSUncompressedVideoFrame* pFrame, [in] long Level);
	[id(205000 + 2)] HRESULT AdjustContrast([in] IAVSUncompressedVideoFrame* pFrame, [in] long Level);
	[id(205000 + 3)] HRESULT AdjustGamma([in] IAVSUncompressedVideoFrame* pFrame, [in] long Level);
};



// CAVSVideoFrameColorTransform

[
	coclass,
	threading(apartment),
	vi_progid("AVSMediaCore3.AVSVideoFrameColorTransfo"),
	progid("AVSMediaCore3.AVSVideoFrameColorTrans.1"),
	version(1.0),
	uuid("7F993343-4C1A-4A1B-BCE4-B87EA40E3A00"),
	helpstring("AVSVideoFrameColorTransform Class")
]
class ATL_NO_VTABLE CAVSVideoFrameColorTransform : 
	public IAVSVideoFrameColorTransform
{
public:
	CAVSVideoFrameColorTransform()
		: m_lColorSpace(0)
		, m_lHeight(0)
		, m_lWidth(0)
	{
	}
public:
	STDMETHOD(AdjustBrightness)(IAVSUncompressedVideoFrame* pFrame, long Level)
	{
		if ((NULL==pFrame)||(0==Level))
			return S_OK;
		
		Level = max(-255,min(Level,255));

		BYTE arTable[256];
		BYTE *pTable = arTable;
		for (int i=0;i<256;i++,pTable++)
		{
			*pTable = Byte(i + Level);
		}
		if (!GetFrameProperties(pFrame))
			return S_OK;

		switch (m_lColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			{
				BYTE *pYPlane = NULL;
				pFrame->get_Plane(0, &pYPlane);
				if (NULL==pYPlane)
					break;
				long lYStride = 0;
				pFrame->get_Stride(0, &lYStride);
				lYStride -= m_lWidth;
				if (0>lYStride)
					break;
				for (int i=0; i<m_lHeight; i++)
				{
					for (int j=0; j<m_lWidth; j++, pYPlane++)
					{
						*pYPlane = arTable[*pYPlane];
					}
					pYPlane += lYStride;
				}

/*
				BYTE *pTable = arTable;
				Level /= 2;
				for (int i=0;i<128;i++,pTable++)
				{
					*pTable = (i + Level)<=128 ? (i + Level) : 128;
				}
				for (int i=128;i<256;i++,pTable++)
				{
					*pTable = (i - Level)>=128 ? (i - Level) : 128;
				}

				BYTE *pUPlane = NULL;
				pFrame->get_Plane(1, &pUPlane);
				BYTE *pVPlane = NULL;
				pFrame->get_Plane(2, &pVPlane);
				if ((NULL==pUPlane)||(NULL==pVPlane))
					break;
				long lUVStride = 0;
				pFrame->get_Stride(1, &lUVStride);
				lUVStride -= (lWidth/2);
				if (0>lUVStride)
					break;
				for (int i=0; i<lHeight/2; i++)
				{
					for (int j=0; j<lWidth/2; j++, pUPlane++, pVPlane++)
					{
						*pUPlane = arTable[*pUPlane];
						*pVPlane = arTable[*pVPlane];
					}
					pUPlane += lUVStride;
					pVPlane += lUVStride;
				}
*/
			}
			break;
		case CSP_YUY2:
			{
				BYTE *pPlane = NULL;
				pFrame->get_Plane(0, &pPlane);
				if (NULL==pPlane)
					break;
				long lStride = 0;
				pFrame->get_Stride(0, &lStride);
				lStride -= 4*m_lWidth;
				if (0>lStride)
					break;
				for (int i=0; i<m_lHeight/2; i++)
				{
					for (int j=0; j<m_lWidth; j++)
					{
						*pPlane = arTable[*pPlane]; pPlane +=2;
						*pPlane = arTable[*pPlane]; pPlane +=2;
					}
					pPlane += lStride;
				}
			}
			break;
		case CSP_BGRA:
		case CSP_RGBA:
		case CSP_ABGR:
		case CSP_ARGB:
			{
				BYTE *pPlane = NULL;
				pFrame->get_Plane(0, &pPlane);
				if (NULL==pPlane)
					break;
				if ((CSP_ABGR==m_lColorSpace)||(CSP_ARGB==m_lColorSpace))
					pPlane++;
				long lStride = 0;
				pFrame->get_Stride(0, &lStride);
				lStride -= 4*m_lWidth;
				if (0>lStride)
					break;
				for (int i=0; i<m_lHeight; i++)
				{
					for (int j=0; j<m_lWidth; j++)
					{
						*pPlane = arTable[*pPlane]; pPlane++;
						*pPlane = arTable[*pPlane]; pPlane++;
						*pPlane = arTable[*pPlane]; pPlane++;
						pPlane++;
					}
					pPlane += lStride;
				}
			}
			break;
		case CSP_BGR:
			{
				BYTE *pPlane = NULL;
				pFrame->get_Plane(0, &pPlane);
				if (NULL==pPlane)
					break;
				long lStride = 0;
				pFrame->get_Stride(0, &lStride);
				lStride -= 3*m_lWidth;
				if (0>lStride)
					break;
				for (int i=0; i<m_lHeight; i++)
				{
					for (int j=0; j<m_lWidth; j++)
					{
						*pPlane = arTable[*pPlane]; pPlane++;
						*pPlane = arTable[*pPlane]; pPlane++;
						*pPlane = arTable[*pPlane]; pPlane++;
					}
					pPlane += lStride;
				}
			}
			break;
		}
		return S_OK;
	}
	STDMETHOD(AdjustContrast)(IAVSUncompressedVideoFrame* pFrame, long Level)
	{
		if ((NULL==pFrame)||(0==Level))
			return S_OK;
		// correct contrast level value
		Level = max(-255,min(Level,255));

		double dLevel = 1.0;
		// compute contrast level
		//if (Level <= 0)
		//	dLevel = 1.0 + (double)Level/255.0;
		//else
		//	dLevel = pow(255, (double)Level/255.0);
		if (Level <= 0)
			dLevel = 1.0 + (double)Level/256.0;
		else
			dLevel = 256.0/(256.0 - Level);
		
		// compose table
		BYTE arTable[256];
		BYTE *pTable = arTable;
		for (int i=0; i<256; i++,pTable++)
		{
			*pTable = Byte(127.5 + (i - 127.5)*dLevel);
		}
		if (!GetFrameProperties(pFrame))
			return S_OK;


		switch (m_lColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			{
				BYTE *pYPlane = NULL;
				pFrame->get_Plane(0, &pYPlane);
				if (NULL==pYPlane)
					break;
				long lYStride = 0;
				pFrame->get_Stride(0, &lYStride);
				lYStride -= m_lWidth;
				if (0>lYStride)
					break;
				for (int i=0; i<m_lHeight; i++)
				{
					for (int j=0; j<m_lWidth; j++, pYPlane++)
					{
						*pYPlane = arTable[*pYPlane];
					}
					pYPlane += lYStride;
				}
/*
				BYTE *pTable = arTable;
				Level /= 2;
				for (int i=0;i<128;i++,pTable++)
				{
					*pTable = (i + Level)<=128 ? (i + Level) : 128;
				}
				for (int i=128;i<256;i++,pTable++)
				{
					*pTable = (i - Level)>=128 ? (i - Level) : 128;
				}

				BYTE *pUPlane = NULL;
				pFrame->get_Plane(1, &pUPlane);
				BYTE *pVPlane = NULL;
				pFrame->get_Plane(2, &pVPlane);
				if ((NULL==pUPlane)||(NULL==pVPlane))
					break;
				long lUVStride = 0;
				pFrame->get_Stride(1, &lUVStride);
				lUVStride -= (m_lWidth/2);
				if (0>lUVStride)
					break;
				for (int i=0; i<m_lHeight/2; i++)
				{
					for (int j=0; j<m_lWidth/2; j++, pUPlane++, pVPlane++)
					{
						*pUPlane = arTable[*pUPlane];
						*pVPlane = arTable[*pVPlane];
					}
					pUPlane += lUVStride;
					pVPlane += lUVStride;
				}
*/
			}
			break;
		case CSP_YUY2:
			{
				BYTE *pPlane = NULL;
				pFrame->get_Plane(0, &pPlane);
				if (NULL==pPlane)
					break;
				long lStride = 0;
				pFrame->get_Stride(0, &lStride);
				lStride -= 4*m_lWidth;
				if (0>lStride)
					break;
				for (int i=0; i<m_lHeight/2; i++)
				{
					for (int j=0; j<m_lWidth; j++)
					{
						*pPlane = arTable[*pPlane]; pPlane +=2;
						*pPlane = arTable[*pPlane]; pPlane +=2;
					}
					pPlane += lStride;
				}
			}
			break;
		case CSP_BGRA:
		case CSP_RGBA:
		case CSP_ABGR:
		case CSP_ARGB:
			{
				BYTE *pPlane = NULL;
				pFrame->get_Plane(0, &pPlane);
				if (NULL==pPlane)
					break;
				if ((CSP_ABGR==m_lColorSpace)||(CSP_ARGB==m_lColorSpace))
					pPlane++;
				long lStride = 0;
				pFrame->get_Stride(0, &lStride);
				lStride -= 4*m_lWidth;
				if (0>lStride)
					break;
				for (int i=0; i<m_lHeight; i++)
				{
					for (int j=0; j<m_lWidth; j++)
					{
						*pPlane = arTable[*pPlane]; pPlane++;
						*pPlane = arTable[*pPlane]; pPlane++;
						*pPlane = arTable[*pPlane]; pPlane++;
						pPlane++;
					}
					pPlane += lStride;
				}
			}
			break;
		case CSP_BGR:
			{
				BYTE *pPlane = NULL;
				pFrame->get_Plane(0, &pPlane);
				if (NULL==pPlane)
					break;
				long lStride = 0;
				pFrame->get_Stride(0, &lStride);
				lStride -= 3*m_lWidth;
				if (0>lStride)
					break;
				for (int i=0; i<m_lHeight; i++)
				{
					for (int j=0; j<m_lWidth; j++)
					{
						*pPlane = arTable[*pPlane]; pPlane++;
						*pPlane = arTable[*pPlane]; pPlane++;
						*pPlane = arTable[*pPlane]; pPlane++;
					}
					pPlane += lStride;
				}
			}
			break;
		}
		return S_OK;
	}


	STDMETHOD(AdjustGamma)(IAVSUncompressedVideoFrame* pFrame, long Level)
	{
		if ((NULL==pFrame)||(0==Level))
			return S_OK;
		// correct contrast level value
		Level = max(-255,min(Level,255));

		// compose table
		BYTE arTable[256];
		BYTE *pTable = arTable;

		// compute gamma level
		if (Level <= 0)
		{
			// compute gamma values
			double dPower = 1.0/max(0.0001, 1.0 + Level/256.0);
			double dNumerator = 255.0/pow(255.0, dPower);

			// compose table
			for (int nIndex = 0; nIndex < 256; ++nIndex,pTable++)
				*pTable = Byte(dNumerator*pow(nIndex, dPower));
		}
		else
		{
			// compute gamma values
			double dPower = 1.0/pow(255, Level/256.0);
			double dNumerator = 255.0/pow(255.0, dPower);

			// compose table
			for (int nIndex = 0; nIndex < 256; ++nIndex, pTable++)
				*pTable = Byte(dNumerator*pow(nIndex, dPower));
		}
		if (!GetFrameProperties(pFrame))
			return S_OK;


		switch (m_lColorSpace)
		{
		case CSP_I420:
		case CSP_YV12:
			{
				BYTE *pYPlane = NULL;
				pFrame->get_Plane(0, &pYPlane);
				if (NULL==pYPlane)
					break;
				long lYStride = 0;
				pFrame->get_Stride(0, &lYStride);
				lYStride -= m_lWidth;
				if (0>lYStride)
					break;
				for (int i=0; i<m_lHeight; i++)
				{
					for (int j=0; j<m_lWidth; j++, pYPlane++)
					{
						*pYPlane = arTable[*pYPlane];
					}
					pYPlane += lYStride;
				}
/*
				BYTE *pTable = arTable;
				Level /= 2;
				for (int i=0;i<128;i++,pTable++)
				{
					*pTable = (i + Level)<=128 ? (i + Level) : 128;
				}
				for (int i=128;i<256;i++,pTable++)
				{
					*pTable = (i - Level)>=128 ? (i - Level) : 128;
				}

				BYTE *pUPlane = NULL;
				pFrame->get_Plane(1, &pUPlane);
				BYTE *pVPlane = NULL;
				pFrame->get_Plane(2, &pVPlane);
				if ((NULL==pUPlane)||(NULL==pVPlane))
					break;
				long lUVStride = 0;
				pFrame->get_Stride(1, &lUVStride);
				lUVStride -= (m_lWidth/2);
				if (0>lUVStride)
					break;
				for (int i=0; i<m_lHeight/2; i++)
				{
					for (int j=0; j<m_lWidth/2; j++, pUPlane++, pVPlane++)
					{
						*pUPlane = arTable[*pUPlane];
						*pVPlane = arTable[*pVPlane];
					}
					pUPlane += lUVStride;
					pVPlane += lUVStride;
				}
*/
			}
			break;
		case CSP_YUY2:
			{
				BYTE *pPlane = NULL;
				pFrame->get_Plane(0, &pPlane);
				if (NULL==pPlane)
					break;
				long lStride = 0;
				pFrame->get_Stride(0, &lStride);
				lStride -= 4*m_lWidth;
				if (0>lStride)
					break;
				for (int i=0; i<m_lHeight/2; i++)
				{
					for (int j=0; j<m_lWidth; j++)
					{
						*pPlane = arTable[*pPlane]; pPlane +=2;
						*pPlane = arTable[*pPlane]; pPlane +=2;
					}
					pPlane += lStride;
				}
			}
			break;
		case CSP_BGRA:
		case CSP_RGBA:
		case CSP_ABGR:
		case CSP_ARGB:
			{
				BYTE *pPlane = NULL;
				pFrame->get_Plane(0, &pPlane);
				if (NULL==pPlane)
					break;
				if ((CSP_ABGR==m_lColorSpace)||(CSP_ARGB==m_lColorSpace))
					pPlane++;
				long lStride = 0;
				pFrame->get_Stride(0, &lStride);
				lStride -= 4*m_lWidth;
				if (0>lStride)
					break;
				for (int i=0; i<m_lHeight; i++)
				{
					for (int j=0; j<m_lWidth; j++)
					{
						*pPlane = arTable[*pPlane]; pPlane++;
						*pPlane = arTable[*pPlane]; pPlane++;
						*pPlane = arTable[*pPlane]; pPlane++;
						pPlane++;
					}
					pPlane += lStride;
				}
			}
			break;
		case CSP_BGR:
			{
				BYTE *pPlane = NULL;
				pFrame->get_Plane(0, &pPlane);
				if (NULL==pPlane)
					break;
				long lStride = 0;
				pFrame->get_Stride(0, &lStride);
				lStride -= 3*m_lWidth;
				if (0>lStride)
					break;
				for (int i=0; i<m_lHeight; i++)
				{
					for (int j=0; j<m_lWidth; j++)
					{
						*pPlane = arTable[*pPlane]; pPlane++;
						*pPlane = arTable[*pPlane]; pPlane++;
						*pPlane = arTable[*pPlane]; pPlane++;
					}
					pPlane += lStride;
				}
			}
			break;
		}
		return S_OK;
	}

protected:
	long m_lColorSpace; 
	long m_lHeight;
	long m_lWidth;
	BOOL GetFrameProperties(IAVSUncompressedVideoFrame* pFrame)
	{
		if (NULL==pFrame)
			return FALSE;
		pFrame->get_ColorSpace(&m_lColorSpace);
		pFrame->get_Width(&m_lWidth);
		pFrame->get_Height(&m_lHeight);
		return TRUE;
	}
	BYTE Byte(double dValue)
	{
		return ((dValue < 0) ? 0 : (dValue > 255) ? 255 : (BYTE)dValue); 
	}
};

